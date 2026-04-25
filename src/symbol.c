/*
 * PL/0 compiler - symbol table (符号表管理)
 */

#include "pl0.h"

/* Global variable */
struct tablestruct table[txmax];

int position(char* idt, int tx)
{
    int i;
    strcpy(table[0].name, idt);
    i = tx;
    while (strcmp(table[i].name, idt) != 0)
    {
        i--;
    }
    return i;
}

void enter(enum object k, int* ptx, int lev, int* pdx)
{
    (*ptx)++;

    strcpy(table[(*ptx)].name, id);

    table[(*ptx)].kind = k;

    switch (k)
    {
    case constant:
        if (num > amax)
        {
            error(31);
            num = 0;
        }
        table[(*ptx)].val = num;
        break;
    case variable:
        table[(*ptx)].level = lev;
        table[(*ptx)].adr = (*pdx);
        (*pdx)++;
        break;
    case procedur:
        table[(*ptx)].level = lev;
        break;
    case array:
        // 若有 array 类型的符号处理逻辑，可在此补充
        break;
    default:
        // 未处理的枚举值
        break;
    }
}
