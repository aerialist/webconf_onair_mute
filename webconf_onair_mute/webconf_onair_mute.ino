/* Web conference MUTE button
 * easy make with M5StickC
 * 
 * Requires:
 * * https://docs.m5stack.com/#/en/core/m5stickc
 * * https://github.com/T-vK/ESP32-BLE-Keyboard
 * 
 * You may want to apply authentification modification 
 * https://github.com/T-vK/ESP32-BLE-Keyboard/pull/28/commits/f9755a0035cc0fa9ee907f8fdcad5aa78ca9d60f
 */

#include <M5StickC.h>
#include <BleKeyboard.h>

BleKeyboard bleKeyboard;

enum ConfSystem {
  zoom = 0,
  webex
} conf;
int n_conf = 2;
bool on_air;

void setup() {
  M5.begin();
  bleKeyboard.begin();

  Serial.println("Webconf Mute button");
  Serial.println("Push Button A (M5) to toggle MUTE");
  Serial.println("Push Button B to switch Zoom or WebEx");
  M5.Lcd.setRotation(3); // BtnA is left side to LCD

  // default
  conf = zoom;
  on_air = true;
  update_display();
}

void loop() {
  M5.update();
  
  if(M5.BtnA.wasPressed()){
    Serial.println("BtnA was pressed");
    if(bleKeyboard.isConnected()){
      on_air = !on_air;
      if(conf==zoom){
        send_mute_zoom();
      }
      else if(conf==webex){
        send_mute_webex();
      }
    }
    else {
      Serial.println("BLE is not connected");
    }
    update_display();
  }

  if(M5.BtnB.wasPressed()){
    Serial.println("BtnB was pressed");
    conf = ConfSystem((conf+1) % n_conf);
    Serial.print("New ConfSystem: ");
    Serial.println(conf);
    update_display();
  }
  delay(10);
}

void update_display(){
  Serial.println("update_display");
  Serial.print("On air?: ");
  Serial.println(on_air);
  if(on_air){
    show_onair();
  }
  else {
    show_inmute();
  }
}

void show_onair(){
  M5.Lcd.fillScreen(TFT_RED);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_RED);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setTextDatum(MC_DATUM);
  M5.Lcd.drawString("ON AIR",80,40);

  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextDatum(BR_DATUM);
  if(conf==zoom){
    M5.Lcd.drawString("ZOOM",158,78);
  }
  else if (conf==webex){
    M5.Lcd.drawString("WebEx", 158,78);  
  }
}

void show_inmute(){
  M5.Lcd.fillScreen(TFT_DARKGREY);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_DARKGREY);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setTextDatum(MC_DATUM);
  M5.Lcd.drawString("Muted...",80,40);

  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextDatum(BR_DATUM);
  if(conf==zoom){
    M5.Lcd.drawString("ZOOM",158,78);
  }
  else if (conf==webex){
    M5.Lcd.drawString("WebEx", 158,78);  
  }
}

void send_mute_zoom(){
  // https://support.zoom.us/hc/en-us/articles/205683899-Hot-Keys-and-Keyboard-Shortcuts-for-Zoom
  // Command(⌘)+Shift+A: Mute/unmute audio
  Serial.println("Toggle mute on zoom");
  bleKeyboard.press(KEY_LEFT_GUI);
  bleKeyboard.press(KEY_LEFT_SHIFT);
  bleKeyboard.press(65); // "A"
  delay(100);
  bleKeyboard.releaseAll();
}

void send_mute_webex(){
  // https://www.cisco.com/c/en/us/td/docs/collaboration/CWMS/2_5/b_manage_meetings/b_manage_meetings_chapter_0100.html
  // Ctrl+M Mute or unmute yourself
  Serial.println("Toggle mute on WebEx");
  //bleKeyboard.press(KEY_LEFT_CTRL);
  bleKeyboard.press(KEY_LEFT_GUI);
  //bleKeyboard.press(KEY_LEFT_SHIFT);
  bleKeyboard.press(77); // "M"
  delay(100);
  bleKeyboard.releaseAll();
}
