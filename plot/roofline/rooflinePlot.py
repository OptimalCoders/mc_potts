#!/usr/bin/python3
# -*- coding: utf-8 -*-
# Author:  Dominik Gresch <greschd@ethz.ch>
# Date:    23.05.2014 07:41:07 CEST
# File:    rooflinePlot.py

import os
import sys

#measurement imports:
mcpath = '/'.join(os.path.realpath(__file__).split("/")[:-3])
sys.path.insert(0, mcpath + '/addons_python/')
sys.path.insert(0, mcpath + '/performance/')

import build_dir
import runtime as ro
import mem_traffic as mt

#General imports:

import math
import pickle
import subprocess
from matplotlib.pyplot import rc
rc('text', usetex=True) # this is if you want to use latex to print text. If you do you can create strings that go on labels or titles like this for example (with an r in front): r"$n=$ " + str(int(n))
from numpy import *
from pylab import *
import random
from matplotlib.font_manager import FontProperties
import matplotlib.pyplot as plt
import matplotlib.lines as lns

def to_roofline_dir():
    os.chdir(build_dir.build_dir + "/plot/")
    try:
        subprocess.call("mkdir roofline", shell = True)
    except:
        pass
    os.chdir("./roofline")
    

def measure_run_opt(T, sizes, name = "runtime_opt", full = False):
    sizes, runtime = ro.measure_opt(T, sizes, verbose = True, full = full)

    to_roofline_dir()
    f = open(name + ".txt", "wb")
    pickle.dump([sizes, runtime], f)
    f.close()
    
def measure_mem_opt(T, sizes, name = "traffic_opt", full = False):
    sizes, traffic = mt.pcm_measure_opt(T, sizes, verbose = True, full = full)
    
    to_roofline_dir()
    f = open(name + ".txt", "wb")
    pickle.dump([sizes, traffic], f)
    f.close()
    
def measure_run(T, sizes, sim, rng, grid, matrix, name = "runtime"):
    sizes, runtime = ro.measure(T, sizes, sim, rng, grid, matrix, verbose = True)
    
    to_roofline_dir()
    f = open(name + ".txt", "wb")
    pickle.dump([sizes, runtime], f)
    f.close()
    
def measure_mem(T, sizes, sim, rng, grid, matrix, name = "traffic"):
    sizes, traffic = mt.pcm_measure(T, sizes, sim, rng, grid, matrix, verbose = True)
    
    to_roofline_dir()
    f = open(name + ".txt", "wb")
    pickle.dump([sizes, traffic], f)
    f.close()
    
def read_pickle(name_run = "runtime_opt", name_mem = "traffic_opt"):
    to_roofline_dir()
    
    f1 = open(name_run + ".txt", "rb")
    sizes_run, runtime = pickle.load(f1)
    f1.close()
    
    f2 = open(name_mem + ".txt", "rb")
    sizes_mem, traffic = pickle.load(f2)
    f2.close()
    
    #check for consistency of sizes
    if(not sizes_run == sizes_mem):
        raise ValueError("sizes for traffic and runtime do not match")
    
    return sizes_run, runtime, traffic

def plot(sizes, performance, op_intensity, labels, to_annotate):
    background_color = '#eeeeee' 
    grid_color = 'white' #FAFAF7'
    rc('axes', facecolor = background_color)
    rc('axes', edgecolor = grid_color)
    rc('axes', linewidth = 1.2)
    rc('axes', grid = True )
    rc('axes', axisbelow = True)
    rc('grid',color = grid_color)
    rc('grid',linestyle='-' )
    rc('grid',linewidth=0.7 )
    rc('xtick.major',size =0 )
    rc('xtick.minor',size =0 )
    rc('ytick.major',size =0 )
    rc('ytick.minor',size =0 )
    rc('font',**{'family':'sans-serif', 'sans-serif':['Gill Sans MT']})


    matplotlib.pylab.rc('text.latex', preamble=r'\usepackage{cmbright}')
    matplotlib.pylab.rc('mathtext', fontset='stixsans')

    X_MIN=8e-4
    X_MAX=8e-2
    Y_MIN=8e-4
    Y_MAX=8e-2
    # 
    PEAK_PERF=3.0
    PEAK_BW=5.2
    ASPECT_RATIO=0.6 * (np.log(X_MAX) - np.log(X_MIN))/(np.log(Y_MAX) - np.log(Y_MIN))
    LOG_X=True
    LOG_Y=True


    fig = plt.figure()
    # Returns the Axes instance
    ax = fig.add_subplot(111, aspect = ASPECT_RATIO)
    ax.tick_params(labelsize = 10)
    colors = ['r', 'b', 'g', 'o']

    #Log scale
    if LOG_Y: ax.set_yscale('log')
    if LOG_X: ax.set_xscale('log')

    #formatting:
    ax.set_title(r"\textbf{Roofline Plot}",fontsize=15, position = (0.5, 1.1))
    ax.set_xlabel("Inverse Memory Traffic [Steps/Byte]", fontsize=12, horizontalalignment = "right")
    ax.set_ylabel("Inverse Runtime [Steps/Cycle]", fontsize=12, rotation = "horizontal", horizontalalignment = "left")

    ax.yaxis.set_label_coords(0, 1.05)
    ax.xaxis.set_label_coords(1, -0.09)


    #~ #x-y range
    ax.axis([X_MIN,X_MAX,Y_MIN,Y_MAX])
    
    for i in range(len(performance)):
        ax.plot(op_intensity[i], performance[i], 'o-', label = labels[i], linewidth = 0.5, markersize = 2, color = colors[i])
    
    ax.legend(loc = 'lower right')


    #Percentile boxes
    #ax.boxplot((x[0,],y[0,]))

    #~ annotate_dirs = [[(-4, +20), (-4, 20)] for i in range(len(sizes))]
    #~ annotate_dirs[15][0] = (-4, -20)
    #~ annotate_dirs[15][1] = (-15, 20)
    #~ annotate_dirs[0][1] = (+4, -20)
    #~ annotate_dirs[4][1] = (-4, -20)
    #~ annotate_dirs[5][1] = (-4, -20)
    #~ annotate_dirs[9][1] = (-4, +15)

    #Anotate 
    #~ for j in range(len(performance)):
        #~ for i, s in enumerate(sizes):
            #~ if(s in to_annotate):
                #~ ax.annotate(str(s),
                         #~ xy=(op_intensity[j][i], performance[j][i]), xycoords='data',
                         #~ xytext=annotate_dirs[i][j], textcoords='offset points', fontsize=8, color = colors[j], horizontalalignment = 'bottom', verticalalignment = 'middle', arrowprops=dict(facecolor = 'black', arrowstyle = 'wedge', alpha = 0.5))

    #Peak performance line and text
    #~ ax.axhline(y=PEAK_PERF, linewidth=1, color='black')
    #~ ax.text(X_MAX/10.0, PEAK_PERF+0.15, "Peak Performance ("+str(PEAK_PERF)+" F/C)", fontsize=10)


    #BW line and text
    x = np.linspace(X_MIN, X_MAX, 100)
    y = x*PEAK_BW 
    ax.plot(x, y, linewidth=1, color='black')


    l2 = array((0.01,0.01))
    angle = np.arctan(ASPECT_RATIO) * 180 / np.pi

    ax.text(X_MIN*1.1,(X_MIN*1.1*PEAK_BW)*1.1 ,'MemLoad ('+str(PEAK_BW)+' B/C)',fontsize=10,
               rotation=angle , verticalalignment ='bottom' )

    
    flags = '-Wall -std=c++11 -O3 -DNDEBUG -march=core-avx2'
    ax.text(0,-0.3, "system: Ubuntu 13.10 on Intel Core i7 (Haswell) @2.4 GHz\ncompiler: gcc v4.8.1\nflags: " + flags, bbox=dict(facecolor='white', alpha=1, boxstyle = 'round'), transform = ax.transAxes)
    fig.subplots_adjust(bottom = 0.25, top = 0.85)


    #save file
    fig.savefig("rooflinePlot.pdf", dpi=250,  bbox_inches='tight')

