#!/usr/bin/python3
# -*- coding: utf-8 -*-
#
# Author:  Dominik Gresch <greschd@ethz.ch>
# Date:    21.05.2014 12:32:51 CEST
# File:    install_routine.py

import os   
import sys

    

#------------------import from the addons folder------------------------#

mcpath = '/'.join(os.path.realpath(__file__).split("/")[:-2])
sys.path.insert(0, mcpath + '/performance/')
sys.path.insert(0, mcpath + '/addons_python/')

import build_dir
import performance_search as ps

def progressbar(val, maxval, width = 50):
    n = int(val * width / float(maxval))
    sys.stdout.write("\r[" + n * "*" + (width - n) * "-" + "]")
    sys.stdout.flush()

if __name__ == "__main__":
    #-------------------------------------------------------------------#
    #                       installation parameters                     #
    #-------------------------------------------------------------------#
    sizes = [2, 4, 5]
    temperature = 5
    num_runs = 1
    search_engine = lambda t, s, r: ps.search_performance(t, s, s, s, r, verbose = False)[0]
    #-------------------------------------------------------------------#
    sizes = list(sorted(sizes))

    string_start = "\
// this file is automatically generated by the install routine\n\
// File:    install.hpp\n\
\n\
#ifndef __INSTALL_HEADER\n\
#define __INSTALL_HEADER\n\
\n\
#include <beta.hpp>\n\
\n\
namespace mc_potts {\n\
    namespace install {\n\n\
"


    string_end = "\
    }// namespace install\n\
}// namespace mc_potts\n\
\n\
#endif //__INSTALL_HEADER\n\
"

    string_base = "\
        template<int N>\n\
        struct opt {\n\
            template<int S>\n\
            using impl = baseline_greschd_sim::impl<S, S, S, addon::std_mt_rng, baseline_greschd_grid, baseline_greschd_matrix>;\n\
        };\n\n\
"


    def tpl_spec(N, versions):
        string = "\
        template<>\n\
        struct opt<" + str(N) + "> {\n\
            template<int S>\n\
            using impl = " + str(versions[0]) + "::impl<S, S, S, addon::" + str(versions[3]) + ", " + str(versions[1]) + ", " + str(versions[2]) + ">; \n\
        };\n\n"
        return string

    progressbar(0, len(sizes))
    string_spec = ""
    for i, s in enumerate(sizes):
        try:
            string_spec += tpl_spec(s, search_engine(temperature, s, num_runs))
            progressbar(i + 1, len(sizes))
        except:
            print("Error at size " + str(s))
            sizes.remove(s)
            
    
    string_sizes = "        constexpr int num_sizes = " + str(len(sizes)) + ";\n"
    string_sizes += "        constexpr int sizes[] {"
    for s in sizes[:-2]:
        string_sizes += str(s) + ", "
    string_sizes += str(sizes[-1]) + "};\n\n"
    
    
    f = open(mcpath + "/install/install.hpp", "w")
    f.write(string_start)
    f.write(string_sizes)
    f.write(string_base)
    f.write(string_spec)
    f.write(string_end)
    f.close()