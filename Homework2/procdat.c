#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#include "procdat.h"
/* -- utils functions--*/
static void swap(ProcDat* left, ProcDat* right){
    ProcDat tmp = *left;
    *left = *right; 
    *right = tmp;
}

// different function use different swap && judge
static void uni_swap(ProcDat* left, ProcDat* right, const char* funcname){
    if((!strcmp(funcname, "FCFS") || !strcmp(funcname, "RR") || !strcmp(funcname, "SRTF")) && (left -> arrtime > right -> arrtime || 
        (left -> arrtime == right -> arrtime && left -> jobtime >= right -> jobtime))) {swap(left, right); return;}

    if((!strcmp(funcname, "SJF") || !strcmp(funcname, "PS")) && 
    left -> arrtime > right -> arrtime) {swap(left, right); return;}

    if(!strcmp(funcname, "nPS") && (left -> priority < right -> priority || 
        (left -> priority == right -> priority && left -> jobtime >= right -> jobtime))) {swap(left, right); return;}

    if(!strcmp(funcname, "nSJF") && (left -> jobtime > right -> jobtime || (left -> jobtime ==
         right -> jobtime && left -> arrtime > right -> arrtime))) {swap(left, right); return;}
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
        result[head].start[0] = MAX(ntime, sdata -> proclist[head].arrtime);
        result[head].end[0] = result[head].start[0] + sdata -> proclist[head].jobtime;

        ntime = result[head].end[0];

        ++head;
    }
}

void SRTF(SchDat* sdata, RProcDat *result){
    arrorder(sdata, __func__);

    for(uint i = 0; i < sdata -> listlen; ++i) {
        result[i].tpnum = 0;// init len
        result[i].procname = sdata -> proclist[i].procname;// init name
    }

    typedef struct QNode{
        uint index;
        uint leftime;
    }QNode;
    QNode* readyque = (QNode*)calloc(sdata -> listlen, sizeof(uint));

    uint head = 0, tail = 1;// head && tail ptr
    uint ntime = sdata -> proclist[0].arrtime;

    while(head != tail || tail != sdata -> listlen){
        uint index = readyque[head].index, endtime = 0;
        if(!result[index].tpnum){
            result[index].start[result[index].tpnum] = MAX(sdata -> proclist[index].arrtime, ntime);
            endtime = result[index].start[result[index].tpnum] + sdata -> proclist[index].jobtime;

            // find arrival event
            for(uint i = index + 1; i < sdata -> listlen; ++i)
                if(sdata -> proclist[i].arrtime <= endtime){
                    readyque[tail].index = i;
                    ++tail;
                }else break;
            
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
        result[head].start[0] = MAX(ntime, sdata -> proclist[head].arrtime);
        result[head].end[0] = result[head].start[0] + sdata -> proclist[head].jobtime;

        ntime = result[head].end[0];

        ++head;
    }
}

// tool functions
void printGt(RProcDat* result){

}

int main(){
    ProcDat tmp[5] = {
        {.procname = "A", .priority = 0, .arrtime = 40, .jobtime = 20},
        {.procname = "B", .priority = 0, .arrtime = 10, .jobtime = 5},
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
    RR(&sdata,result);
    printf("set points");

    free(result);
}