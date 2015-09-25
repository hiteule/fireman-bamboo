#include <stdio.h>
#include <stdlib.h>
#include "bash.h"

void runBashScript(char *bashFilePath)
{
    if (getConfigDebug() == 1) {
        printf("Run bash script : %s\n", bashFilePath);
    }

    char line[LINE_BUFSIZE];
    int linenr;
    FILE *pipe;
    
    pipe = popen(bashFilePath, "r");
    if (NULL == pipe) {
        fprintf(stderr, "Can't open bash file '%s'.\n", bashFilePath);
        exit(-1);
    }

    linenr = 1;
    while (NULL != fgets(line, LINE_BUFSIZE, pipe)) {
        ++linenr;
    }
    
    pclose(pipe);
}
