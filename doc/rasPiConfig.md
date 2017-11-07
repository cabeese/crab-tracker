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

This document will be updated as more configuration is added.
