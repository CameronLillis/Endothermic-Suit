// Include the libraries we need
#include <DallasTemperature.h>
#include <OneWire.h>
#include <PID_v1.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
#define MOSFET_PIN   3 // PWM PIN

// Setup a oneWire instance to communicate with any OneWire devices (not just
// Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// PID Variables
double setTemp, tempRead, Output;

const int maxPower = 127;

// Specify the links and initial tuning parameters
double Kp = 4, Ki = 7, Kd = 2;

/*
  Kp reacts to the current error.
    - Bigger Kp makes the system respond faster, (to much causes overshoot)
  Ki reacts to accumulated past error.
    - Helps remove steady offset; to much can make the system sluggish, or "windup" 
  Kd reacts to how fast the error changing
    - It acts like damping, helping calm the motion and reduce overshoot
      can amplify noise if used badly.
*/

PID myPID(&tempRead, &Output, &setTemp, Kp, Ki, Kd, DIRECT);

void setup() {
  // Start serial port
  Serial.begin(9600);

  // Start up temp sensor libary
  sensors.begin();

  pinMode(MOSFET_PIN, OUTPUT);

  // Set temp variable
  setTemp = 20;
  setTemp *= -1;
  myPID.SetOutputLimits(0, maxPower);

  // turn the PID on
  myPID.SetMode(AUTOMATIC);
}

void loop() {
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  sensors.requestTemperatures(); // Send the command to get temperatures
  delay(500);
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the
  // first sensor only.
  // setTemp = analogRead() used for analog input
  tempRead = sensors.getTempCByIndex(0);
  // float tempC = sensors.getTempCByIndex(0);
  // float tempF = sensors.getTempFByIndex(0);
  tempRead = tempRead * -1;
  // Check if reading was successful
  if (tempRead != DEVICE_DISCONNECTED_C) {
    myPID.Compute();
    analogWrite(MOSFET_PIN, (int)Output);

    Serial.print("Temp:");
    Serial.print(abs(tempRead));
    Serial.print(",");
    Serial.print("Setpoint:");
    Serial.print(abs(setTemp));
    Serial.print(",");
    Serial.print("Output:");
    Serial.println(Output);
  } else {
    analogWrite(MOSFET_PIN, 0);
    Serial.println("Error: Could not read temperature data");
  }
}
