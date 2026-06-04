# control-panel-perfex-mod
A modified Perfex emergency service performance testing unit.

Built with Arduino using an ESP32-S3.


The main Arduino file is `control-panel-perfex-mod/control_panel_perfex_mod.ino`

## Installations/Dependencies

### Arduino

#### 1. Install the Arduino IDE

Download link: [https://www.arduino.cc/en/software/](https://www.arduino.cc/en/software/)

#### 2. Install support for the ESP32-S3 board (in Arduino IDE)

`Boards Manager` > Install `esp32 by Espressif Systems`

#### 3. Install the OSC by CNMAT library (in Arduino IDE)

[OSC by CNMAT](https://github.com/CNMAT/OSC) enables OpenSoundControl (OSC) communication. 

You can download the OSC library directly from Github, or find it in this repo under `Libraries > OSC-master.zip`

Then, manually install by opening the .ino file in Arduino IDE,

`Sketch > Include Library > Add .ZIP Library... >` [Select OSC-master.zip from the Libraries folder of this repo]


## Testing and Integrating

The control panel sends out OSC broadcasts as described below:

![2026-06-04_Control_Panel_Feature_Map_OSC_v1.PNG](Doc/2026-06-04_Control_Panel_Feature_Map_OSC_v1.png)

### Max/MSP

To use OSC within Max/MSP:

See file `Tests > test-max-osc.maxpat` for an example use case with Max/MSP

Install the CNMAT Externals by CNMAT library if you don't already have it:

1. In the top toolbar in your Max/MSP patch, go to `File > Show Package Manager >` Search for "CNMAT"

2. Select and install `"CNMAT Externals by CNMAT"`

![2026-05-29_Control_Panel_OSC_example_maxmsp.PNG](Doc/2026-05-29_Control_Panel_OSC_example_maxmsp.png)

### Unity

Official OSC Protocol Support for Unity Editor

[https://github.com/Unity-Technologies/UnityOSCProtocolSupport](https://github.com/Unity-Technologies/UnityOSCProtocolSupport)