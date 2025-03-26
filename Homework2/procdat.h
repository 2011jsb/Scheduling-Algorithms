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
    uint arrtime, jobtime; // help analysis
}RProcDat;

// scheduler data
typedef struct SchDat{
    ProcDat* proclist;// process data list
    uint listlen;// list length
    uint tmlen;// time period length
}SchDat;

// analysis data
typedef struct AnalDat{
    double AvgTtime;// Average Turnaround Time
    double AvgRtime;// Average Response Time
    double AvgWtime;// Average Waiting Time
    double CPUtil;// CPU Utilization
    double JobTput;// Job Throughput
}AnalDat;

//tool functions
AnalDat analysis(RProcDat*, uint);
void print_Analdat(AnalDat);
void print_RProcDat(RProcDat);
boole get_from_file(char*, SchDat*);

// scheduler functions
void FCFS(SchDat*, RProcDat*);
void SJF(SchDat*, RProcDat*);
void SRTF(SchDat*, RProcDat*);
void RR(SchDat*, RProcDat*);
void PS(SchDat*, RProcDat*);
#endif