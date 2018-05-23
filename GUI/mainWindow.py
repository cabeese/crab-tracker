import sys
from PyQt4 import QtGui, QtCore
from db import create
from radar import RadarWindow
from listenForCrabs import ListenForCrabs

class MainWindow(QtGui.QMainWindow):

    def __init__(self):
        super().__init__()
        # Sorry I don't have time to find a better way :/ --Kurt
        #what the hell did kurt think this was about?
        try:
            self.create()
        except:
            print("an error occured")
        self.initUI()

    def initUI(self):
        self.window = RadarWindow()
        self.setCentralWidget(self.window)
        self.setWindowTitle("Crab Tracker")
        #TODO Crab Tracker should be in arial, can I change the color of the bar at the top of the screen?
        self.showMaximized()
        self.show()
