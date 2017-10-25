#include <Arduino.h>

#ifdef _VARIANT_WAVESHARE_BLE400_
  #include <Wire.h>
  #include "angry_cookie_128x64_xbm.h"
  #define OLED_WIDTH 128
  #define OLED_HEIGHT 64
  #include <SSD1306Wire.h>
  SSD1306Wire oled(0x3C);
#else
  #include <SPI.h>
  #include "angry_cookie_64x32_xbm.h"
  #define OLED_WIDTH 64
  #define OLED_HEIGHT 32
  #include <SSD1306Spi.h>
  SSD1306Spi oled(OLED_RST, OLED_DC, OLED_CS);
#endif

uint32_t tButton;
#define debounceTime_ms 200

bool on_off_state = false;
bool wake_sleep_state = false;

void setup()
{
  Serial.begin(9600);
  Serial.println(__FILE__);
  
  #ifdef _VARIANT_WAVESHARE_BLE400_
  pinMode(PIN_BUTTON1, INPUT);
  #else
  pinMode(PIN_BUTTON1, INPUT_PULLUP);
  #endif
  pinMode(PIN_BUTTON2, INPUT_PULLUP);
  
  oled.setScreenSize(OLED_WIDTH, OLED_HEIGHT);
  oled.init(); 
  oled.drawXbm(14, 0, angry_cookie::width, angry_cookie::height, angry_cookie::bits);
  oled.display();
  
  oled.flipScreenVertically();
  oled.setTextAlignment(TEXT_ALIGN_LEFT);
  oled.setFont(ArialMT_Plain_10);
  oled.drawString(0,  0, "github.com/");
  oled.drawString(0, 10, "micooke");
  oled.drawString(0, 20, __TIME__);
  oled.drawRect(0, 0, OLED_WIDTH, OLED_HEIGHT);
  
  oled.display();
  
  tButton = millis();
}

void loop()
{
  if (millis() - tButton > debounceTime_ms)
  {
    if (!digitalRead(PIN_BUTTON1))
    {
      if (wake_sleep_state)
      {
        Serial.println("sleep");
        oled.sleep();
      }
      else
      {
        Serial.println("wake");
        oled.wake();
      }
      wake_sleep_state = !wake_sleep_state;

      tButton = millis();
    }
    #ifdef _VARIANT_WAVESHARE_BLE400_
    if (!digitalRead(PIN_BUTTON2))
    #else
    if (digitalRead(PIN_BUTTON2))
    #endif
    {
      if (on_off_state)
      {
        Serial.println("off");
        oled.displayOff();
      }
      else
      {
        Serial.println("on");
        oled.displayOn();
      }
      on_off_state = !on_off_state;

      tButton = millis();
    }
  }
  
  yield();
}