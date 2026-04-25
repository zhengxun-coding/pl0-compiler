/*
 * PL/0 compiler - parser (语法分析)
 */

#include "pl0.h"

/* Global variables */
bool declbegsys[symnum];
bool statbegsys[symnum];
bool facbegsys[symnum];

void initparser()
{
    int i;

    for (i = 0; i < symnum; i++)
    {
        declbegsys[i] = false;
        statbegsys[i] = false;
        facbegsys[i] = false;
    }

    declbegsys[constsym] = true;
    declbegsys[varsym] = true;
    declbegsys[procsym] = true;

    statbegsys[beginsym] = true;
    statbegsys[callsym] = true;
    statbegsys[ifsym] = true;
    statbegsys[whilesym] = true;
    statbegsys[readsym] = true;
    statbegsys[writesym] = true;
    statbegsys[ident] = true;
    statbegsys[elsesym] = true;

    facbegsys[ident] = true;
    facbegsys[number] = true;
    facbegsys[lparen] = true;
    facbegsys[plus] = true;
    facbegsys[minus] = true;
}

int test(bool* s1, bool* s2, int n)
{
    if (!inset(sym, s1))
    {
        error(n);

        while ((!inset(sym, s1)) && (!inset(sym, s2)))
        {
            getsymdo;
        }
    }
    return 0;
}

int block(int lev, int tx, bool* fsys)
{
    int i;
    int dx;
    int tx0;
    int cx0;
    bool nxtlev[symnum];

    dx = 3;
    tx0 = tx;
    table[tx].adr = cx;

    gendo(jmp, 0, 0);

    if (lev > levmax)
    {
        error(32);
    }

    do {

        if (sym == constsym)
        {
            getsymdo;
            constdeclarationdo(&tx, lev, &dx);
            while (sym == comma)
            {
                getsymdo;
                constdeclarationdo(&tx, lev, &dx);
            }
            if (sym == semicolon)
            {
                getsymdo;
            }
            else
            {
                error(5);
            }

        }

        if (sym == varsym)
        {
            getsymdo;
            vardeclarationdo(&tx, lev, &dx);
            while (sym == comma)
            {
                getsymdo;
                vardeclarationdo(&tx, lev, &dx);
            }
            if (sym == semicolon)
            {
                getsymdo;
            }
            else
            {
                error(5);
            }

        }

        while (sym == procsym)
        {
            getsymdo;

            if (sym == ident)
            {
                enter(procedur, &tx, lev, &dx);
                getsymdo;
            }
            else
            {
                error(4);
            }

            if (sym == semicolon)
            {
                getsymdo;
            }
            else
            {
                error(5);
            }

            memcpy(nxtlev, fsys, sizeof(bool) * symnum);
            nxtlev[semicolon] = true;

            if (-1 == block(lev + 1, tx, nxtlev))
            {
                return -1;
            }

            if (sym == semicolon)
            {
                getsymdo;

                memcpy(nxtlev, statbegsys, sizeof(bool) * symnum);
                nxtlev[ident] = true;
                nxtlev[procsym] = true;
                testdo(nxtlev, fsys, 6);
            }
            else
            {
                error(5);
            }
        }

        memcpy(nxtlev, statbegsys, sizeof(bool) * symnum);
        nxtlev[ident] = true;
        testdo(nxtlev, declbegsys, 7);

    } while (inset(sym, declbegsys));

    code[table[tx0].adr].a = cx;
    table[tx0].size = dx;
    cx0 = cx;

    gendo(inte, 0, dx);

    if (tableswitch)
    {
        printf("TABLE:\n");
        if (tx0 + 1 > tx)
        {
            printf("    NULL\n");
        }
        for (i = tx0 + 1; i <= tx; i++)
        {

            switch (table[i].kind)
            {
            case constant:
                printf("    %d const %s ", i, table[i].name);
                printf("val=%d\n", table[i].val);
                fprintf(fas, "    %d const %s ", i, table[i].name);
                fprintf(fas, "val=%d\n", table[i].val);
                break;
            case variable:
                printf("    %d var   %s ", i, table[i].name);
                printf("lev=%d addr=%d\n", table[i].level, table[i].adr);
                fprintf(fas, "    %d var   %s ", i, table[i].name);
                fprintf(fas, "lev=%d addr=%d\n", table[i].level, table[i].adr);
                break;
            case array:
                printf("    %d array %s ", i, table[i].name);
                printf("lev=%d addr=%d size=%d lower=%d\n", table[i].level, table[i].adr, table[i].size, table[i].lower);
                fprintf(fas, "    %d array %s ", i, table[i].name);
                fprintf(fas, "lev=%d addr=%d size=%d lower=%d\n", table[i].level, table[i].adr, table[i].size, table[i].lower);
                break;
            case procedur:
                printf("    %d proc  %s ", i, table[i].name);
                printf("lev=%d addr=%d size=%d\n", table[i].level, table[i].adr, table[i].size);
                fprintf(fas, "    %d proc  %s ", i, table[i].name);
                fprintf(fas, "lev=%d addr=%d size=%d\n", table[i].level, table[i].adr, table[i].size);
                break;
            }
        }
        printf("\n");
    }

    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
    nxtlev[semicolon] = true;
    nxtlev[endsym] = true;

    statementdo(nxtlev, &tx, lev);

    gendo(opr, 0, 0);

    memset(nxtlev, 0, sizeof(bool)* symnum);
    testdo(fsys, nxtlev, 8);
    listcode(cx0);

    return 0;
}

