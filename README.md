## PID 
### Variables 
``` 
double Kp, Ki, Kd; 
```
These are the PID tuning constants. Controls how aggressively the system reacts to error. 


### Relay 
Since a relay cannot do smooth analog output, and is only **ON/OFF**.

To solve this we must use **time proportioning control**.
- Pick a fixed time window: 5000ms
- Let the PID output a number: 0 - 5000
- Interpret that number as how many milliseconds the relay should be **ON** during the window





> PWM Threshhold to get relay voltage is 130/140 pwm dutyCycle 



# TODO List
- [ ] Read voltage in from battery to peltier
- [ ] Battery capacity reading 