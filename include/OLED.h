#include <U8g2lib.h>

class OLED{

    U8X8_SH1106_128X64_NONAME_HW_I2C u8x8;

    void alignStringHorizontalCenter(uint8_t y, String text, uint8_t textSize = 1); // wysrodkowanie tekstu

    void alignGlyphHorizontalCenter(uint8_t y, uint8_t encoding, uint8_t glyphSize); // wysrodkowanie ikony

    void alignTwoGlyphsHorizontalCenter(uint8_t y, uint8_t encoding1, uint8_t encoding2, uint8_t glyphSize); // wysrodkowanie dwoch ikon

public:
    OLED();
    
    void clearDisplay(); // wyczyszczenie wyswietlacza

    void powerOn(); // wyswietlenie informacji o wlaczaniu urzadzenia

    void restarting(); // wyswietlenie informacji o restarcie urzadzenia

    void battery(uint8_t batteryLevel, bool fullScreen = true); // wyswietlenie stanu baterii

    void WiFi(String SSID = ""); // wyswietlenie informacji o sieci WiFi

    void success(String message, String additionalMessage = ""); // wyswietlenie informacji o sukcesie

    void error(String errorMessage, String component); // wyswietlenie informacji o bledzie
};