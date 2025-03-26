# !usr/bin python3
# -* coding : utf-8 *-
# wrote by Ian Li, 26/3/2025

''' generate test files as style in README.md '''
from random import randint
from sys import argv

# generate files with name "testcase_##index(index)""
def generator(nums: int = 20):
    filepath = argv[0].rsplit('/', 2)[0]
    for i in range(nums):
        tmpfile = open(f"{filepath}/testcases/testcase{i}.txt","w+")
        listlen, tmlen = randint(10, 20), randint(5, 20)
        tmpfile.write(f"{listlen} {tmlen}\n")
        for j in range(listlen): 
            tmpfile.write(f"{randint(1, 100)} {randint(1, 100)} {randint(1,40)} {randint(100000, 999999)}\n")
        
        tmpfile.close()


if __name__ == "__main__":
    length = int(input("number of test files:"))
    generator(length)