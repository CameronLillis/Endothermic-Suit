#include <Wire.h>
#include <U8g2lib.h>

// SH1106 128x64 I2C display
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  Serial.begin(9600);
  u8g2.begin();
  Serial.println("SH1106 1.3\" OLED - Ready");
}

void loop() {
  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 12, "Hello from");

  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.drawStr(0, 36, "SH1106!");

  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 56, "1.3\" OLED 128x64");

  u8g2.sendBuffer();
  delay(2000);
}
