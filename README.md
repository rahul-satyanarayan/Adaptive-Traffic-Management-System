# Adaptive Traffic Management System: Dual-MCU Real-Time Density & Microclimate Sentinel

An intelligent intersection control system designed around a distributed **Dual-Microcontroller I2C Master-Slave Architecture**. The platform mitigates urban intersection gridlock and commuter heat-stress by replacing static interval timers with dynamic, density-weighted phase allocation. A dedicated slave hub ingests and serializes a 12-sensor proximity array, streaming real-time queue states to a central decision engine that drives traffic phases and diagnostics.

---

## Academic & Engineering Context

- **Course:** Bachelor of Engineering (B.E.) in Electronics and Communication Engineering
- **Semester:** Semester 6
- **Subject:** Design Engineering - II B (Course Code: `3160001`)
- **Domain:** Distributed Embedded Systems, Sensor Ingestion Hubs, Traffic Automation
- **Key Focus:** Master-Slave I2C bus serialization, multi-zone proximity sensing, 2-page LCD diagnostic telemetry, and fail-safe scheduling.

---

## Key Features

- **Distributed Dual-MCU Processing:** Offloads continuous digital sensor sampling from the decision core, preserving execution cycles for timing allocation and failsafe routines.
- **12-Point Directional Sensor Grid:** Monitors 4 intersection approaches (North, East, South, West) across 3 progressive density zones per approach (Entry, Mid, and Queue tail).
- **Bitwise 16-Bit Serialization:** Condenses 12 parallel Active-LOW inputs into a 2-byte binary payload transmitted over I2C on interrupt request (`Wire.onRequest`).
- **High-Baud Serial Telemetry:** Streams formatted real-time intersection density matrices at $115200\text{ baud}$ for diagnostic monitoring.
- **Flicker-Free 2-Page HMI Diagnostics:** Alternates North-East and South-West occupancy maps every 1000 ms on a shared-bus $16\times2$ I2C LCD.
- **Microclimate Heat-Stroke Guard:** Evaluates ambient conditions via a calibrated DHT22 sensor to dynamically shorten wait intervals or activate yellow caution cycles during critical heat index conditions.

---

## System Architecture & Circuit Schematic

### Circuit Connection Diagram
![Circuit Schematic](docs/Adaptive%20Traffic%20Management%20System%20Dual%20MCU%20Schematic%20Diagram.png)

> **Note:** The complete high-resolution vector PDF schematic is archived under [`docs/Adaptive Traffic Management System Dual MCU Schematic Diagram.pdf`](docs/Adaptive%20Traffic%20Management%20System%20Dual%20MCU%20Schematic%20Diagram.pdf).

---

## Hardware Pinout & Wiring Matrix

| Subsystem / Component | Module Pin | Controller Pin | Voltage Rail | Description |
| :--- | :--- | :--- | :--- | :--- |
| **North Approach Array** | **N1 / N2 / N3** | **Slave D2 / D3 / D4** | `5VDC` Pull-up | Active-LOW digital proximity inputs (Bits 0, 1, 2) |
| **East Approach Array** | **E1 / E2 / E3** | **Slave D5 / D6 / D7** | `5VDC` Pull-up | Active-LOW digital proximity inputs (Bits 3, 4, 5) |
| **South Approach Array** | **S1 / S2 / S3** | **Slave D8 / D9 / D10** | `5VDC` Pull-up | Active-LOW digital proximity inputs (Bits 6, 7, 8) |
| **West Approach Array** | **W1 / W2 / W3** | **Slave D11 / D12 / D13** | `5VDC` Pull-up | Active-LOW digital proximity inputs (Bits 9, 10, 11) |
| **Inter-MCU I2C Bus** | **SDA / SCL** | **Slave A4 / A5** $\leftrightarrow$ **Master A4 / A5** | `5VDC` Logic | Hardware I2C communication bus (Slave Addr: `0x08`) |
| **16x2 Diagnostic LCD** | **SDA / SCL** | **Master A4 / A5** (Shared) | `5VDC` Logic | 2-page alternating intersection occupancy display (Addr: `0x27`) |
| **DHT22 Microclimate** | **DATA** | **Master D3** | `5VDC` Logic | Ambient temperature and humidity tracking for heat-stroke protocols |
| **North Approach LEDs** | **RED / YELLOW / GREEN** | **Master D6 / D7 / D8** | `5VDC` Active-HIGH | Lane control indicators for Northbound traffic (Stop, Transit, Go) |
| **East Approach LEDs** | **RED / YELLOW / GREEN** | **Master D9 / D10 / D11** | `5VDC` Active-HIGH | Lane control indicators for Eastbound traffic (Stop, Transit, Go) |
| **South Approach LEDs** | **RED / YELLOW / GREEN** | **Master D12 / D13 / A0** | `5VDC` Active-HIGH | Lane control indicators for Southbound traffic (Stop, Transit, Go) |
| **West Approach LEDs** | **RED / YELLOW / GREEN** | **Master A1 / A2 / A3** | `5VDC` Active-HIGH | Lane control indicators for Westbound traffic (Stop, Transit, Go) |
| **LED Common Ground** | **CATHODE (-)** | **Common System GND** | `0V` Reference | Shared ground return bus for all 12 indicator LEDs |
---

## Software & Library Requirements

- **Platform:** Arduino IDE / VS Code + PlatformIO
- **Core Microcontrollers:** ATmega328P (Arduino Nano as Slave Hub, Arduino/ESP as Master)
- **Required Libraries:**
  - `LiquidCrystal_I2C` (by Frank de Brabander / Marco Schwartz)
  - `Wire` (Built-in Arduino AVR Core library)

---

## Project Structure

```text
Adaptive-Traffic-Management-System/
├── .gitignore
├── LICENSE
├── libraries.txt
├── README.md
├── src/
│   ├── Master.ino
│   └── Slave.ino
└── docs/
    ├── Adaptive Traffic Management System Dual MCU Schematic Diagram.pdf
    └── Adaptive Traffic Management System Dual MCU Schematic Diagram.png