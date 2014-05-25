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
import xtermcolor
import subprocess
import numpy as np
from matplotlib import pyplot as plt

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
        
#-----------------------------------------------------------------------#

if __name__ == "__main__":
    
    
    print("I, " + ', '.join(sim_versions))
    print("I, " + ', '.join(grid_versions))
    print("I, " + ', '.join(matrix_versions))
    print("I, " + ', '.join(rng_versions))
    
    #------------------- output ------------------- 
    #~ I, msk_v1_sim, greschd_v1_sim, baseline_greschd_sim
    #~ I, baseline_greschd_grid, msk_v1_pbc
    #~ I, msk_v2_dynamic_zip, msk_v0_std_vec, msk_v1_zorder, msk_v0_c_array_dynamic, baseline_greschd_matrix,msk_v0_c_array_static, msk_v2_static_zip, int2t_v01_matrix
    #~ I, mkl_mt_rng, std_mt_rng, custom_mt_rng
    
    plot = 4
    L = 1000
    max_cycles = 1000
    
    for i in range(plot):
        #------------------- combinations to plot ------------------- 
        if i == 0:
            index = [ 
                  ["msk_v0_sim", "msk_v0_pbc", "msk_v0_c_array_dynamic", "mkl_mt_rng"]
                , ["msk_v0_sim", "msk_v0_pbc", "msk_v0_c_array_dynamic", "std_mt_rng"]
                ]
            #~ continue
        elif i == 1:
            index = [ 
                  ["msk_v0_sim", "msk_v1_pbc", "msk_v0_c_array_dynamic", "mkl_mt_rng"]
                , ["msk_v0_sim", "msk_v0_pbc", "msk_v0_c_array_dynamic", "mkl_mt_rng"]
                ]
            #~ continue
        elif i == 2:
            index = [ 
                  ["msk_v1_sim", "msk_v1_pbc", "msk_v0_c_array_dynamic", "mkl_mt_rng"]
                , ["msk_v0_sim", "msk_v1_pbc", "msk_v0_c_array_dynamic", "mkl_mt_rng"]
                ]
            #~ continue
        elif i == 3:
            index = [ 
                  ["msk_v1_sim", "msk_v1_pbc", "msk_v3_zip_order", "mkl_mt_rng"]
                , ["msk_v1_sim", "msk_v1_pbc", "msk_v2_dynamic_zip", "mkl_mt_rng"]
                #~ , ["msk_v1_sim", "msk_v1_pbc", "msk_v1_zorder", "mkl_mt_rng"]
                , ["msk_v1_sim", "msk_v1_pbc", "msk_v0_c_array_dynamic", "mkl_mt_rng"]
                ]
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
        lefts = np.insert(np.cumsum(values, axis=1), 0, 0, axis=1)[:, :-1]
        orders = np.array(data_orders)
        bottoms = np.arange(len(data_orders))
        
        for name, color in zip(names, colors):
            idx = np.where(orders == name)
            value = values[idx]
            left = lefts[idx]
            
            plt.bar(left=left, height=0.8, width=value, bottom=bottoms, 
                    color=color, orientation="horizontal", label=name)
        plt.yticks(bottoms+0.4, ["\n".join(t).replace("_", " ").replace("msk", "") for t in index])
        plt.legend(loc="best", bbox_to_anchor=(1.0, 1.00))
        plt.xlim((0,max_cycles))
        plt.xlabel('cycles / single spin update')
        plt.subplots_adjust(right=0.85)
        #~ plt.show()
        plt.gcf().set_size_inches(14, 2 * len(index));
        plt.savefig("plot" + str(i) + ".png", format='png')
        plt.clf()
