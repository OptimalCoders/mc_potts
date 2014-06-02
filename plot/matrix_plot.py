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
from matplotlib.pyplot import rc

rc('text', usetex=True)

rc('text.latex', preamble=r'\usepackage{cmbright}')
rc('mathtext', fontset='stixsans')

T_list = [1, 5, 10, 50]
N_list = [10, 20, 50, 100, 150, 200, 250, 300, 350, 400, 500, 750, 1000]

def plot(name, specs):
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
                            , fontsize = 12
                            , color = ["white", "black"][y > 6]
                            )
        cb = fig.colorbar(res)
        plt.yticks(range(width), T_list)
        plt.xticks(range(height), N_list)
        
        plt.xlabel(r'Size N (N\textsuperscript{3} Particles)', horizontalalignment = "right", x = 1, y = 0)
        plt.ylabel('Temperature T', rotation = "horizontal", horizontalalignment = "left", verticalalignment="top")
        plt.title(r"\textbf{Runtime Screening}", size=16, x = 0, y = 1.15, horizontalalignment = "left")
        ax.yaxis.set_label_coords(x = 0, y = 1.1)
        
        
        flags = '-Wall -std=c++11 -O3 -DNDEBUG -march=core-avx2'
        #~ ax.text(0,-0.55, specs + flags, bbox=dict(facecolor='white', alpha=1, boxstyle = 'round'), transform = ax.transAxes)
        plt.gcf().set_size_inches(8, 2);
        fig.savefig("matrix.pdf", dpi=250,  bbox_inches='tight')
        
def plot_modules(name, specs):
        f = open(name + ".txt", "rb")
        res = pickle.load(f)
        f.close()
        
        dg_msk_mapping = [[0, 3, 2, 1], [1, 0], [3, 2, 1, 4, 0], [0, 1, 2]]
        msk_report_mapping = [[3, 2, 3, 2, 4], [0, 1], [2, 0, 3, 1, 4], [2, 0, 1]]
        
        #~ mapping = dg_msk_mapping
        mapping = lambda idx, x: msk_report_mapping[idx][x]
        #~ mapping = lambda idx, x: msk_report_mapping[idx][dg_msk_mapping[idx][x]]
        
        def remap(m, idx):
            m = mapping(idx, m)
            return m
            
        def remap_l(m):
            for i in range(len(m)):
                m[i] = remap(m[i], i)
                
        for r in res:
            remap_l(r[2][0])
        
        #------------------- import decode list ------------------- 
        name2 = "/".join(name.split("/")[:-1]) + "/data.txt"
        f = open(name2, "r")
        mods = f.readlines()[:4]
        f.close()
        
        for i in range(len(mods)):
            mods[i] = (mods[i].split("\n")[0]).split(", ")[1:]
            mods[i] = [[mods[i][j], remap(j, i), 0] for j in range(len(mods[i]))]
            mods[i] = sorted(mods[i], key=lambda x: x[1])
        
        print(mods)
        #------------------- setup colors equidistant ------------------- 
        cnt = dict()
        
        for r in res:
            for i in range(len(r[2][0])):
                mods[i][r[2][0][i]][-1] = 1
            
            cnt["".join([str(i) for i in r[2][0]])] = 1
        
        #------------------- clear unused ------------------- 
        for i in range(len(mods)):
            for mm in mods[i]:
                if mm[-1] == 0:
                    mods[i].remove(mm)
        
        mod_text = ''
        #------------------- create mod text ------------------- 
        for i in range(len(mods)):
            for mm in mods[i]:
                mod_text += str(mm[1]) + ": " + mm[0]
                if mm != mods[i][-1]:
                    mod_text += " / "
            mod_text += "\n"
        mod_text = mod_text.replace("_", " ")
        
        i = 0
        for key in sorted(cnt.keys()):
            cnt[key] = i
            i += 1
        
        conf_arr = []
        corr_arr = []
        
        
        #------------------- prepare plot ------------------- 
        for i in range(len(T_list)):
            conf_arr.append([])
            corr_arr.append([])
            for j in range(len(N_list)):
                code = "".join([str(i) for i in res[i * len(N_list) + j][2][0]])
                conf_arr[i].append(code)
                corr_arr[i].append(cnt[code])
                #~ conf_arr[i].append(res[i * len(N_list) + j][2][1])
        
        fig = plt.figure()
        plt.clf()
        ax = fig.add_subplot(111)
        ax.set_aspect(1)
        
        res = ax.imshow(np.array(corr_arr), cmap=plt.cm.jet, interpolation='nearest')

        width = len(conf_arr)
        height = len(conf_arr[0])

        for x in range(width):
            for y in range(height):
                ax.annotate(conf_arr[x][y], xy=(y, x)
                            , horizontalalignment='center'
                            , verticalalignment='center'
                            , fontsize = 12
                            , color = ["white", "black"][(corr_arr[x][y] > len(cnt)/2.0 - 3) and (corr_arr[x][y] < (len(cnt)-1))]
                            )
        cb = fig.colorbar(res)
        plt.yticks(range(width), T_list)
        plt.xticks(range(height), N_list)
        plt.xlabel(r'Size N (N\textsuperscript{3} Particles)', horizontalalignment = "right", x = 1, y = 0)
        plt.ylabel('Temperature T', rotation = "horizontal", horizontalalignment = "left", verticalalignment="top")
        plt.title(r"\textbf{Optimal Modules}", size=16, x = 0, y = 1.15, horizontalalignment = "left")
        ax.yaxis.set_label_coords(x = 0, y = 1.1)
        
        flags = '-Wall -std=c++11 -O3 -DNDEBUG -march=core-avx2'
        
        #~ ax.text(0,-0.65, mod_text[:-1], bbox=dict(facecolor='white', alpha=1, boxstyle = 'round'), transform = ax.transAxes)
        
        fig.delaxes(fig.axes[1]) 
        plt.gcf().set_size_inches(8, 2)
        fig.savefig("module.pdf", dpi=250,  bbox_inches='tight')

if __name__ == "__main__":
    dg  = "system: Ubuntu 13.10 on Intel Core i7 (Haswell) @2.4 GHz\ncompiler: gcc v4.8.1\nflags: "
    msk = "system: Ubuntu 14.04 on Intel Core 2 (Wolfdale) @2.4 GHz\ncompiler: gcc v4.8.1\nflags: "
    #~ plot("../mc_potts/plot/mskoenz_plots/full_screen_msk", msk)
    #~ plot_modules("../mc_potts/plot/mskoenz_plots/full_screen_msk", msk)
    
    plot("../mc_potts/plot/greschd_plots/full_screen_dg", dg)
    #~ plot_modules("../mc_potts/plot/greschd_plots/full_screen_dg", dg)
    
    
