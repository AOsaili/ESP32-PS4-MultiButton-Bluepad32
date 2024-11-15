
# PS4 Multi-Control ESP32 Framework
This framework enables real-time control and feedback for PS4 controllers using an ESP32, making it ideal for gaming interfaces, IoT devices, and robotics applications.

## Description
This project leverages the ESP32-Bluepad32 library to enable seamless integration with PS4 controllers. Designed for gaming, IoT, and robotics applications, it offers advanced features for handling simultaneous button presses, real-time joystick inputs, and battery-level feedback through RGB LEDs.

## Features
- Multi-button input handling.
- Re-centered joystick values for improved control.
- Battery percentage feedback using fine-grained RGB LED transitions.
- Support for rumble and LED customization.

## Getting Started
### Prerequisites
- **Hardware**: ESP32, PS4 Controller.
- **Software**: Arduino IDE or PlatformIO.

### Installation
1. Clone the repository:
    ```bash
    git clone https://github.com/AOsaili/ESP32-PS4-MultiButton-Bluepad32
    ```
2. Install the ESP32-Bluepad32 library:
    - [Library Documentation](https://github.com/ricardoquesada/bluepad32)

### Usage
1. Flash the code to your ESP32 using your preferred IDE.
2. Pair the PS4 controller to your ESP32.
3. Monitor the Serial output to see button events, joystick values, and battery feedback.

## Contribution
Contributions are welcome! Please refer to the [CONTRIBUTING.md](CONTRIBUTING.md) file.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
