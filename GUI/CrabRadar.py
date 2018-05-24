from __future__ import unicode_literals
import sys
import os
import random
from matplotlib.backends import qt_compat
use_pyside = qt_compat.QT_API == qt_compat.QT_API_PYSIDE
if use_pyside:
    from PySide import QtGui, QtCore
else:
    from PyQt4 import QtGui, QtCore

from numpy import arange, sin, pi
from matplotlib.backends.backend_qt4agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure



from matplotlib.backends.backend_qt4agg import FigureCanvasQTAgg as FigureCanvas
import matplotlib.pyplot as plt

import matplotlib.transforms as mtransforms
import numpy as np

from matplotlib.transforms import offset_copy
import csv

class CrabRadar(FigureCanvas):
    #decisions we need to make
    #f = open("data.txt", "w")
    xcoord = [0, 0, 0, 0]
    ycoord = [0, 0, 0, 0]

    def __init__(self, parent=None, width=5, height=4, dpi=100,linewidth=1.0):
        fig = Figure(figsize=(width, height), dpi=dpi,linewidth=1.0)
        self.axes = fig.add_subplot(111, projection= 'polar')


        self.compute_initial_figure()

        FigureCanvas.__init__(self, fig)
        timer = QtCore.QTimer(self)
        timer.timeout.connect(self.update_figure)
        timer.start(1000)
        self.setParent(parent)

        FigureCanvas.setSizePolicy(self,
                                   QtGui.QSizePolicy.Expanding,
                                   QtGui.QSizePolicy.Expanding)
        FigureCanvas.updateGeometry(self)

    def compute_initial_figure(self):
        self.axes.set_theta_zero_location('N')
        self.axes.set_xticklabels(['N', 'NE', 'E', 'SE', 'S', 'SW', 'W', 'NW'])
        self.axes.plot([0, 1, 2, 3], [1, 2, 0, 4], 'ro')

    def update_figure(self):
        self.axes.set_theta_zero_location('N')
        self.axes.set_xticklabels(['N', 'NE', 'E', 'SE', 'S', 'SW', 'W', 'NW'])
        # Build a list of 4 random integers between 0 and 10 (both inclusive)
        l = [random.randint(0, 10) for i in range(4)]
        #following line clears the dots
        self.axes.cla()
        self.axes.plot([0, 1, 2, 3], l, 'ro')
        self.axes.set_theta_zero_location('N')
        self.axes.set_xticklabels(['N', 'NE', 'E', 'SE', 'S', 'SW', 'W', 'NW'])
        self.draw()

    #def get_coordinates(self):
        #somehow has to be listening for crabs
        #when it gets a crab, it gets the coordinates
