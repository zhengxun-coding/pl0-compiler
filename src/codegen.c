/*
 * PL/0 compiler - code generation (代码生成)
 */

#include "pl0.h"

/* Global variables */
struct instruction code[cxmax];
char mnemonic[fctnum][5];
int cx;

void initcodegen()
{
    strcpy(&mnemonic[0][0], "lit");
    strcpy(&mnemonic[1][0], "opr");
    strcpy(&mnemonic[2][0], "lod");
    strcpy(&mnemonic[3][0], "sto");
    strcpy(&mnemonic[4][0], "cal");
    strcpy(&mnemonic[5][0], "int");
    strcpy(&mnemonic[6][0], "jmp");
    strcpy(&mnemonic[7][0], "jpc");
}

int gen(enum fct x, int y, int z)
{
    if (cx >= cxmax)
    {
        printf("Program too long");
        return -1;
    }

    code[cx].f = x;
    code[cx].l = y;
    code[cx].a = z;
    cx++;
    return 0;
}
