# Endothermic Suit

The **Endothermic Suit** is a wearable cooling system prototype designed to help regulate body temperature using thermoelectric cooling. The project uses an Arduino-based control system to read temperature data, adjust a target temperature, and control cooling output through PWM.

This project is part of an experimental thermal-management design focused on portable cooling, wearable electronics, and user-controlled temperature regulation.

## Project Overview

The system uses a temperature sensor to monitor the cooling surface, a PID controller to calculate the needed cooling power, and a MOSFET-driven output to control the thermoelectric cooling element. A rotary encoder and OLED display allow the user to view the current temperature and adjust the target set temperature.

The goal of the project is to create a compact cooling insert that could be integrated into a vest or wearable system without requiring bulky external attachments.

## Features

- Reads temperature using a DS18B20 temperature sensor
- Displays live temperature and set temperature on an OLED screen
- Uses PID control for smoother temperature regulation
- Allows user adjustment with a rotary encoder
- Outputs PWM control signal to a MOSFET driver
- Designed for thermoelectric cooling / peltier-based cooling experiments
- Built using PlatformIO and the Arduino framework

## Hardware Used

- Arduino Uno
- DS18B20 temperature sensor
- MOSFET driver/module
- Thermoelectric cooling module / peltier element
- OLED display using U8g2
- Rotary encoder
- External battery or DC power supply
- Heat sink and fan for heat dissipation
- Wiring, connectors, and mounting hardware

## Software / Libraries

This project is built with **PlatformIO** using the Arduino framework.

Libraries used:

- `PID`
- `OneWire`
- `DallasTemperature`
- `RotaryEncoder`
- `U8g2`

## Current Pin Setup

| Component | Pin |
|---|---:|
| MOSFET PWM Output | D5 |
| DS18B20 Data Pin | D7 |
| Rotary Encoder A | D2 |
| Rotary Encoder B | D3 |
| Rotary Encoder Button | D4 |
| OLED Display | I2C |

## How It Works

1. The DS18B20 sensor reads the current temperature.
2. The user sets a desired temperature using the rotary encoder.
3. The PID controller compares the current temperature to the set temperature.
4. The PID output is converted into a PWM value.
5. The PWM signal controls the MOSFET, which controls power to the cooling element.
6. The OLED displays the live temperature and target set temperature.

## PID Control

The PID controller uses three tuning constants:

```cpp
double Kp, Ki, Kd;
```

These constants control how aggressively the system reacts to temperature error.

- `Kp` controls the immediate response to error.
- `Ki` helps correct accumulated error over time.
- `Kd` helps reduce overshoot by reacting to the rate of change.

The current project uses PWM output for MOSFET control. Earlier relay-based notes used time-proportioning control because relays can only switch fully ON or OFF.

## Temperature Range

The current software limits the set temperature range to:

```cpp
#define MIN_SET_TEMP 65.00
#define MAX_SET_TEMP 80.00
```

> The default set temperature is based around 70°F and can be adjusted with the rotary encoder.

## Safety Notes

This project uses external power, thermoelectric cooling modules, and MOSFET switching. Use caution when testing.

Important considerations:

- Do not power the peltier directly from the Arduino.
- Use a proper MOSFET or driver module rated for the required current.
- Make sure all grounds are connected together when using an external power supply.
- Use a heat sink and fan on the hot side of the peltier.
- Monitor temperature carefully during testing.
- Avoid short circuits, loose wiring, or exposed power connections.

## License

This project is licensed under the MIT License.
