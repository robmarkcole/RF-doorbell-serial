#include <RCSwitch.h>

/*
  Simple example for receiving
  
  https://github.com/sui77/rc-switch/
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(9600);
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
}

void loop() {
  if (mySwitch.available()) {
    int sensorValue = mySwitch.getReceivedValue();    
    if (sensorValue == 6146){
      Serial.print("Doorbell pressed at ");
      Serial.print(millis());       // Get milliseconds since code executiuon as no real world clock
      Serial.println();             // print next statement on new line
      delay(2000);                  // waits for a second
    }
    mySwitch.resetAvailable();
  }
}
