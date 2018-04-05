
import sys, os
from PyQt4 import QtGui 
from PyQt4 import QtCore 
# later have one file called buttons that has all the button classes in it
#TODO change all the other button class names to capitals
from buttons import *


from listenForCrabs import ListenForCrabs

class RadarWindow(QtGui.QWidget):
    def __init__(self):
        super().__init__()
        self.createGrid()
        
        #load database?
        #the gui needs to recognize crab data as it's coming in

#    def show(self):
#        super().show()
#

    def createGrid(self):
        
        background = self.background() # add radar circles with kayak
        logo = self.logo() # add logo

        # create buttons on side of screen
        buttons = self.buttons()

        grid = QtGui.QGridLayout()
        
        grid.setHorizontalSpacing(7)
        grid.setVerticalSpacing(4)
        
        
        # row of grid, column of grid, number of rows element will span, number of columns element will span
        for i in range(0,7):
            grid.addWidget(buttons[i], i, 0, 1, 1)
            grid.setRowStretch(i, 1)    
        #grid.addWidget(background, 0, 1, QtCore.Qt.AlignCenter)
        
        spacer = QtGui.QSpacerItem(90, 20, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        grid.addItem(spacer, 0, 1, 7, 1)
        grid.addWidget(background, 0, 2, 7, 1)
        #having the crab logo span all 7 rows may cause issues later
        grid.addWidget(logo, 0, 3, 7, 1)
        self.setLayout(grid)

        #TODO still figuring out where crabs shoudl be listend for
        # this seems like a good place but I'm not sure of the mechanics
        #self.listen = ListenForCrabs()

        
    # crab log in upper right hand corner
    def logo(self):
        pixmap = QtGui.QPixmap("logo.png")

        #scales crab logo to fit screen
        # TODO this  is not quite the right size
        pixmap = pixmap.scaled(120, 120, QtCore.Qt.KeepAspectRatio)
        logo = QtGui.QLabel(self)
        logo.setPixmap(pixmap)
        logo.setAlignment(QtCore.Qt.AlignRight)
        return logo

    def background(self):
        listen = ListenForCrabs()
        #can I add the code here to draw the crab dots?
        pixmap = QtGui.QPixmap("radar.png")

        #this needs to be changed later
        # should never divide by an actual number
        # figure out what it is in realtion to the window/screen
        pixmap = pixmap.scaled(self.size()/1.25, QtCore.Qt.KeepAspectRatio)
        #Note if you add the pixmap to the label before drawing the crab dots they won't show up on the screen
        # this will cause issues as the crab dots need to update regulalry and will not be a "one time thing"

        ### added to adjust crab dots origin
        pixmapH = pixmap.size().height()
        pixmapW = pixmap.size().width()

        ### code added to try and draw crab dots over background
        # added more out of desperation to make a crab apear than an actual solution
        #dot = QtGui.QPainter() 
        #dot.begin(pixmap)
        #pen = QtGui.QPen(QtGui.QColor(0, 0, 0)) 
        #pen.setWidth(10) 
        #dot.setPen(pen)
        #for crab in listen.crabDots:
        #    if not(crab == None):
        #        dot.drawPoint(crab.x+(pixmapH/2), crab.y+(pixmapW/2))
        #dot.end()

        background = QtGui.QLabel(self)
        background.setPixmap(pixmap)
        background.setAlignment(QtCore.Qt.AlignRight)
        
        return background


    #buttons
    def buttons(self):
        buttons = []
        #will need to do each button
        buttons.append(QtGui.QPushButton("Add Crab"))
        #set up what happens when they're clicked
        buttons[0].clicked.connect(self.addCrabClicked)
        #each button will need its own clicked button

        buttons.append(QtGui.QPushButton("Crab Data")) 
        buttons[1].clicked.connect(self.crabDataClicked)
        
        buttons.append(QtGui.QPushButton("Display")) 
        buttons[2].clicked.connect(self.displayClicked)
        
        buttons.append(QtGui.QPushButton("Set Up")) 
        buttons[3].clicked.connect(self.setUpClicked)
        
        buttons.append(QtGui.QPushButton("Heat Map")) 
        buttons[4].clicked.connect(self.heatMapClicked)
        
        buttons.append(QtGui.QPushButton("Export")) 
        buttons[5].clicked.connect(self.exportClicked)

        buttons.append(QtGui.QPushButton("Quit")) 
        buttons[6].clicked.connect(self.quitClicked)
        
        # change style for all of the buttons    
        for i in range(7):
            #buttons.append(QtGui.QPushButton(str(i)))
            buttons[i].setStyleSheet("background-color: #282833; color: white;")
            buttons[i].setFixedSize(120,50)
        return buttons


    def addCrabClicked(self):
        self.add_crab_window = AddCrabWindow()
        self.add_crab_window.show()

    def crabDataClicked(self):
        print("Crab Data!")

    def displayClicked(self):
        self.displayWindow = displayWindow(self.geometry().height())
        self.displayWindow.show()

    def setUpClicked(self):
        #calls class that deals with what happens when the Set Up button is pressed
        self.setUpwindow = setUpsubwindow()
        self.setUpwindow.show()


    def heatMapClicked(self):
        print("Heat Map!")

    def exportClicked(self):
        Export()
        print("Export!")

    def quitClicked(self):
        #save the state of the database

        # quit application
        sys.exit()




