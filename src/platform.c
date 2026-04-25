/*
 * PL/0 compiler - platform specific functions (平台相关)
 */

#include "pl0.h"

/* Global variables - definitions */
FILE* fas;
FILE* fa;
FILE* fa1;
FILE* fa2;
bool listswitch;
bool tableswitch;
FILE* fin;
char fname[al];
int err;

void error(int n)
{
    char space[81];
    memset(space, 32, 81);

    space[cc - 1] = 0;

    printf("****%s!%d\n", space, n);
    fprintf(fa1, "****%s!%d\n", space, n);

    err++;
}

void listcode(int cx0)
{
    int i;
    if (listswitch)
    {
        for (i = cx0; i < cx; i++)
        {
            printf("%d %s %d %d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
            fprintf(fa, "%d %s %d %d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
        }
    }
}
