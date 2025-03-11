# Lora over Serial

## Features
- Control, Tx, and Rx lora messages from any device with a serial connection! No need to hardwire SPI!
- Control, Tx and Rx MessageAPI messages in the same mannor!

## Commands (over serial)
Request|Command 
--|--
lora rx|           lora get
lora tx|          lora send 0x00 0x00....
msgAPI tx|         message send 0x00 0x00....
msgAPI rx|         message get
msgAPI set source| message set source 0x00
msgAPI set key|    message set key 0x00

## Debugging Notes
As defined in Pico Documenation, the USB subsystem cannot be run at the same time as the SWD debugger. The end result of this is that while debugging it will appear as though the interface is broken. There does seem to be a workaround for this (running the USB portion on a second core). However for this instance I have not done that hence you will run into issues if you attempt to debug. See this discussion post for more details: https://forums.raspberrypi.com/viewtopic.php?t=351814

## Build Instructions
```
1) make build directory and enter directory
2) Verify or set required defines:
    export PICO_SDK_PATH=/Users/natelenze/Developer/pico/pico-sdk
    export PICO_BOARD=pico_w
3) cmake ..
4) make all
```


## Hardware list 
- https://www.adafruit.com/product/3072
- https://www.raspberrypi.com/products/raspberry-pi-pico/
 
## Requirements
Currently this project has a number of dependecies that really arent needed (ie. Console Interface) mostly due to how I setup those classes. The goal is to remove these dependencies down the line. 

## Quick Start (Mac)
1) connect the pico device
2) open a serial connection w/ baudrate of 115200
3) away you go!

## Acknowledgments
Pico C SDK 