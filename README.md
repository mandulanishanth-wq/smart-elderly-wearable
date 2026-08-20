# Smart Wearable for Elderly People Using ESP32

A prototype smart wearable system designed to support elderly-person monitoring through **health sensing, fall detection, GPS tracking, geofencing, and local/Wi-Fi-based status monitoring**.

> **Project status:** Prototype / academic mini project  
> **Controller:** ESP32  
> **Framework:** Arduino / Embedded C++

## Features

- ❤️ Heart-rate monitoring using a MAX3010x-family optical sensor
- 🫁 Estimated SpO₂ monitoring
- ⚠️ Basic fall detection using MPU6050 acceleration
- 📍 GPS location acquisition using a GPS module and TinyGPS++
- 🛡️ Geofencing using a predefined geographic boundary
- 🖥️ Local status display on an I²C OLED
- 🌐 ESP32-hosted web dashboard over Wi-Fi
- 📡 Periodic browser updates using a JSON endpoint

## Hardware

| Component | Purpose |
|---|---|
| ESP32 DevKit | Main microcontroller, processing and Wi-Fi |
| MAX3010x-family sensor | Heart-rate and SpO₂ sensing |
| MPU6050 | Fall detection |
| GPS module | Latitude and longitude |
| 0.96-inch I²C OLED | Local display |

## Software

- Arduino framework for ESP32
- WiFi
- WebServer
- MAX30105
- Adafruit GFX
- Adafruit SH110X
- MPU6050
- TinyGPS++

## Repository Structure

```text
smart-elderly-wearable/
├── src/
│   └── smart_elderly_wearable.ino
├── docs/
│   ├── hardware/
│   │   ├── hardware_1.jpeg
│   │   ├── hardware_2.jpeg
│   │   └── hardware_3.jpeg
│   ├── screenshots/
│   │   ├── web_dashboard_1.jpeg
│   │   └── web_dashboard_2.jpeg
│   ├── circuit/
│   │   └── ckt.jpeg
│   └── report/
│       └── Smart_Elderly_Wearable_Project_Report.pdf.pdf
├── .gitignore
└── README.md
