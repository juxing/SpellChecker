#include <string.h>

/* Functions for calculating edit distance. */

int min(int x, int y, int z) {
    int tmp = (x < y ? x : y);
    tmp = (tmp < z ? tmp : z);
    return tmp;
}

int levDis(const char *s, const char *t) {
    int i, j;
    int slen = strlen(s);
    int tlen = strlen(t);
    
    if(slen == 0)
        return tlen;
    if(tlen == 0)
        return slen;

    int matrix[slen+1][tlen+1];
    for(i = 0; i < slen + 1; i++)
        matrix[i][0] = i;
    for(i = 0; i < tlen + 1; i++)
        matrix[0][i] = i;

    for(i = 1; i < slen + 1; i++)
        for(j = 1; j < tlen + 1; j++) {
            int cost = 0;
            if(s[i-1] != t[j-1])
                cost = 1;
            matrix[i][j] = min(matrix[i-1][j]+1, matrix[i][j-1]+1, matrix[i-1][j-1]+cost);
        }

    return matrix[slen][tlen];
}
