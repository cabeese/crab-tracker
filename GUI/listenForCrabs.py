# Class that sits in a busy loop and spends its time waiting to reciev information about a crab's location so it can call all the other things that will display that crab on the screen
from crabDot import CrabDot

# have this be some sort of widget that overlays the radar screen\
# and draw each crab dot as information comes in
class ListenForCrabs():
    def __init__(self):
        #creates empty array of size 500
        self.crabDots = [None] * 500
        print("Listening for crabs!")

        #do file I/O on the test data
        testData = open("data.txt", "r")
        readData = testData.readlines()

        for line in readData:
            r, theta, UID = line.split(" ")
            self.crabDots[int(UID)] = CrabDot(int(r), int(theta), int(UID))
