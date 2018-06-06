Crab Tracker
============

## Configure, Compile, Run

1. Create a file in `/etc/crab-tracker.conf` for configuration parameters.
   Use the example file `../crab-tracker.conf.example` if needed.
2. Run `make`
3. Run `./crab-tracker`

## Configuration Parameters

| Parameter	| Values	| Meaning	|
|---	|---	|---	|
| DISPLAY_RAW_SPI	| 0 or 1	| If truthy, raw "SPI blocks" are displayed as they come in	|
| DISPLAY_PINGS	| >= 0	| Displays information about pings as they come in. See table below 	|
| HPHONE_ADJ_DIST_CM	| >0	| The distance, in `cm`, between two adjacent hydrophones (that is, the shortest distance between two hydrophones in a square grid).	|

#### `DISPLAY_PINGS` values

The value of `DISPLAY_PINGS` determines what kind of information is printed for each ping.
To determine what to print, simply sum the numbers associated with all desired fields and use that as the value for `DISPLAY_PINGS` in the config file.
A value of 0 will turn of ping printing.

| Value | Meaning                                    |
| ----- | ------------------------------------------ |
|   1   | Start time                                 |
|   2   | Duration                                   |
|   4   | UID                                        |
|   8   | Letter (A,B,C,D) instead of number for pin |

For example, to print out the _duration_ and the _letter_ representation of a ping, we would use the value 2+8=`10` in the config file.
