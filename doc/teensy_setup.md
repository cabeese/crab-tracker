# Downloading and Installing

(These steps may be incomplete)

Download CrossPack (formerly AVR MacPack)
https://www.obdev.at/products/crosspack/download.html

Download the Teensy CLI tool

# Compiling and Uploading

Write code in the Arduino editor.

From the "Tools" menu, select "Teensy 3.5" under the "Board" menu.

To compile, run `Sketch -> Export compiled Binary` and select the location
for the binary, if needed.

To upload, `cd` into the directory containing the `.hex` file that was just
exported in the step above, and then run

`teensy_loader_cli -mmcu=mk64fx512 -w <FILENAME>.ino.TEENSY35.hex`

The command will hang. Now press the reset button on the Teensy.
