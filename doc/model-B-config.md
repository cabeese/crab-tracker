Model B Configuration
=====================

1. Download Stretch with Desktop from
https://www.raspberrypi.org/downloads/raspbian/

- Format the SD card as MSDOS-FAT

- Install the `img` file on the SD card

- Add an empty file called `ssh` in the root of the sd card.
(e.g. `touch /path/to/card/boot/ssh`)

- Boot the Pi and log into it remotely over the network

- Change the default password by running `passwd`

- Install `emacs` and `vim` (because we respect all beliefs on this team)
  with `sudo apt-get install emacs vim git`

- Enable SSH over USB (optional)
  - Open `/boot/config.txt` again and add `dtoverlay=dwc2` at the bottom
  - Open `/boot/cmdline.txt`. After `rootwait` (the last word on the first
      line), add a space and then `modules-load=dwc2,g_ether`.

- Enable SPI
  - Open `/boot/config.txt` and uncomment the line `dtparam=spi=on`.
  - Reboot
  - The device `/dev/spidev0.0` should appear

- Update packages for RasPi screen
```bash
sudo apt-get update
sudo apt-get upgrade
sudo apt-get dist-upgrade
sudo apt-get install raspberrypi-ui-mods
sudo apt-get install raspberrypi-net-mods
```

- Attach the screen (if not already connected) and reboot (with `reboot`)

An image of the SD card that configured up to this point is stored.
