/* Hash table related fuctions. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hashTable.h"

// Initialize hash table
void initTable(hString hashTable[], long tableLen) {
    long i;
    for(i = 0; i < tableLen; i++) {
        hashTable[i].str = NULL;
        hashTable[i].len = 0;
    }
}

// Find index in hash table for strings
long findPos(const char *str, hString hashTable[], long tableLen) {
    int offset = 1;
    int i;
    long key;
    i = key = 0;

    while(str[i]) {
        key = 37 * key + str[i++];
    }
    key %= tableLen;
    if(key < 0)
        key += tableLen;
 
    while((hashTable[key].len != 0) && (strcmp(hashTable[key].str, str) != 0)) {
        key += offset;
        offset += 2;
        if(key >= tableLen)
            key -= tableLen;
    }

    return key;
}

// Insert words into hash table
void insert(const char *str, hString hashTable[], long tableLen) {
    long key = findPos(str, hashTable, tableLen);
    if(hashTable[key].len != 0)
        return;  
    if((hashTable[key].str = (char*)malloc(strlen(str) + 1)) == NULL) {
        printf("Failed to malloc.\n");
        exit(1);
    }
    strcpy(hashTable[key].str, str);
    hashTable[key].len = strlen(str);
}

// Release the memory when exit from program
void deTable(hString hashTable[], long tableLen) {
    long i;
    for(i = 0; i < tableLen; i++) {
        if(hashTable[i].str != NULL) {
            free(hashTable[i].str);
            hashTable[i].str = NULL;
            hashTable[i].len = 0;
        }
    }
}
