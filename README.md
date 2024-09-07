

## Multipurpose Robotics Control System
![Screenshot 2024-08-22 211430](https://github.com/user-attachments/assets/00be7167-4091-4071-a695-07763f5bfb43)

[Try it out yourself](https://wokwi.com/projects/401440266766602241)

## Overview

This project is a multipurpose robotics control system that integrates multiple components, including an MPU6050 sensor, a stepper motor, a servo motor, an OLED display, and an infrared (IR) receiver. The system reads data from an accelerometer, gyroscope, and temperature sensor, displays the data on an OLED screen, and controls a servo motor and stepper motor based on the input received from an IR remote.

## Features

- **MPU6050 Sensor**: Captures accelerometer, gyroscope, and temperature data.
- **OLED Display**: Displays real-time sensor data or system status messages.
- **Stepper Motor**: Controlled to perform full revolutions based on servo motor position.
- **Servo Motor**: Moves between 0° and 180° with adjustable increments.
- **IR Remote Control**: Allows toggling of the system's running state.
  
## Components

1. **Adafruit MPU6050**: Accelerometer and Gyroscope sensor.
2. **Adafruit SSD1306**: OLED Display (128x64 pixels).
3. **Stepper Motor**: Controlled by a DIR and STEP pin configuration.
4. **Servo Motor**: Controls position between 0° and 180°.
5. **IR Receiver**: Detects and decodes signals from an infrared remote.
6. **Microcontroller**: (e.g., ESP32, Arduino) to control the components.

## Wiring Diagram

### Connections:

- **MPU6050 Sensor**:
  - VCC to 3.3V
  - GND to GND
  - SCL to SCL
  - SDA to SDA
  
- **OLED Display**:
  - VCC to 3.3V
  - GND to GND
  - SCL to SCL
  - SDA to SDA

- **Stepper Motor**:
  - STEP_PIN (GP15)
  - DIR_PIN (GP14)
  - ENABLE_PIN (GP13)
  
- **Servo Motor**:
  - Signal to GP20
  - VCC to 5V
  - GND to GND
  
- **IR Receiver**:
  - Signal to IR_PIN (GP16)
  - VCC to 3.3V
  - GND to GND

## Dependencies

- Adafruit MPU6050 Library
- Adafruit Unified Sensor Library
- Adafruit SSD1306 Library
- Adafruit GFX Library
- IRremote Library
- Servo Library

Install the above libraries using the Arduino Library Manager.

## Code Description

1. **Setup Section**:
   - Initializes the MPU6050 sensor, OLED display, and stepper motor pins.
   - Displays a welcome message on the OLED display.
   - Attaches the servo motor and sets up the IR receiver.

2. **Loop Section**:
   - Continuously checks for IR input to toggle system operation.
   - If running, reads sensor data, updates the OLED display, and moves the servo motor.
   - Triggers a stepper motor revolution based on the servo motor's position.
   - Displays a "System Stopped" message when the system is paused.

## How to Run

1. Wire all components as per the wiring diagram.
2. Upload the provided code to your microcontroller.
3. Open the Serial Monitor at 115200 baud rate to view any debug messages.
4. Use the IR remote to start and stop the system. The OLED will show either sensor data or a "System Stopped" message.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- [Adafruit](https://www.adafruit.com/) for the hardware and libraries.
- [IRremote Library](https://github.com/Arduino-IRremote/Arduino-IRremote) for the IR receiver functionality.

## Future Enhancements

- Adding more sensors or actuators.
- Integrating wireless control using Wi-Fi or Bluetooth.
- Implementing data logging to an SD card.
