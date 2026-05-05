#include <DallasTemperature.h>
#include <OneWire.h>
#include <PID_v1.h>
#include <RotaryEncoder.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <Wire.h>

// TODO: Make PID more aggressive on downturn

// GLOBALS
// #define MOSFET_PIN 1

//*---- Temp Sensor ---- *//
#define ONE_WIRE_BUS 7

// OneWire / Temperature setup
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//*---- PID ---- *//f
double setTemp, tempReadC, tempReadF, Output;
double Kp = 4, Ki = 7, Kd = 2;
const int maxPower = 255; // pwm max power

#define MAX_SET_TEMP 80.00
#define MIN_SET_TEMP 65.00
#define MIN_OFFSET -5
#define MAX_OFFSET 10

PID myPID(&tempReadF, &Output, &setTemp, Kp, Ki, Kd, REVERSE);

//*---- Encoder ----*//
constexpr int PIN_ENC_A = 2;
constexpr int PIN_ENC_B = 3;
constexpr int PIN_ENC_BTN = 4;

RotaryEncoder encoder(PIN_ENC_A, PIN_ENC_B, RotaryEncoder::LatchMode::FOUR3);

// UI Libary
U8G2_SH1106_128X64_NONAME_1_HW_I2C interface(U8G2_R0, U8X8_PIN_NONE);

// Shared state (updated by ISR)
static volatile int encoderPosition = 0;

// Render throttle
static const unsigned long RENDER_INTERVAL_MS = 33;
static unsigned long lastRenderMs = 0;


// ISR
void encoderISR() {
  encoder.tick();
  int dir = (int)encoder.getDirection();
  if (dir != 0) {
    encoderPosition -= dir;
  }
}

void render(int setTemp, double tempF, bool encBtn) {
  char buf[24];
  char tempStr[10];

  interface.firstPage();

  do {
    interface.setFont(u8g2_font_ncenB10_tr);

    // interface.drawStr(0, 10, "Hardware Test");

    dtostrf(tempF, 5, 2, tempStr);
    snprintf(buf, sizeof(buf), "Temp: %s F", tempStr);
    interface.drawStr(12, 24, buf);

    snprintf(buf, sizeof(buf), "Set Temp: %d", setTemp);
    interface.drawStr(12, 64, buf);

    // interface.drawStr(0, 48, encBtn ? "Enc Btn: PRESSED" : "Enc Btn: ---");

  } while (interface.nextPage());
}

void setup() {
  Serial.begin(9600);

  sensors.begin();
  interface.begin();
  Wire.begin();

  Wire.setClock(400000);
  // pinMode(MOSFET_PIN, OUTPUT);

  myPID.SetOutputLimits(0, maxPower);
  myPID.SetMode(AUTOMATIC);

  pinMode(PIN_ENC_A, INPUT_PULLUP); // pos right
  pinMode(PIN_ENC_B, INPUT_PULLUP); // neg left
  pinMode(PIN_ENC_BTN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(PIN_ENC_A), encoderISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_B), encoderISR, CHANGE);

  render(0, 0, false);
  Serial.println("Ready");
}

void loop() {
  unsigned long now = millis(); // how many milliseconds since arduino started

  // Read temperature
  sensors.requestTemperatures();
  tempReadC = sensors.getTempCByIndex(0);
  tempReadF = sensors.getTempFByIndex(0);

  noInterrupts();
  int pos = encoderPosition;
  interrupts();

  if (setTemp < MIN_SET_TEMP && pos < MIN_OFFSET) {
    setTemp = 65.0;
    pos = MIN_OFFSET;

    noInterrupts();
    encoderPosition = MIN_OFFSET;
    interrupts();
  }

  if (setTemp > MAX_SET_TEMP && pos > MAX_OFFSET) {
    setTemp = 80.0;

    pos = MAX_OFFSET;

    noInterrupts();
    encoderPosition = MAX_OFFSET;
    interrupts();
  }

  setTemp = 70.0 + pos;

  myPID.Compute();

  // analogWrite(MOSFET_PIN, (int)Output);

  Serial.print("Temp: ");
  Serial.println(tempReadF);

  if (now - lastRenderMs < RENDER_INTERVAL_MS)
    return;
  lastRenderMs = now;

  Serial.print("Set Temp: ");
  Serial.println(setTemp);

  Serial.print("PWM: ");
  Serial.println((int)Output);

  // analogWrite(MOSFET_PIN, 0);
  // Serial.println("Error: Could not read temperature data");

  /*
   chat gpt says that this means only run the rest of the code every
   RENDER_INTERVAL_MS  (e.g. 33ms)

   - Without this, your loop would run:
   - thousands of times per second and spam the OLED constantly
 */

  bool encBtn = (digitalRead(PIN_ENC_BTN) == LOW);

  render(setTemp, tempReadF, encBtn);
}

/**
 *
 * high point is 75
 * low point is 65
 * initial set value is 70
 *
 *
 */