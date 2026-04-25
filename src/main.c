/*
 * PL/0 compiler - main entry point
 */

#include "pl0.h"

int main()
{
    bool nxtlev[symnum];

    printf("Input pl/0 file?   ");
    scanf("%s", fname);

    fin = fopen(fname, "r");

    if (fin)
    {
        printf("List object code?(Y/N)");
        scanf("%s", fname);
        listswitch = (fname[0] == 'y' || fname[0] == 'Y');

        printf("List symbol table?(Y/N)");
        scanf("%s", fname);
        tableswitch = (fname[0] == 'y' || fname[0] == 'Y');

        fa1 = fopen("fa1.tmp", "w");
        fprintf(fa1, "Input pl/0 file?   ");
        fprintf(fa1, "%s\n", fname);

        init();
        initcodegen();
        initparser();

        err = 0;
        cc = cx = ll = 0;
        ch = ' ';

        if (-1 != getsym())
        {
            fa = fopen("fa.tmp", "w");
            fas = fopen("fas.tmp", "w");
            addset(nxtlev, declbegsys, statbegsys, symnum);
            nxtlev[period] = true;

            if (-1 == block(0, 0, nxtlev))
            {
                fclose(fa);
                fclose(fa1);
                fclose(fas);
                fclose(fin);
                printf("\n");
                return 0;
            }
            fclose(fa);
            fclose(fa1);
            fclose(fas);

            if (sym != period)
            {
                error(9);
            }

            if (err == 0)
            {
                fa2 = fopen("fa2.tmp", "w");
                interpret();
                fclose(fa2);
            }
            else
            {
                printf("Errors in pl/0 program");
            }
        }

        fclose(fin);
    }
    else
    {
        printf("Can't open file!\n");
    }

    printf("\n");
    getchar();
    return 0;
}
