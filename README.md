# project struct
## *Homework2*
*README.md*<br>
*utils.h* - miscs/define files<br>
*procdat.h/procdat.c* - data structutre/ algorithm functions defination/implementation<br>
*homewrok2.c* - main function<br>
## *pytest*
*generator.py* - random files generator<br>
*test.py* - main test file<br>

# Format Specification of the Input File  
***First line***: Number of processes `n` / Time slice length `tmlen` <br>  
***Next `n` lines***: Process data, arranged in the order of `arrival time` / `job time` / `priority` / `procname`, separated by spaces  
*(The `procname` should not contain characters that may split the string, e.g., spaces.)* <br>  

## Special Notes on the Input  
***1.* Some POSIX-standard extensions are used in file operations. Please use a GNU environment with a compiler that supports POSIX extensions (e.g., gcc) for execution.** <br>  
***2.* During testing, the files in the `testcases` directory are all generated using `generator.py`.** <br> 

# Explanations of Some Algorithms  
***1.*** In the implementations of FCFS, SJF, and RR, for convenience, additional structures like ready queues and waiting queues are not explicitly implemented. Instead, methods such as sorting and simulating queues directly on the array storing process information are used. Some of these approaches may not be suitable for timing simulations (e.g., in SJF, there is a risk of out-of-bounds access during timing simulations). <br> 
***2.*** In the FCFS implementation, the ready queue is arranged first by `arrival time` and then by runtime (i.e., if the arrival times are the same, processes are ordered by runtime). In SJF, the opposite is done. <br>  
***3.*** In Priority Scheduling (PS), the ready queue is arranged first by `priority` and then by runtime. Only non-preemptive PS is implemented. <br> 

# Some Shortcomings of the Project and Possible Future Improvements  
***1.*** Due to time constraints, the code lacks a simple error-handling mechanism, which could be improved in the future. <br>
***2.*** Some parts of the code are not optimized, leading to efficiency issues. <br>
***3.*** **If you identify any problems in the project, feel free to provide feedback.**
<br> 
<br>
*Portions of the README.md were AI-generated for expediency. Should any inaccuracies be found, please scrutinize and report them in detail.*<br>
