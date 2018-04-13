# has x,y offset for the compass having a different orientation on the pi
#!/usr/bin/python
import smbus
import time
import math

bus = smbus.SMBus(1)
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

write_byte(0, 0b01110000) # Set to 8 samples @ 15Hz
write_byte(1, 0b00100000) # 1.3 gain LSb / Gauss 1090 (default)
write_byte(2, 0b00000000) # Continuous sampling

scale = 0.92

x_offset = 153
y_offset = 352

#for i in range(0,500):
while(True):
    x_out = (read_word_2c(3) - x_offset) * scale
    y_out = (read_word_2c(7) - y_offset) * scale
    z_out = read_word_2c(5) * scale

    bearing  = math.atan2(y_out, x_out) 
    
    #print "orignial bearing: ", bearing,
    adjustment = 90 * (math.pi / 180) # in the new orientation the direction of north seems to be off by about 45 degrees
    #print "bearing + adjustment: ", bearing+adjustment,
    #print math.degrees(adjustment), 

   # if (bearing < 0):
   #     bearing += 2 * math.pi
   # 
    adjustedBearing = bearing + adjustment

    if (adjustedBearing < 0):
        adjustedBearing += 2 * math.pi
    if (adjustedBearing > 2*math.pi):
        adjustedBearing -= 2*math.pi

    
    time.sleep(0.2)    
   # print "Original Bearing: ", math.degrees(bearing), "Adjusted Bearing: ", math.degrees(bearing + adjustment)
    print "adjusted Heading: ", math.degrees(adjustedBearing)
