Compass Configuration 
======================

0. Wire the compass to the pi
```bash
Compass:      Pi:
VCC       ->  pin 1
GND       ->  pin 14
SCL       ->  pin 5
SDA       ->  pin 3

```

1. In a terminal type `sudo raspi-config`

- select option 5 `Interfacing Options`
- go to P5 I2C and press enter
- A screen will pop up that asks "Would you like the ARM I2C interface to be enabled?" press `select`

2. Type `vim /etc/modules`
- add the line `i2c-bcm2708`

3. Type `vim /boot/config.txt`
- uncomment or add the lines:
```bash
dtparam=i2c1=on
dtparam=i2c_arm=on 

```

4. Type `sudo reboot`

5. Bring up another terminal. Type `sudo i2cdetect -y 1`
- Should see the i2c address of the compass 1e, this means the pi recognizes the compass. 

