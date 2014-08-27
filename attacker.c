#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX 256

#define NELEMS(x) ((sizeof(x)) / (sizeof((x)[0])))

char *prog = "./spellChecker";

int main(int argc, char **argv) {
    int i;
    int retval;
    char cmd[MAX];

    char *args[] = {
        "", "-v", "--version", "-h", "--help",
        "-n 3", "--count 5", "-d ./myDict", "--dictionary ./myDict",
        "-v -h -n 3 -d ./myDict",
        "-n 0", "-n 3a",
        "-d ./emptyDict", "-d abcdefg",
        "-n", "-d"
    };

    for(i = 0; i < NELEMS(args); i++) {
        snprintf(cmd, sizeof(cmd), "%s %s", prog, args[i]);
        fprintf(stdout, "\nEexcuting: %s\n\n", cmd);
        retval = system(cmd);
        fprintf(stdout, "\n\nReturn value: %d (%s)\n", retval, strerror(retval));
    }
    
    return 0;
}
