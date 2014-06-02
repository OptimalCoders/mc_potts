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
import numpy as np
from matplotlib import pyplot as plt

from matplotlib.pyplot import rc
rc('text', usetex=True) # this is if you want to use latex to print text. If you do you can create strings that go on labels or titles like this for example (with an r in front): r"$n=$ " + str(int(n))
from pylab import *
import matplotlib.pyplot as plt
from matplotlib.pyplot import rc

rc('text', usetex=True)

rc('text.latex', preamble=r'\usepackage{cmbright}')
rc('mathtext', fontset='stixsans')

#------------------import from the addons folder------------------------#

mcpath = '/'.join(os.path.realpath(__file__).split("/")[:-3])
sys.path.insert(0, mcpath + '/addons_python/')

import build_dir
import collect_versions as co
import parameter as pm
from helper import *

sim_versions, grid_versions, matrix_versions, rng_versions = co.collect_all(["beta"])

#-----------------------------------------------------------------------#

#change folder so that cmake happens in the right place
os.chdir(build_dir.build_dir)

def measure_micro(idx, T, size):
    sim, grid, matrix, rng = idx
    pm.bash("cmake " + mcpath   + " -DUSE_SIM:STRING="    + sim
                                + " -DUSE_GRID:STRING="   + grid
                                + " -DUSE_MATRIX:STRING=" + matrix
                                + " -DUSE_RNG:STRING="    + rng
                                + " -DUSE_TEMP:STRING="   + str(T)
                                + " -DUSE_LENGTH:STRING=" + str(size[0])
                                + " -DUSE_HEIGHT:STRING=" + str(size[1])
                                + " -DUSE_DEPTH:STRING="  + str(size[2])
                                )
    pm.bash("make -B micro_bench_plot_impl")
    return pm.popen("./plot/micro_bench_v0/micro_bench_plot_impl")
    
def idx_print(idx):
    print(sim_versions[idx[0]] + " " + grid_versions[idx[1]] + " " + matrix_versions[idx[2]] + " " + rng_versions[idx[3]])

def screen_performance(T, L, H, D):
    temp_runtime = measure_wrapper(temp_idx, T, L, H, D)
    print(temp_runtime)
    
def run(name, L):
    plot = 4
   
    #~ L = 1000
    #~ max_cycles = 700
    #~ L = 20
    #~ max_cycles = 650
    pickle_data = []
    
    for i in range(plot):
        #------------------- combinations to plot ------------------- 
        if i == 0:
            index = [ 
                  ["msk_v0_sim", "msk_v0_pbc", "msk_v0_c_array_dynamic", "mkl_mt_rng"]
                , ["msk_v0_sim", "msk_v0_pbc", "msk_v0_c_array_dynamic", "custom_mt_rng"]
                , ["msk_v0_sim", "msk_v0_pbc", "msk_v0_c_array_dynamic", "std_mt_rng"]
                ]
            highlight = 3
            #~ continue
        elif i == 1:
            index = [ 
                  ["msk_v0_sim", "msk_v1_pbc", "msk_v0_c_array_dynamic", "mkl_mt_rng"]
                , ["msk_v0_sim", "msk_v0_pbc", "msk_v0_c_array_dynamic", "mkl_mt_rng"]
                ]
            highlight = 1
            #~ continue
        elif i == 2:
            index = [ 
                  ["msk_v1_sim", "msk_v1_pbc", "msk_v0_c_array_dynamic", "mkl_mt_rng"]
                , ["msk_v0_sim", "msk_v1_pbc", "msk_v0_c_array_dynamic", "mkl_mt_rng"]
                ]
            highlight = 0
            #~ continue
        elif i == 3:
            index = [ 
                  ["msk_v1_sim", "msk_v1_pbc", "msk_v3_zip_order", "mkl_mt_rng"]
                , ["msk_v1_sim", "msk_v1_pbc", "msk_v2_dynamic_zip", "mkl_mt_rng"]
                #~ , ["msk_v1_sim", "msk_v1_pbc", "msk_v1_zorder", "mkl_mt_rng"]
                , ["msk_v1_sim", "msk_v1_pbc", "msk_v0_c_array_dynamic", "mkl_mt_rng"]
                ]
            highlight = 2
            #~ continue
        else:
            continue
        
        res = []
        
        
        for idx in index:
            x = measure_micro(idx, .2, [L, L, L])
            
            code = str(sim_versions.index(idx[0])) + str(grid_versions.index(idx[1])) + str(matrix_versions.index(idx[2])) + str(rng_versions.index(idx[3]))
            
            x = x.decode("utf-8").split("\n")[:-1]
            key = []
            perc = []
            cyc = []
            for el in x:
                t = el.split(" ")
                key.append(t[0].replace("_", " "))
                perc.append(to_number(t[1]))
                cyc.append(to_number(t[2]))
            
            cyc_sum = sum(cyc)
            perc_dif = 100 - sum(perc)
            
            perc.append(perc_dif)
            cyc.append(cyc_sum / sum(perc) * perc_dif)
            
            key.append("rest")
            
            res.append([code, key, perc, cyc])
        
        
        data = []
        vers = []
        names = res[0][1]
        for v in res:
            vers.append(v[0])
            data.append(v[3])
            if v[1] != names:
                error("names do not match for all data")
        
        data_orders = [names] * len(data)

        colors = ["r","g","b","orange","gray"]
        
        values = np.array(data)
        bottoms = np.insert(np.cumsum(values, axis=1), 0, 0, axis=1)[:, :-1]
        orders = np.array(data_orders)
        lefts = np.arange(len(data_orders))
        
        for t in index:
            t[highlight] = "\\textbf{"+ t[highlight] + "}"
            
        pickle_data.append([index, highlight,  res, data, names, values, lefts, orders, bottoms])
    
    f = open(name + ".txt", "wb")
    pickle.dump([plot, L, pickle_data], f)
    f.close()
    
