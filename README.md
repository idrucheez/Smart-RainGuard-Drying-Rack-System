# Smart Rain Cover System

An Arduino-based automatic rain cover system that detects rainfall and retracts or extends a clothes-drying cover using a 12 V DC geared motor.

The system uses a YL-83 rain sensor, Arduino Uno, BTS7960 motor driver, JGB37-3530 geared DC motor, LM2596 buck converter, and a 3-cell 18650 battery pack.

## Features

- Automatic rain detection
- Automatic cover retraction when rain is detected
- Automatic cover extension when the sensor becomes dry
- Bidirectional DC motor control
- PWM soft-start to reduce mechanical shock and startup current
- State-based logic to prevent repeated motor activation
- Battery-powered architecture
- Serial Monitor messages for testing and debugging

## Main Components

| Component | Specification / Function |
|---|---|
| Arduino Uno | Main microcontroller |
| YL-83 rain sensor module | Digital rain detection |
| BTS7960 / IBT-2 | High-current bidirectional motor driver |
| JGB37-3530 DC gear motor | 12 V, 111 RPM, 1.2 A rated current, 4.3 A stall current |
| LM2596 buck converter | Reduces battery voltage to regulated 5 V |
| 3 × 18650 cells | 3S battery pack: 11.1 V nominal, 12.6 V fully charged |
| Rain-cover mechanism | String and spool connected to a sliding shield |

## Power Architecture

```text
3S 18650 battery pack
        |
        +----> BTS7960 B+ / B- ----> DC motor
        |
        +----> LM2596 adjusted to 5.0 V
                     |
                     +----> Arduino 5V
                     +----> Rain sensor VCC
                     +----> BTS7960 VCC
```

All grounds must be connected together:

```text
Battery negative
LM2596 OUT-
Arduino GND
Rain sensor GND
BTS7960 GND
```

## Important Battery Safety

The three 18650 cells must be used as a properly assembled **3S battery pack with a suitable BMS**.

Do not:

- Mix cells of different types, capacities, ages, or charge levels.
- Charge the cells directly without a 3S lithium-ion charger/BMS.
- Short-circuit the battery pack.
- Trust unrealistic capacity labels without testing. Genuine 18650 cells are normally far below 12,000 mAh per cell.
- Work on the battery pack while metal tools or loose wires can bridge its terminals.

Recommended protection:

- 3S BMS capable of at least 10 A continuous current
- 7.5–10 A fuse near the battery positive terminal
- Proper insulated battery holder or spot-welded pack
- Main power switch
- Correct 12.6 V lithium-ion charger for a 3S pack

## Pin Connections

### Arduino to BTS7960

| BTS7960 pin | Arduino / supply connection |
|---|---|
| VCC | Regulated 5 V from LM2596 |
| GND | Common ground |
| RPWM | Arduino D5 |
| LPWM | Arduino D6 |
| R_EN | Regulated 5 V |
| L_EN | Regulated 5 V |
| R_IS | Not connected in the current version |
| L_IS | Not connected in the current version |

`R_IS` and `L_IS` are current-sense outputs. They are optional and are left unconnected in this version.

### Rain Sensor

| YL-83 pin | Connection |
|---|---|
| VCC | Regulated 5 V |
| GND | Common ground |
| DO | Arduino D7 |
| AO | Not used |

Most YL-83 modules produce `LOW` when wet and `HIGH` when dry. Confirm this using the Arduino Serial Monitor.

### Motor Power

| Terminal | Connection |
|---|---|
| BTS7960 B+ | Battery positive through fuse |
| BTS7960 B- | Battery negative |
| BTS7960 M+ / M- | DC motor terminals |

## Software

The Arduino sketch is located at:

```text
code/SmartRainCover/SmartRainCover.ino
```

Open this file using Arduino IDE, select **Arduino Uno**, select the correct COM port, and upload it.

## Control Logic

1. The Arduino continuously reads the digital output of the YL-83 rain sensor.
2. When rain is detected, the motor retracts the cover.
3. When the sensor becomes dry, the motor reverses and extends the cover.
4. A state variable records the assumed cover position.
5. The motor runs for a calibrated time and then stops.
6. PWM soft-start gradually increases motor speed.

## Runtime Calibration

The following value controls how long the motor continues running after completing the soft-start ramp:

```cpp
const int runTime = 7000;
```

The value is measured in milliseconds:

```text
7000 ms = 7 seconds
```
The motor does not immediately begin at its final PWM value. It first completes the following soft-start sequence:

