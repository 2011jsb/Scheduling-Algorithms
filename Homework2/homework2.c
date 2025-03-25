#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "procdat.h"

int main(){
    ProcDat tmp[5] = {
        {.procname = "A", .priority = 0, .arrtime = 40, .jobtime = 20},
        {.procname = "B", .priority = 0, .arrtime = 12, .jobtime = 5},
        {.procname = "C", .priority = 0, .arrtime = 10, .jobtime = 10},
        {.procname = "D", .priority = 0, .arrtime = 10, .jobtime = 15},
        {.procname = "E", .priority = 0, .arrtime = 80, .jobtime = 17},
    };
    SchDat sdata = {
        .listlen = 5,
        .tmlen = 10,
        .proclist = tmp,
    };
    
    RProcDat* result = (RProcDat*)malloc(sdata.listlen * sizeof(RProcDat));;
    SRTF(&sdata,result);
    AnalDat tmp1 = analysis(result, sdata.listlen);
    printf("set points");

    free(result);
}