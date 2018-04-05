from PyQt4 import QtGui, QtCore
from crab import Crab
from db import insert_crab, read_crab

class AddCrabWindow(QtGui.QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowFlags(QtCore.Qt.WindowStaysOnTopHint)
        self.setWindowTitle("Add Crab")
        self.showMaximized()
        self.createGrid()
        self.current = None  # Currently selected QLineEdit

    def createGrid(self):
        buttonH = 100
        buttonW = 40
        grid = QtGui.QGridLayout()
        grid.setHorizontalSpacing(6)
        grid.setVerticalSpacing(5)
        
        hspacer = QtGui.QSpacerItem(20, 20, QtGui.QSizePolicy.Minimum) 
        grid.addItem(hspacer, 0, 0)
        
        #TODO add same spacer under bottom buttons
        #TODO add some space between Save and Cancel buttons
       
        self.uid_text_field = QtGui.QLineEdit()
        self.uid_text_field.setPlaceholderText('UID')

        self.sex_drop_down = QtGui.QComboBox()
        self.sex_drop_down.addItem('Sex')
        self.sex_drop_down.addItem('Female')
        self.sex_drop_down.addItem('Male')

        self.species_drop_down = QtGui.QComboBox()
        self.species_drop_down.addItem('Species')
        self.species_drop_down.addItem('European Green')
        self.species_drop_down.addItem('Dungeness')
        self.species_drop_down.addItem('Rock')

        self.color_drop_down = QtGui.QComboBox()
        self.color_drop_down.addItem('Color')
        self.color_drop_down.addItem('Green')
        self.color_drop_down.addItem('Red')
        self.color_drop_down.addItem('Yellow')

        self.damage_drop_down = QtGui.QComboBox()
        self.damage_drop_down.addItem('Damage')
        self.damage_drop_down.addItem('Yes')
        self.damage_drop_down.addItem('No')

        self.carapace_width_field = QtGui.QLineEdit()
        self.carapace_width_field.setPlaceholderText('Width (mm)')

        self.mass_field = QtGui.QLineEdit()
        self.mass_field.setPlaceholderText('Mass (g)')

        self.epibiont_drop_down = QtGui.QComboBox()
        self.epibiont_drop_down.addItem('Epibiont Load')
        self.epibiont_drop_down.addItem('None')
        self.epibiont_drop_down.addItem('Minimal')
        self.epibiont_drop_down.addItem('Moderate')
        self.epibiont_drop_down.addItem('Substantial')

        self.molt_drop_down = QtGui.QComboBox()
        self.molt_drop_down.addItem('Molt')
        self.molt_drop_down.addItem('Soft')
        self.molt_drop_down.addItem('Hard')

        def set_uid(_):
            self.current = self.uid_text_field
        def set_carapace(_):
            self.current = self.carapace_width_field
        def set_mass(_):
            self.current = self.mass_field

        self.uid_text_field.mousePressEvent = set_uid
        self.carapace_width_field.mousePressEvent = set_carapace
        self.mass_field.mousePressEvent = set_mass

        grid.addWidget(self.uid_text_field, 1, 0)
        grid.addWidget(self.sex_drop_down, 2, 0)
        grid.addWidget(self.species_drop_down, 3, 0)
        grid.addWidget(self.carapace_width_field, 4, 0)
        grid.addWidget(self.mass_field, 5, 0)
        grid.addWidget(self.epibiont_drop_down, 6, 0)
        grid.addWidget(self.damage_drop_down, 7, 0)
        grid.addWidget(self.molt_drop_down, 8, 0)
        grid.addWidget(self.color_drop_down, 9, 0)



       # onscreen 10 key to input the info
        self.clearButton = QtGui.QPushButton("Clear")
        self.clearButton.setStyleSheet("background-color: #282833; color: white;")
        self.clearButton.setFixedSize(buttonH, buttonW)
        grid.addWidget(self.clearButton, 4, 5)

        self.decimalButton = QtGui.QPushButton(".")  
        self.decimalButton.setStyleSheet("background-color: #282833; color: white;")
        self.decimalButton.setFixedSize(buttonH, buttonW)
        grid.addWidget(self.decimalButton, 4, 4)

        # spacer in between text edit boxes and keypad
        vspacer = QtGui.QSpacerItem(60, 20, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        grid.addItem(vspacer, 1, 2, 5, 1)

        zero = QtGui.QPushButton("0")
        grid.addWidget(zero, 4, 3)
        one  = QtGui.QPushButton("1") 
        grid.addWidget(one, 3, 3)
        two  = QtGui.QPushButton("2")
        grid.addWidget(two, 3, 4)
        three = QtGui.QPushButton("3") 
        grid.addWidget(three, 3, 5)
        four = QtGui.QPushButton("4")
        grid.addWidget(four, 2 ,3)
        five = QtGui.QPushButton("5")
        grid.addWidget(five, 2, 4)
        six = QtGui.QPushButton("6")
        grid.addWidget(six, 2, 5)
        seven = QtGui.QPushButton("7") 
        grid.addWidget(seven, 1, 3)
        eight = QtGui.QPushButton("8") 
        grid.addWidget(eight, 1, 4)
        nine = QtGui.QPushButton("9") 
        grid.addWidget(nine, 1, 5)

        numberButtons = [zero, one, two, three, four, five, six, seven, eight, nine]
        for i in range(len(numberButtons)):
            numberButtons[i].setStyleSheet("background-color: #282833; color: white;")
            numberButtons[i].setFixedSize(buttonH, buttonW)
            numberButtons[i].clicked.connect(self.number_button_event(i))


        self.decimalButton.clicked.connect(self.number_button_event('.'))
        self.clearButton.clicked.connect(self.clear_button_event())
        saveButton = QtGui.QPushButton("Save and Exit")  
        saveButton.setStyleSheet("background-color: #282833; color: white;")
        saveButton.clicked.connect(self.saveButtonClicked)
        saveButton.setFixedSize(120,50)
        grid.addWidget(saveButton, 12, 0)
    
        cancelButton = QtGui.QPushButton("Cancel")
        cancelButton.setStyleSheet("background-color: #282833; color: white;")
        cancelButton.setFixedSize(120, 50)
        cancelButton.clicked.connect(self.cancelButtonClicked)
        grid.addWidget(cancelButton, 12, 1)




       # TODO: how will this send the information back to Chloe's algorithm?
        self.setLayout(grid)

    def number_button_event(self, i):
        return lambda _: self.current.setText(self.current.text()+"{0}".format(i))

    def clear_button_event(self):
        return lambda _: self.current.setText('')

    def saveButtonClicked(self):
        # TODO later figure out how this will talk to Chloe's algorithm and update these values
        if self.sex_drop_down.currentIndex() == 0 or self.species_drop_down.currentIndex() == 0 or self.color_drop_down.currentIndex() == 0 or\
           self.damage_drop_down.currentIndex() == 0 or self.epibiont_drop_down.currentIndex() == 0 or self.molt_drop_down.currentIndex() == 0:
            msg = QtGui.QMessageBox()
            msg.setIcon(QtGui.QMessageBox.Information)
            msg.setText("All dropdowns must have a valid selection!")
            msg.setWindowTitle("Dropdowns Invalid")
            msg.setStandardButtons(QtGui.QMessageBox.Ok)

            retval = msg.exec_()
        else:
            new_crab = Crab(self.uid_text_field.text(), self.sex_drop_down.currentText(), self.species_drop_down.currentText(),
                            self.color_drop_down.currentText(), self.damage_drop_down.currentText(),
                            float(self.carapace_width_field.text()), float(self.mass_field.text()), self.epibiont_drop_down.currentText(),
                            self.molt_drop_down.currentText())
            insert_crab(new_crab)
            self.close()

    def cancelButtonClicked(self, event):
        self.close()
# class that responds when the display button is clicked

# from PyQt4 import QtGui, QtCore

class displayWindow(QtGui.QWidget):
    def __init__(self, height):
        super().__init__()
        #self.QtGui.QFont().setBold(True)
        self.height = height
        # make it be on top
        self.setWindowFlags(QtCore.Qt.WindowStaysOnTopHint)

        # TODO change font to arial change color of bar at top
        self.setWindowTitle("Display")

        #set window size
        self.resize(325 ,self.height)
        self.createLayout()


    def createLayout(self):
        
        #grid to layout all the elements
        grid = QtGui.QGridLayout() 
        grid.setVerticalSpacing(2)
        grid.setHorizontalSpacing(13)

#        #TODO this button needs a better name
#        # title is for "only show crabs not tagged in  the last .."
        self.showCrabTimes = QtGui.QCheckBox("Only show crabs not tagged in the past ...")
        # connects check box to function 
        self.showCrabTimes.stateChanged.connect(self.showCrabTimesChecked)
        grid.addWidget(self.showCrabTimes, 0, 0)   
        
        #drop down menu with crab times
        dropDown = QtGui.QComboBox(self)
        #TODO find out later the actual numbers that patrick will need
        dropDown.addItem("1 hour")
        dropDown.addItem("6 hours")
        dropDown.addItem("9 hours")
        dropDown.addItem("12 hours")
        
        # when a menu item is selected
        dropDown.activated[str].connect(self.dropDownActivated)
        # add drop downmenu to grid
        grid.addWidget(dropDown, 1, 0)
        
        #TODO do the radio button effect the check boxes in any way?
        #TODO if not we need some way of checking all the buttons and some way of checking none of the buttons
        # track all radio button
        self.trackAll = QtGui.QRadioButton("Track All")
        self.trackAll.setChecked(True)
        #connects radio button to a function
        self.trackAll.toggled.connect(lambda:self.trackingState(self.trackAll))
        grid.addWidget(self.trackAll, 2, 0)

        # track none radio button
        self.trackNone = QtGui.QRadioButton("Track None")
        self.trackNone.setChecked(False)
        self.trackNone.toggled.connect(lambda:self.trackingState(self.trackNone))
        grid.addWidget(self.trackNone, 3, 0)

        #check box show crabs 0-10 m away 
        self.zeroTo10meters = QtGui.QCheckBox("Crabs 0-10 m")
        self.zeroTo10meters.stateChanged.connect(self.zeroTo10metersChecked)
        grid.addWidget(self.zeroTo10meters, 4, 0)

        #check box show crabs 11-100 m
        self.elevenTo100meters = QtGui.QCheckBox("Crabs 11-100 m")  
        self.elevenTo100meters.stateChanged.connect(self.elevenTo100metersChecked)
        grid.addWidget(self.elevenTo100meters, 5, 0)

        # check box show crab 101+ meters
        self.moreThan101meters = QtGui.QCheckBox("Crabs 101+ m")
        self.moreThan101meters.stateChanged.connect(self.moreThan101metersChecked)
        grid.addWidget(self.moreThan101meters, 6, 0)

        # check box show Dungeness
        self.dungeness = QtGui.QCheckBox("Dungeness")
        self.dungeness.stateChanged.connect(self.dungenessChecked)
        grid.addWidget(self.dungeness, 7, 0)

        # check box show European Green
        self.europeanGreen = QtGui.QCheckBox("European Green")
        self.europeanGreen.stateChanged.connect(self.europeanGreenChecked)
        grid.addWidget(self.europeanGreen, 8, 0)

        # check box for Rock 
        self.rock = QtGui.QCheckBox("Rock")
        self.rock.stateChanged.connect(self.rockChecked)
        grid.addWidget(self.rock, 9, 0)

        # check box for active
        #TODO how do I make the font green?
        self.active = QtGui.QCheckBox("Active")
        #TODO this doesn't work to change the label color
        self.active.setStyleSheet("color: green")
        self.active.stateChanged.connect(self.activeChecked)
        grid.addWidget(self.active, 10, 0)

        #check box for inert
        #TODO how do I make the font red?
        self.inert = QtGui.QCheckBox("Inert")
        self.inert.stateChanged.connect(self.inertChecked)
        grid.addWidget(self.inert, 11, 0)

        # check box for expired
        #TODO how do I make the font grey?
        self.expired = QtGui.QCheckBox("Expired")
        self.expired.stateChanged.connect(self.expiredChecked)
        grid.addWidget(self.expired, 12, 0)

        self.setLayout(grid)
    
    # when drop down menu is activated
    def dropDownActivated(self, text):
        print(text)

    # when radio button is selected
    def showCrabTimesChecked(self, state):
        if state == QtCore.Qt.Checked:
            print("show crab times")
        else:
            print("don't show crab times")

    # when track all radio button is selected
    # by default it is selected?
    def trackingState(self, button):
        if button.text() == "Track All":
            if button.isChecked() == True:
                print("Tracking All the Crabs is selected")
            else:
                print("Tracking All of the Crabs is deselected")
        if button.text() == "Track None":
            if button.isChecked() == True:
                print("Tracking None of the crabs is selected")
            else: 
                print("Tracking None of the crabs is deselected")

    def zeroTo10metersChecked(self, state):
        if state == QtCore.Qt.Checked:
            print("show crabs 0-10 meters away")

    def elevenTo100metersChecked(self, state):
        if state == QtCore.Qt.Checked:
            print("show crabs 11-100 meters away")

    def moreThan101metersChecked(self, state):
        if state == QtCore.Qt.Checked:
            print("show crabs more than 101 meters away")

    def dungenessChecked(self, state):
        if state == QtCore.Qt.Checked:
            print("show dungeness crabs")

    def europeanGreenChecked(self, state):
        if state == QtCore.Qt.Checked:
            print("show european green crabs")
    
    def rockChecked(self, state):
        if state == QtCore.Qt.Checked:
            print("show rock crabs")

    def activeChecked(self, state):
        if state == QtCore.Qt.Checked:
            print("show active crabs")

    def inertChecked(self, state):
        if state == QtCore.Qt.Checked:
            print("show inert crabs")

    def expiredChecked(self, state):
        if state == QtCore.Qt.Checked:
            print("show expired crabs")
         

# class that defines behaivor when user presses export button

 #import os, sys
 #from PyQt4 import QtGui, QtCore

#TODO this class needs to be completely rewritten to export a copy of the database, with a timestamp in the file name and then have a pop up that tells the user where the file is stored
class Export(QtGui.QWidget):
    def __init__(self):
        super().__init__()
        self.saveFile = QtGui.QFileDialog(self)
        fileName = self.saveFile.getSaveFileName(self, 'save database', ' ')
        copyDatabase = open(fileName, "w")
        #TODO set this up so it actually connects to the database and exports its contents as a csv file
        copyDatabase.write("Eventually the entire crab database will be exported to this file")
        copyDatabase.close()

# TODO the specific buttons do not type in the text boxes

# from PyQt4 import QtGui, QtCore

class setUpsubwindow(QtGui.QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowFlags(QtCore.Qt.WindowStaysOnTopHint)
        self.setWindowTitle("Hydrophone Setup")
        self.showMaximized()
        self.createGrid()
        self.current = None  # Currently selected QLineEdit

    def createGrid(self):
        grid = QtGui.QGridLayout()
        grid.setHorizontalSpacing(6)
        grid.setVerticalSpacing(5)
        
        hspacer = QtGui.QSpacerItem(20, 20, QtGui.QSizePolicy.Minimum) 
        grid.addItem(hspacer, 0, 0)
        
        #TODO add same spacer under bottom buttons
        #TODO add some space between Save and Cancel buttons
       
        # text boxes for input

        self.tempEdit = QtGui.QLineEdit()
        self.tempEdit.setPlaceholderText('Temperature')

        self.salinityEdit = QtGui.QLineEdit()
        self.salinityEdit.setPlaceholderText('Salinity')

        self.sideEdit = QtGui.QLineEdit()
        self.sideEdit.setPlaceholderText('Hydrophone Side Width')

        grid.addWidget(self.tempEdit, 1, 0)
        grid.addWidget(self.salinityEdit, 2, 0)
        grid.addWidget(self.sideEdit, 3, 0)

        def set_temp(_):
            self.current = self.tempEdit
        def set_salinity(_):
            self.current = self.salinityEdit
        def set_sideEdit(_):
            self.current = self.sideEdit

        self.tempEdit.mousePressEvent = set_temp
        self.salinityEdit.mousePressEvent = set_salinity
        self.sideEdit.mousePressEvent = set_sideEdit

        buttonH = 100
        buttonW = 40
       # onscreen 10 key to input the info
        clearButton = QtGui.QPushButton("Clear")
        clearButton.setStyleSheet("background-color: #282833; color: white;")
        clearButton.clicked.connect(self.clearButtonClicked())
        clearButton.setFixedSize(buttonH, buttonW)
        grid.addWidget(clearButton, 4, 5)

        self.decimalButton = QtGui.QPushButton(".")  
        self.decimalButton.setStyleSheet("background-color: #282833; color: white;")
        self.decimalButton.clicked.connect(self.numberButtonsClicked('.'))
        self.decimalButton.setFixedSize(buttonH, buttonW)
        grid.addWidget(self.decimalButton, 4, 4)

        # spacer in between text edit boxes and keypad
        vspacer = QtGui.QSpacerItem(60, 20, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        grid.addItem(vspacer, 1, 2, 5, 1)

        zero = QtGui.QPushButton("0")
        grid.addWidget(zero, 4, 3)
        one  = QtGui.QPushButton("1") 
        grid.addWidget(one, 3, 3)
        two  = QtGui.QPushButton("2")
        grid.addWidget(two, 3, 4)
        three = QtGui.QPushButton("3") 
        grid.addWidget(three, 3, 5)
        four = QtGui.QPushButton("4")
        grid.addWidget(four, 2 ,3)
        five = QtGui.QPushButton("5")
        grid.addWidget(five, 2, 4)
        six = QtGui.QPushButton("6")
        grid.addWidget(six, 2, 5)
        seven = QtGui.QPushButton("7") 
        grid.addWidget(seven, 1, 3)
        eight = QtGui.QPushButton("8") 
        grid.addWidget(eight, 1, 4)
        nine = QtGui.QPushButton("9") 
        grid.addWidget(nine, 1, 5)

        numberButtons = [zero, one, two, three, four, five, six, seven, eight, nine]
        for i in range(len(numberButtons)):
            numberButtons[i].setStyleSheet("background-color: #282833; color: white;")
            numberButtons[i].setFixedSize(buttonH, buttonW)
            numberButtons[i].clicked.connect(self.numberButtonsClicked(i))


        saveButton = QtGui.QPushButton("Save and Exit")  
        saveButton.setStyleSheet("background-color: #282833; color: white;")
        #saveAndExitButton.setStyleSheet("background-color: #282833; font-size: 12pt; font-family: Arial; color: white; font-style: bold")
        saveButton.clicked.connect(self.saveAndExitButtonClicked)
        saveButton.setFixedSize(120,50)
        grid.addWidget(saveButton, 6, 0)
    
        cancelButton = QtGui.QPushButton("Cancel")
        cancelButton.setStyleSheet("background-color: #282833; color: white;")
        cancelButton.clicked.connect(self.cancelButtonClicked)
        cancelButton.setFixedSize(120, 50)
        grid.addWidget(cancelButton, 6, 1)




       #how will this send the information back to Choe's algorithm?
        self.setLayout(grid)


       # what happens when the buttons are clicked
    def numberButtonsClicked(self, i):
        return lambda _: self.current.setText(self.current.text()+"{0}".format(i))

    def clearButtonClicked(self):
        return lambda _: self.current.setText('')

    def decimalButtonClicked(self):
        print(".")

    def saveAndExitButtonClicked(self):
        # close window
        # TODO later figure out how this will talk to Chloe's algorith and update these values
        self.close()

    def cancelButtonClicked(self, event):
        # close window without udpating the values
        self.close()
