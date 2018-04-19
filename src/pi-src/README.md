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
| DISPLAY_PINGS	| 0 or 1	| If truthy, pings are displayed as they come in 	|
| HPHONE_ADJ_DIST_CM	| >0	| The distance, in `cm`, between two adjacent hydrophones (that is, the shortest distance between two hydrophones in a square grid).	|
