import numpy as mypy
import threading
import time
import random
import sys



with open('movie.csv', 'r') as maincsv:
    lines = maincsv.readlines()
i = 100
while(i>0):
    outputFile = open("file%d.csv" % i, 'w')
    outputFile.write(lines[0])
    j = 50
    while(j>0):
        outputFile.write(lines[random.randint(2,5000)])
        j = j-1
    i = i - 1
    outputFile.close
 
    