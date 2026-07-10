# Laboratory Test Checklist

## Before applying power

- [ ] Battery cells are matched and installed in a protected 3S pack.
- [ ] Battery polarity has been verified.
- [ ] Fuse is installed near battery positive.
- [ ] LM2596 input polarity is correct.
- [ ] LM2596 output has been adjusted to 5.0 V using a multimeter.
- [ ] Motor is not connected to the 5 V rail.
- [ ] Arduino, sensor, and BTS7960 logic share common ground.
- [ ] BTS7960 VCC, R_EN, and L_EN receive regulated 5 V.
- [ ] RPWM is connected to D5.
- [ ] LPWM is connected to D6.
- [ ] Rain sensor DO is connected to D7.
- [ ] Electrolytic capacitors have correct polarity.
- [ ] Exposed wires cannot touch each other.
- [ ] The mechanism can be disconnected quickly in an emergency.

## Sensor-only test

- [ ] Disconnect motor power.
- [ ] Upload the sketch.
- [ ] Open Serial Monitor at 9600 baud.
- [ ] Confirm dry state.
- [ ] Apply a small amount of water.
- [ ] Confirm rain-detected message.
- [ ] Dry the sensor.
- [ ] Confirm dry-state message.

## Motor test without mechanical load

- [ ] Set `RUN_TIME_MS` to 500–1000 ms.
- [ ] Raise the wheel or detach the string.
- [ ] Connect motor power.
- [ ] Confirm retraction direction.
- [ ] Confirm extension direction.
- [ ] Reverse motor leads or software direction if required.
- [ ] Check for excessive heating or noise.

## Mechanical test

- [ ] Attach string with low tension.
- [ ] Test short movements.
- [ ] Increase runtime gradually.
- [ ] Observe spool alignment and string tracking.
- [ ] Confirm motor does not reach a hard stop.
- [ ] Stop immediately if the motor stalls.
- [ ] Check battery, wires, driver, converter, and motor temperature.
- [ ] Record actual travel time.

## Do not leave unattended until

- [ ] Two limit switches are installed.
- [ ] An emergency stop or main switch is accessible.
- [ ] Motor and electronics are protected from rain.
- [ ] Battery enclosure and charging system are verified.
