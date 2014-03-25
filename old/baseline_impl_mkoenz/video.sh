#!/bin/sh

for f in images/*ppm ; do convert -quality 100 $f images/`basename $f ppm`jpg; done
mencoder "mf://images/*.jpg" -mf fps=12 -o potts.avi -ovc lavc -lavcopts vcodec=msmpeg4v2:vbitrate=800 
