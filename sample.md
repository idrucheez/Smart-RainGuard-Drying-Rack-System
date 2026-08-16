## Hardware & Circuit Schematic

<table align="left" width="100%" style="border-collapse: collapse; border: none;">
  <tr>
    <!-- Left Column: Compact Pinout Table -->
    <td width="42%" valign="top" style="padding: 0 10px 0 0; border: none;">
      <details open>
        <summary><b>System Pin Interconnect</b></summary>
        <br>
        <table width="100%" style="font-size: 11px; border-collapse: collapse;">
          <thead>
            <tr>
              <th align="left" style="width: 28%;">Pin</th>
              <th align="left">Interface & Hardware Target</th>
            </tr>
          </thead>
          <tbody>
            <tr><td><code>D5</code></td><td>BTS7960 <code>RPWM</code> (Extend / Forward)</td></tr>
            <tr><td><code>D6</code></td><td>BTS7960 <code>LPWM</code> (Retract / Reverse)</td></tr>
            <tr><td><code>D7</code></td><td>YL-83 Sensor <code>DO</code> (Active LOW = Wet)</td></tr>
            <tr><td><code>5V</code></td><td>LM2596 Output (Powers Uno, Sensor, Driver logic)</td></tr>
            <tr><td><code>GND</code></td><td><strong>Common System Ground Reference</strong></td></tr>
            <tr><td><code>R_EN/L_EN</code></td><td>Tied to 5V (Continuous Driver Enable)</td></tr>
            <tr><td><code>B+/B-</code></td><td>Direct 3S Battery Rail (High Current)</td></tr>
            <tr><td><code>M+/M-</code></td><td>JGB37-3530 DC Motor Terminals</td></tr>
          </tbody>
        </table>
      </details>
    </td>
    <!-- Right Column: Circuit Schematic Card -->
    <td width="58%" valign="top" style="padding: 0 0 0 10px; border: none;">
      <div style="background: #0d1117; border: 1px solid #30363d; border-radius: 8px; padding: 12px; text-align: center;">
        <strong style="color: #58a6ff; font-size: 13px; display: block; margin-bottom: 6px;">Wiring & Power Schematic</strong>
        <a href="images/circuit_schematic.png" target="_blank">
          <img src="images/circuit_schematic.png" alt="Schematic" width="100%" style="border-radius: 6px; border: 1px solid #21262d; cursor: pointer;">
        </a>
        <p style="color: #8b949e; font-size: 11px; margin: 6px 0 0 0; line-height: 1.3;">
          Dual-rail topology: 12V battery bus drives high-current inductive motor loads; regulated 5V buck rail powers logic.
        </p>
      </div>
    </td>
  </tr>
</table>

---

## Firmware & Control Profile

### Soft-Start Acceleration Profile
To eliminate inrush current spikes ($I_{\text{stall}} \approx 4.3\text{A}$), mechanical gear shock, and MCU brownouts, the motor is accelerated via a linear 41-step PWM ramp:

$$\text{Ramp Duration} = \left(\frac{200 - 0}{5} + 1\right) \times 30\,\text{ms} = 1{,}230\,\text{ms}\ (1.23\,\text{s})$$

$$\text{Total Actuation Time} = T_{\text{soft-start}} + T_{\text{runTime}} = 1.23\,\text{s} + 7.00\,\text{s} = 8.23\,\text{s}$$

```cpp
// Retract cover with smooth acceleration profile
void retractCover() {
  analogWrite(RPWM, 0);
  for (int speed = 0; speed <= 200; speed += 5) {
    analogWrite(LPWM, speed);
    delay(30); // 1.23s acceleration window
  }
}
