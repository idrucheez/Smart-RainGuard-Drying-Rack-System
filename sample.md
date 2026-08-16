# RainGuard 🌧️

<p align="center">
  <img alt="MCU" src="https://img.shields.io/badge/MCU-ATmega328P%20%2F%20Arduino%20Uno-00979D?style=for-the-badge&logo=arduino&logoColor=white">
  <img alt="Motor Driver" src="https://img.shields.io/badge/Driver-BTS7960%2043A%20H--Bridge-E34F26?style=for-the-badge">
  <img alt="Actuator" src="https://img.shields.io/badge/Actuator-12V%20DC%20Geared%20Motor-4A90E2?style=for-the-badge">
  <img alt="Sensor" src="https://img.shields.io/badge/Sensor-YL--83%20Raindrop-2ECC71?style=for-the-badge">
  <img alt="Power" src="https://img.shields.io/badge/Power-3S%20Li--ion%20%2B%20LM2596-F39C12?style=for-the-badge">
  <img alt="License" src="https://img.shields.io/badge/License-MIT-34495E?style=for-the-badge">
</p>

<p align="center">
  <strong>Autonomous mechatronic rain-defense canopy for semi-outdoor laundry drying racks.</strong><br>
  <em>Engineering Team Project II (MFB2102) • Universiti Teknologi PETRONAS</em>
</p>

<p align="center">
  <a href="#-project-overview">Overview</a> •
  <a href="#-system-architecture">Architecture</a> •
  <a href="#-hardware--circuit-schematic">Hardware & Pinout</a> •
  <a href="#-firmware--control-profile">Control Logic</a> •
  <a href="#-bill-of-materials">BOM</a> •
  <a href="#-quick-start">Quick Start</a> •
  <a href="#-team--credits">Team</a>
</p>

---

## Overview

**RainGuard** is an automated, non-invasive motorized roof system engineered for student dormitories and residential balconies. When sudden precipitation occurs, a conductive raindrop sensor triggers an Arduino Uno to deploy a waterproof parachute canopy via a high-torque 12V DC geared motor. When clear weather returns, the mechanism reverses to retract the cover and resume natural airflow drying.

> [!NOTE]
> **Non-Permanent Clamp Mounting:** Designed for universal attachment to standard laundry racks without structural drilling, welding, or hostel safety violations.

---

## Gallery

<table align="center" width="100%" style="border-collapse: collapse; border: none;">
  <tr>
    <td width="50%" align="center" valign="middle" style="padding: 4px; border: none;">
      <div align="center"><b>Deployed System on Drying Rack</b></div>
      <a href="images/rack_deployment.png" target="_blank" style="display: block; margin-top: 4px;">
        <img src="images/rack_deployment.png" alt="Rack Deployment" width="100%" height="200" style="object-fit: cover; border-radius: 6px; border: 1px solid #30363d; cursor: pointer; display: block;">
      </a>
    </td>
    <td width="50%" align="center" valign="middle" style="padding: 4px; border: none;">
      <div align="center"><b>Weather-Sealed Electronics Box</b></div>
      <a href="images/control_enclosure.png" target="_blank" style="display: block; margin-top: 4px;">
        <img src="images/control_enclosure.png" alt="Control Enclosure" width="100%" height="200" style="object-fit: cover; border-radius: 6px; border: 1px solid #30363d; cursor: pointer; display: block;">
      </a>
    </td>
  </tr>
  <tr>
    <td width="50%" align="center" valign="middle" style="padding: 4px; border: none;">
      <div align="center"><b>Mechanical Frame CAD Model</b></div>
      <a href="images/cad_model.png" target="_blank" style="display: block; margin-top: 4px;">
        <img src="images/cad_model.png" alt="CAD Model" width="100%" height="200" style="object-fit: cover; border-radius: 6px; border: 1px solid #30363d; cursor: pointer; display: block;">
      </a>
    </td>
    <td width="50%" align="center" valign="middle" style="padding: 4px; border: none;">
      <div align="center"><b>Benchtop Verification Testing</b></div>
      <a href="images/bench_test.png" target="_blank" style="display: block; margin-top: 4px;">
        <img src="images/bench_test.png" alt="Benchtop Testing" width="100%" height="200" style="object-fit: cover; border-radius: 6px; border: 1px solid #30363d; cursor: pointer; display: block;">
      </a>
    </td>
  </tr>
</table>

---

## System Architecture

<details open>
<summary><strong>📊 Interactive Signal & Power Topology</strong></summary>
<br>

```mermaid
graph LR
    subgraph Power ["⚡ Isolated Power Subsystem"]
        BAT["3S 18650 Li-ion<br/>(11.1V–12.6V)"] --> FUSE["Inline Fuse"]
        FUSE -->|"12V Motor Rail"| BTS_PWR["BTS7960 (B+/B-)"]
        FUSE -->|"12V Input"| BUCK["LM2596 Buck (5.0V)"]
    end

    subgraph Control ["🧠 Controller & Sensing"]
        MCU["Arduino Uno<br/>(ATmega328P)"]
        SENSOR["YL-83 Rain Sensor<br/>(LM393 Comparator)"]
        BUCK -->|"5V Logic"| MCU
        BUCK -->|"5V VCC"| SENSOR
        SENSOR -->|"D7 (LOW = Rain)"| MCU
    end

    subgraph Drive ["⚙️ Actuation Mechanism"]
        DRIVER["BTS7960 43A H-Bridge"]
        MOTOR["JGB37-3530 DC Motor<br/>(12V, 111 RPM, 7 kg·cm)"]
        SPOOL["Pulley & Canopy Spool"]

        BUCK -->|"5V Logic / Enables"| DRIVER
        MCU -->|"D5 (RPWM) / D6 (LPWM)"| DRIVER
        BTS_PWR --> DRIVER
        DRIVER -->|"Bidirectional Drive"| MOTOR
        MOTOR --> SPOOL
    end
	
    class Power pwr;
    class Control ctrl;
    class Drive act;
```

