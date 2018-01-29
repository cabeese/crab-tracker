SD Card Cloning
===============

Instructions on how to clone an SD card using the shell.

Note that when `diskN` is written, it should be replaced with the appropriate
disk name determined from the `diskutil list` command.
For example, `/dev/disk2`.

# Get image from SD card

1. Determine the device name with `diskutil list`

- Get the image with `sudo dd if=/dev/diskN of=ct-img-2018mmdd.dmg`
  (update the filename as appropriate).

# Image SD card from existing DMG file

1. Determine the device name with `diskutil list`

- Unmount the disk with `diskutil unmountDist /dev/diskN`

- Format disk if needed with `sudo newfs_msdos -F 16 /dev/diskN`

- Image card with `sudo dd if=./ct-img-2018mmdd.dmg of=/dev/diskN`
