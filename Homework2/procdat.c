#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#include "procdat.h"
/* -- utils functions--*/
static void swap_ProcDat(ProcDat* left, ProcDat* right){
    ProcDat tmp = *left;
    *left = *right; 
    *right = tmp;
}

static void swap_RProcDat(RProcDat* left, RProcDat* right){
    RProcDat tmp = *left;
    *left = *right; 
    *right = tmp;
}

static void swap_uint(uint* left, uint* right){
    uint tmp = *left;
    *left = *right; 
    *right = tmp;
}

// different function use different swap && judge
static void uni_swap(ProcDat* left, ProcDat* right, const char* funcname){
    if((!strcmp(funcname, "FCFS") || !strcmp(funcname, "RR") || !strcmp(funcname, "SRTF")) && (left -> arrtime > right -> arrtime || 
        (left -> arrtime == right -> arrtime && left -> jobtime >= right -> jobtime))) {swap(ProcDat, left, right); return;}

    if((!strcmp(funcname, "SJF") || !strcmp(funcname, "PS")) && 
    left -> arrtime > right -> arrtime) {swap(ProcDat, left, right); return;}

    if(!strcmp(funcname, "nPS") && (left -> priority < right -> priority || 
        (left -> priority == right -> priority && left -> jobtime >= right -> jobtime))) {swap(ProcDat, left, right); return;}

    if(!strcmp(funcname, "nSJF") && (left -> jobtime > right -> jobtime || (left -> jobtime ==
         right -> jobtime && left -> arrtime > right -> arrtime))) {swap(ProcDat, left, right); return;}
}

static void arrorder(SchDat* sdata, const char* funcname){
    for(uint i = 0; i < sdata -> listlen; ++i)
        for(uint j = sdata -> listlen - 1; j > i; j--)
            uni_swap(&sdata -> proclist[i], &sdata -> proclist[j], funcname);
}
/* -- utils functions--*/

void FCFS(SchDat *sdata, RProcDat *result){
    arrorder(sdata, __func__);

    uint ntime = sdata -> proclist[0].arrtime;

    for(uint i = 0; i < sdata -> listlen; ++i){
        result[i].tpnum = 1; 
        result[i].procname = sdata -> proclist[i].procname;
        result[i].arrtime = sdata -> proclist[i].arrtime;
        result[i].jobtime = sdata -> proclist[i].jobtime;
        result[i].start[0] = MAX(ntime, sdata -> proclist[i].arrtime);
        result[i].end[0] = result[i].start[0] + sdata -> proclist[i].jobtime;

        ntime = result[i].end[0];
    }
}

void SJF(SchDat* sdata, RProcDat *result){
    arrorder(sdata, __func__);

    uint tail = 0, head = 0;// head && tail ptr
    uint ntime = 0;// now time

    while(head != tail || tail != sdata -> listlen){
        if(head == tail) ntime = sdata -> proclist[head].arrtime;

        if(sdata -> proclist[tail].arrtime <= ntime && tail != sdata -> listlen){
            while(sdata -> proclist[tail].arrtime <= ntime && tail != sdata -> listlen) ++tail;

            for(uint i = head; i < tail; ++i)
                for(uint j = tail - 1; j > i; --j)
                    uni_swap(&sdata -> proclist[i], &sdata -> proclist[j], "nSJF");
        }// opt:if add none, pass sort

        result[head].tpnum = 1; 
        result[head].procname = sdata -> proclist[head].procname;
        result[head].arrtime = sdata -> proclist[head].arrtime;
        result[head].jobtime = sdata -> proclist[head].jobtime;
        result[head].start[0] = MAX(ntime, sdata -> proclist[head].arrtime);
        result[head].end[0] = result[head].start[0] + sdata -> proclist[head].jobtime;

        ntime = result[head].end[0];

        ++head;
    }
}

void SRTF(SchDat* sdata, RProcDat *result){
    arrorder(sdata, __func__);

    uint* leftime =  (uint*)malloc(sdata -> listlen * sizeof(uint));// the left job time
    for(uint i = 0; i < sdata -> listlen; ++i){
        leftime[i] = sdata -> proclist[i].jobtime; // init left list
        result[i].tpnum = 0;// init len
        result[i].procname = sdata -> proclist[i].procname;// init name
        result[i].arrtime = sdata -> proclist[i].arrtime;// init arrival time
        result[i].jobtime = sdata -> proclist[i].jobtime;// init job time
    }

    uint head = 0, tail= 0; // head && tail ptr
    uint ntime = 0;

    while(head != tail || tail != sdata -> listlen){
        if(head == tail){
            while(sdata -> proclist[tail].arrtime == sdata -> proclist[head].arrtime) ++tail;
            ntime = sdata -> proclist[head].arrtime;// jump the time gap
        }else{
            // sort the list
            for(uint i = head; i < tail; ++i){
                for(uint j = tail - 1; j > i; --j){
                    if(leftime[i] > leftime[j] || (leftime[i] == leftime[j] && 
                    sdata -> proclist[i].arrtime >= sdata -> proclist[j].arrtime)){
                        swap(ProcDat, &sdata -> proclist[i], & sdata -> proclist[j]);
                        swap(RProcDat, &result[i], &result[j]);
                        swap(uint, &leftime[i], &leftime[j]);
                    }
                }
            }

            uint nindex = result[head].tpnum; // the run time period index

            // new run time period
            result[head].start[nindex] = ntime;

            // test the next arrival time is enough
            // because of the list already sorted by arrorder
            if(tail != sdata -> listlen){
                if(sdata -> proclist[tail].arrtime < result[head].start[nindex] + 
                    leftime[head]){
                    // if there's arrival events in run time, abort and finish this time period
                    // change the ready queue as well
                    result[head].end[nindex] = sdata -> proclist[tail].arrtime;
                    leftime[head] -= sdata -> proclist[tail].arrtime - result[head].start[nindex];
                    ++ tail;

                }else{
                    result[head].end[nindex] = result[head].start[nindex] + leftime[head];
                    leftime[head] = 0;
                }
            }else{
                result[head].end[nindex] = result[head].start[nindex] + leftime[head];
                leftime[head] = 0;
            }

            ntime = result[head].end[nindex];// change ntime

            ++result[head].tpnum;

            if(!leftime[head]) ++head;// pop finished proc
        }
    }
}

