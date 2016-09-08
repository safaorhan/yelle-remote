# yelle-remote
Connected air-conditioner remote of project Yelle

Yelle is a project developed in [Fikrimuhal](http://fikrimuhal.com) Head Quarters. It allows the team to control air-conditioning system via slack messages.

//TODO insert a link to blog post about the whole Yelle project.

See also yelle-backend to check how easy to implement a iot backend with nodejs and deployd.

//TODO insert links to yelle-backend and deployd.

## Hardware Specs
This program runs on chip esp8266 and with the help of [Arduino core for esp8266](https://github.com/esp8266/Arduino), it's developed in Arduino environment.

yelle-remote currently runs in [NodeMCU](http://nodemcu.com/index_en.html) which is an esp8266 based development board. However it's supposed to run in other esp8266 boards as well.

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

Please note that Vcc is connected to NodeMCU's Vin and GND in emmiter of Q1 is connected to NodeMCU's GND. I kept it this way to make the scheme easy on the eye.

Node mcu is powered via a usb power adapter.

![Photo of yelle-remote][yelle-remote]


[wiring-scheme]: https://cloud.githubusercontent.com/assets/4990386/18355942/81d9ffb2-75f3-11e6-9d7d-5a8b02e07ab7.png
[yelle-remote]: https://cloud.githubusercontent.com/assets/4990386/18359343/30fd484c-7602-11e6-8ac5-7f7c4dad7549.png
