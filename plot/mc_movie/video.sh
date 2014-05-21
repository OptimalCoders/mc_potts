#!/bin/sh

for f in frames/*ppm ; do convert -quality 100 $f frames/`basename $f ppm`jpg; done
mencoder "mf://frames/*.jpg" -mf fps=12 -o ising.avi -ovc lavc -lavcopts vcodec=msmpeg4v2:vbitrate=800 
