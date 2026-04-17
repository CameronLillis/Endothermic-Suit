#include <DallasTemperature.h>
#include <OneWire.h>
#include <PID_v1.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>
// #include <SPI.h>
// #include <Wire.h>


// #define SCREEN_WIDTH 132 // OLED display width, in pixels
// #define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Pins
#define VOLTAGE_SENSOR_PIN A0
#define ONE_WIRE_BUS 2
#define MOSFET_PIN 3
#define A A1
#define B A3
// OneWire / Temperature setup
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// PID Variables
double setTemp, tempRead, Output;
double Kp = 4, Ki = 7, Kd = 2;
const int maxPower = 255;

PID myPID(&tempRead, &Output, &setTemp, Kp, Ki, Kd, REVERSE);

// Voltage sensing variables
float adc_voltage = 0.0;
float in_voltage = 0.0;

float R1 = 30000.0;
float R2 = 7500.0;
float ref_voltage = 5.0;

int adc_value = 0;

void setup() {
  Serial.begin(9600);

  sensors.begin();

  pinMode(MOSFET_PIN, OUTPUT);

  setTemp = 20;

  myPID.SetOutputLimits(0, maxPower);
  myPID.SetMode(AUTOMATIC);
}

void loop() {
  // Read temperature
  sensors.requestTemperatures();
  tempRead = sensors.getTempCByIndex(0);

  // Read voltage through divider
  adc_value = analogRead(VOLTAGE_SENSOR_PIN);
  adc_voltage = (adc_value * ref_voltage) / 1023.0;
  in_voltage = adc_voltage / (R2 / (R1 + R2));

  if (tempRead != DEVICE_DISCONNECTED_C) {
    myPID.Compute();

    analogWrite(MOSFET_PIN, (int)Output);

    Serial.print("Temp: ");
    Serial.print(tempRead);
    Serial.print(" C, Set: ");
    Serial.print(setTemp);
    Serial.print(" C, PWM: ");
    Serial.print((int)Output);
    Serial.print(", ADC: ");
    Serial.print(adc_value);
    Serial.print(", A0 Voltage: ");
    Serial.print(adc_voltage, 2);
    Serial.print(" V, Input Voltage: ");
    Serial.print(in_voltage, 2);
    Serial.println(" V");
  } else {
    analogWrite(MOSFET_PIN, 0);
    Serial.println("Error: Could not read temperature data");
  }

  delay(500);
}