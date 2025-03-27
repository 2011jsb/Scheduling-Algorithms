# !usr/bin python3
# -* coding : utf-8 *-
# wrote by Ian Li, 26/3/2025

from generator import generator
from sys import argv,exit
from os import popen
import matplotlib.pyplot as plt 
import numpy as np

# const algorithm types
_ALG_T = ("FCFS", "SJF", "SRTF", "RR", "PS")
_IDX_T = {"ATT":0, "ART":1, "AWT":2, "CPU":3, "JTP":4}

def test(num:int = 20) -> list:
    generator(num)
    datalist = []
    for i in range(num):
        tmpp = []
        
        for j in range(5):
            cmmd = argv[0].rsplit('/', 2)[0]+ f"/homework2 {_ALG_T[j]} {argv[0].rsplit('/', 2)[0]}/testcases/testcase{i}.txt"
            result = popen(cmmd).readlines()
            if len(result) != 5:
                print("The output is unexcepted!")
                exit(1)
            
            tmp = tuple([float(i.split(' ')[-1][:-1]) for i in result])
            tmpp.append(tmp)
            
        datalist.append(tmpp)
        
    return datalist


def draw(result:list, indexname:str) -> None:
    index = _IDX_T[indexname]
    
    avg = [0 for i in range(5)]
    
    for i in result:
        for j in range(5):
            avg[j] += i[j][index]
    
    y = np.array([i / len(result) for i in avg])
    x = _ALG_T
    
    plt.title(f"{len(result)} Random Tests {indexname}-Algorithm Names Picture")
    plt.ylabel(f"y - Average {indexname} Values")
    plt.xlabel(f"x - Algorithm Names")
    plt.bar(x, y, width=0.2)
    plt.savefig(f"{argv[0].rsplit('/', 2)[0]}/pictures/{indexname}_{len(result)}.png")
    
    plt.close()
    
if __name__ == "__main__":
    testnums = [10, 20, 50, 100, 500, 1000]
    for i in testnums:
        result = test(i)
        for j in _IDX_T.keys():
            draw(result, j)
    