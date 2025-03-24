#ifndef PROCDAT_H
#define PROCDAT_H
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
// process data
typedef struct ProcDat{
    char* procname;//pid
    uint arrtime, jobtime;// arrive time and job time
    uint priority;
}ProcDat;

// process runtime data
typedef struct RProcDat{
    char *procname;
    uint start[256], end[256]; // time period start time and end time 
    uint tpnum;// time period num
}RProcDat;

// scheduler data
typedef struct SchDat{
    ProcDat* proclist;// process data list
    uint listlen;// list length
    uint tmlen;// time period length
}SchDat;

//tool functions
void printGt(RProcDat*);

// scheduler functions
void FCFS(SchDat*, RProcDat*);
void SJF(SchDat*, RProcDat*);
void SRTF(SchDat*, RProcDat*);
void RR(SchDat*, RProcDat*);
void PS(SchDat*, RProcDat*);
#endif