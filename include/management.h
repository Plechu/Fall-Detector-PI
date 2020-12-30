#include "A9G.h"
#include "communication.h"
#include "OLED.h"
#include "accelerometer.h"


class Management{
    Communication comms;
    A9G gsm;
    OLED display;
    Accelerometer accel;

    void errorHandler(String errorMessage, String component); // obsluzenie bledu komponentow

public:
    void powerOnGSM(); // wlaczenie modulu GSM

    bool hasConfiguration(); // sprawdzenie obecnosci konfiguracji

    bool initializeAccelerometer(); // inicjalizacja akcelerometru

    void enableWiFi(); // wlaczenie WiFi

    void disableWiFi(); // wylaczenie WiFi

    bool isWiFiEnabled(); // zwraca informacje czy WiFi jest wlaczone

    bool isConfigurated(); // zwraca informacje czy urzadzenie jest skonfigurowane

    void handleClient(); // obsluga klienta

    unsigned long refreshedTime(); // zwraca licznik

    void resetConfiguration(); // usuwanie konfiguracji

    void updateBattery(); // aktualizacja stanu baterii

    bool isFallDetected(); // zwraca informacje czy wystapil upadek

    void sendSMSAlerts(); // wysylanie wiadomosci SMS

    void actualizeAccountBalance(); // aktualizacja stanu konta

    void handleFall(); // obsluga wystapienia upadku

};

