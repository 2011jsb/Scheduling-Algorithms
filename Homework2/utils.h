#ifndef UTILS_H // some utils
#define UTILS_H

#define uint unsigned int // uint -> unsigned int

#define boole unsigned short // define bool type, also can use stdbool.h
#define true 1
#define false 0

#define PRE true
#define N_PRE false

#define swap(type, A, B) swap_##type(A, B) 
#define print(type, A) print_##type(A)

#define MAX(A, B) ((A) >= (B) ? (A) : (B))
#endif