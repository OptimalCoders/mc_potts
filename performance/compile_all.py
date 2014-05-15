#!/usr/bin/python3
# -*- coding: utf-8 -*-
#
# Author:  Dominik Gresch <greschd@ethz.ch>
# Date:    15.05.2014 13:13:44 CEST
# File:    compile_all.py

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

# many versions are in keepout to avoid massive file spawning

sim_versions, grid_versions, matrix_versions, rng_versions = co.collect_all(["beta"
                                                                            , "msk_v0_sim"
                                                                            , "msk_v1_sim"
                                                                            , "greschd_v1_sim"
                                                                            , "greschd_v2_sim"
                                                                            , "baseline_greschd_sim"
                                                                            , "msk_v0_std_vec"
                                                                            , "int2t_v01_matrix"
                                                                            , "msk_v0_c_array_static"
                                                                            , "baseline_greschd_matrix"
                                                                            , "std_mt_rng"
                                                                            , "custom_mt_rng"
                                                                            ])
#-----------------------------------------------------------------------#

#change folder so that cmake happens in the right place
os.chdir(build_dir.build_dir)

def compile_version(idx, T, L, H, D):
    subprocess.call("cmake " + mcpath   + " -DUSE_SIM:STRING=" + sim_versions[idx[0]]
                                        + " -DUSE_GRID:STRING=" + grid_versions[idx[1]]
                                        + " -DUSE_MATRIX:STRING=" + matrix_versions[idx[2]]
                                        + " -DUSE_RNG:STRING=" + rng_versions[idx[3]]
                                        + " -DUSE_TEMP:STRING=" + str(T)
                                        + " -DUSE_LENGTH:STRING=" + str(L)
                                        + " -DUSE_HEIGHT:STRING=" + str(H)
                                        + " -DUSE_DEPTH:STRING=" + str(D)
                                        , shell = True, stdout = subprocess.DEVNULL)
    subprocess.call("make -B perf_raw", shell = True, stdout = subprocess.DEVNULL)
    subprocess.call("mv " + build_dir.build_dir + "/performance/perf_raw " + build_dir.build_dir + "/performance/compiled/" + idx_string(idx), shell = True)

def idx_string(idx):
    return "".join([str(val) for val in idx])

#-----------------------------------------------------------------------#
#                       compiles all versions                           #
#-----------------------------------------------------------------------#
def compile_single(sim, grid, matrix, rng, T, L, H, D, name):
    try:
        subprocess.call("mkdir " + build_dir.build_dir + "/performance/compiled", shell = True)
    except:
        pass
    subprocess.call("cmake " + mcpath   + " -DUSE_SIM:STRING=" + sim
                                        + " -DUSE_GRID:STRING=" + grid
                                        + " -DUSE_MATRIX:STRING=" + matrix
                                        + " -DUSE_RNG:STRING=" + rng
                                        + " -DUSE_TEMP:STRING=" + str(T)
                                        + " -DUSE_LENGTH:STRING=" + str(L)
                                        + " -DUSE_HEIGHT:STRING=" + str(H)
                                        + " -DUSE_DEPTH:STRING=" + str(D)
                                        , shell = True, stdout = subprocess.DEVNULL)
    subprocess.call("make -B perf_raw", shell = True, stdout = subprocess.DEVNULL)
    subprocess.call("mv " + build_dir.build_dir + "/performance/perf_raw " + build_dir.build_dir + "/performance/compiled/" + name, shell = True)

def compile_all(T, L, H, D):
    try:
        subprocess.call("mkdir " + build_dir.build_dir + "/performance/compiled", shell = True)
    except:
        pass
        
    f = open(build_dir.build_dir + "/performance/compiled/versions_index.txt", "w")
    f.write("SIM:\n")
    for i in range(len(sim_versions)):
        f.write(str(i) + ": " + sim_versions[i] + ", ")
    f.write("\nGRID:\n")
    for i in range(len(grid_versions)):
        f.write(str(i) + ": " + grid_versions[i] + ", ")
    f.write("\nMATRIX:\n")
    for i in range(len(matrix_versions)):
        f.write(str(i) + ": " + matrix_versions[i] + ", ")
    f.write("\nRNG:\n")
    for i in range(len(rng_versions)):
        f.write(str(i) + ": " + rng_versions[i] + ", ")
    f.write("\n\n")

    for i in range(len(sim_versions)):
        for j in range(len(grid_versions)):
            for k in range(len(matrix_versions)):
                for l in range(len(rng_versions)):
                    f.write(idx_string([i, j, k, l]) + " " + sim_versions[i] + " " + grid_versions[j] + " " + matrix_versions[k] + " " + rng_versions[l] + "\n")
    f.close()
    
    for i in range(len(sim_versions)):
        for j in range(len(grid_versions)):
            for k in range(len(matrix_versions)):
                for l in range(len(rng_versions)):
                    compile_version([i, j, k, l], T, L, H, D)
                    
def remove_all():
    subprocess.call("rm -rf" + build_dir.build_dir + "/compiled", shell= True)
#-----------------------------------------------------------------------#

if __name__ == "__main__":
    #~ compile_all(5, 400, 400, 400)
    compile_single("greschd_v2_sim", "msk_v1_pbc", "msk_v2_dynamic_zip", "custom_mt_rng", 5, 400, 400, 400, "best_for_400")
    #~ remove_all()
    

