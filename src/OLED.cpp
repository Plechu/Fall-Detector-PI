#include <OLED.h>

#define DISPLAY_WIDTH 16
#define DISPLAY_HEIGHT 8

void OLED::alignStringHorizontalCenter(uint8_t y, String text, uint8_t textSize){ 
    u8x8.drawString((DISPLAY_WIDTH  - text.length() * textSize)/2, y, text.c_str());
    }

void OLED::alignGlyphHorizontalCenter(uint8_t y, uint8_t encoding, uint8_t glyphSize){ 
    u8x8.drawGlyph((DISPLAY_WIDTH - glyphSize)/2, y, encoding); 
    }

void OLED::alignTwoGlyphsHorizontalCenter(uint8_t y, uint8_t encoding1, uint8_t encoding2, uint8_t glyphSize){
    u8x8.drawGlyph((DISPLAY_WIDTH - glyphSize*2)/2, y, encoding1);
    u8x8.drawGlyph((DISPLAY_WIDTH - glyphSize*2)/2 + glyphSize, y, encoding2);
}

OLED::OLED(){
    u8x8.begin();
    u8x8.setPowerSave(0);
}
void OLED::clearDisplay(){ 
    u8x8.clearDisplay(); 
    }

void OLED::powerOn(){
        u8x8.setFont(u8x8_font_open_iconic_embedded_4x4);
        alignGlyphHorizontalCenter(0, 78, 4);
        u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
        alignStringHorizontalCenter(5, "Startuje");
        alignStringHorizontalCenter(7, "Prosze czekac...");
}

void OLED::success(String message, String additionalMessage){
    u8x8.setFont(u8x8_font_open_iconic_check_4x4);
    alignGlyphHorizontalCenter(0, 64, 4);
    u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
    alignStringHorizontalCenter(5, message, 1);
    if(additionalMessage != ""){
        u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
        alignStringHorizontalCenter(5, message, 1);
        alignStringHorizontalCenter(7, additionalMessage, 1);
    }else{ 
        u8x8.setFont(u8x8_font_8x13B_1x2_f);
        alignStringHorizontalCenter(5, message, 1);
    } 
}

void OLED::restarting(){
        u8x8.setFont(u8x8_font_open_iconic_embedded_4x4);
        alignGlyphHorizontalCenter(0, 79, 4);
        u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
        alignStringHorizontalCenter(5, "Restartuje");
        alignStringHorizontalCenter(7, "Prosze czekac...");
}

void OLED::battery(uint8_t batteryLevel, bool fullScreen){
    String strBatteryLevel = String(batteryLevel) + "%";

    if(fullScreen){
        u8x8.setFont(u8x8_font_open_iconic_embedded_4x4);
        (batteryLevel > 25) ? alignGlyphHorizontalCenter(0, 64, 4) : alignTwoGlyphsHorizontalCenter(0, 67, 64, 4);
        u8x8.setFont(u8x8_font_inr21_2x4_f);
        alignStringHorizontalCenter(4, strBatteryLevel.c_str(), 2);
        
    }else{
        u8x8.setFont(u8x8_font_open_iconic_embedded_2x2);
        (batteryLevel > 25) ? u8x8.drawGlyph(DISPLAY_WIDTH - strBatteryLevel.length() - 2, 0, 64)
        : u8x8.drawGlyph(DISPLAY_WIDTH - strBatteryLevel.length() - 4, 0, 67), u8x8.drawGlyph(DISPLAY_WIDTH - strBatteryLevel.length() - 2, 0, 64);
        u8x8.setFont(u8x8_font_8x13B_1x2_f);
        u8x8.drawString(DISPLAY_WIDTH - strBatteryLevel.length(), 0, strBatteryLevel.c_str());
    }
}

void OLED::WiFi(String SSID){
    u8x8.setFont(u8x8_font_open_iconic_embedded_2x2);
    u8x8.drawGlyph(0, 0, 80);

    if(SSID != ""){
        u8x8.setFont(u8x8_font_8x13B_1x2_f);
        alignStringHorizontalCenter(4, SSID);
        u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
        alignStringHorizontalCenter(2, "SSID");
        alignStringHorizontalCenter(7, "IP 192.168.1.1");
    }else{
        u8x8.drawGlyph(14, 0, 66);
        u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
        alignStringHorizontalCenter(1, "SSID");
        alignStringHorizontalCenter(2, "ConfigFD");
        alignStringHorizontalCenter(4, "Haslo"); 
        alignStringHorizontalCenter(5, "qlihjwpzs7");
        alignStringHorizontalCenter(7, "IP 192.168.1.1");
    }
}

void OLED::error(String errorMessage, String component){
    u8x8.setFont(u8x8_font_open_iconic_embedded_4x4);
    alignGlyphHorizontalCenter(0, 71, 4);
    u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
    alignStringHorizontalCenter(5, errorMessage, 1);
    alignStringHorizontalCenter(7, component, 1);
}