#!/usr/bin/python3
# -*- coding: utf-8 -*-
#
# Author:  Dominik Gresch <greschd@ethz.ch>
# Date:    18.04.2014 18:05:19 CEST
# File:    collect_versions.py

import os
import glob

#-----------------------------------------------------------------------#
#                          collecting typenames                         #
#-----------------------------------------------------------------------#

mcpath = '/'.join(os.path.realpath(__file__).split("/")[:-2])

def remove_if_contains(string, vector):
    return list(filter(lambda x: x.find(string) == -1, vector))

def collect_filenames(name, collector, keepout):
    for x in glob.glob(mcpath + "/src/" + name + "/*.hpp"):
        temp = x.split("/")[-1].split(".hpp")[0]
        add = True
        for k in keepout:
            if(temp.find(k) != -1):
                add = False
        if add:
            collector.append( x.split("/")[-1].split(".hpp")[0])
            
def collect_all(keepout):
    sim_versions = []
    grid_versions = []
    matrix_versions = []
    rng_versions = []
    
    if not isinstance(keepout, list):
        keepout = [keepout]
    
    collect_filenames("sim", sim_versions, keepout)
    collect_filenames("grid", grid_versions, keepout)
    collect_filenames("matrix", matrix_versions, keepout)
    collect_filenames("rng", rng_versions, keepout)
    return [sim_versions, grid_versions, matrix_versions, rng_versions]
    
