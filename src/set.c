/*
 * PL/0 compiler - set operations (集合操作)
 */

#include "pl0.h"

int inset(int e, bool* s)
{
    return s[e];
}

int addset(bool* sr, bool* s1, bool* s2, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        sr[i] = s1[i] || s2[i];
    }
    return 0;
}

int subset(bool* sr, bool* s1, bool* s2, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        sr[i] = s1[i] && (!s2[i]);
    }
    return 0;
}

int mulset(bool* sr, bool* s1, bool* s2, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        sr[i] = s1[i] && s2[i];
    }
    return 0;
}
