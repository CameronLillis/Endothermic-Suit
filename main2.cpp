#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <PID_v1.h>
#include <RotaryEncoder.h>
#include <SPI.h>
#include <Wire.h>



// Pins
// #define ONE_WIRE_BUS 2
// #define MOSFET_PIN 3

// OneWire / Temperature setup
// OneWire oneWire(ONE_WIRE_BUS);
// DallasTemperature sensors(&oneWire);

// PID Variables
// double setTemp, tempReadC, tempReadF, Output;
// double Kp = 4, Ki = 7, Kd = 2;
// const int maxPower = 255;

// PID myPID(&tempReadC, &Output, &setTemp, Kp, Ki, Kd, REVERSE);

// Voltage sensing variables
//! start from max somehow
void setup() {
  /*
  Serial.begin(9600);

  sensors.begin();

  pinMode(MOSFET_PIN, OUTPUT);

  setTemp = 20;

  myPID.SetOutputLimits(0, maxPower);
  myPID.SetMode(AUTOMATIC);
  */
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
