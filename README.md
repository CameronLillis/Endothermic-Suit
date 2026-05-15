# Endothermic Suit

The **Endothermic Suit** is a wearable cooling system prototype designed to help regulate body temperature using thermoelectric cooling. The ideal application is in the entertainment, film, and television production industries particularly for cosplay.

This project is part of an experimental thermal-management design focused on portable cooling, wearable electronics, and user-controlled temperature regulation.

## Project Overview

The system uses a temperature sensor to monitor the cooling surface, a PID controller to calculate the needed cooling power, and a MOSFET-driven output to control the thermoelectric cooling element. A rotary encoder and OLED display allow the user to view the current temperature and adjust the target set temperature.

The goal of the project is to create a compact cooling insert that could be integrated into a vest or wearable system without requiring bulky external attachments.

## Features
- Reads live temperature data and displays to OLED screen
- Uses PID to adjust voltage output to Peltier devices for power efficiency.
- A set temperature may be set using rotary dial (displayed on OLED)
- Built using PlatformIO and the Arduino framework

## Hardware interfaced directly with Arduino 
- DS18B20 temperature sensor
- MOSFET driver/module
- OLED Display & EC11 Rotary encoder module
- External battery 

## Software / Libraries

This project is built with **PlatformIO** using the Arduino framework.

Libraries used:

- `PID`
- `OneWire`
- `DallasTemperature`
- `RotaryEncoder`
- `U8g2`


## How It Works

1. The DS18B20 sensor reads the current temperature.
2. The user sets a desired temperature using the rotary encoder.
3. The PID controller compares the current temperature to the set temperature.
4. The PID output is converted into a PWM value.
5. The PWM signal controls the MOSFET, which controls power to the cooling element.
6. The OLED displays the live temperature and target set temperature.

> Currently the temperature range is set between 65.00F and 80.00F. The default set temperature is based around 70°F and can be adjusted with the rotary encoder.

## License

This project is licensed under the MIT License.
