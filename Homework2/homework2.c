#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "procdat.h"

int main(int argc, char **argv){
    if(argc != 2){
        printf("[usage:]\n %s <input file path> ...", argv[0]);
        exit(EXIT_FAILURE);
    }else{
        SchDat sdata; 
        RProcDat* result = (RProcDat*) calloc(sdata.listlen, sizeof(RProcDat));
        get_from_file(argv[1], &sdata);
        FCFS(&sdata, result);
        AnalDat anldata = analysis(result, sdata.listlen);
        print(AnalDat, anldata);
        free(result);
    }
    return 0;
}