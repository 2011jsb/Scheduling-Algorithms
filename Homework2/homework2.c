#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "procdat.h"

int main(int argc, char **argv){
    if(argc != 3){
        printf("[usage:]\n %s <algorithm name:FCFS, SJF, SRTF, RR, PS> <input file path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }else{
        procdat_main(argv[2], argv[1]);
    }

    return 0;
}