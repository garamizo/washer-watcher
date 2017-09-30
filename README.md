# Introduction
Washing clothes is an a weekly activity all people must go through. That is, if you are not filthy rich, or just filthy. Otherwise, in a common home, multiple residents have to share the washing machine, each resident have to use the resources time-efficiently. The clothes have to be timely transferred from the washer to the dryer, to the drawers, so that all can live happy and in harmony in their homes.

Washing machines cycles consistently follow a function of load size, allowing to predict the end of the cycle with a simple watch. Old dogs in the house have determined how long the washing cycle spans, for any of the load settings. However, new residents do not have the same insight, so they are often perceived as a lazy villains that disrupt the harmony in the home.

We addressed this problem by developing the Washer-Watcher: a low-cost system that can inform all the residents the progress of the machine cycle. This device uses a accelerometer and a wifi-enabled microcontroller to measure the vibration level of the machine and broadcast it to a IoT channel on the cloud. The system is small, allowing it to be placed on top of any washing machine; and energy self-sufficient, not requiring a power cord.

[[images/graph.png]]

# Methods
## Materials
* IMU, MPU6050
* Wifi-enabled microcontroller, Feather HUZZAH ESP8266
* 3.7 V Lipo battery (JST jack)
* Breadboard and jumper wires

[[images/hardware.png]]

## Required Arduino libraries
1. [HUZZAH board library](https://learn.adafruit.com/adafruit-feather-huzzah-esp8266/using-arduino-ide)
2. [MPU6050 library](https://github.com/adafruit/Adafruit_BNO055)
3. [Adafruit sensor library](https://github.com/adafruit/Adafruit_Sensor)
4. [Thingspeak library](https://github.com/mathworks/thingspeak-arduino)

## Setup IoT channel
Create analog channel with ThingSpeak