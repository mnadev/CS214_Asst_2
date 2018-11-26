import numpy as mypy
import time
import random
import sys
import os

path = os.getcwd()
path = path + "/"
i = 100

while(i>0):
    path = path + "%d" % i + "/"
    os.mkdir(path)
    i = i -1
 
    