```cpp
for (int speed = 0; speed <= 200; speed += 5) {
  analogWrite(LPWM, speed);
  delay(30);
}
```


The same ramp is used in the opposite direction through RPWM.

The approximate soft-start duration is:

```text
Number of steps = 200 / 5 + 1 = 41 steps
Ramp time = 41 × 30 ms
Ramp time ≈ 1230 ms
Ramp time ≈ 1.23 seconds
```

Therefore, the approximate total motor operating time for one movement is:

```text
Total movement time = soft-start time + runTime
Total movement time ≈ 1.23 s + 7.00 s
Total movement time ≈ 8.23 seconds
```

## Recommended Electrical Protection

### BTS7960 supply capacitor

Install a **1000–2200 µF, 25 V electrolytic capacitor** close to the BTS7960 motor-power terminals:

```text
Capacitor positive  -> B+
Capacitor negative  -> B-
```

Observe capacitor polarity.

### Arduino 5 V capacitor

Install a **100–470 µF, 10 V or higher electrolytic capacitor** across the regulated logic supply:

```text
Capacitor positive  -> Arduino 5V rail
Capacitor negative  -> Common ground
```

A 100 nF ceramic capacitor may also be placed near the Arduino and sensor supply pins for high-frequency noise suppression.

### Motor noise suppression

A 100 nF ceramic capacitor may be connected directly across the motor terminals. Keep its leads short.

## Major Limitation

The current design uses time-based positioning. It assumes that a fixed motor runtime always produces the same travel distance. This is not fully reliable because:

- Battery voltage changes
- Mechanical load changes
- The string may slip
- The spool diameter changes as string accumulates
- The mechanism may jam
- The motor may continue pulling at the end of travel

## Strongly Recommended Upgrade: Limit Switches

Install two normally-closed or normally-open limit switches:

- Fully retracted limit
- Fully extended limit

The motor should stop immediately when the corresponding limit switch is activated. This prevents prolonged stall current, string damage, mechanical breakage, and overheating.

Until limit switches are installed, test using short runtimes and keep an emergency power-disconnect switch within reach.

## Weather Protection

The JGB37-3530 motor, Arduino Uno, BTS7960, LM2596, and battery pack should be treated as **not waterproof unless explicitly rated otherwise**.

Recommended construction:

- Install electronics in a weather-resistant enclosure.
- Mount the motor under a protective hood or inside an enclosure.
- Use a shaft seal, rubber grommet, or protected pulley arrangement.
- Place cable entry points on the lower side of the enclosure.
- Use cable glands and drip loops.
- Keep the exposed rain-sensing plate outside the enclosure.
- Add drainage and avoid trapping condensation.

## Testing Procedure

1. Disconnect the motor.
2. Assemble the low-voltage logic circuit.
3. Adjust the LM2596 output to exactly 5.0 V using a multimeter.
4. Connect the Arduino, rain sensor, and BTS7960 logic.
5. Upload the sketch.
6. Verify sensor readings in Serial Monitor.
7. Check that the BTS7960 enable pins receive 5 V.
8. Connect the motor with the mechanism unloaded.
9. Use a short runtime such as 500–1000 ms.
10. Verify both directions.
11. Attach the string and increase runtime gradually.
12. Monitor motor, driver, wiring, battery, and converter temperature.
13. Disconnect power immediately if the motor stalls, wiring heats up, or the battery voltage collapses.

## Repository Structure

```text
Smart-Rain-Cover-System/
├── README.md
├── LICENSE
├── .gitignore
├── code/
│   └── SmartRainCover/
│       └── SmartRainCover.ino
├── docs/
│   ├── BILL_OF_MATERIALS.md
│   ├── CONNECTIONS.md
│   ├── TEST_CHECKLIST.md
│   └── FUTURE_IMPROVEMENTS.md
└── images/
    └── README.md
```

## Future Improvements

- Add two limit switches
- Add motor-current monitoring
- Add low-battery detection
- Add automatic timeout fault handling
- Add manual open, close, and emergency-stop controls
- Replace time-based positioning with encoder feedback
- Add solar charging
- Add waterproof enclosure and cable glands
- Add OLED status display
- Add remote monitoring using ESP8266 or ESP32

## Skills Demonstrated

- Arduino and embedded C++
- Sensor interfacing
- PWM motor control
- H-bridge motor-driver integration
- Battery power distribution
- DC-DC conversion
- Mechatronic system integration
- State-based control logic
- Electrical protection and debugging

## License

This project is released under the MIT License.