def plot(filename, modopts, labels, module_numbers, max_cycles):
    background_color = '#eeeeee' 
    grid_color = 'white' #FAFAF7'
    rc('axes', facecolor = background_color)
    rc('axes', edgecolor = grid_color)
    rc('axes', linewidth = 1.2)
    rc('axes', axisbelow = True)
    rc('grid',color = grid_color)
    rc('grid',linestyle='-' )
    rc('grid',linewidth=0.7 )
    rc('xtick.major',size =0 )
    rc('xtick.minor',size =0 )
    rc('ytick.major',size =0 )
    rc('ytick.minor',size =0 )
    rc('font',**{'family':'sans-serif', 'sans-serif':['Gill Sans MT']})
    
    f = open(filename + ".txt", "rb")
    plot, L, pickle_data = pickle.load(f)
    f.close()
    
    colors = ["#DD6666","#6688FF","#66DD66","#FFFF66","#AAAAAA"]
    
    for i in range(plot):
        index, highlight, res, data, names, values, lefts, orders, bottoms = pickle_data[i]
        modopt = modopts[i]
        part_labels = ["SIM", "RNG", "GRID/MATRIX (s.s.)", "GRID/MATRIX (n.n.)", "rest"]
        fig = plt.figure()
        ax = fig.add_subplot(111)
        lefts = lefts[::-1]
        for j, (name, color) in enumerate(zip(names, colors)):
            idx = np.where(orders == name)
            value = values[idx]
            bottom = bottoms[idx]
            plt.bar(left=lefts, height=value, width=0.5, bottom=bottom, 
                    color=color, label=part_labels[j], edgecolor = "none")
                    
        current_labels = [r"\textbf{" + labels[i][j] + "}\n(" + module_numbers[i][j] + ")" for j in range(len(index))]
        plt.xticks(lefts+0.25, current_labels, fontsize = 14)
        plt.yticks(fontsize = 14)
        plt.legend(loc="best", bbox_to_anchor=(1.0, 1.0), fontsize = 12)
        plt.ylim((0,max_cycles))
        plt.xlim(-0.2, 2.7)
        ax.yaxis.grid(True)
        ax.set_ylabel('Cycles / Single Spin Update', fontsize=14, rotation = "horizontal", horizontalalignment = "left")
        ax.yaxis.set_label_coords(0, 1.05)
        #~ plt.xlabel('Cycles / Single Spin Update')
        plt.title(r"\textbf{Module Optimization: " + modopt + ", N = " + str(L)+"}",fontsize=15, position = (0.0, 1.1), horizontalalignment = "left")
        plt.subplots_adjust(right=0.85)
        #~ plt.show()
        scale = 0.7
        plt.gcf().set_size_inches(scale * 12, scale * 2 * len(index));
        plt.savefig(filename + "_" + str(i) + ".pdf", dpi=250,  bbox_inches='tight')
        plt.clf()
        
#-----------------------------------------------------------------------#

if __name__ == "__main__":
    
    
    #~ print("I, " + ', '.join(sim_versions))
    #~ print("I, " + ', '.join(grid_versions))
    #~ print("I, " + ', '.join(matrix_versions))
    #~ print("I, " + ', '.join(rng_versions))
    
    #------------------- output ------------------- 
    #~ I, msk_v1_sim, greschd_v1_sim, baseline_greschd_sim
    #~ I, baseline_greschd_grid, msk_v1_pbc
    #~ I, msk_v2_dynamic_zip, msk_v0_std_vec, msk_v1_zorder, msk_v0_c_array_dynamic, baseline_greschd_matrix,msk_v0_c_array_static, msk_v2_static_zip, int2t_v01_matrix
    #~ I, mkl_mt_rng, std_mt_rng, custom_mt_rng
    
    run("msk_20", 20)
    #~ run("msk_300", 300)
    module_numbers = [["0012","0011","0010"],["0112","0012"],["1112","0112"],["1142","1122","1112"]]
    plot("msk_20",["RNG", "GRID", "SIM", "MATRIX"],[["economic MKL mt", "economic STL mt", "STL mt"],["boundary lookup table", "baseline"],["probability precomputation", "baseline"],["compressed Z-order", "compressed", "C array"]], module_numbers, 400)
    #~ plot("msk_300",["RNG", "GRID", "SIM", "MATRIX"],[["economic MKL mt", "economic STL mt", "STL mt"],["boundary lookup table", "baseline"],["probability precomputation", "baseline"],["compressed Z-order", "compressed", "C array"]], module_numbers, 1000)
        
