#include <SoftwareSerial.h>
#include <vector>

#define powerOnPin 16

class A9G{
    float accountBalance;
    uint8_t batteryLevel = 0;
    SoftwareSerial SS;

    String decodeHex(String hex); // konwersja wiadomosci USSD z kodowania HEX na GSM 7bit

    void checkBatteryLevel(); // sprawdzenie poziomu baterii

    String sendCommand(String command); // wysylanie komendy AT do modulu

public:
    A9G();

    void checkAccountBalance(); // sprawdzenie stanu konta za pomoca kodu USSD

    void sendTextMessage(String message, String phoneNumber); // wysylanie wiadomosci SMS
    
    float getAccountBalance(); // pobranie stanu konta

    uint8_t getBatteryLevel(); // pobranie poziomu baterii

    bool powerOn(); // wlaczenie modulu
};