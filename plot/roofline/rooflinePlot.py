#!/usr/bin/python3
# -*- coding: utf-8 -*-

#General imports:

import sys
import os
import math
from matplotlib.pyplot import rc
rc('text', usetex=True) # this is if you want to use latex to print text. If you do you can create strings that go on labels or titles like this for example (with an r in front): r"$n=$ " + str(int(n))
from numpy import *
from pylab import *
import random
from matplotlib.font_manager import FontProperties
import matplotlib.pyplot as plt
import matplotlib.lines as lns

   
background_color = '#eeeeee' 
grid_color = 'white' #FAFAF7'
matplotlib.pylab.rc('axes', facecolor = background_color)
matplotlib.pylab.rc('axes', edgecolor = grid_color)
matplotlib.pylab.rc('axes', linewidth = 1.2)
matplotlib.pylab.rc('axes', grid = True )
matplotlib.pylab.rc('axes', axisbelow = True)
matplotlib.pylab.rc('grid',color = grid_color)
matplotlib.pylab.rc('grid',linestyle='-' )
matplotlib.pylab.rc('grid',linewidth=0.7 )
matplotlib.pylab.rc('xtick.major',size =0 )
matplotlib.pylab.rc('xtick.minor',size =0 )
matplotlib.pylab.rc('ytick.major',size =0 )
matplotlib.pylab.rc('ytick.minor',size =0 )
matplotlib.pylab.rc('font',**{'family':'sans-serif', 'sans-serif':['Gill Sans MT']})


matplotlib.pylab.rc('text.latex', preamble=r'\usepackage{cmbright}')
matplotlib.pylab.rc('mathtext', fontset='stixsans')

X_MIN=0.01
X_MAX=30.0
Y_MIN=0.01
Y_MAX=5.0
# 
PEAK_PERF=3.0
PEAK_BW=5.2
ASPECT_RATIO=0.618
LOG_X=True
LOG_Y=True


fig = plt.figure()
# Returns the Axes instance
ax = fig.add_subplot(111, aspect = ASPECT_RATIO)
ax.tick_params(labelsize = 10)

#Log scale
if LOG_Y: ax.set_yscale('log')
if LOG_X: ax.set_xscale('log')

#formatting:
ax.set_title(r"\textbf{Roofline Plot}",fontsize=15, position = (0.5, 1.1))
ax.set_xlabel("Operational Intensity [Flop/Byte]", fontsize=12, horizontalalignment = "right")
ax.set_ylabel("Performance [Flop/Cycle]", fontsize=12, rotation = "horizontal", horizontalalignment = "left")

ax.yaxis.set_label_coords(0, 1.05)
ax.xaxis.set_label_coords(1, -0.09)


#~ #x-y range
ax.axis([X_MIN,X_MAX,Y_MIN,Y_MAX])

x, y  = loadtxt('input.dat',unpack=True, usecols=[0,1])
ax.scatter([x[0],],[y[0],],15, color='blue', alpha= 0.95, label='Variant 0') 
ax.scatter([x[1],],[y[1],],15, color='red', alpha = 0.95, label='Variant 1')


#Percentile boxes
#ax.boxplot((x[0,],y[0,]))


#Anotate 
# TODO: make it if anotate
ax.annotate('Variant 0',
         xy=(x[0], y[0]), xycoords='data',
         xytext=(+3, +1), textcoords='offset points', fontsize=10)

ax.annotate('Variant 1',
         xy=(x[1],y[1]), xycoords='data',
         xytext=(+3, +1), textcoords='offset points', fontsize=10)



#Peak performance line and text
ax.axhline(y=PEAK_PERF, linewidth=1, color='black')
ax.text(X_MAX/10.0, PEAK_PERF+0.15, "Peak Performance ("+str(PEAK_PERF)+" F/C)", fontsize=10)


#BW line and text
x = np.linspace(X_MIN, X_MAX, X_MAX)
y = x*PEAK_BW 
ax.plot(x, y, linewidth=1, color='black')


l2 = array((0.01,0.01))
angle = 45*(ASPECT_RATIO+0.1)
trans_angle = gca().transData.transform_angles(array((angle,)),
                                               l2.reshape((1,2)))[0]

ax.text(X_MIN,X_MIN*PEAK_BW + 0.15,'MemLoad ('+str(PEAK_BW)+' B/C)',fontsize=10,
           rotation=trans_angle)



#save file
fig.savefig("rooflinePlot.pdf", dpi=250,  bbox_inches='tight')