int statement(bool* fsys, int* ptx, int lev)
{
    int i, cx1, cx2;
    bool nxtlev[symnum];

    if (sym == ident)
    {
        i = position(id, *ptx);
        if (i == 0)
        {
            error(11);
            return 0;
        }
        else
        {

            if (table[i].kind == variable)
            {
                getsymdo;
                if(sym==becomes){
                    getsymdo;
                }
                else{
                    error(13);
                }
                memcpy(nxtlev,fsys,sizeof(bool)*symnum);
                expressiondo(nxtlev,ptx,lev);
                gendo(sto,lev-table[i].level,table[i].adr);
            }
            else if(table[i].kind==array)
            {
                getsymdo;
                if (sym == lparen){
                    getsymdo;
                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                    nxtlev[rparen] = true;
                    expressiondo(nxtlev, ptx, lev);
                    gendo(lit, 0, table[i].lower);
                    gendo(opr, 0, 3);
                    gendo(lit, 0, table[i].adr);
                    gendo(opr, 0, 2);
                    gendo(lod, lev - table[i].level, 0);
                    if (sym == rparen)
                    {
                        getsymdo;
                    }
                    else
                    {
                        error(22);
                    }
                    if (sym == becomes)
                    {
                        getsymdo;
                    }
                    else
                    {
                        error(13);
                    }
                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                    expressiondo(nxtlev, ptx, lev);
                    gendo(sto, lev - table[i].level, 0);
                }
                else
                {
                    error(43);
                }
            }
            else
            {
                error(12);
            }
        }
    }
    else
    {
        if (sym == readsym)
        {
            getsymdo;
            if (sym != lparen)
            {
                error(34);
            }
            else
            {
                do {
                    getsymdo;
                    if (sym == ident)
                    {
                        i = position(id, *ptx);
                    }
                    else
                    {
                        i = 0;
                    }

                    if (i == 0)
                    {
                        error(35);
                    }
                    else if (table[i].kind != variable)
                    {
                        error(32);
                    }
                    else
                    {
                        gendo(opr, 0, 16);
                        gendo(sto, lev - table[i].level, table[i].adr);
                    }
                    getsymdo;

                } while (sym == comma);
            }
            if (sym != rparen)
            {
                error(33);
                while (!inset(sym, fsys))
                {
                    getsymdo;
                }
            }
            else
            {
                getsymdo;
            }
        }
        else
        {
            if (sym == writesym)
            {
                getsymdo;
                if (sym == lparen)
                {
                    do {
                        getsymdo;
                        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                        nxtlev[rparen] = true;
                        nxtlev[comma] = true;

                        expressiondo(nxtlev, ptx, lev);
                        gendo(opr, 0, 14);
                    } while (sym == comma);
                    if (sym != rparen)
                    {
                        error(33);
                    }
                    else
                    {
                        getsymdo;
                    }
                }
                gendo(opr, 0, 15);
            }
            else
            {
                if (sym == callsym)
                {
                    getsymdo;
                    if (sym != ident)
                    {
                        error(14);
                    }
                    else
                    {
                        i = position(id, *ptx);
                        if (i == 0)
                        {
                            error(11);
                        }
                        else
                        {
                            if (table[i].kind == procedur)
                            {
                                gendo(cal, lev - table[i].level, table[i].adr);
                            }
                            else
                            {
                                error(15);
                            }
                        }
                        getsymdo;
                    }
                }
                else
                {
                    if (sym == ifsym)
                    {
                        getsymdo;

                        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                        nxtlev[thensym] = true;
                        nxtlev[dosym] = true;

                        conditiondo(nxtlev, ptx, lev);

                        if (sym == thensym)
                        {
                            getsymdo;
                        }
                        else
                        {
                            error(16);
                        }

                        cx1 = cx;
                        gendo(jpc, 0, 0);

                        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                        nxtlev[elsesym] = true;
                        statementdo(nxtlev, ptx, lev);

                        if (sym == elsesym)
                        {
                            getsymdo;

                            cx2 = cx;
                            gendo(jmp, 0, 0);

                            code[cx1].a = cx;

                            statementdo(fsys, ptx, lev);

                            code[cx2].a = cx;
                        }
                        else
                        {
                            code[cx1].a = cx;
                        }
                    }
                    else
                    {
                        if (sym == beginsym)
                        {
                            getsymdo;

                            memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                            nxtlev[semicolon] = true;
                            nxtlev[endsym] = true;

                            statementdo(nxtlev, ptx, lev);

                            while (inset(sym, statbegsys) || sym == semicolon)
                            {
                                if (sym == semicolon)
                                {
                                    getsymdo;
                                }
                                else
                                {
                                    error(10);
                                }
                                statementdo(nxtlev, ptx, lev);
                            }

                            if (sym == endsym)
                            {
                                getsymdo;
                            }
                            else
                            {
                                error(17);
                            }
                        }
                        else
                        {
                            if (sym == whilesym)
                            {
                                cx1 = cx;
                                getsymdo;

                                memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                                nxtlev[dosym] = true;

                                conditiondo(nxtlev, ptx, lev);

                                cx2 = cx;
                                gendo(jpc, 0, 0);

                                if (sym == dosym)
                                {
                                    getsymdo;
                                }
                                else
                                {
                                    error(18);
                                }

                                statementdo(fsys, ptx, lev);

                                gendo(jmp, 0, cx1);

                                code[cx2].a = cx;
                            }
                            else
                            {
                                memset(nxtlev, 0, sizeof(bool) * symnum);
                                testdo(fsys, nxtlev, 19);
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

int expression(bool* fsys, int* ptx, int lev)
{
    enum symbol addop;
    bool nxtlev[symnum];

    if (sym == plus || sym == minus)
    {
        addop = sym;
        getsymdo;

        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;

        termdo(nxtlev, ptx, lev);

        if (addop == minus)
        {
            gendo(opr, 0, 1);
        }
    }
    else
    {
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;

        termdo(nxtlev, ptx, lev);
    }

    while (sym == plus || sym == minus)
    {
        addop = sym;
        getsymdo;

        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;

        termdo(nxtlev, ptx, lev);

        if (addop == plus)
        {
            gendo(opr, 0, 2);
        }
        else
        {
            gendo(opr, 0, 3);
        }
    }
    return 0;
}

int term(bool* fsys, int* ptx, int lev)
{
    enum symbol mulop;
    bool nxtlev[symnum];

    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
    nxtlev[times] = true;
    nxtlev[slash] = true;

    factordo(nxtlev, ptx, lev);

    while (sym == times || sym == slash)
    {
        mulop = sym;
        getsymdo;

        factordo(nxtlev, ptx, lev);

        if (mulop == times)
        {
            gendo(opr, 0, 4);
        }
        else
        {
            gendo(opr, 0, 5);
        }

    }
    return 0;
}

int factor(bool* fsys, int* ptx, int lev)
{
    int i;
    bool nxtlev[symnum];
    testdo(facbegsys, fsys, 24);

    if (inset(sym, facbegsys))
    {
        if (sym == ident)
        {
            i = position(id, *ptx);
            if (i == 0)
            {
                error(11);
                return 0;
            }
            switch (table[i].kind)
            {
            case constant:
                gendo(lit, 0, table[i].val);
                getsymdo;
                break;
            case variable:
                gendo(lod, lev - table[i].level, table[i].adr);
                getsymdo;
                break;
            case array:
                getsymdo;
                if(sym==lparen){
                    getsymdo;
                    memcpy(nxtlev,fsys,sizeof(bool)*symnum);
                    nxtlev[rparen]=true;
                    expressiondo(nxtlev, ptx, lev);
                    gendo(lit,0,table[i].lower);
                    gendo(opr,0,3);
                    gendo(lit,0,table[i].adr);
                    gendo(opr,0,2);
                    gendo(lod,lev-table[i].level,0);
                    if(sym==rparen){
                        getsymdo;
                    }else{
                        error(22);
                    }
                }else{
                    error(42);
                }
                break;
            case procedur:
                error(21);
                break;
            }
        }
        else
        {
            if (sym == number)
            {
                if (num > amax)
                {
                    error(31);
                    num = 0;
                }

                gendo(lit, 0, num);
                getsymdo;
            }
            else
            {
                if (sym == lparen)
                {
                    getsymdo;
                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                    nxtlev[rparen] = true;

                    expressiondo(nxtlev, ptx, lev);

                    if (sym == rparen)
                    {
                        getsymdo;
                    }
                    else
                    {
                        error(22);
                    }
                }
                testdo(fsys, facbegsys, 23);
            }
        }
    }
    return 0;
}

int condition(bool* fsys, int* ptx, int lev)
{
    enum symbol relop;
    bool nxtlev[symnum];

    if (sym == oddsym)
    {
        getsymdo;

        expressiondo(fsys, ptx, lev);

        gendo(opr, 0, 6);
    }
    else
    {
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[eql] = true;
        nxtlev[neq] = true;
        nxtlev[lss] = true;
        nxtlev[leq] = true;
        nxtlev[gtr] = true;
        nxtlev[geq] = true;

        expressiondo(nxtlev, ptx, lev);

        if (sym != eql && sym != neq && sym != lss && sym != leq && sym != gtr && sym != geq)
        {
            error(20);
        }
        else
        {
            relop = sym;
            getsymdo;

            expressiondo(fsys, ptx, lev);

            switch (relop)
            {
            case eql:
                gendo(opr, 0, 8);
                break;
            case neq:
                gendo(opr, 0, 9);
                break;
            case lss:
                gendo(opr, 0, 10);
                break;
            case geq:
                gendo(opr, 0, 11);
                break;
            case gtr:
                gendo(opr, 0, 12);
                break;
            case leq:
                gendo(opr, 0, 13);
                break;
            default:
                // 未处理的枚举值
                break;
            }
        }
    }
    return 0;
}

int constdeclaration(int* ptx, int lev, int* pdx)
{
    if (sym == ident)
    {
        getsymdo;
        if (sym == eql || sym == becomes)
        {
            if (sym == becomes)
            {
                error(1);
            }
            getsymdo;
            if (sym == number)
            {
                enter(constant, ptx, lev, pdx);
                getsymdo;
            }
            else
            {
                error(2);
            }
        }
        else
        {
            error(3);
        }
    }
    else
    {
        error(4);
    }
    return 0;
}

int vardeclaration(int* ptx, int lev, int* pdx)
{
    if (sym == ident)
    {
        char array_name[al+1];
        strcpy(array_name, id);
        getsymdo;
        if (sym == lparen)
        {
            int lower = 0, upper = 0, size = 0;
            getsymdo;
            if (sym == number) {
                lower = num;
                getsymdo;
            } else if (sym == ident) {
                int pos = position(id, *ptx);
                if (pos > 0 && table[pos].kind == constant) {
                    lower = table[pos].val;
                } else {
                    error(37);
                }
                getsymdo;
            } else {
                error(37);
            }
            if (sym == colon) {
                getsymdo;
            } else {
                error(38);
            }
            if (sym == number) {
                upper = num;
                getsymdo;
            } else if (sym == ident) {
                int pos = position(id, *ptx);
                if (pos > 0 && table[pos].kind == constant) {
                    upper = table[pos].val;
                } else {
                    error(39);
                }
                getsymdo;
            } else {
                error(39);
            }
            if (sym == rparen) {
                getsymdo;
            } else {
                error(40);
            }
            size = upper - lower + 1;
            if (size <= 0) {
                error(41);
            }

            (*ptx)++;
            strcpy(table[*ptx].name, array_name);
            table[*ptx].kind = array;
            table[*ptx].level = lev;
            table[*ptx].adr = *pdx;
            table[*ptx].size = size;
            table[*ptx].lower = lower;
            *pdx += size;
        }
        else
        {
            enter(variable, ptx, lev, pdx);
        }
    }
    else
    {
        error(4);
    }
    return 0;
}
