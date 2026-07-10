# Future Improvements

## Highest priority

1. Add a fully retracted limit switch.
2. Add a fully extended limit switch.
3. Add a maximum movement timeout.
4. Add a manual emergency-stop switch.
5. Enclose the motor and electronics against rain.
6. Add low-battery voltage monitoring.

## Control improvements

- Replace blocking `delay()` control with a non-blocking state machine using `millis()`.
- Require the rain sensor to remain wet for several seconds before retracting.
- Require a longer dry period before extending, preventing movement between rain droplets.
- Add motor-current monitoring to detect jams.
- Add fault states and LED/OLED status indications.

## Mechanical improvements

- Use a spool with flanges to prevent string escape.
- Add a tensioner or spring mechanism.
- Add a clutch or slip mechanism to reduce damage during jams.
- Use a slower, higher-torque gear motor if 111 RPM is too fast.
- Add weather-resistant bearings or shaft seals.

## Energy improvements

- Put the Arduino into low-power sleep mode while idle.
- Add solar charging with a proper lithium charge controller.
- Measure actual battery capacity and motor energy usage.
