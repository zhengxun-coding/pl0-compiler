/*
 * PL/0 compiler - shared types and structures
 */

#ifndef PL0_H
#define PL0_H

#include <stdio.h>
#include <string.h>

typedef enum {
    false,
    true
} bool;

#define norw 14
#define txmax 100
#define nmax 14
#define al 10
#define amax 2047
#define levmax 3
#define cxmax 500

enum symbol {
    nul,         ident,     number,     plus,      minus,
    times,       slash,     oddsym,     eql,       neq,
    lss,         leq,       gtr,        geq,       lparen,
    rparen,      comma,     semicolon,  period,    becomes,
    beginsym,    endsym,    ifsym,      thensym,   whilesym,
    writesym,    readsym,   dosym,      callsym,   constsym,
    varsym,      procsym,   colon,      elsesym,
};
#define symnum 34

enum object {
    constant,
    variable,
    procedur,
    array
};

enum fct {
    lit,     opr,     lod,
    sto,     cal,     inte,
    jmp,     jpc,
};
#define fctnum 8

struct instruction
{
    enum fct f;
    int l;
    int a;
};

struct tablestruct
{
    char name[al];
    enum object kind;
    int val;
    int level;
    int adr;
    int size;
    int lower;
};

/* Global variables */
extern FILE* fas;
extern FILE* fa;
extern FILE* fa1;
extern FILE* fa2;
extern bool listswitch;
extern bool tableswitch;
extern char ch;
extern enum symbol sym;
extern char id[al+1];
extern int num;
extern int cc, ll;
extern int cx;
extern char line[81];
extern char a[al+1];
extern struct instruction code[cxmax];
extern char word[norw][al];
extern enum symbol wsym[norw];
extern enum symbol ssym[256];
extern char mnemonic[fctnum][5];
extern bool declbegsys[symnum];
extern bool statbegsys[symnum];
extern bool facbegsys[symnum];
extern struct tablestruct table[txmax];
extern FILE* fin;
extern char fname[al];
extern int err;

#define getsymdo                      if(-1 == getsym()) return -1
#define getchdo                       if(-1 == getch()) return -1
#define testdo(a, b, c)               if(-1 == test(a, b, c)) return -1
#define gendo(a, b, c)                if(-1 == gen(a, b, c)) return -1
#define expressiondo(a, b, c)         if(-1 == expression(a, b, c)) return -1
#define factordo(a, b, c)             if(-1 == factor(a, b, c)) return -1
#define termdo(a, b, c)               if(-1 == term(a, b, c)) return -1
#define conditiondo(a, b, c)          if(-1 == condition(a, b, c)) return -1
#define statementdo(a, b, c)          if(-1 == statement(a, b, c)) return -1
#define constdeclarationdo(a, b, c)   if(-1 == constdeclaration(a, b, c)) return -1
#define vardeclarationdo(a, b, c)     if(-1 == vardeclaration(a, b, c)) return -1

/* lexer.c */
void init();
int getsym();
int getch();

/* symbol.c */
int position(char* idt, int tx);
void enter(enum object k, int* ptx, int lev, int* pdx);

/* codegen.c */
void initcodegen();
int gen(enum fct x, int y, int z);

/* parser.c */
void initparser();
int test(bool* s1, bool* s2, int n);
int block(int lev, int tx, bool* fsys);
int statement(bool* fsys, int* ptx, int lev);
int expression(bool* fsys, int* ptx, int lev);
int term(bool* fsys, int* ptx, int lev);
int factor(bool* fsys, int* ptx, int lev);
int condition(bool* fsys, int* ptx, int lev);
int vardeclaration(int* ptx, int lev, int* pdx);
int constdeclaration(int* ptx, int lev, int* pdx);

/* vm.c */
void interpret();
int base(int l, int* s, int b);

/* platform.c */
void error(int n);
void listcode(int cx0);

/* set.c - set operations */
int inset(int e, bool* s);
int addset(bool* sr, bool* s1, bool* s2, int n);
int subset(bool* sr, bool* s1, bool* s2, int n);
int mulset(bool* sr, bool* s1, bool* s2, int n);

#endif
