Configuration for Project Raspberry Pis
=======================================

1. Download Raspian Stretch Lite from
https://www.raspberrypi.org/downloads/raspbian/

2. If you have the appropriate cables and hardware, boot the pi and log in via
keyboard + monitor to the shell.

3. Enable `ssh` by `cd`ing to the `/boot` sector and running `touch ssh`.

4. If `ssh` does not work after a reboot, log in again and run
`sudo systemctl enable ssh.service` and/or `sudo systemctl start ssh.service`.

5. Change your default password by running the `passwd` command!

Additionally, it may be useful to **enable SSH over a USB connection**.

6. Open `/boot/config.txt` again and add `dtoverlay=dwc2` at the bottom.

7. Open `/boot/cmdline.txt`. After `rootwait` (the last word on the first line),
add a space and then `modules-load=dwc2,g_ether`.

8. You can now connect the pi to a host computer via USB (be sure to use the
port labelled "USB," not "PWR") and ssh as usual (`ssh pi@raspberrypi.local`).

For more info on SSH over USB, see
[this article](https://learn.adafruit.com/turning-your-raspberry-pi-zero-into-a-usb-gadget/ethernet-gadget)

This document will be updated as more configuration is added.
