#!/usr/bin/python3
# -*- coding: utf-8 -*-
#
# Author:  Dominik Gresch <greschd@ethz.ch>
# Date:    22.05.2014 09:45:50 CEST
# File:    mem_traffic.py

import os
import sys
import subprocess

#------------------import from the addons folder------------------------#

mcpath = '/'.join(os.path.realpath(__file__).split("/")[:-2])
sys.path.insert(0, mcpath + '/addons_python/')

import build_dir
import collect_versions as co


#change folder so that cmake happens in the right place
os.chdir(build_dir.build_dir)

def compile_pcm_opt(T, N, full = False):
    os.chdir(build_dir.build_dir)
    subprocess.call("cmake " + mcpath   + " -DUSE_TEMP:STRING=" + str(T)
                                        + " -DUSE_LENGTH:STRING=" + str(N)
                                        , shell = True, stdout = subprocess.DEVNULL)
    if(full):
        subprocess.call("make -B perf_pcm_mem_opt_full", shell = True, stdout = subprocess.DEVNULL)
    else:
        subprocess.call("make -B perf_pcm_mem_opt", shell = True, stdout = subprocess.DEVNULL)
    
def compile_pcm(T, N, sim, rng, grid, matrix):
    os.chdir(build_dir.build_dir)
    subprocess.call("cmake " + mcpath   + " -DUSE_SIM:STRING=" + sim
                                        + " -DUSE_GRID:STRING=" + grid
                                        + " -DUSE_MATRIX:STRING=" + matrix
                                        + " -DUSE_RNG:STRING=" + rng
                                        + " -DUSE_TEMP:STRING=" + str(T)
                                        + " -DUSE_LENGTH:STRING=" + str(N)
                                        , shell = True, stdout = subprocess.DEVNULL)
    subprocess.call("make -B perf_pcm_mem", shell = True, stdout = subprocess.DEVNULL)
    
    
def run_pcm(name = "perf_pcm_mem_opt", folder = "pcm_files_opt"):
    os.chdir(build_dir.build_dir + "/performance")
    # clean out folder / create folder
    try:
        subprocess.call("rm -rf " + folder, shell = True)
    except:
        pass
    try:
        subprocess.call("mkdir " + folder, shell = True)
    except:
        pass
    os.chdir("./" + folder)
    subprocess.call("sudo ../" + name, shell = True)
    f1 = open("MC_read.txt", "r")
    read = int(f1.read())
    f1.close()
    f2 = open("MC_write.txt", "r")
    write = int(f2.read()) 
    f2.close()
    return read, write
    
def pcm_opt(T, N, full = False):
    compile_pcm_opt(T, N, full = full)
    if(full):
        return run_pcm(name = "perf_pcm_mem_opt_full")
    else:
        return run_pcm()
    
def pcm(T, N, sim, rng, grid, matrix):
    compile_pcm(T, N, sim, rng, grid, matrix)
    return run_pcm(name = "perf_pcm_mem", folder = "pcm_files")
    
def pcm_measure_opt(temp, sizes, verbose = False, full = False):
    traffic = []
    if(verbose):
        print("measuring data traffic:")
        print("sizes:")
        print(sizes)
        print("currently at:")
    for s in sizes:
        if(verbose):
            sys.stdout.write("\r" + str(s))
        traffic.append(sum(pcm_opt(temp, s, full = full)) / float(2e6))
    return sizes, traffic
    
def pcm_measure(temp, sizes, sim, rng, grid, matrix, verbose = False):
    traffic = []
    if(verbose):
        print("measuring data traffic:")
        print("sizes:")
        print(sizes)
        print("currently at:")
    for s in sizes:
        if(verbose):
            sys.stdout.write("\r" + str(s))
        traffic.append(sum(pcm(temp, s, sim, rng, grid, matrix)) / float(2e6))
    return sizes, traffic
    

if __name__ == "__main__":
    
#-----------------------------------------------------------------------#
#                       input variables                                 #
#-----------------------------------------------------------------------#
    sizes = [2, 4, 8, 10, 15, 20, 25, 30]#, 40, 50, 75, 100, 150, 200, 250, 350, 500, 600, 700, 800, 900, 1000] 
    temp = 5
#-----------------------------------------------------------------------#
    print(pcm_measure_opt(temp, sizes, verbose = True))
    print("mem_traffic.py")
    
