# RF-doorbell-serial
Using an Arduino with RF receiver to detect when my doorbell has been pressed.

I purchased a cheap RF doorbell and receiver on Amazon. I wish to integrate it to Home-assistant so that I can receive mobile notifications when someone presses the doorbell. I previously did this using a dedicated raspberry pi that relayed RF events via MQTT. Here I use an Arduino relaying RF messages over the serial port. My reason for swapping pi for Arduino is mostly becuase it is overkill to use a pi and I would rather use mine for other projects. The reason for swapping MQTT for serial is that I can read from serial using Home-assistant and I now am not reliant on an MQTT server.

## Refs
* Arduino library https://github.com/sui77/rc-switch
* HA blog post on reading over the serial https://home-assistant.io/blog/2017/10/23/simple-analog-sensor/
* HA serial platform docs https://home-assistant.io/components/sensor.serial/

<img src="https://github.com/robmarkcole/RF-doorbell-serial/blob/master/Arduino-rf-remote.JPG">
