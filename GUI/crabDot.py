#crab dot object that is displayed on the screen
from PyQt4 import QtGui
from PyQt4 import QtCore 
import math
class CrabDot():
    def __init__(self, r, theta, UID):
        self.r = r
        self.theta = theta
        self.UID = UID
        self.x, self.y = self.polar_to_cartesian(r, theta)

        self.drawCrab(self.r, self.theta)
    #self.startTimer(self.UID)

    def drawCrab(self, r, theta):
        # need to imbed matlibplot and use polar coordinates
        # to draw the crab's location 
        # use paint event 
        #dot = QtGui.QPainter()

        #dot.begin()
        #dot.drawPoint(self.polar_to_cartesian(r, theta))
        #dot.end()

        #print(self.polar_to_cartesian(r, theta))
        pass

    def startTime(self, UID):
        pass 

    # translates the polar coordinates into cartesian coordinates
    def polar_to_cartesian(self, r, theta):
        #theta += 45
        return int(r*math.cos(theta)), int(r*math.sin(theta))
        #return abs(int(r*math.cos(theta))), abs(int(r*math.sin(theta)))
