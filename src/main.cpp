#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RotaryEncoder.h>
#include <Wire.h>

// --- Pin Definitions ---
// Change these to match your wiring

// Rotary Encoder
constexpr int PIN_ENC_A = 6;
constexpr int PIN_ENC_B = 5;
constexpr int PIN_ENC_BTN = 7;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3D

// -- Globals ---
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RotaryEncoder encoder(PIN_ENC_A, PIN_ENC_B, RotaryEncoder::LatchMode::FOUR3);

// State updated by ISRs
static volatile int encoderPosition = 0;
static volatile bool encBtnPressed = false;
static volatile bool confBtnPressed = false;
static volatile bool backBtnPressed = false;

// 30 Hz render (~33ms)
static const unsigned long RENDER_INTERVAL_MS = 33;
static unsigned long lastRenderMs = 0;

// --- ISRs ---
void encoderISR() {
  encoder.tick();
  int dir = (int)encoder.getDirection();
  if (dir != 0) {
    encoderPosition += -dir;
  }
}

void encBtnISR() { encBtnPressed = (digitalRead(PIN_ENC_BTN) == LOW); }

// --- Render ---

void render(int pos, bool encBtn, bool confBtn, bool backBtn) {
  display.clearDisplay();
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.print("Hardware Test");
  display.drawLine(0, 10, 127, 10, 1);

  display.setCursor(0, 14);
  display.print("Encoder  ");
  display.print(pos);

  display.setCursor(0, 26);
  display.print("Enc Btn  ");
  display.print(encBtn ? "PRESSED" : "---");

  display.setCursor(0, 38);
  display.print("Conf Btn ");
  display.print(confBtn ? "PRESSED" : "---");

  display.setCursor(0, 50);
  display.print("Back Btn ");
  display.print(backBtn ? "PRESSED" : "---");

  display.display();
}

// --- Setup / Loop ---

void setup() {
  // Serial.begin(115200);
  // delay(1000);

  // Wire.begin();
  // Wire.setClock(400000);

  // if (!display.begin(0x3C, true)) {
  //   Serial.println("SSD1306 failed!");
  //   while (1)
  //     delay(1000);
  // }
  // display.clearDisplay();
  // display.setTextColor(SSD1306_WHITE);
  // display.display();

  // pinMode(PIN_ENC_A, INPUT_PULLUP);
  // pinMode(PIN_ENC_B, INPUT_PULLUP);
  // pinMode(PIN_ENC_BTN, INPUT_PULLUP);

  // attachInterrupt(digitalPinToInterrupt(PIN_ENC_A), encoderISR, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(PIN_ENC_B), encoderISR, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(PIN_ENC_BTN), encBtnISR, CHANGE);

  // render(0, false, false, false);
  // Serial.println("Ready");
}

void loop() {
  // unsigned long now = millis();
  // if (now - lastRenderMs < RENDER_INTERVAL_MS)
  //   return;
  // lastRenderMs = now;

  // noInterrupts();
  // int pos = encoderPosition;
  // bool encBtn = encBtnPressed;
  // bool confBtn = confBtnPressed;
  // bool backBtn = backBtnPressed;
  // interrupts();

  // render(pos, encBtn, confBtn, backBtn);
}
