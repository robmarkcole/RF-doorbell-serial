# RF-doorbell-serial
Using an Arduino with RF receiver to detect when my doorbell has been pressed.

I purchased a cheap RF doorbell and receiver on Amazon. I wish to integrate it to Home-assistant so that I can receive mobile notifications when someone presses the doorbell. I previously did this using a dedicated raspberry pi that relayed RF events via MQTT. Here I use an Arduino relaying RF messages over the serial port. My reason for swapping pi for Arduino is mostly because it is overkill to use a pi and I would rather use mine for other projects. The reason for swapping MQTT for serial is that I can read from serial using Home-assistant and I now am not reliant on an MQTT server.

## Usage
I'm using a Mac. To check which port the Arduino is on I run:
```
ls /dev/cu.*
```
In my case the Arduino is connected on **/dev/cu.usbmodem14341**. I now use [screen](https://geekinc.ca/using-screen-as-a-serial-terminal-on-mac-os-x/) to check the data transmitted over the serial when the Arduino receives an RF input. In the terminal I launch screen with:
```
screen /dev/cu.usbmodem14341 9600
```
I then press the doorbell remote and see **Doorbell pressed at 730**. I then kill the screen session using **ctrl + a + k**, and can now configure Home-assistant to display the message from the Arduino. I add to my Home-assistant config:
```
sensor:
  - platform: serial
    serial_port: /dev/cu.usbmodem14341
```
I restart Home-assistant and confirm that the state of this sensor is the message sent from the Arduino. Now I am only interested in displaying when the doorbell is pressed, and I would like a binary sensor which is ON when the doorbell is pressed. I will use a [little trick](https://community.home-assistant.io/t/binary-sensor-triggered-after-time-in-state/35826) I learned recently, of creating an [input_boolean](https://home-assistant.io/components/input_boolean/), and using an automation to toggle this ON/OFF when the state of the serial sensor changes. I first add the input_boolean to my Home-assistant config. However I wan't to see a boolean indicator on the front-end (rather than the input-slider) so I also add a [template binary sensor](https://home-assistant.io/components/binary_sensor.template/) and restart Home-assistant to make the changes take effect.
```
input_boolean:
  doorbell:
    name: Doorbell
    icon: mdi:alarm-bell

binary_sensor:
  - platform: template
    sensors:
      doorbell:
        value_template: "{{ is_state('input_boolean.doorbell', 'on') }}"
```
I now use the automations editor to create automations to toggle the input_boolean when the serial sensor changes state (after a doorbell button press). Additionally I publish ON/OFF on the MQTT topic 'doorbell' so that a remote HA instance can receive the doorbell state. I now have in automations.yaml.
```
- action:
  - service: input_boolean.turn_on
  - alias: ''
    data:
      payload: 'ON'
      topic: doorbell
    service: mqtt.publish
  alias: Doorbell_on
  condition: []
  id: '1513433912908'
  trigger:
  - entity_id: sensor.serial_sensor
    platform: state
- action:
  - service: input_boolean.turn_off
  - data:
      payload: 'OFF'
      topic: doorbell
    service: mqtt.publish
  alias: Doorbell_off
  condition: []
  id: '1513433982733'
  trigger:
  - entity_id: input_boolean.doorbell
    for:
      seconds: 2
    platform: state
    to: 'on'
```
I now have a boolean on my Home-assistant front end which turns on when the doorbell is pressed. I can also view the state of the doorbell on a remote HA instance using an [MQTT binary sensor](https://home-assistant.io/components/binary_sensor.mqtt/), with the following in my config:
```
binary_sensor:
  - platform: mqtt
    name: "Doorbell"
    state_topic: "doorbell"
```

## Hassio
I'm running Hassio on a pi3. To check the serial port that the Arduino is connected on I SSH into the Hassio and run **hassio host hardware**, and find that the Arduino is on **/dev/ttyACM0**.

## Synology
My main HA instance runs in Docker on a Synology. To try https://philhawthorne.com/installing-home-assistant-io-on-a-synology-diskstation-nas/

## Refs
* Doorbell https://www.amazon.co.uk/gp/product/B01LQBRJFA/ref=oh_aui_detailpage_o01_s00?ie=UTF8&psc=1
* Arduino library https://github.com/sui77/rc-switch
* HA blog post on reading over the serial https://home-assistant.io/blog/2017/10/23/simple-analog-sensor/
* HA serial platform docs https://home-assistant.io/components/sensor.serial/

<img src="https://github.com/robmarkcole/RF-doorbell-serial/blob/master/Arduino-rf-remote.JPG">
