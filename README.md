# Smart Wearable for Elderly People Using ESP32

A prototype smart wearable system designed to support elderly-person monitoring through **health sensing, fall detection, GPS tracking, geofencing, and local/Wi-Fi-based status monitoring**.

> **Project status:** Prototype / academic mini project  
> **Controller:** ESP32  
> **Framework:** Arduino / Embedded C++  
> **Note:** Health values in this prototype are intended for demonstration and educational purposes and are not a substitute for certified medical equipment.

## Features

- ❤️ Heart-rate monitoring using a MAX3010x-family optical sensor
- 🫁 Estimated SpO₂ monitoring
- ⚠️ Basic fall detection using MPU6050 acceleration
- 📍 GPS location acquisition using a GPS module and TinyGPS++
- 🛡️ Geofencing using a predefined geographic boundary
- 🖥️ Local status display on an I²C OLED
- 🌐 ESP32-hosted web dashboard over Wi-Fi
- 📡 Periodic browser updates using a JSON endpoint

## System Overview

```text
                    SMART ELDERLY WEARABLE
                              │
                            ESP32
                              │
            ┌─────────────────┼─────────────────┐
            │                 │                 │
            ▼                 ▼                 ▼
        MAX3010x            MPU6050            GPS
            │                 │                 │
            ▼                 ▼                 ▼
      Heart Rate +        Fall Detection     Location
          SpO₂                                  │
            │                 │                 │
            └─────────────────┼─────────────────┘
                              ▼
                       Data Processing
                              │
                    ┌─────────┴─────────┐
                    ▼                   ▼
               OLED Display        Wi-Fi Server
                                        │
                                        ▼
                                Web Dashboard
                                        │
                                  ┌─────┴─────┐
                                  ▼           ▼
                              Health Data  Geofence
```

## Hardware

| Component | Purpose |
|---|---|
| ESP32 DevKit | Main microcontroller, processing and Wi-Fi |
| MAX3010x-family sensor module | Heart-rate and optical SpO₂ sensing |
| MPU6050 | Accelerometer/gyroscope for fall detection |
| GPS module (e.g. GY-GPS6MV2) | Latitude and longitude |
| 0.96-inch I²C OLED | Local display |
| Breadboard and jumper wires | Prototype interconnection |
| USB/power source | Power during testing |

## Software

The firmware uses:

- Arduino framework for ESP32
- `Wire.h` for I²C communication
- `WiFi.h` for ESP32 Wi-Fi
- `WebServer.h` for the local dashboard
- `MAX30105.h` for the MAX3010x-family sensor interface
- `Adafruit_GFX.h` and `Adafruit_SH110X.h` for the OLED
- `MPU6050.h` for motion sensing
- `TinyGPS++.h` for GPS parsing

## Repository Structure

```text
smart-elderly-wearable/
├── src/
│   └── smart_elderly_wearable.ino
├── docs/
│   ├── hardware/
│   │   ├── prototype_01.jpeg
│   │   ├── prototype_02.jpeg
│   │   └── prototype_03.jpeg
│   ├── screenshots/
│   │   ├── web_dashboard_01.jpeg
│   │   └── web_dashboard_02.jpeg
│   ├── circuit/
│   │   └── circuit_diagram.jpeg
│   └── report/
│       └── REPORT_PLACEHOLDER.txt
├── .gitignore
└── README.md
```

## Prototype Hardware

### Prototype 1
![Prototype 1](docs/hardware/prototype_01.jpeg)

### Prototype 2
![Prototype 2](docs/hardware/prototype_02.jpeg)

### Prototype 3
![Prototype 3](docs/hardware/prototype_03.jpeg)

## Web Dashboard

The ESP32 hosts a lightweight web dashboard that displays BPM, SpO₂, fall status, geofence status, latitude, and longitude.

### Dashboard Screenshot 1
![Web dashboard](docs/screenshots/web_dashboard_01.jpeg)

### Dashboard Screenshot 2
![Web dashboard](docs/screenshots/web_dashboard_02.jpeg)

## Circuit Diagram

![Circuit diagram](docs/circuit/circuit_diagram.jpeg)

## Setup

1. Install the ESP32 board support package in Arduino IDE.
2. Install the required sensor/display/GPS libraries.
3. Open `src/smart_elderly_wearable.ino`.
4. Set the Wi-Fi credentials in the firmware for local testing.
5. Connect the hardware according to the circuit diagram.
6. Select the correct ESP32 board and serial port.
7. Upload the firmware.
8. Open Serial Monitor at `115200` baud.
9. After Wi-Fi connection, note the ESP32 local IP address.
10. Open that IP address in a browser connected to the same network.

## Important Notes

- Do **not** commit real Wi-Fi passwords or other secrets to GitHub.
- The current firmware is a prototype and should be tested and calibrated before being used for real-world health or safety decisions.
- The SpO₂ calculation in the current firmware is an estimated prototype calculation, not a clinically validated measurement.
- The current fall logic latches after an acceleration threshold is crossed and should be improved for a production-quality wearable.
- GPS requires suitable satellite reception; initial location acquisition may take time.

## Future Improvements

- Improve fall detection using acceleration magnitude, orientation, and time-window analysis.
- Add an alert mechanism for falls or geofence violations.
- Replace hard-coded Wi-Fi credentials with a secure configuration method.
- Improve GPS/geofence robustness and numerical edge-case handling.
- Add data logging and historical graphs.
- Add a mobile-friendly dashboard.
- Add battery monitoring and power optimization.
- Package the prototype into an actual wearable enclosure.
- Calibrate and validate health-sensing algorithms against appropriate reference equipment.

## Project Documentation

The final project report will be added later:

`docs/report/`

Replace the placeholder with the final PDF when it is ready.

## License

Add the license required by your institution/team before publishing the repository publicly.
