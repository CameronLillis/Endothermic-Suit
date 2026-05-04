#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <PID_v1.h>
#include <RotaryEncoder.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <Wire.h>

// TODO: Make PID more aggressive on downturn

// GLOBALS
#define MOSFET_PIN 3

//*---- Temp Sensor ---- *//
#define ONE_WIRE_BUS 2

// OneWire / Temperature setup
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//*---- PID ---- *//
double setTemp, tempReadC, tempReadF, Output;
double Kp = 4, Ki = 7, Kd = 2;
const int maxPower = 255; // pwm max power

PID myPID(&tempReadC, &Output, &setTemp, Kp, Ki, Kd, REVERSE);

//*---- Encoder ----*//
constexpr int PIN_ENC_A = 2;
constexpr int PIN_ENC_B = 3;
constexpr int PIN_ENC_BTN = 4;

constexpr int PIN_CONF_BTN = 9;
constexpr int PIN_BACK_BTN = 10;

RotaryEncoder encoder(PIN_ENC_A, PIN_ENC_B, RotaryEncoder::LatchMode::FOUR3);

//*---- Display ----*//
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define DISPLAY_OFFSET -1

//* On an arduino UNO:       A4(SDA), A5(SCL)
Adafruit_SH1106G display(DISPLAY_HEIGHT, DISPLAY_WIDTH, &Wire, DISPLAY_OFFSET);

// UI Libary 
U8G2_SH1106_128X64_NONAME_F_HW_I2C interface(U8G2_R0, U8X8_PIN_NONE);

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

void setup() {
  Serial.begin(9600);
  delay(1000);

  sensors.begin();
  interface.begin();
  Wire.begin();

  Wire.setClock(400000);
  pinMode(MOSFET_PIN, OUTPUT);

  setTemp = 20;

  myPID.SetOutputLimits(0, maxPower);
  myPID.SetMode(AUTOMATIC);

  // check if display is at right address or not
  if (!display.begin(0x3C, true)) {
    Serial.println("SH1106 failed!");
    while (1)
      delay(1000);
  }

  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.display();

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

void loop() {
  /*
  // Read temperature
  sensors.requestTemperatures();
  tempReadC = sensors.getTempCByIndex(0);
  tempReadF = sensors.getTempFByIndex(0);

  // Read voltage through divider

  myPID.Compute();

  analogWrite(MOSFET_PIN, (int)Output);

  Serial.print("Temp: ");
  Serial.println(tempReadC);

  Serial.print("Set: ");
  Serial.println(setTemp);

  Serial.print("PWM: ");
  Serial.println((int)Output);
  // float inputVoltage = analogRead(A0) * 12.0/1024;
  // Serial.print("Input Voltage: ");
  // Serial.println(inputVoltage);
  // Serial.println(" V");

  // analogWrite(MOSFET_PIN, 0);
  // Serial.println("Error: Could not read temperature data");

  delay(500);
  */
}
