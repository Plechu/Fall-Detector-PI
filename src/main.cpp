#include "management.h"

#define timeOut 60000
Management device;
bool buttonFlag = false; 
unsigned long latchedTime = millis();


void ICACHE_RAM_ATTR handleButton(){ // przerwanie wywolane przez nacisniecie przycisku pomiaru
  detachInterrupt(digitalPinToInterrupt(0));
  latchedTime = millis();
  buttonFlag = true;
}

void setup(){
  if(device.hasConfiguration()){
    attachInterrupt(digitalPinToInterrupt(0), handleButton, FALLING);
    device.initializeAccelerometer();
    device.powerOnGSM();
    device.updateBattery();
  }
}

void loop(){
  if (device.isConfigurated()){

    if(millis() - latchedTime > timeOut && !device.isWiFiEnabled()){
      latchedTime = millis();
      device.updateBattery();
    }
    
    if (millis() - latchedTime > 250 && buttonFlag && digitalRead(0) == HIGH){
      device.enableWiFi();
      buttonFlag = false;
    }

    if (millis() - device.refreshedTime() > timeOut && device.isWiFiEnabled()){ // wlaczenie przycisku po timeoucie
      device.disableWiFi();
      attachInterrupt(digitalPinToInterrupt(0), handleButton, FALLING);
    }

    if (millis() - latchedTime > 5000 && buttonFlag && !device.isWiFiEnabled() && digitalRead(0) == LOW){
      device.resetConfiguration();
    }

    if (device.isWiFiEnabled())
      device.handleClient();
    else if(device.isFallDetected())
      device.handleFall();
  }
  else 
    device.handleClient();
}
