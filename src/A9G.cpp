#include <A9G.h>

String A9G::decodeHex(String hex){
    String decodedHex {};
    uint8_t retrivedByte {};
    uint16_t temp {};
    uint8_t cutedBits {};

    for(uint16_t i {}; i < hex.length(); i += 2){
        retrivedByte = (hex[i] >= '0' && hex[i] <= '9') ? hex[i] - '0' : hex[i] - 'A' + 10;
        retrivedByte <<= 4;
        retrivedByte |= (hex[i+1] >= '0' && hex[i+1] <= '9') ? hex[i+1] - '0' : hex[i+1] - 'A' + 10;

        temp |= retrivedByte << cutedBits;
        decodedHex += (char)(temp & 0x007F);
        temp >>= 7;
        if(cutedBits == 6){
            cutedBits = 0;
            decodedHex += (char)(temp & 0x007F);
            temp = 0;
        }
        else 
            ++cutedBits;
    }
    return decodedHex;
}

void A9G::checkBatteryLevel(){
    float previousBatteryLevel = -1;
    bool flag = false;
    while(!flag){
        String response = sendCommand("AT+CBC?");
        batteryLevel = response.substring(response.indexOf(",") + 1).toInt();
        (previousBatteryLevel == batteryLevel) ? flag = true : previousBatteryLevel = batteryLevel;
        delay(100);
    }
}

String A9G::sendCommand(String command){
    String response = "";
    uint16_t timeout = 1000; // timeout w ms
    unsigned long latchedTime = millis();

    SS.print(command + "\r");

    while((latchedTime + timeout) > millis()){
        while(SS.available()) 
            response += (char)SS.read();
    }
    return response;        
}

A9G::A9G(){
    pinMode(powerOnPin, OUTPUT);
    digitalWrite(powerOnPin, HIGH); 

    SS.begin(115200, 14, 12, SWSERIAL_8N1, false, 256);
}

void A9G::checkAccountBalance(){
    
    String tempString;
    sendCommand("AT+CSCS=\"HEX\"");
    sendCommand("AT+CUSD=1,\"*101#\",15");

    uint16_t timeout = 5000;
    unsigned long latchedTime = millis();
    while((latchedTime + timeout) > millis()){
        while (SS.available() > 0)
            tempString += (char)SS.read();
    }
    sendCommand("AT+CSCS=\"GSM\"");

    tempString = decodeHex(tempString.substring(tempString.indexOf("\"") + 1, tempString.lastIndexOf("\"")));

    for(uint8_t i {}; i < tempString.indexOf("zl"); ++i){
        if(tempString[i] >= '0' && tempString[i] <= '9'){
            tempString = tempString.substring(i, tempString.indexOf("zl"));
            tempString[tempString.indexOf(",")] = '.';
            break;
        }
    }

    accountBalance = tempString.toFloat();
}

void A9G::sendTextMessage(String message, String phoneNumber){ 
    sendCommand("AT+CMGF=1"); // format wiadomosci SMS, 1 dla Text
    sendCommand("AT+CMGS=\""+ phoneNumber + "\"");
    SS.print(message); // SMS
    SS.write(26); // wyslanie wiadomosci
}

float A9G::getAccountBalance(){
    return accountBalance;
}

uint8_t A9G::getBatteryLevel(){
    checkBatteryLevel();
    return batteryLevel;
}

bool A9G::powerOn(){
    digitalWrite(powerOnPin, LOW);
    delay(3000);
    digitalWrite(powerOnPin, HIGH);
    delay(5000);

    if(sendCommand("AT").indexOf("OK") != -1){ // jesli odpowiedz zawiera OK zwraca true
        checkBatteryLevel();
        return true;
    }
    return false;
}