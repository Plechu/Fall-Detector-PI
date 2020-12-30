#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <FS.h>

class Communication{

    ESP8266WebServer server;
    bool configurated;
    bool wifiEnabled;
    unsigned long refreshedTime;
    String configData[2] = {"", ""};
    std::vector<std::pair<String, String>> contacts; // wektor przechowujacy wszystkie numery

    bool checkConfiguration(); // sprawdzenie czy urzadzenie jest skonfigurowane

    void pageFile(String filename); // wyslanie pliku

    void saveConfiguration(); // zapis konfiguracji

    void loadConfiguration(); // wczytanie konfiguracji

    void sendContacts(); // wyslanie kontaktow na strone internetowa

    void loadContacts(); // zaladowanie listy kontaktow do wektora z pamieci

    void saveContact(); // zapis kontaktu 

    void deleteContact(); // usuwanie kontaktu

public:
    Communication();

    void refreshTimer(); // odswiezenie licznika

    bool isConfigurated(); // zwraca informacje czy urzadzenie jest skonfigurowane

    bool isWiFiEnabled(); // zwraca informacje czy wlaczone jest WiFi

    String getSSID(); // zwraca ustawione SSID urzadzenia

    void enableWiFi(); // wlaczanie siec WiFi

    void disableWiFi(); // wylaczanie sieci WiFi

    void deleteConfiguration(); // usuwanie konfiguracji

    void startServer(); // wlaczanie serwera HTTP i ustawienie endpointow

    void getClientHandler(); // obsluzenie klienta

    unsigned long getRefreshedTime(); // zwraca licznik

    std::vector<std::pair<String, String>> getContacts(); // zwraca wektor kontaktow

};