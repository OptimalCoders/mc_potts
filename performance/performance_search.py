#!/usr/bin/python3
# -*- coding: utf-8 -*-
#
# Author:  Dominik Gresch <greschd@ethz.ch>
# Date:    16.04.2014 09:37:20 CEST
# File:    performance_search.py

import build_dir

import os
import sys
import glob
import subprocess

#-----------------------------------------------------------------------#
#                          collecting typenames                         #
#-----------------------------------------------------------------------#

mcpath = '/'.join(os.path.realpath(__file__).split("/")[:-2])
print(mcpath)

sim_versions = []
grid_versions = []
matrix_versions = []
rng_versions = []

def remove_if_contains(string, vector):
    return list(filter(lambda x: x.find(string) == -1, vector))


def collect_filenames(name, collector, keepout):
    for x in glob.glob(mcpath + "/src/" + name + "/*.hpp"):
        temp = x.split("/")[-1].split(".hpp")[0]
        if(temp.find(keepout) == -1):
            collector.append( x.split("/")[-1].split(".hpp")[0])
            
def collect_all(keepout):
    collect_filenames("sim", sim_versions, keepout)
    collect_filenames("grid", grid_versions, keepout)
    collect_filenames("matrix", matrix_versions, keepout)
    collect_filenames("rng", rng_versions, keepout)
    
collect_all("beta")


#-----------------------------------------------------------------------#

#change folder so that cmake happens in the right place
os.chdir(build_dir.build_dir)

def measure(sim, grid, matrix, rng, T, L, H, D):
    os.system("cmake " + mcpath + " -DUSE_SIM:STRING=" + sim
                                + " -DUSE_GRID:STRING=" + grid
                                + " -DUSE_MATRIX:STRING=" + matrix
                                + " -DUSE_RNG:STRING=" + rng
                                + " -DUSE_TEMP:STRING=" + str(T)
                                + " -DUSE_LENGTH:STRING=" + str(L)
                                + " -DUSE_HEIGHT:STRING=" + str(H)
                                + " -DUSE_DEPTH:STRING=" + str(D)
                                )
    os.system("make -B perf")
    return int(list(filter(None, str(subprocess.check_output("./performance/perf")).split(" ")))[1].split("\\x1b[0m")[0])

def measure_wrapper(idx, T, L, H, D):
    return measure(sim_versions[idx[0]], grid_versions[idx[1]], matrix_versions[idx[2]], rng_versions[idx[3]], T, L, H, D)
    
def idx_print(idx):
    print("sim: " + sim_versions[idx[0]] + "; grid: " + grid_versions[idx[1]] + "; matrix: " + matrix_versions[idx[2]] + "; rng: " + rng_versions[idx[3]])

def search_performance(T, L, H, D, num_runs):
    versions_length = [len(sim_versions), len(grid_versions), len(matrix_versions), len(rng_versions)]
    opt_idx = [0, 0, 0, 0]
    opt_runtime = measure_wrapper(opt_idx, T, L, H, D)
    conv_flag = False
    tested_versions = [opt_idx.copy()]
    
    for run in range(num_runs):
        
        if(conv_flag):
            break
        conv_flag = True
        
        for i in range(4):
            conv_flag = True
            temp_idx = opt_idx
        
            for j in list(range(versions_length[i])):
                
                # set up version
                temp_idx[i] = j
                
                # skip version if already tested
                if(temp_idx in tested_versions):
                    print("skipping:")
                    idx_print(temp_idx)
                    continue
                tested_versions.append(temp_idx.copy())
        
                # testing
                print("testing:")
                idx_print(temp_idx)
                temp_runtime = measure_wrapper(temp_idx, T, L, H, D)
                print("runtime: " + str(temp_runtime))
                
                # update opt_ variables
                if(temp_runtime < opt_runtime):
                    opt_idx = temp_idx
                    opt_runtime = temp_runtime
                    conv_flag = False
                    
    print("best estimate:")
    idx_print(opt_idx)
    print("with runtime:")
    print(opt_runtime)
    print("amongst")
    for version in tested_versions:
        idx_print(version)

search_performance(5, 32, 32, 32, 3)