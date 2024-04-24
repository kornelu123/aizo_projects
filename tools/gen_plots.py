#!/bin/python

import numpy as np
import matplotlib.pyplot as plt

file = open("../report/res.txt", "r").readlines()

presort = [
    0.00,
    0.50,
    0.33
]

colors = [
    'green',
    'blue',
    'red'
]

for i in range(9):
    sizes = np.array([])
    times = np.array([])
    name = ""
    option = ""

    max_time = 0

    for j in range(3):
        for k in range(50):
            line = file[i*150 + j*50 + k]
            name = line.split("sort no")[0].strip()
            option = line.split("options:")[1].strip()
            sizes = np.append(sizes, line.split("size:")[1].split(",")[0].strip())
            times = np.append(times, float(line.split("time:")[1].split("[ms]")[0].strip()))
            if max_time < float(line.split("time:")[1].split("[ms]")[0].strip()):
                max_time = float(line.split("time:")[1].split("[ms]")[0].strip())

        plt.scatter(sizes, times, s=3, color=colors[j], label="presort:"+str(presort[j]))

    plt.legend(fontsize="7")
    plt.yticks(np.arange(0, max_time, max_time/20))

    plt.xlabel("Element count")
    plt.ylabel("Sorting time [ms]")

    plt.title(name+" sort option : "+option)
    plt.savefig(name+"_"+option)
    plt.clf()
