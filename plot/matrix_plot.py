#!/usr/bin/python3
# -*- coding: utf-8 -*-
#
# Author:  Dominik Gresch <greschd@ethz.ch>
#          Mario S. Könz <mskoenz@gmx.net>
# Date:    16.04.2014 09:37:20 CEST
# File:    performance_search.py

import os
import sys
import glob
import pickle
import xtermcolor
import subprocess
import copy

#------------------import from the addons folder------------------------#
mcpath = '/'.join(os.path.realpath(__file__).split("/")[:-2])
sys.path.insert(0, mcpath + '/addons_python/')

import build_dir
import collect_versions as co

os.chdir(build_dir.build_dir)

#-----------------------------------------------------------------------#
from matplotlib.pyplot import rc
rc('text', usetex=True) # this is if you want to use latex to print text. If you do you can create strings that go on labels or titles like this for example (with an r in front): r"$n=$ " + str(int(n))
from pylab import *
import matplotlib.pyplot as plt

rc('text', usetex=True)

rc('text.latex', preamble=r'\usepackage{cmbright}')
rc('mathtext', fontset='stixsans')

if __name__ == "__main__":
    
    T_list = [1, 5, 10, 50]
    N_list = [10, 20, 50, 100, 150, 200, 250, 300, 350, 400, 500, 750, 1000]
    
    def plot(name = "res"):
        f = open(name + ".txt", "rb")
        res = pickle.load(f)
        
        f.close()
        conf_arr = []
        
        
        for i in range(len(T_list)):
            conf_arr.append([])
            for j in range(len(N_list)):
                #~ conf_arr[i].append(int("".join([str(i) for i in res[i * len(N_list) + j][2][0]])))
                conf_arr[i].append(res[i * len(N_list) + j][2][1])
        
        print(conf_arr)
        
        fig = plt.figure()
        plt.clf()
        ax = fig.add_subplot(111)
        ax.set_aspect(1)
        
        res = ax.imshow(np.array(conf_arr), cmap=plt.cm.jet, interpolation='nearest', vmin=80, vmax=550)

        width = len(conf_arr)
        height = len(conf_arr[0])

        for x in range(width):
            for y in range(height):
                ax.annotate(str(conf_arr[x][y]), xy=(y, x)
                            , horizontalalignment='center'
                            , verticalalignment='center'
                            , fontsize = 10
                            , color = ["white", "black"][y > 6]
                            )
        cb = fig.colorbar(res)
        plt.yticks(range(width), T_list)
        plt.xticks(range(height), N_list)
        plt.xlabel(r'size N (N\textsuperscript{3} particles)')
        plt.ylabel('temperature T', rotation = "horizontal", horizontalalignment = "left", verticalalignment="top")
        ax.yaxis.set_label_coords(x = 0, y = 1.1)
        plt.gcf().set_size_inches(10, 3);
        fig.savefig("matrix_plot.pdf", dpi=250,  bbox_inches='tight')
        
    #~ run()
    #~ plot("../mc_potts/plot/mskoenz_plots/full_screen_msk")
    plot("../plot/greschd_plots/full_screen_dg")

