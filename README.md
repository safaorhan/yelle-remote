# yelle-remote
Connected air-conditioner remote of project Yelle

Yelle is a project developed in [Fikrimuhal](http://fikrimuhal.com) Head Quarters. It allows the team to control air-conditioning system via slack messages.

## Intro

This program runs on chip esp8266 and with the help of [Arduino core for esp8266](https://github.com/esp8266/Arduino), it's developed in Arduino environment.

yelle-remote currently runs in [NodeMCU](http://nodemcu.com/index_en.html) which is an esp8266 based development board. However it's supposed to run in other esp8266 boards as well.


## How to run yelle-remote?

To run the code you should follow these steps:

#### 1. Download the latest Arduino IDE

You can download the Arduino IDE to your computer from this link: https://www.arduino.cc/en/Main/Software

#### 2. Install ESP8266 Core for Arduino

The easiest way is to use Boards Manager. You can follow these instructions the install it: https://github.com/esp8266/Arduino#installing-with-boards-manager

#### 3. Download IRremoteESP8266 Library

This library provides a very easy to use interface to receive and send infrared waves via your ESP chip. It also has protocol implementations and command mappings for lots of brands.

So download this library and add to your Arduino sketch using Library Manager or any method you want. You can follow this instruction set in case you have trouble adding the library: https://www.arduino.cc/en/Guide/Libraries

#### 4. Run yelle.ino

Select your board, port and do other configuration. Now, you should be ready to go with yelle-remote!

## How to setup the hardware?

//TODO add a photo of board here..

### Used Components
* NodeMCU v1.0
* IR LED (SB-5010IRB-CT) x2
* General purpose transistor (NPN)
* 10R Resistor (could be differ according to transistor and IR LED of your choice)
* Power adapter (or any kind of 5V power source)
* Breadboard, jumpers, cables, etc.

### Wiring
Here is the wiring scheme of the hardware:
![Wiring scheme of the hardware][wiring-scheme]

Please note that Vcc is connected to NodeMCU's Vin and GND in emitter of the transistor is connected to NodeMCU's GND. I kept it this way to make the scheme easy on the eye.

Node mcu is powered via a usb power adapter.

![Photo of yelle-remote][yelle-remote]


[wiring-scheme]: https://cloud.githubusercontent.com/assets/4990386/18359557/1a5dd128-7603-11e6-842c-8a2fe17c6484.png
[yelle-remote]: https://cloud.githubusercontent.com/assets/4990386/18359343/30fd484c-7602-11e6-8ac5-7f7c4dad7549.png
