#!/usr/bin/python3.2
# -*- coding: utf-8 -*-
#
# Author:  Dominik Gresch <greschd@ethz.ch>
# Date:    16.04.2014 09:37:20 CEST
# File:    performance_search.py

import os
import sys
import glob

#----------------------collecting typenames-----------------------------#

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

print(rng_versions)
