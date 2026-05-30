# control-panel-perfex-mod
A modified Perfex emergency service performance testing unit.

Built with Arduino using an ESP32-S3.

## Dependencies

### Arduino
[OSC by CNMAT](https://github.com/CNMAT/OSC)
Enables OpenSoundControl (OSC) communication.

Manually install by opening the .ino file in Arduino IDE,

`Sketch > Include Library > Add .ZIP Library... >` [Select OSC-master.zip from the Libraries folder of this repo]

### Max/MSP (Optional, for testing or integrating only)

To use OSC with Max/MSP:

`File > Show Package Manager >` Search for "CNMAT"

Select and install `"CNMAT Externals by CNMAT"`

## Testing

To test OSC with Max/MSP:

See file `Tests > test-max-osc.maxpat` 