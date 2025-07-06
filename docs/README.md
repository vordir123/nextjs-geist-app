# ESP32 eBike Tuning Chip - Development Platform

A comprehensive development platform for creating ESP32-WROOM-32 based tuning chips for Bosch eBike drive systems. This project provides both the embedded firmware and a modern web-based configuration interface.

## 🚀 Project Overview

This platform enables the development of intelligent tuning solutions that can lift the 25 km/h speed limitation on Bosch eBike systems through real-time signal manipulation and CAN-bus communication.

### Key Features

- **Intelligent Signal Processing**: Real-time speed signal manipulation with adaptive algorithms
- **Multi-Mode Operation**: Eco, Sport, Unlimited, and Stealth modes
- **OTA Updates**: WiFi-based firmware updates with version management
- **Safety Features**: Anti-tamper detection, temperature monitoring, emergency shutdown
- **Web Configuration**: Modern React-based configuration interface
- **Multi-Generation Support**: Compatible with Bosch Active Line, Performance Line, Performance CX, and Smart System

## 🛠 Hardware Specifications

### Core Components
- **Microcontroller**: ESP32-WROOM-32 (Dual-Core, 240 MHz)
- **Connectivity**: WiFi 802.11 b/g/n + Bluetooth 4.2
- **CAN Interface**: TJA1050 or MCP2551 transceiver
- **Power Supply**: 12V/5V step-down to 3.3V
- **Form Factor**: 40x30x10mm (compact design)
- **Protection**: IP65 rating (dust and water resistant)

### Supported Bosch Systems
- Bosch Active Line (Gen 1-4)
- Bosch Performance Line (Gen 1-4)
- Bosch Performance CX (Gen 1-4)
- Bosch Smart System (BES3, Gen 5) - *In Development*

## 📁 Project Structure

```
├── src/                          # Next.js web application
│   ├── app/                      # App router pages
│   ├── components/               # React components
│   └── lib/                      # Utility functions
├── firmware/                     # ESP32 firmware
│   ├── src/                      # Source code
│   │   ├── main.cpp             # Main application
│   │   └── modules/             # Firmware modules
│   └── include/                  # Header files
├── docs/                         # Documentation
│   ├── hardware/                 # Hardware documentation
│   ├── firmware/                 # Firmware documentation
│   └── installation/             # Installation guides
└── public/                       # Static assets
```

## 🚀 Quick Start

### Prerequisites
- Node.js 18+ and npm
- Arduino IDE or PlatformIO
- ESP32 development board
- CAN transceiver module

### Web Application Setup
```bash
# Clone the repository
git clone <repository-url>
cd esp32-ebike-tuning-chip

# Install dependencies
npm install

# Start development server
npm run dev
```

The web application will be available at `http://localhost:8000`

### Firmware Development
1. Open the `firmware/` directory in Arduino IDE or PlatformIO
2. Configure your ESP32 board settings
3. Install required libraries:
   - CAN library
   - WiFi library
   - ArduinoJson
4. Upload the firmware to your ESP32

## 🔧 Configuration

### Web Interface
The web application provides a comprehensive interface for:
- Device status monitoring
- Operating mode selection
- Speed limit configuration
- OTA firmware updates
- Diagnostic logging

### Firmware Configuration
Key configuration parameters in `firmware/src/config.h`:
```cpp
#define FIRMWARE_VERSION "1.0.0"
#define CAN_TX_PIN 5
#define CAN_RX_PIN 4
#define SPEED_SENSOR_PIN 2
#define DEFAULT_SPEED_LIMIT 35
```

## 📊 Operating Modes

### Eco Mode
- Conservative speed increase
- Battery-friendly operation
- Smooth acceleration curves

### Sport Mode
- Balanced performance and efficiency
- Enhanced responsiveness
- Moderate speed increase

### Unlimited Mode
- Maximum performance
- Removes speed limitations
- Advanced signal processing

### Stealth Mode
- Undetectable operation
- Minimal diagnostic footprint
- Automatic fallback capability

## 🛡 Safety Features

### Hardware Safety
- ESD protection circuits
- Overvoltage protection
- Thermal shutdown
- Reverse polarity protection

### Software Safety
- Watchdog timer protection
- Temperature monitoring
- Battery voltage checking
- Anti-tamper detection
- Emergency shutdown capability

## 📖 Documentation

### Hardware Documentation
- [Schematic Diagrams](docs/hardware/schematics.md)
- [PCB Layout Guide](docs/hardware/pcb-layout.md)
- [Assembly Instructions](docs/hardware/assembly.md)
- [Testing Procedures](docs/hardware/testing.md)

### Firmware Documentation
- [API Reference](docs/firmware/api-reference.md)
- [Module Documentation](docs/firmware/modules.md)
- [Development Guide](docs/firmware/development.md)
- [Debugging Guide](docs/firmware/debugging.md)

### Installation & Usage
- [Installation Guide](docs/installation/installation-guide.md)
- [Configuration Manual](docs/installation/configuration.md)
- [Troubleshooting](docs/installation/troubleshooting.md)

## ⚖️ Legal & Safety Notice

**IMPORTANT**: This device is intended for use on private property only. Modifying your eBike may:
- Void manufacturer warranty
- Violate local traffic laws and regulations
- Affect insurance coverage
- Impact safety systems

Users are solely responsible for compliance with applicable laws and regulations in their jurisdiction.

### Compliance
- CE marking and EMV conformity
- RoHS compliance
- FCC Part 15 (for US markets)

## 🧪 Testing & Validation

### Hardware Testing
- Signal integrity validation
- EMC/EMI testing
- Environmental testing (IP65)
- Thermal testing

### Software Testing
- Unit tests for all modules
- Integration testing
- Real-world validation
- Safety system testing

## 🤝 Contributing

We welcome contributions from the community! Please read our [Contributing Guide](CONTRIBUTING.md) for details on:
- Code style guidelines
- Pull request process
- Issue reporting
- Development setup

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🆘 Support

- **Documentation**: Check the `docs/` directory
- **Issues**: Report bugs via GitHub Issues
- **Discussions**: Join our community discussions
- **Email**: support@esp32-ebike-tuning.com

## 🗺 Roadmap

### Version 1.1 (Q2 2024)
- [ ] Bosch Gen 5 Smart System support
- [ ] Mobile app for iOS/Android
- [ ] Advanced diagnostics dashboard
- [ ] Cloud-based configuration sync

### Version 1.2 (Q3 2024)
- [ ] Machine learning-based optimization
- [ ] Multi-bike profile support
- [ ] Enhanced stealth algorithms
- [ ] Bluetooth Low Energy support

### Version 2.0 (Q4 2024)
- [ ] Support for other eBike manufacturers
- [ ] Advanced telemetry and analytics
- [ ] Professional installer tools
- [ ] Enterprise management features

## 🙏 Acknowledgments

- ESP32 community for excellent documentation and libraries
- Bosch eBike systems for inspiring this project
- Open source contributors and testers
- eBike enthusiast community for feedback and support

---

**Disclaimer**: This project is for educational and research purposes. Always comply with local laws and regulations when modifying electric vehicles.
