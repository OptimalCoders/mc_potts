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

#------------------import from the addons folder------------------------#

mcpath = '/'.join(os.path.realpath(__file__).split("/")[:-2])
sys.path.insert(0, mcpath + '/addons_python/')

import build_dir
import collect_versions as co


sim_versions, grid_versions, matrix_versions, rng_versions = co.collect_all(["beta"
                                                                            , "std_mt_rng"
                                                                            , "msk_v0_c_array_static"
                                                                            , "msk_v2_static_zip"
                                                                            , "msk_v0_std_vec"
                                                                            , "greschd_v2_sim"
                                                                            , "msk_v0_sim"
                                                                            , "msk_v1_sim"])
                                                                            
print(sim_versions)

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
    subprocess.call("make -B screen", shell = True, stdout = subprocess.DEVNULL)
    return int(list(filter(None, str(subprocess.check_output("./performance/screen")).split(" ")))[1].split("\\x1b[0m")[0])

def measure_wrapper(idx, T, L, H, D):
    return measure(sim_versions[idx[0]], grid_versions[idx[1]], matrix_versions[idx[2]], rng_versions[idx[3]], T, L, H, D)
    
def idx_print(idx):
    print(sim_versions[idx[0]] + " " + grid_versions[idx[1]] + " " + matrix_versions[idx[2]] + " " + rng_versions[idx[3]])

#-----------------------------------------------------------------------#
#              searches for best-performing config                      #
#-----------------------------------------------------------------------#
#   T:          temperature
#   L/D/H:      dimensions
#   num_runs:   max. number of iterations in the search (# times each tpl-argument is optimized)

def screen_performance(T, L, H, D):
    versions_length = [len(sim_versions), len(grid_versions), len(matrix_versions), len(rng_versions)]
    opt_idx = [0, 0, 0, 0]
    temp_idx = [0, 0, 0, 0]
    
    opt_runtime = 10000000000.0;
    
    for i in list(range(versions_length[0])):
        temp_idx[0] = i
        for j in list(range(versions_length[1])):
            temp_idx[1] = j
            for k in list(range(versions_length[2])):
                temp_idx[2] = k
                for l in list(range(versions_length[3])):
                    temp_idx[3] = l
                    
                    temp_runtime = measure_wrapper(temp_idx, T, L, H, D)
                    print("A, " + ''.join([str(x) for x in temp_idx]) + ", " + str(N) + ", " + str(T) + ", " + str(temp_runtime))
                    
                    # update opt_ variables
                    if(temp_runtime < opt_runtime):
                        opt_idx = temp_idx.copy()
                        opt_runtime = temp_runtime
                
    print("B, " + ''.join([str(x) for x in opt_idx]) + ", " + str(N) + ", " + str(T) + ", " + str(opt_runtime))
        
#-----------------------------------------------------------------------#

if __name__ == "__main__":
    
    T_list = [1, 5, 10, 50, 100]
    N_list = [2, 4, 8, 15, 30, 60, 120, 240]
    
    print("I, " + ', '.join(sim_versions))
    print("I, " + ', '.join(grid_versions))
    print("I, " + ', '.join(matrix_versions))
    print("I, " + ', '.join(rng_versions))
    
    for T in T_list:
        for N in N_list:
            screen_performance(T, N, N, N)
    

