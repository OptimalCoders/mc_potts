#!/usr/bin/python3
# -*- coding: utf-8 -*-
#
# Author:  Dominik Gresch <greschd@ethz.ch>
# Date:    22.05.2014 12:11:53 CEST
# File:    runtime.py

import os
import sys
import subprocess

#------------------import from the addons folder------------------------#

mcpath = '/'.join(os.path.realpath(__file__).split("/")[:-2])
sys.path.insert(0, mcpath + '/addons_python/')
sys.path.insert(0, mcpath + '/performance/')

import build_dir
import collect_versions as co
import performance_search as ps


#change folder so that cmake happens in the right place
os.chdir(build_dir.build_dir)
def compile_opt(T, N, full = False):
    os.chdir(build_dir.build_dir)
    subprocess.call("cmake " + mcpath   + " -DUSE_TEMP:STRING=" + str(T)
                                        + " -DUSE_LENGTH:STRING=" + str(N)
                                        , shell = True, stdout = subprocess.DEVNULL)
    if(full):
        subprocess.call("make -B perf_opt_full", shell = True, stdout = subprocess.DEVNULL)
    else:
        subprocess.call("make -B perf_opt", shell = True, stdout = subprocess.DEVNULL)
    
def run_opt(full = False):
    os.chdir(build_dir.build_dir + "/performance")
    if(full):
        return int(list(filter(None, str(subprocess.check_output("./perf_opt_full")).split(" ")))[1].split("\\x1b[0m")[0])
    else:
        return int(list(filter(None, str(subprocess.check_output("./perf_opt")).split(" ")))[1].split("\\x1b[0m")[0])
    
def measure_opt(T, sizes, verbose = False, full = False):
    runtimes = []
    if(verbose):
        print("measuring runtime")
        print("sizes:")
        print(sizes)
        print("currently at:")
    for s in sizes:
        if(verbose):
            sys.stdout.write("\r" + str(s))
        compile_opt(T, s, full = full)
        runtimes.append(run_opt(full = full))
    return sizes, runtimes
    
def measure(T, sizes, sim, rng, grid, matrix, verbose = False):
    runtimes = []
    if(verbose):
        print("measuring runtime")
        print("sizes:")
        print(sizes)
        print("currently at:")
    for s in sizes:
        if(verbose):
            sys.stdout.write("\r" + str(s))
        runtimes.append(ps.measure(sim, grid, matrix, rng, T, s, s, s))
    return sizes, runtimes

if __name__ == "__main__":
    compile_opt(10, 10)
    print(run_opt())
    print("runtime.py")
    
