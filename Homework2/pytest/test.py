# !usr/bin python3
# -* coding : utf-8 *-
# wrote by Ian Li, 26/3/2025

from generator import generator
from sys import argv,exit
from os import popen
import matplotlib.pyplot as plt 

# const algorithm types
_ALG_T = ("FCFS", "SJF", "SRTF", "RR", "PS")

def test(num:int = 20) -> list:
    generator(num)
    datalist = []
    for i in range(num):
        tmpp = []
        
        for j in _ALG_T:
            cmmd = argv[0].rsplit('/', 2)[0]+ f"/homework2 {j} {argv[0].rsplit('/', 2)[0]}/testcases/testcase{i}.txt"
            result = popen(cmmd).readlines()
            if len(result) != 5:
                print("The output is unexcepted!")
                exit(1)
            
            tmp = tuple([float(i.split(' ')[-1][:-2]) for i in result])
            tmpp.append(tmp)
            
        datalist.append(tmpp)
        
    return datalist


def draw(datalist:list):
    
        
if __name__ == "__main__":
    