#### ESP_Sensors_Integration

### Main Script

## Task-1      Failures
* Low Priority.
* Functions (Init, Fail, Normal)
* Failure includes dry contacts / switches.

## Task-2      Sensors
* Medium Priority.
* Reading Sensors.
* Thresholding based on different conditions.

## Task-3      Cooling System
* Mode1:
        Fan (will be dependent on Average value of Temperature Sensors).
* Mode2:
        Fan + Air Conditioner (Hybride Mode, Dependent on Temperature Difference).

## Task-4      CSU (Comms with ATMEGA 328P)
* High Priority.
* Using Serial Communication with ESP32.
* Read Sensors and update a JSON structure for passing to Front End.

## Task-5      485
* High Priority
* Third Party Hardware support.
* Mod-bus Protocol.

## Task-6      Wifi OPS
* Webserver of Read/Write.

## Task-7      GSM Integration
* Internet Connectivity Via GSM.

## Task-8      Over the Air (OTA) 
* May not include in first release.

## Task-9      MQTT
* Back-End Server Communication.

## Task-10     Data Logger
* Initialization (Includes RTC and Card)






