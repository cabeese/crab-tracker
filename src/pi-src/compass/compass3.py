#!/usr/bin/python2.7
# the one John helped with


# source: http://blog.bitify.co.uk/2013/11/connecting-and-calibrating-hmc5883l.html
# needed to run sudo apt-get install build-essential libi2c-dev i2c-tools python-dev libffi-dev
import smbus
import time
import math

bus = smbus.SMBus(1) #should be 1 on pi3
#bus = smbus.SMBus(0)
address = 0x1e


def read_byte(adr):
    return bus.read_byte_data(address, adr)

def read_word(adr):
    high = bus.read_byte_data(address, adr)
    low = bus.read_byte_data(address, adr+1)
    val = (high << 8) + low
    return val

def read_word_2c(adr):
    val = read_word(adr)
    if (val >= 0x8000):
        return -((65535 - val) + 1)
    else:
        return val

def write_byte(adr, value):
    bus.write_byte_data(address, adr, value)

#write_byte(0, 0b00000000) # Set to 8 samples @ 15Hz
#write_byte(1, 0b11111111) # 1.3 gain LSb / Gauss 1090 (default)
#write_byte(2, 0b00000000) # Continuous sampling

write_byte(0, 0b01110000) # Set to 8 samples @ 15Hz
write_byte(1, 0b00100000) # 1.3 gain LSb / Gauss 1090 (default)
write_byte(2, 0b00000000) # Continuous sampling

scale = 0.92
scale_x = (2.30/3.4)

x_offset = 170.0
#y_offset = 140.0
y_offset = -45.0

x_out = (read_word_2c(3) - x_offset) * scale
y_out = (read_word_2c(7) - y_offset) * scale
z_out = read_word_2c(5) * scale

#bearing  = math.atan2(y_out, x_out) 
#print "orignal bearing: ", bearing
#if (bearing < 0):
#    bearing += 2 * math.pi



#print "Bearing: ", bearing, math.degrees(bearing)

for i in range(0,500):
    x_out = (read_word_2c(3) - x_offset) * scale_x
    y_out = (read_word_2c(7) - y_offset)
    z_out = read_word_2c(5)
    
    bearing  = math.atan2(y_out, x_out) 
    if (bearing < 0): 
        bearing += 2 * math.pi
    
    #print x_out, y_out, (x_out * scale), (y_out * scale)
    print "Bearing: ", math.degrees(bearing), x_out, " ", y_out
    time.sleep(0.1)
