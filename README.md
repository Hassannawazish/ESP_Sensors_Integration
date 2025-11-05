# 🌡️ ESP32 Multi Sensor Project with FreeRTOS

This project uses an **ESP32** board with **FreeRTOS** to read data from different sensors at the same time.  
It checks **temperature**, **humidity**, **soil moisture**, **smoke**, and **door status** and shows everything on the Serial Monitor.

---

## 🧠 What It Does

- Reads **temperature and humidity** from a DHT11 sensor  
- Checks **wet or dry soil** using a soil moisture sensor  
- Detects **smoke or gas** using an MQ2 sensor  
- Tells if the **door is open or closed**  
- Uses **FreeRTOS tasks** so all sensors work together  
- Shows all values on the **Serial Monitor**

---

## 🔧 Parts You Need

| Part | Use |
|------|-----|
| ESP32 board | Main microcontroller |
| DHT11 | Measures temperature and humidity |
| MQ2 | Detects smoke or gas |
| Soil moisture sensor | Checks wet or dry condition |
| Magnetic door sensor | Detects door open or close |
| Jumper wires | For connections |
| Breadboard | For building the circuit |

---

## ⚙️ Pin Setup

| Sensor | ESP32 Pin | Description |
|---------|------------|-------------|
| DHT11 | GPIO16 (D0) | Data pin |
| Soil Moisture (Digital) | GPIO5 | Dry/Wet output |
| MQ2 | A0 | Analog smoke value |
| Door Sensor | GPIO34 | Digital input |

---

## 🧩 Task List

| Task | What it Does | Delay |
|------|---------------|--------|
| `taskTemperatureHumidity()` | Reads DHT11 sensor | 2 sec |
| `taskDigitalOutput()` | Reads soil sensor | 2 sec |
| `taskSmokeDetection()` | Checks MQ2 smoke sensor | 2 sec |
| `taskDoorStatus()` | Checks door status | 2 sec |

Each task runs on its own using FreeRTOS, so all work happens smoothly at once.
The circuit Diagram is shown here.
![Circuit](Circuit%20Diagram/Sensors_1_bb.png)

---
# ESP_Sensors_Integration
    1 : xTaskCreate(function_name,"Descriptive name for debugging purposes, stack size for the task ig 128, parameter we want to pass to the task,priority of task, task handler)
    
    Essential when you need to run multiple independent tasks concurrently in a FreeRTOS environment.It is infinite loop. We can do the profiling by printing integers within each thread.

    xTaskCreate(
    taskFunction,          // Task function
    "Task1",               // Task name
    128,                   // Stack size (in words)
    NULL,                  // No parameters
    1,                     // Task priority (1 is low, FreeRTOS supports multiple priorities)
    NULL                   // No task handle required
              );

# ESP IDF Implementation

