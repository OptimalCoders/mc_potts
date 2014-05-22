#!/usr/bin/python3
# -*- coding: utf-8 -*-

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
    

def measure_run_opt(T, sizes, name = "runtime_opt"):
    sizes, runtime = ro.measure_opt(T, sizes)

    to_roofline_dir()
    f = open(name + ".txt", "wb")
    pickle.dump([sizes, runtime], f)
    f.close()
    
def measure_mem_opt(T, sizes, name = "traffic_opt"):
    sizes, traffic = mt.pcm_measure_opt(T, sizes, verbose = True)
    
    to_roofline_dir()
    f = open(name + ".txt", "wb")
    pickle.dump([sizes, traffic], f)
    f.close()
    
def measure_run(T, sizes, sim, rng, grid, matrix, name = "runtime"):
    sizes, runtime = ro.measure(T, sizes, sim, rng, grid, matrix)
    
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

def plot(sizes, performance, op_intensity, labels):
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

    X_MIN=0.01
    X_MAX=1
    Y_MIN=0.01
    Y_MAX=5.0
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

    #Log scale
    if LOG_Y: ax.set_yscale('log')
    if LOG_X: ax.set_xscale('log')

    #formatting:
    ax.set_title(r"\textbf{Roofline Plot}",fontsize=15, position = (0.5, 1.1))
    ax.set_xlabel("Operational Intensity [Flop/Byte]", fontsize=12, horizontalalignment = "right")
    ax.set_ylabel("Performance [Flop/Cycle]", fontsize=12, rotation = "horizontal", horizontalalignment = "left")

    ax.yaxis.set_label_coords(0, 1.05)
    ax.xaxis.set_label_coords(1, -0.09)


    #~ #x-y range
    ax.axis([X_MIN,X_MAX,Y_MIN,Y_MAX])
    
    for i in range(len(performance)):
        ax.plot(performance[i], op_intensity[i], 'o-', label = labels[i])
    
    ax.legend(loc = 'lower right')


    #Percentile boxes
    #ax.boxplot((x[0,],y[0,]))


    #Anotate 
    for j in range(len(performance)):
        for i, s in enumerate(sizes):
            ax.annotate(str(s),
                     xy=(performance[j][i], op_intensity[j][i]), xycoords='data',
                     xytext=(+6, -3), textcoords='offset points', fontsize=10)

    #Peak performance line and text
    ax.axhline(y=PEAK_PERF, linewidth=1, color='black')
    ax.text(X_MAX/10.0, PEAK_PERF+0.15, "Peak Performance ("+str(PEAK_PERF)+" F/C)", fontsize=10)


    #BW line and text
    x = np.linspace(X_MIN, X_MAX, 100)
    y = x*PEAK_BW 
    ax.plot(x, y, linewidth=1, color='black')


    l2 = array((0.01,0.01))
    angle = 45*(ASPECT_RATIO+0.4)
    trans_angle = gca().transData.transform_angles(array((angle,)),
                                                   l2.reshape((1,2)))[0]

    ax.text(X_MIN,X_MIN*PEAK_BW + 0.09 ,'MemLoad ('+str(PEAK_BW)+' B/C)',fontsize=10,
               rotation=trans_angle)



    #save file
    fig.savefig("rooflinePlot.pdf", dpi=250,  bbox_inches='tight')

if __name__ == "__main__":
#-----------------------------------------------------------------------#
#                          input variables                              #
#-----------------------------------------------------------------------#
    sizes = [2, 4, 10, 20, 30, 50, 100, 200, 300, 400, 500]
    T = 5
    flops_per_step = 3.947946 + 25 * 0.657991
    measure = True
#-----------------------------------------------------------------------#
    to_roofline_dir()
    if(measure):
        measure_run_opt(T, sizes)
        measure_mem_opt(T, sizes)
        measure_run(T, sizes, "greschd_v3_sim", "mkl_mt_rng", "msk_v1_pbc", "msk_v0_c_array_dynamic", name = "run_carray")
        measure_mem(T, sizes, "greschd_v3_sim", "mkl_mt_rng", "msk_v1_pbc", "msk_v0_c_array_dynamic", name = "mem_carray")
        measure_run(T, sizes, "greschd_v3_sim", "mkl_mt_rng", "msk_v1_pbc", "msk_v2_dynamic_zip", name = "run_zip")
        measure_mem(T, sizes, "greschd_v3_sim", "mkl_mt_rng", "msk_v1_pbc", "msk_v2_dynamic_zip", name = "mem_zip")
    sizes1, runtime1, traffic1 = read_pickle()
    sizes2, runtime2, traffic2 = read_pickle("run_carray", "mem_carray")
    sizes3, runtime3, traffic3 = read_pickle("run_zip", "mem_zip")
    performance = [[flops_per_step / time for time in runtime] for runtime in [runtime1, runtime2, runtime3]]
    op_intensity = [[flops_per_step / mem for mem in traffic] for traffic in [traffic1, traffic2, traffic3]]
    plot(sizes, performance, op_intensity, ["opt", "carray", "zip"])
    print("rooflinePlot.py")
    
