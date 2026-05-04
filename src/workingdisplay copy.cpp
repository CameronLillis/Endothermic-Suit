#include <RotaryEncoder.h>
#include <U8g2lib.h>
#include <Wire.h>

// --- Pin Definitions ---
constexpr int PIN_ENC_A = 2;
constexpr int PIN_ENC_B = 3;
constexpr int PIN_ENC_BTN = 4;

constexpr int PIN_CONF_BTN = 9;
constexpr int PIN_BACK_BTN = 10;

// --- Display (U8g2 renamed to "interface") ---
U8G2_SH1106_128X64_NONAME_F_HW_I2C interface(U8G2_R0, U8X8_PIN_NONE);

// --- Encoder ---
RotaryEncoder encoder(PIN_ENC_A, PIN_ENC_B, RotaryEncoder::LatchMode::FOUR3);

// --- Shared state (updated by ISR) ---
static volatile int encoderPosition = 0;

// --- Render throttle ---
static const unsigned long RENDER_INTERVAL_MS = 33;
static unsigned long lastRenderMs = 0;

// --- ISR ---
void encoderISR() {
  encoder.tick();
  int dir = (int)encoder.getDirection();
  if (dir != 0) {
    encoderPosition -= dir;
  }
}

// --- Render ---
void render(int pos, bool encBtn, bool confBtn, bool backBtn) {
  interface.clearBuffer();

  interface.setFont(u8g2_font_6x10_tr);

  interface.drawStr(0, 10, "Hardware Test");
  interface.drawHLine(0, 12, 128);

  char buf[32];

  sprintf(buf, "Encoder: %d", pos);
  interface.drawStr(0, 24, buf);

  interface.drawStr(0, 36, encBtn ? "Enc Btn: PRESSED" : "Enc Btn: ---");
  interface.drawStr(0, 46, confBtn ? "Conf Btn: PRESSED" : "Conf Btn: ---");
  interface.drawStr(0, 56, backBtn ? "Back Btn: PRESSED" : "Back Btn: ---");

  interface.sendBuffer();
}

// --- Setup ---
void setup() {
  Serial.begin(115200);
  delay(1000);

  interface.begin();

  pinMode(PIN_ENC_A, INPUT_PULLUP);
  pinMode(PIN_ENC_B, INPUT_PULLUP);
  pinMode(PIN_ENC_BTN, INPUT_PULLUP);
  pinMode(PIN_CONF_BTN, INPUT_PULLUP);
  pinMode(PIN_BACK_BTN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(PIN_ENC_A), encoderISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_B), encoderISR, CHANGE);

  render(0, false, false, false);
  Serial.println("Ready");
}

// --- Loop ---
void loop() {
  unsigned long now = millis(); // how many milliseconds since arduino started

  /*
    chat gpt says that this means only run the rest of the code every
    RENDER_INTERVAL_MS  (e.g. 33ms)

    - Without this, your loop would run:
    - thousands of times per second and spam the OLED constantly
  */
  if (now - lastRenderMs < RENDER_INTERVAL_MS)
    return;
  lastRenderMs = now;

  noInterrupts();
  int pos = encoderPosition;
  interrupts();

  bool encBtn = (digitalRead(PIN_ENC_BTN) == LOW);
  bool confBtn = (digitalRead(PIN_CONF_BTN) == LOW);
  bool backBtn = (digitalRead(PIN_BACK_BTN) == LOW);

  render(pos, encBtn, confBtn, backBtn);
}