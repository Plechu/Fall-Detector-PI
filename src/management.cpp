#include <management.h>

void Management::errorHandler(String errorMessage, String component){
    display.clearDisplay();
    display.error(errorMessage, component);
    delay(2500);
    display.clearDisplay();
    display.restarting();
    delay(2500);
    ESP.restart();
}

void Management::powerOnGSM(){
    display.powerOn();
    if(!gsm.powerOn())
        errorHandler("Power on failed!", "GSM module");
}

bool Management::hasConfiguration(){
    comms.disableWiFi();
    comms.startServer();

    if(!comms.isConfigurated()){
        comms.enableWiFi();
        display.WiFi();
        return false;
    }
    return true;
}

bool Management::initializeAccelerometer(){
    if(accel.getStatus()){
        accel.setActualVector();
        return true;
    }
    else{
        errorHandler("Init failed!", "Accelerometer");
        return false;
    }
}

void Management::enableWiFi(){
    comms.enableWiFi();
    comms.refreshTimer();
    uint8_t battery = gsm.getBatteryLevel();
    display.clearDisplay();
    display.WiFi(comms.getSSID());
    display.battery(battery, false);
}

void Management::disableWiFi(){
    comms.disableWiFi();
    uint8_t battery = gsm.getBatteryLevel();
    display.clearDisplay();
    display.battery(battery);
}

bool Management::isWiFiEnabled(){
    return comms.isWiFiEnabled();
}

bool Management::isConfigurated(){
    return comms.isConfigurated();
}

void Management::handleClient(){
    comms.getClientHandler();
}

unsigned long Management::refreshedTime(){
    return comms.getRefreshedTime();
}

void Management::resetConfiguration(){
    display.clearDisplay();
    display.restarting();
    delay(5000);
    comms.deleteConfiguration();
}

void Management::updateBattery(){
    uint8_t battery = gsm.getBatteryLevel();
    display.clearDisplay();
    display.battery(battery);
}

bool Management::isFallDetected(){
    accel.setActualVector();
    return accel.isFall();
}

void Management::sendSMSAlerts(){
    for(auto &contact : comms.getContacts()){
        gsm.sendTextMessage("Wykryto upadek", "+48" + contact.second);
        delay(5000);
    }
}

void Management::actualizeAccountBalance(){
    if(gsm.getAccountBalance() == 0.0) gsm.checkAccountBalance(); // redundancja
    if(gsm.getAccountBalance() < 10){
        gsm.sendTextMessage("Na koncie pozostalo " + String(gsm.getAccountBalance()) + ". Doladuj konto", "+48" + comms.getContacts()[0].second); // wysylane informacji o doladowaniu do pierwszego numeru
    }
}

void Management::handleFall(){
    display.clearDisplay();
    display.success("Wykryto upadek", "Wysylanie SMS");
    sendSMSAlerts();
    actualizeAccountBalance();
}