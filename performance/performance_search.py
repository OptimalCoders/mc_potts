#!/usr/bin/python3
# -*- coding: utf-8 -*-
#
# Author:  Dominik Gresch <greschd@ethz.ch>
# Date:    16.04.2014 09:37:20 CEST
# File:    performance_search.py

import os
import sys
import glob
import xtermcolor
import subprocess

#------------------import from the addons folder------------------------#

mcpath = '/'.join(os.path.realpath(__file__).split("/")[:-2])
sys.path.insert(0, mcpath + '/addons_python/')

import build_dir
import collect_versions as co


sim_versions, grid_versions, matrix_versions, rng_versions = co.collect_all([   "beta", 
                "msk_v1_sim",
                "greschd_v6_sim",
                "greschd_v5_sim",
                "greschd_v1_sim",
                "msk_v0_sim",
                "msk_v1_sim",
                "std_mt_rng",
                "baseline_greschd_grid",
                "baseline_greschd_matrix",
                "msk_v0_std_vec",
                "static"
                ])

#-----------------------------------------------------------------------#

#change folder so that cmake happens in the right place
os.chdir(build_dir.build_dir)

def measure(sim, grid, matrix, rng, T, L, H, D):
    subprocess.call("cmake " + mcpath   + " -DUSE_SIM:STRING=" + sim
                                        + " -DUSE_GRID:STRING=" + grid
                                        + " -DUSE_MATRIX:STRING=" + matrix
                                        + " -DUSE_RNG:STRING=" + rng
                                        + " -DUSE_TEMP:STRING=" + str(T)
                                        + " -DUSE_LENGTH:STRING=" + str(L)
                                        + " -DUSE_HEIGHT:STRING=" + str(H)
                                        + " -DUSE_DEPTH:STRING=" + str(D)
                                        , shell = True, stdout = subprocess.DEVNULL)
    subprocess.call("make -B perf", shell = True, stdout = subprocess.DEVNULL)
    return int(list(filter(None, str(subprocess.check_output("./performance/perf")).split(" ")))[1].split("\\x1b[0m")[0])

def measure_wrapper(idx, T, L, H, D):
    return measure(sim_versions[idx[0]], grid_versions[idx[1]], matrix_versions[idx[2]], rng_versions[idx[3]], T, L, H, D)
    
def idx_print(idx):
    print("sim: " + sim_versions[idx[0]] + "; grid: " + grid_versions[idx[1]] + "; matrix: " + matrix_versions[idx[2]] + "; rng: " + rng_versions[idx[3]])

#-----------------------------------------------------------------------#
#              searches for best-performing config                      #
#-----------------------------------------------------------------------#
#   T:          temperature
#   L/D/H:      dimensions
#   num_runs:   max. number of iterations in the search (# times each tpl-argument is optimized)

def search_performance(T, L, H, D, num_runs, verbose = True):
    versions_length = [len(sim_versions), len(grid_versions), len(matrix_versions), len(rng_versions)]
    opt_idx = [0, 0, 0, 0]
    
    # initial test
    if(verbose):
        print(xtermcolor.colorize("testing:", ansi = 46))
        idx_print(opt_idx)
    opt_runtime = measure_wrapper(opt_idx, T, L, H, D)
    if(verbose):
        print(xtermcolor.colorize("runtime: " + str(opt_runtime), rgb = 0x0080FF))
    
    
    conv_flag = False
    tested_versions = [opt_idx.copy()]
    
    for run in range(num_runs):
        
        if(conv_flag):
            if(verbose):
                print(xtermcolor.colorize("search converged!", rgb = 0xFFFF00))
            break
        conv_flag = True
        
        for i in range(4):
            temp_idx = opt_idx.copy()
        
            for j in list(range(versions_length[i])):
                
                # set up version
                temp_idx[i] = j
                
                # skip version if already tested
                if(temp_idx in tested_versions):
                    if(verbose):
                        print(xtermcolor.colorize("skipping:", ansi = 196))
                        idx_print(temp_idx)
                    continue
                tested_versions.append(temp_idx.copy())
        
                # testing
                if(verbose):
                    print(xtermcolor.colorize("testing:", ansi = 46))
                    idx_print(temp_idx)
                temp_runtime = measure_wrapper(temp_idx, T, L, H, D)
                if(verbose):
                    print(xtermcolor.colorize("runtime: " + str(temp_runtime), rgb = 0x0080FF))
                
                # update opt_ variables
                if(temp_runtime < opt_runtime):
                    opt_idx = temp_idx.copy()
                    opt_runtime = temp_runtime
                    conv_flag = False
    if(verbose):                
        print(xtermcolor.colorize("best estimate:", rgb = 0x00FFFF))
        idx_print(opt_idx)
        print(xtermcolor.colorize("with runtime:", rgb = 0x00FFFF))
        print(opt_runtime)
        print("amongst")
        for version in tested_versions:
            idx_print(version)
    
    return [sim_versions[opt_idx[0]], grid_versions[opt_idx[1]],  matrix_versions[opt_idx[2]], rng_versions[opt_idx[3]]], opt_runtime
        
        
#-----------------------------------------------------------------------#

if __name__ == "__main__":
    search_performance(5, 240, 240, 240, 3)
    

