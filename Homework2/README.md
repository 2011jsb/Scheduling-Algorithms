# project struct
## Homework2
- README.md
- 杂项文件 utils.h
- 算法与数据结构头文件 procdat.h, 实现文件 procdat.c
- 主程序文件 homewrok2.c
## pytest
- python自动化测试文件 test.py 与 随机测试文件生成器 generator.py

# 输入文件的格式说明
- 第一行：进程数量n/ 时间片长度 tmlen
- 接下来n行，进程数据，按照 arrival time /job time /priority /procname的顺序排列，以空格间隔
- procname中应不包含空格等会分割字符串的字符
## 对输入的特殊说明
- 在文件操作中使用了一些POSIX标准的扩展，请在GNU环境中使用gcc等支持POSIX的扩展的编译器执行
- 在测试过程中, testcases中的文件均采用generator.py生成

# 一些算法的说明
- 在FCFS、SJF 、RR的实现中，出于方便考虑并未额外实现就绪队列和等待队列等结构，而是采用了排序和在存储进程信息的数组上实现队列等方式来代替。这些方式有些可能并不适合时序仿真（如在SJF中，若进行时序仿真，访问有越界风险）；
- 在FCFS的实现中，就绪队列按先arrival time后运行时间排列（即若arrival time相同，则按运行时间长短安排就绪队列），在SJF中则反之；
- 在Priorities Scheduling中，就绪队列按先Priority后运行时间排列, 且仅实现了非抢占式PS;

# 项目的一些缺点和后续可能的改进
- 由于时间等因素，代码没有加入简单的错误处理机制，后续可以进行改进；
- 部分代码的实现并没有进行优化，效率上存在一定问题；