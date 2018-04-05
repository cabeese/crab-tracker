
from PyQt4 import QtGui
#from PyQt4.QtGui import *
from radar import RadarWindow
from mainWindow import MainWindow
import sys

app = QtGui.QApplication(sys.argv)

# sets font type for entire application
font = QtGui.QFont("Arial")
font.setBold(True)
font.setPointSize(12)
app.setFont(font)
#app.setFont(QtGui.QFont("Arial"))


window = MainWindow()
# program ends when window closes
sys.exit(app.exec_())