void RR(SchDat* sdata, RProcDat *result){
    arrorder(sdata, __func__);

    boole *eflags = (boole*) calloc(sdata -> listlen, sizeof(boole));// eflags, = true if proc is done
    uint head = 0, cnt = sdata -> listlen;// head in circular que && left counts
    uint ntime = sdata -> proclist[0].arrtime;// now time

    for(uint i = 0; i < sdata -> listlen; ++i) {
        result[i].tpnum = 0;// init len
        result[i].procname = sdata -> proclist[i].procname;// init name
        result[i].arrtime = sdata -> proclist[i].arrtime;// init arrival time
        result[i].jobtime = sdata -> proclist[i].jobtime;// init job time
    }

    while(cnt){
        result[head].start[result[head].tpnum] = MAX(ntime, sdata -> proclist[head].arrtime);
        if(sdata -> proclist[head].jobtime > (result[head].tpnum + 1) * sdata -> tmlen)
            result[head].end[result[head].tpnum] = result[head].start[result[head].tpnum] + sdata -> tmlen;
        else{
            result[head].end[result[head].tpnum] = result[head].start[result[head].tpnum] + 
                sdata -> proclist[head].jobtime - result[head].tpnum * sdata -> tmlen;
            eflags[head] = true;
            --cnt;
        }
        ntime = result[head].start[result[head].tpnum] + sdata -> tmlen;

        ++result[head].tpnum;
        uint nhead = head;

        do{
            head = (head + 1) % sdata -> listlen;

            if(head == nhead){// if none of procs in ready que , find and wait for the next proc ready
                while(eflags[head] && cnt) head = (head + 1) % sdata -> listlen;
                break;
            }
        }while((eflags[head] || sdata -> proclist[head].arrtime > ntime) && cnt);// find the next proc
    }
}

void PS(SchDat* sdata, RProcDat *result){
    arrorder(sdata, __func__);

    uint tail = 0, head = 0;// head && tail ptr
    uint ntime = 0;// now time

    while(head != tail || tail != sdata -> listlen){
        if(head == tail) ntime = sdata -> proclist[head].arrtime;

        if(sdata -> proclist[tail].arrtime <= ntime && tail != sdata -> listlen){
            while(sdata -> proclist[tail].arrtime <= ntime && tail != sdata -> listlen) ++tail;

            for(uint i = head; i < tail; ++i)
                for(uint j = tail - 1; j > i; --j)
                    uni_swap(&sdata -> proclist[i], &sdata -> proclist[j], "nPS");
            

        }// opt:if add none, pass sort

        result[head].tpnum = 1; 
        result[head].procname = sdata -> proclist[head].procname;
        result[head].arrtime = sdata -> proclist[head].arrtime;
        result[head].jobtime = sdata -> proclist[head].jobtime;
        result[head].start[0] = MAX(ntime, sdata -> proclist[head].arrtime);
        result[head].end[0] = result[head].start[0] + sdata -> proclist[head].jobtime;

        ntime = result[head].end[0];

        ++head;
    }
}

// tool functions
AnalDat analysis(RProcDat* result, uint length){
    AnalDat tmp;
    uint AllTtime = 0, AllRtime = 0, AllWtime = 0, Alltime = 0, AllJobtime = 0;
    for(uint i = 0; i < length; ++i){
        uint burstime = result[i].end[result[i].tpnum - 1] - result[i].arrtime;
        AllTtime += burstime;
        AllRtime += result[i].start[0] - result[i].arrtime;
        AllWtime += burstime - result[i].jobtime;
        AllJobtime += result[i].jobtime;
        Alltime = MAX(Alltime, result[i].end[result[i].tpnum - 1]);
    }

    tmp.AvgTtime = (double)AllTtime / length ;
    tmp.AvgRtime = (double)AllRtime / length ;
    tmp.AvgWtime = (double)AllWtime / length ;
    tmp.CPUtil = (double)AllJobtime / Alltime;
    tmp.JobTput = (double)length / Alltime;

    return tmp;
}

void print_AnalDat(AnalDat analdata){
    printf("Average Turnaround Time: %0.6lf\n", analdata.AvgTtime);
    printf("Average Response Time: %0.6lf\n", analdata.AvgRtime);
    printf("Average Waiting Time: %0.6lf\n", analdata.AvgWtime);
    printf("CPU Utilization: %0.6lf\n", analdata.CPUtil);
    printf("Job Throughput: %0.6lf\n", analdata.JobTput);
}

void print_RProcDat(RProcDat rprocdata){
    printf("procname: %s\n", rprocdata.procname);
    printf("arrtime: %u\n", rprocdata.arrtime);
    printf("time period numbers: %u\n", rprocdata.tpnum);
    printf("time periods: \n");
    for(uint j = 0; j < rprocdata.tpnum; ++j)
        printf("%u-%u ", rprocdata.start[j], rprocdata.end[j]);
    printf("\n");
}

boole get_from_file(char * filepath, SchDat* sdata){

}