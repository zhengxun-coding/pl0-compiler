/*
 * PL/0 compiler - lexer (词法分析)
 */

#include "pl0.h"

/* Global variables */
char ch;
enum symbol sym;
char id[al+1];
int num;
int cc, ll;
char line[81];
char a[al+1];
char word[norw][al];
enum symbol wsym[norw];
enum symbol ssym[256];

void init()
{
    int i;

    for (i = 0; i <= 255; i++)
    {
        ssym[i] = nul;
    }
    ssym['+'] = plus;
    ssym['-'] = minus;
    ssym['*'] = times;
    ssym['/'] = slash;
    ssym['('] = lparen;
    ssym[')'] = rparen;
    ssym['='] = eql;
    ssym[','] = comma;
    ssym['.'] = period;
    ssym['#'] = neq;
    ssym[';'] = semicolon;
    ssym[':'] = colon;

    strcpy(&(word[0][0]), "begin");
    strcpy(&(word[1][0]), "call");
    strcpy(&(word[2][0]), "const");
    strcpy(&(word[3][0]), "do");
    strcpy(&(word[4][0]), "else");
    strcpy(&(word[5][0]), "end");
    strcpy(&(word[6][0]), "if");
    strcpy(&(word[7][0]), "odd");
    strcpy(&(word[8][0]), "procedure");
    strcpy(&(word[9][0]), "read");
    strcpy(&(word[10][0]), "then");
    strcpy(&(word[11][0]), "var");
    strcpy(&(word[12][0]), "while");
    strcpy(&(word[13][0]), "write");

    wsym[0] = beginsym;
    wsym[1] = callsym;
    wsym[2] = constsym;
    wsym[3] = dosym;
    wsym[4] = elsesym;
    wsym[5] = endsym;
    wsym[6] = ifsym;
    wsym[7] = oddsym;
    wsym[8] = procsym;
    wsym[9] = readsym;
    wsym[10] = thensym;
    wsym[11] = varsym;
    wsym[12] = whilesym;
    wsym[13] = writesym;
}

int getch()
{
    if (cc == ll)
    {
        if (feof(fin))
        {
            printf("program incomplete");
            return -1;
        }

        ll = 0;
        cc = 0;

        printf("%d ", cx);
        fprintf(fa1, "%d ", cx);
        ch = ' ';
        while (ll < 80 && ch != 10)
        {
            if (EOF == fscanf(fin, "%c", &ch))
            {
                line[ll] = 0;
                break;
            }

            printf("%c", ch);
            fprintf(fa1, "%c", ch);
            line[ll] = ch;
            ll++;
        }
        line[80] = 0;
        printf("\n");
        fprintf(fa1, "\n");
    }
    ch = line[cc];
    cc++;
    return 0;
}

int getsym()
{
    int i, j, k;

    while (ch == ' ' || ch == 10 || ch == 13 || ch == 9 || ch=='{')
    {
        if(ch=='{'){
            do{
                getchdo;
            }while(ch!='}' && ch!=EOF);
            getchdo;
        }else{
            getchdo;
        }
    }
    if (ch >= 'a' && ch <= 'z')
    {
        k = 0;
        do {
            if (k < al)
            {
                a[k] = ch;
                k++;
            }
            getchdo;
        } while ((ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9'));
        a[k] = 0;
        strcpy(id, a);
        i = 0;
        j = norw - 1;
        do {
            k = (i + j) / 2;
            if (strcmp(id, word[k]) < 0)
            {
                j = k - 1;
            }
            else if (strcmp(id, word[k]) > 0)
            {
                i = k + 1;
            }
        } while (i <= j);
        if (i - 1 > j)
        {
            sym = wsym[k];
        }
        else
        {
            sym = ident;
        }
    }
    else
    {
        if (ch >= '0' && ch <= '9')
        {
            k = 0;
            num = 0;
            sym = number;
            do {
                num = 10 * num + ch - '0';
                k++;
                getchdo;
            } while (ch >= '0' && ch <= '9');
            k--;
            if (k > nmax)
            {
                error(30);
            }
        }
        else
        {
            if (ch == ':')
            {
                getchdo;
                if (ch == '=')
                {
                    sym = becomes;
                    getchdo;
                }
                else
                {
                    sym = colon;
                }
            }
            else
            {
                if (ch == '<')
                {
                    getchdo;
                    if (ch == '=')
                    {
                        sym = leq;
                        getchdo;
                    }
                    else
                    {
                        sym = lss;
                    }
                }
                else
                {
                    if (ch == '>')
                    {
                        getchdo;
                        if (ch == '=')
                        {
                            sym = geq;
                            getchdo;
                        }
                        else
                        {
                            sym = gtr;
                        }
                    }
                    else
                    {
                        sym = ssym[(unsigned char)ch];

                        if (sym != period)
                        {
                            getchdo;
                        }
                    }
                }
            }
        }
    }
    return 0;
}
