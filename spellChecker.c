#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include "levDis.h"
#include "hashTable.h"

#define STRLEN 46 //Restrict length of words can be handled

/* Recursive method for calculating edit distance,
do not use in this program
int ld(char *s, char *t) {
    int len_s = strlen(s);
    int len_t = strlen(t);
    int cost;
    
    if(len_s == 0)
        return len_t;
    if(len_t == 0)
        return len_s;
    
    if(s[len_s-1] == t[len_t-1])
        cost = 0;
    else
        cost = 1;

    char tmp_s[len_s];
    char tmp_t[len_t];
    strncpy(tmp_s, s, len_s-1);
    tmp_s[len_s-1] = '\0';
    strncpy(tmp_t, t, len_t-1);
    tmp_t[len_t-1] = '\0';

    int a = ld(tmp_s, t) + 1;
    int b = ld(s, tmp_t) + 1;
    int c = ld(tmp_s, tmp_t) + cost;

    if(a <= b) {
        if(a <= c)
            return a;
        else
            return c;
    }
    else {
        if(b <= c)
            return b;
        else
            return c;
    }
}
*/

// Calculating length(number of words) of dict
long scanLine(const char *file) {
    char command[255] = "wc -l ";
    char *output = " > ./output";
    if((strlen(file) + strlen(command) + strlen(output)) > 254) {
        printf("Filepath is too long.\n");
        exit(1);
    }
    strcat(command, file);
    strcat(command, output);
    if(system(command) != 0) {
        printf("Failed to open dictionary.\n");
        exit(1);
    }

    FILE *fp;
    if((fp = fopen("./output", "r")) == NULL) {
        printf("Failed to open ./output.\n");
        exit(1);
    }

    char tmp[STRLEN];
    char ch;
    int i = 0;
    while((ch=getc(fp)) != EOF) {
        if(ch != ' ')
            tmp[i++] = ch;
        else
            break;
    }
    fclose(fp);
    tmp[i] = '\0';

    if(strlen(tmp) == 0) {
        printf("No number in output file.\n");
        exit(1);
    }
    long res = atoi(tmp);

    return res;
}

int main(int argc, char **argv) {
    //Get dictionary file name and suggestion list length from arguments
    struct option long_options[] = {
        {"dictionary", required_argument, 0, 'd'},
        {"count", required_argument, 0, 'n'},
        {"version", no_argument, 0, 'v'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };
    int option_index = 0;
    int c;
    char *fileName = "/usr/share/dict/linux.words"; //Default dict
    int sugLen = 12; //Default suggestion list length
    char *version = "My speller checker version 1.0\n";
    char *help = "Usage: ./spellChecker\n\t[--version | -v]\n\t[--help | -h]\n\t[--dictionary | -d] dict\n\t[--count | -c] number\n";

    int k;
    while(1) {
        c = getopt_long(argc, argv, "d:n:vh", long_options, &option_index);
        if(c == -1)
            break;
        switch(c) {
            case 'd':
                fileName = optarg;
                break;
            case 'n':
                k = 0;
                while(optarg[k] != '\0') {
                    if(isdigit(optarg[k]))
                        ;
                    else {
                        printf("Invalid argument for [--count | n].\n");
                        return 1;
                    }
                    k++;
                }
                sugLen = atoi(optarg);
                break;
            case 'v':
                printf("%s", version);
                break;
            case 'h':
                printf("%s", help);
                break;
            default:
                abort();
        }
    }

    //Get dict length and hash table length, make the load factor 0.5
    long dictLen = scanLine(fileName);
    if(dictLen <= 0) {
        printf("The dictionary is empty.\n");
        return 1;
    } 
    long tableLen = 2 * dictLen;

    //printf("file: %s len: %d\n", fileName, tableLen);

    //Hash table to store dictionary
    hString hashTable[tableLen];
    
    //Initial hashtable
    initTable(hashTable, tableLen);

    //Open dictionary file
    FILE *fp = NULL;
    if((fp = fopen(fileName, "r")) == NULL) {
        printf("Failed to open dictionary %s.\n", fileName);
        exit(1);
    }

    //Read words from dict to hashtable
    int i = 0;
    char ch;
    char tmpStr[STRLEN] = {0};
    while((ch = getc(fp)) != EOF) {
        if(ch == '\n') {
            tmpStr[i] = '\0';
            if(strlen(tmpStr) > 0)
                insert(tmpStr, hashTable, tableLen);
            i = 0;
            continue;
        }
        if(i >= (STRLEN - 1)) {           //If word is longer than STRLEN, abandon it
            printf("Abandon one word.\n");
            while((ch = getc(fp)) != '\n')
                ;
            i = 0;
            continue;
        }
            
        tmpStr[i++] = ch;
    }
    fclose(fp);

    for(i = 0; i < STRLEN; i++)
        tmpStr[i] = 0;

    // Calculate edit distance and give suggestions to user
    i = 0;
    int res, diff; //diff -- Edit distance
    long j = 0;
    char *diffOne[sugLen]; //Store 1 distance words
    char *diffTwo[sugLen]; //Store 2 distance words
    int diff1; //Number of 1 distance words
    int diff2; //Number of 2 distance words

    //Read words from console
    while((ch = getchar()) != EOF) {
        if(ch == '\n') {
            tmpStr[i] = '\0';
            if(strlen(tmpStr) == 0) { //If user input blank line
                printf("<end>\n");
                continue;
            }
            if(strcmp(tmpStr, "<quit>") == 0)
                return 0;
            res = findPos(tmpStr, hashTable, tableLen);
            if(hashTable[res].len != 0) //If find the word in dict
                printf("%s\n", hashTable[res].str);
            else { //If not found, give suggestions                        
                printf("\n");
                if(sugLen == 0) { //Do not give any suggestion
                    printf("<end>\n");
                    i = 0;
                    continue;
                }
                diff1 = 0;
                diff2 = 0;
                for(j = 0; j < tableLen; j++) {
                    if(hashTable[j].len != 0) {
                        diff = levDis(tmpStr, hashTable[j].str);
                        if((diff == 1) && (diff1 < sugLen)) {
                            diffOne[diff1++] = hashTable[j].str;
                        }
                        else if((diff == 2) && (diff2 < sugLen)) {
                            diffTwo[diff2++] = hashTable[j].str;
                        }
                        
                        if((diff1 >= sugLen))
                            break;
                    }
                }
                
                int m;
                for(m = 0; m < diff1; m++)
                    printf("%s\n", diffOne[m]);
                if(diff1 < sugLen) {
                    int oneTwoGap = ((sugLen - diff1) <= diff2) ? (sugLen - diff1) : diff2;
                    int n;
                    for(n = 0; n < oneTwoGap; n++)
                        printf("%s\n", diffTwo[n]);
                }
            }
            printf("<end>\n");

            i = 0; 
            continue;
        }
  
        if(i >= STRLEN-1) { //If user input word is longer than STRLEN
            printf("Input word is too long.\n");
            while((ch = getchar()) != '\n')
                ;
            i = 0;
            continue;
        }

        tmpStr[i++] = ch;
    }
 
    deTable(hashTable, tableLen);

    return 0;
}