---

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
```

## Bill of Materials

| Component | Technical Specification | Function | Cost (MYR) |
|---|---|---|:---:|
| **Arduino Uno R3** | ATmega328P, 16 MHz, 5V logic | Main controller executing FSM and PWM routines | Dev Board |
| **JGB37-3530 DC Motor** | 12V DC, 111 RPM, 7 kg·cm rated / 24 kg·cm stall | High-torque canopy spool drive | RM 39.90 |
| **BTS7960 H-Bridge** | 43A peak dual H-bridge motor driver | Bidirectional high-current motor control | RM 19.42 |
| **YL-83 Rain Sensor** | Nickel-plated rain board with LM393 comparator | Real-time water droplet detection via digital output | RM 6.50 |
| **LM2596 Buck Converter** | Step-down converter (12V to 5V, 3A max) | Regulated logic power rail | RM 3.00 |
| **3S 18650 Battery Pack** | 11.1V nominal (12.6V peak) lithium-ion cells | Portable DC power source | RM 20.68 |
| **Canopy & Mechanical Rig** | Hydrophobic parachute fabric, 4mm starter rope, rollers, PVC | Weatherproof shelter and linear sliding assembly | RM 31.70 |
| **Protection & Housing** | Weather-sealed box, inline fuse, switch, cabling | Electrical isolation and safety housing | RM 39.89 |
| **Total Prototype Cost** | | | **RM 161.09** |

> **Commercial Viability:** With a targeted 35% commercial margin, the estimated unit selling price is **RM 217.50** (~$48 USD), offering significant cost and power savings compared to permanent awnings or electric dryers.

---

## Results & Validation

### User Research & Problem Validation
Pre-development field surveys conducted across residential student hostels at Universiti Teknologi PETRONAS established clear market demand:

| Survey Query | Sample Metric | Engineering Takeaway |
|---|:---:|---|
| **Clothes Drenched by Rain** | **77%** affected | High frequency of laundry spoilage in semi-outdoor drying areas. |
| **Product Concept Interest** | **79%** positive | Strong end-user preference for automatic rain-detection mechanisms. |
| **Daily Routine Usefulness** | **89%** useful | Validates the need for autonomous time-saving hostel utilities. |

### Mechanism Kinematics & Benchtop Testing

<table align="center" width="100%" style="border-collapse: collapse; border: none;">
  <tr>
    <td width="50%" align="center" valign="middle" style="padding: 4px; border: none;">
      <div align="center"><b>Retracted State (Normal Sunlight)</b></div>
      <a href="images/canopy_retracted.png" target="_blank" style="display: block; margin-top: 4px;">
        <img src="images/canopy_retracted.png" alt="Retracted Mechanism" width="100%" height="190" style="object-fit: contain; border-radius: 6px; border: 1px solid #30363d; display: block;">
      </a>
    </td>
    <td width="50%" align="center" valign="middle" style="padding: 4px; border: none;">
      <div align="center"><b>Expanded State (Rain Protection)</b></div>
      <a href="images/canopy_expanded.png" target="_blank" style="display: block; margin-top: 4px;">
        <img src="images/canopy_expanded.png" alt="Expanded Mechanism" width="100%" height="190" style="object-fit: contain; border-radius: 6px; border: 1px solid #30363d; display: block;">
      </a>
    </td>
  </tr>
</table>

* **Actuation Response:** Total deployment/retraction completes in **8.23 seconds** (1.23 s soft-start acceleration ramp + 7.00 s calibrated full-speed transit).
* **Electrical Decoupling:** Dual-rail topology successfully suppressed motor back-EMF spikes, maintaining zero microcontroller resets during continuous state transitions.

---

## Project Milestones

The 12-week development lifecycle for Engineering Team Project II (MFB2102) was executed across three core phases:

```mermaid
gantt
    title 12-Week Development Schedule (ETP II)
    dateFormat  YYYY-MM-DD
    axisFormat  W%W
    
    section Phase 1: Planning & Sourcing
    Requirements Meeting & Part Planning :done, p1, 2026-01-05, 2026-01-12
    Component Procurement                :done, p2, 2026-01-12, 2026-02-09
    Design Iteration & Tweaking          :done, p3, 2026-01-12, 2026-02-09
    Product Design Freeze                :done, p4, 2026-02-09, 2026-02-16

    section Phase 2: Build & Integration
    Electronics & Circuit Assembly       :done, b1, 2026-02-09, 2026-03-02
    Mechanical Fabrication (Garage 21)   :done, b2, 2026-02-23, 2026-03-23
    Project Claim Filing                 :done, b3, 2026-02-23, 2026-03-16

    section Phase 3: Validation & Delivery
    Final Report Compilation             :done, d1, 2026-03-02, 2026-03-23
    Advisor Presentation & Pre-Audit     :done, d2, 2026-03-16, 2026-03-23
    ETPx Exhibition & Final Demo         :done, d3, 2026-03-23, 2026-03-30
```
