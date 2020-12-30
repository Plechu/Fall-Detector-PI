#include <communication.h>

void Communication::refreshTimer(){
        refreshedTime = millis();
    }

    bool Communication::checkConfiguration(){
        SPIFFS.begin();
        return SPIFFS.exists("/contacts.txt") && SPIFFS.exists("/conf.txt");
    }

    void Communication::pageFile(String filename){
        refreshTimer();
        SPIFFS.begin();
        File file = SPIFFS.open("/" + filename, "r");
        (filename.endsWith("html")) ? server.streamFile(file, "text/html") : server.streamFile(file, "image/svg+xml");
        file.close();
    }

    void Communication::saveConfiguration(){
        if(server.args()){ 
            StaticJsonDocument<256> input;
            deserializeJson(input, server.arg(0));
            String SSID = input["SSID"];
            String Password = input["Pass"];
            String PhoneName = input["PhoneName"];
            String PhoneNumber = input["PhoneNumber"];

            for(uint8_t i = 0; i < PhoneNumber.length(); ++i){
                if(!isDigit(PhoneNumber[i])){
                    refreshTimer();
                    server.send(400);
                    return;
                }
            }
                
            if(SSID == "" || SSID.length() > 16 || Password.length() < 8 || Password.length() > 63 || PhoneNumber.length() != 9 || PhoneName == "" || PhoneName.length() > 30){
                refreshTimer();
                server.send(400);
                return;
            }
                
            SPIFFS.begin();
            File conf = SPIFFS.open("/conf.txt", "w");
            File phones = SPIFFS.open("/contacts.txt", "w");
            conf.print(SSID + '\x03' + Password + '\n');
            conf.close();
            phones.print(PhoneName + '\x03' + PhoneNumber + '\n');
            phones.close();
            
            server.send(200);
            delay(1000);
            ESP.restart();
        } 
        else server.send(400);
    }

    void Communication::loadConfiguration(){
        SPIFFS.begin();
        File file = SPIFFS.open("/conf.txt", "r");
        configData[0] = file.readStringUntil('\x03');
        configData[1] = file.readStringUntil('\n'); 
        file.close();
    }

    void Communication::sendContacts(){
        String serialized;
        StaticJsonDocument<512> output;
        
        for(uint8_t i = 0; i < contacts.size(); ++i){
            output[i]["Name"] = contacts[i].first;
            output[i]["Phone"] = contacts[i].second;
        }
        serializeJson(output, serialized);
        server.send(200, "application/json", serialized);
    }

    void Communication::loadContacts(){
        SPIFFS.begin();
        File file = SPIFFS.open("/contacts.txt", "r");
        
        while(file.available()){
            String name = file.readStringUntil('\x03');
            String number = file.readStringUntil('\n');
            contacts.emplace_back(name, number);
        }
        
        file.close();
    }

    void Communication::saveContact(){
        refreshTimer();
        if(server.args() && contacts.size() <= 5){
            StaticJsonDocument<128> input;
            deserializeJson(input, server.arg(0));
            String newName = input["newName"];
            String newPhone = input["newPhone"];

            if(newPhone.length() != 9 || newName.length() > 30 || newName == ""){ // jesli nie spelnia wymagan
                refreshTimer();
                server.send(400);
                return;
            }

            for(uint8_t i = 0; i < contacts.size(); ++i){ // jesli taki numer/nazwa istnieje w ksiazce telefonicznej
                if(contacts[i].first == newName || contacts[i].second == newPhone){
                    refreshTimer();
                    server.send(400);
                    return;
                }
            }

            SPIFFS.begin();
            File file = SPIFFS.open("/contacts.txt", "a");
            file.print(newName + '\x03' + newPhone + '\n');
            file.close();
            contacts.emplace_back(newName, newPhone);
        }
        server.send(200);
    }

    void Communication::deleteContact(){
        refreshTimer();
        if(server.args()){ 
            StaticJsonDocument<32> input;
            deserializeJson(input, server.arg(0));
            String phone2delete = input["phone"];
            if(contacts.size() == 1){ 
                server.send(406);
                return;
            }
            
            SPIFFS.begin();
            File file = SPIFFS.open("/contacts.txt", "w");
            uint8_t index = -1;
            bool flag = false;

            for(uint8_t i = 0; i < contacts.size(); ++i) // petla zapisujaca nieusuniete telefony
                (phone2delete == contacts[i].second) ? index = i : file.print(contacts[i].first + '\x03' + contacts[i].second + '\n');
            
            if(index != -1) 
                contacts.erase(contacts.begin() + index); // usuniecie numeru z vectora , jesli flaga false to znaczy ze nie ma takiego numeru
        
            file.close();
        }
        server.send(200);
    }

    Communication::Communication(){
        configurated = checkConfiguration();
        if(configurated){
            disableWiFi();
            loadConfiguration();
            loadContacts();
        }else
            enableWiFi();

        startServer();
    }

    bool Communication::isConfigurated(){
        return configurated;
    }

    bool Communication::isWiFiEnabled(){
        return wifiEnabled;
    }

    String Communication::getSSID(){
        return configData[0];
    }

    void Communication::enableWiFi(){
        WiFi.mode(WIFI_AP);
        WiFi.softAPConfig(IPAddress(192,168,1,1), IPAddress(192,168,1,1), IPAddress(255,255,255,0));
        (configurated) ? WiFi.softAP(configData[0], configData[1], 1, 0, 1) : WiFi.softAP("ConfigFD", "qlihjwpzs7", 1, 0, 1);
        wifiEnabled = true;
    }

    void Communication::disableWiFi(){
        WiFi.softAPdisconnect();
        WiFi.mode(WIFI_STA);
        wifiEnabled = false;
    }

    void Communication::deleteConfiguration(){
        SPIFFS.begin();
        SPIFFS.remove("/contacts.txt"); 
        SPIFFS.remove("/conf.txt");
        ESP.restart();
    }

    void Communication::startServer(){
        if(configurated){
            server.on("/", [&](){ pageFile("index.html"); });
            server.on("/trash.svg", [&](){ pageFile("trash.svg"); });
            server.on("/listcontacts", [&](){ sendContacts(); });
            server.on("/addcontact", [&](){ saveContact(); });
            server.on("/deletecontact", [&](){ deleteContact(); });
        }
        else{
            server.on("/", [&](){ pageFile("configuration.html"); }); // glowna strona konfiguracji
            server.on("/saveconfiguration", [&](){ saveConfiguration(); }); // tutaj zbierane sa dane konfiguracyjne
        }
        server.begin();
    }

    void Communication::getClientHandler(){
        server.handleClient();
    }

    unsigned long Communication::getRefreshedTime(){
        return refreshedTime;
    }

    std::vector<std::pair<String, String>> Communication::getContacts(){
        return contacts;
    }