#!/usr/bin/python3
# -*- coding: utf-8 -*-
#
# Author:  Dominik Gresch <greschd@ethz.ch>
# Date:    18.04.2014 17:55:25 CEST
# File:    validate_all.py

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


sim_versions, grid_versions, matrix_versions, rng_versions = co.collect_all("beta")

#-----------------------------------------------------------------------#

#change folder so that cmake happens in the right place
os.chdir(build_dir.build_dir)

def validate(sim, sim2, grid, grid2, matrix, matrix2, rng):
    subprocess.call("cmake " + mcpath   + " -DUSE_SIM:STRING=" + sim
                                        + " -DUSE_SIM2:STRING=" + sim2
                                        + " -DUSE_GRID:STRING=" + grid
                                        + " -DUSE_GRID2:STRING=" + grid2
                                        + " -DUSE_MATRIX:STRING=" + matrix
                                        + " -DUSE_MATRIX2:STRING=" + matrix2
                                        + " -DUSE_RNG:STRING=" + rng
                                        , shell = True, stdout = subprocess.DEVNULL)
    comp_return = str(subprocess.check_output("make -B validate", shell = True))
    if(comp_return.find("Built target validate") == -1):
        print(xtermcolor.colorize("compilation error", rgb = 0xFF0000))
        return False
    error_msg = ''
    try:
        error_msg = subprocess.check_output("./validation/validate")
        return True
    except:
        print(error_msg)
        return False

def validate_all():
    failed = []
    for sim in sim_versions:
        if(validate("baseline_greschd_sim", sim, "baseline_greschd_grid", "baseline_greschd_grid", "baseline_greschd_matrix", "baseline_greschd_matrix", "std_mt_rng")):
            print("validated " + sim)
        else:
            print(xtermcolor.colorize(sim + " failed", rgb = 0xFF0000))
            failed.append(sim)
    for grid in grid_versions:
        if(validate("baseline_greschd_sim", "baseline_greschd_sim", "baseline_greschd_grid", grid, "baseline_greschd_matrix", "baseline_greschd_matrix", "std_mt_rng")):
            print("validated " + grid)
        else:
            print(xtermcolor.colorize(grid + " failed", rgb = 0xFF0000))
            failed.append(grid)
    for matrix in matrix_versions:
        if(validate("baseline_greschd_sim", "baseline_greschd_sim", "baseline_greschd_grid", "baseline_greschd_grid", "baseline_greschd_matrix", matrix, "std_mt_rng")):
            print("validated " + matrix)
        else:
            print(xtermcolor.colorize(matrix + " failed", rgb = 0xFF0000))
            failed.append(matrix)
    for rng in rng_versions:
        if(validate("baseline_greschd_sim", "baseline_greschd_sim", "baseline_greschd_grid", "baseline_greschd_grid", "baseline_greschd_matrix", "baseline_greschd_matrix", rng)):
            print("validated " + rng)
        else:
            print(xtermcolor.colorize(rng + " failed", rgb = 0xFF0000))
            failed.append(rng)
    print(xtermcolor.colorize("finished validation:", rgb = 0x00FFFF))
    if(len(failed) != 0):
        print(xtermcolor.colorize("the following version(s) failed:", rgb = 0xFF0000))
        for fail in failed:
            print(xtermcolor.colorize(fail, rgb = 0xFF0000))
    else:
        print(xtermcolor.colorize("all tests ran successfully!", rgb = 0xFFFFFF))
    
#-----------------------------------------------------------------------#

if __name__ == "__main__":
    validate_all()

