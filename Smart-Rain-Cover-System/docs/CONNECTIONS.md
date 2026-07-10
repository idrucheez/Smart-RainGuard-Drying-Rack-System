# Connection Guide

## 1. Battery Pack

Use three matched 18650 lithium-ion cells in series as a protected 3S pack.

- Nominal voltage: 11.1 V
- Fully charged voltage: 12.6 V

## 2. High-Current Motor Path

```text
Battery positive -> BTS7960 B+
Battery negative ----------> BTS7960 B-
BTS7960 M+ ----------------> Motor terminal 1
BTS7960 M- ----------------> Motor terminal 2
```

Use sufficiently thick wires for the battery and motor paths.

## 3. LM2596 Logic Supply

```text
Battery positive -> LM2596 IN+
Battery negative -> LM2596 IN-
LM2596 OUT+ set to 5.0 V
LM2596 OUT- = logic ground
```

Adjust the converter with a multimeter before connecting the Arduino.

## 4. Regulated 5 V Distribution

```text
LM2596 OUT+ -> Arduino 5V
LM2596 OUT+ -> YL-83 VCC
LM2596 OUT+ -> BTS7960 VCC
LM2596 OUT+ -> BTS7960 R_EN
LM2596 OUT+ -> BTS7960 L_EN
```

Do not connect the regulated 5 V output to the motor terminals.

## 5. Common Ground

Connect all of the following together:

```text
Battery negative
LM2596 OUT-
Arduino GND
YL-83 GND
BTS7960 GND
```

Without common ground, Arduino control signals may not be interpreted correctly.

## 6. Control Signals

```text
Arduino D5 -> BTS7960 RPWM
Arduino D6 -> BTS7960 LPWM
Arduino D7 -> YL-83 DO
```

Leave `R_IS` and `L_IS` unconnected in the current version.



## 7. First-Test Direction

Use a very short runtime for the first powered test.

If rain causes the mechanism to extend instead of retract:

- Swap the motor wires at M+ and M-, or
- Swap the retraction and extension PWM assignments in software.