if __name__ == "__main__":
#-----------------------------------------------------------------------#
#                          input variables                              #
#-----------------------------------------------------------------------#
    sizes = [5, 10, 25, 50, 100, 200, 250, 300, 350, 400, 500, 600, 700, 800, 900, 1000]
    T = 5
    #~ flops_per_step = (3.947946 + 25 * 0.657991)
    flops_per_step = 1
    measure = False
    to_annotate = [5, 100, 200, 400, 1000]
    #~ to_annotate = [300]
#-----------------------------------------------------------------------#
    to_roofline_dir()
    
    if(measure):
        measure_run_opt(T, sizes)
        measure_mem_opt(T, sizes)
        measure_run_opt(T, sizes, name = "run_full", full = True)
        measure_mem_opt(T, sizes, name = "mem_full", full = True)
        measure_run(T, sizes, "greschd_v3_sim", "mkl_mt_rng", "msk_v1_pbc", "msk_v0_c_array_dynamic", name = "run_carray")
        measure_mem(T, sizes, "greschd_v3_sim", "mkl_mt_rng", "msk_v1_pbc", "msk_v0_c_array_dynamic", name = "mem_carray")
        measure_run(T, sizes, "greschd_v3_sim", "mkl_mt_rng", "msk_v1_pbc", "msk_v2_dynamic_zip", name = "run_zip")
        measure_mem(T, sizes, "greschd_v3_sim", "mkl_mt_rng", "msk_v1_pbc", "msk_v2_dynamic_zip", name = "mem_zip")
        measure_run(T, sizes, "baseline_greschd_sim", "std_mt_rng", "baseline_greschd_grid", "baseline_greschd_matrix", name = "run_baseline")
        measure_mem(T, sizes, "baseline_greschd_sim", "std_mt_rng", "baseline_greschd_grid", "baseline_greschd_matrix", name = "mem_baseline")
        measure_run(T, sizes, "greschd_v3_sim", "mkl_mt_rng", "msk_v1_pbc", "msk_v3_zip_order", name = "run_ziporder")
        measure_mem(T, sizes, "greschd_v3_sim", "mkl_mt_rng", "msk_v1_pbc", "msk_v3_zip_order", name = "mem_ziporder")
    
    sizes = []
    runtime = []
    traffic = []
    names = []
    def read_wrapper(name, *args):
        sizes_new, runtime_new, traffic_new = read_pickle(*args)
        if(sizes == []):
            sizes.extend(sizes_new)
        else:
            if(sizes_new != sizes):
                raise ValueError("Sizes do not match")
        runtime.append(runtime_new)
        traffic.append(traffic_new)
        names.append(name)
        
    #~ read_wrapper("optimized")
    #~ read_wrapper("C array", "run_carray", "mem_carray")
    #~ read_wrapper("compressed", "run_zip", "mem_zip")
    #~ read_wrapper("baseline", "run_baseline", "mem_baseline")
    #~ read_wrapper("ziporder", "run_ziporder", "mem_ziporder")
    #~ read_wrapper("full", "run_full", "mem_full")
    performance = [[flops_per_step / time for time in rtval] for rtval in runtime]
    op_intensity = [[flops_per_step / mem for mem in trval] for trval in traffic]
    plot(sizes, performance, op_intensity, names, to_annotate)
    print("rooflinePlot.py")
    
