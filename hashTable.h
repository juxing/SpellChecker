#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

typedef struct {
    char *str;
    int len;
}hString;

void initTable(hString hashTable[], long tableLen);

long findPos(const char *str, hString hashTable[], long tableLen);

void insert(const char *str, hString hashTable[], long tableLen);

void deTable(hString hashTable[], long tableLen);

#endif
