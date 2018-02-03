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
  with `sudo apt-get install emacs vim`
### Enable SPI
- Open `/boot/config.txt` and uncomment the line `dtparam=spi=on`.

- Reboot

- The device `/dev/spidev0.0` should appear
### Install screen tools

- Update packages for RasPi screen
```bash
sudo apt-get update
sudo apt-get upgrade
sudo apt-get dist-upgrade
sudo apt-get install raspberrypi-ui-mods
sudo apt-get install raspberrypi-net-mods
```

- Attach the screen (if not already connected) and reboot (with `reboot`).
**Note:** An image of the SD card that configured up to this point is stored.
### Change Hostname

- Run `hostname` to see the current hostname (optional).

- Run `sudo hostnamectl set-hostname ct-pi-N` and replace `N` with an
    appropriate digit based on which device you have.
    (We started counting up from 0.)
### Set up SSH Keys and Clone Repo

- Generate a new key with `ssh-keygen -t rsa -b 4096 -C ""`

- Run `cat ~/.ssh/id_rsa.pub` and copy the whole key

- Add this at https://github.com/cabeese/crab-tracker/settings/keys/new (may need admin/owner access to repo)

- On the Pi, run `git clone git@github.com:cabeese/crab-tracker.git`
