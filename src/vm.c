/*
 * PL/0 compiler - virtual machine (虚拟机)
 */

#include "pl0.h"

#define stacksize 500

void interpret()
{
    int p, b, t;
    struct instruction i;
    int s[stacksize];

    printf("start pl0\n");
    t = 0;
    b = 0;
    p = 0;
    s[0] = s[1] = s[2] = 0;
    do {
        i = code[p];
        p++;
        switch (i.f)
        {
        case lit:
            s[t] = i.a;
            t++;
            break;
        case opr:
            switch (i.a)
            {
            case 0:
                t = b;
                p = s[t + 2];
                b = s[t + 1];
                break;
            case 1:
                s[t - 1] = -s[t - 1];
                break;
            case 2:
                t--;
                s[t - 1] = s[t - 1] + s[t];
                break;
            case 3:
                t--;
                s[t - 1] = s[t - 1] - s[t];
                break;
            case 4:
                t--;
                s[t - 1] = s[t - 1] * s[t];
                break;
            case 5:
                t--;
                s[t - 1] = s[t - 1] / s[t];
                break;
            case 6:
                s[t - 1] = s[t - 1] % 2;
                break;
            case 8:
                t--;
                s[t - 1] = (s[t - 1] == s[t]);
                break;
            case 9:
                t--;
                s[t - 1] = (s[t - 1] != s[t]);
                break;
            case 10:
                t--;
                s[t - 1] = (s[t - 1] < s[t]);
                break;
            case 11:
                t--;
                s[t - 1] = (s[t - 1] >= s[t]);
                break;
            case 12:
                t--;
                s[t - 1] = (s[t - 1] > s[t]);
                break;
            case 13:
                t--;
                s[t - 1] = (s[t - 1] <= s[t]);
                break;
            case 14:
                printf("%d", s[t - 1]);
                fprintf(fa2, "%d", s[t - 1]);
                t--;
                break;
            case 15:
                printf("\n");
                fprintf(fa2, "\n");
                break;
            case 16:
                printf("?");
                fprintf(fa2, "?");
                scanf("%d", &(s[t]));
                fprintf(fa2, "%d\n", s[t]);
                t++;
                break;
            }
            break;
        case lod:
            s[t] = s[base(i.l, s, b) + i.a];
            t++;
            break;
        case sto:
            t--;
            s[base(i.l, s, b) + i.a] = s[t];
            break;
        case cal:
            s[t] = base(i.l, s, b);
            s[t + 1] = b;
            s[t + 2] = p;
            b = t;
            p = i.a;
            break;
        case inte:
            t += i.a;
            break;
        case jmp:
            p = i.a;
            break;
        case jpc:
            t--;
            if (s[t] == 0)
            {
                p = i.a;
            }
            break;
        }
    } while (p != 0);
}

int base(int l, int* s, int b)
{
    int b1;
    b1 = b;
    while (l > 0)
    {
        b1 = s[b1];
        l--;
    }
    return b1;
}
