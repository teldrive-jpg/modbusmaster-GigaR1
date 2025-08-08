# ModbusMaster_GigaR1

Arduino library for communicating with Modbus slaves over RS232/485 (via RTU protocol). Optimized for Arduino Giga R1.

## Description

This library provides a simple and efficient way to communicate with Modbus RTU slaves. It has been specifically optimized for the Arduino Giga R1 board, with improved timing and reliability features.

### Features

- **Optimized for Arduino Giga R1**: Enhanced timing parameters and retry mechanisms
- **Full Modbus RTU Support**: All standard Modbus functions
- **RS485 Support**: Built-in support for RS485 transceivers via callback functions
- **Error Handling**: Comprehensive exception handling and status reporting
- **Buffer Management**: Efficient buffer handling for large data transfers
- **Debug Support**: Optional debug pin outputs for troubleshooting

### Supported Modbus Functions

- **Read Operations**:
  - Read Coils (0x01)
  - Read Discrete Inputs (0x02)
  - Read Holding Registers (0x03)
  - Read Input Registers (0x04)

- **Write Operations**:
  - Write Single Coil (0x05)
  - Write Single Register (0x06)
  - Write Multiple Coils (0x0F)
  - Write Multiple Registers (0x10)

- **Advanced Operations**:
  - Mask Write Register (0x16)
  - Read Write Multiple Registers (0x17)

## Installation

1. Download the library
2. Extract to your Arduino libraries folder
3. Restart Arduino IDE
4. Include the library in your sketch: `#include <ModbusMaster_GigaR1.h>`

## Quick Start

```cpp
#include <ModbusMaster_GigaR1.h>

// Create ModbusMaster object
ModbusMaster_GigaR1 node;

void setup() {
  // Initialize serial communication
  Serial1.begin(9600);
  
  // Initialize ModbusMaster
  node.begin(1, Serial1); // Slave ID = 1
  
  // Optional: Set callback functions for RS485
  node.preTransmission([]() {
    digitalWrite(DE_PIN, HIGH); // Enable RS485 driver
  });
  
  node.postTransmission([]() {
    digitalWrite(DE_PIN, LOW);  // Disable RS485 driver
  });
}

void loop() {
  // Read holding registers
  uint8_t result = node.readHoldingRegisters(0, 10);
  
  if (result == node.ku8MBSuccess) {
    // Process data
    uint16_t value = node.getResponseBuffer(0);
    Serial.println(value);
  }
}
```

## API Reference

### Constructor
```cpp
ModbusMaster_GigaR1()
```

### Initialization
```cpp
void begin(uint8_t slave, Stream &serial)
```
- `slave`: Modbus slave ID (1-255)
- `serial`: Reference to serial port (Serial, Serial1, etc.)

### Callback Functions
```cpp
void idle(void (*idle)())
void preTransmission(void (*preTransmission)())
void postTransmission(void (*postTransmission)())
```

### Read Functions
```cpp
uint8_t readCoils(uint16_t address, uint16_t quantity)
uint8_t readDiscreteInputs(uint16_t address, uint16_t quantity)
uint8_t readHoldingRegisters(uint16_t address, uint16_t quantity)
uint8_t readInputRegisters(uint16_t address, uint8_t quantity)
```

### Write Functions
```cpp
uint8_t writeSingleCoil(uint16_t address, uint8_t state)
uint8_t writeSingleRegister(uint16_t address, uint16_t value)
uint8_t writeMultipleCoils(uint16_t address, uint16_t quantity)
uint8_t writeMultipleRegisters(uint16_t address, uint16_t quantity)
uint8_t maskWriteRegister(uint16_t address, uint16_t andMask, uint16_t orMask)
```

### Buffer Management
```cpp
uint16_t getResponseBuffer(uint8_t index)
void clearResponseBuffer()
uint8_t setTransmitBuffer(uint8_t index, uint16_t value)
void clearTransmitBuffer()
```

### Status Constants
```cpp
ku8MBSuccess              // Transaction successful
ku8MBIllegalFunction      // Illegal function code
ku8MBIllegalDataAddress   // Illegal data address
ku8MBIllegalDataValue     // Illegal data value
ku8MBSlaveDeviceFailure   // Slave device failure
ku8MBInvalidSlaveID       // Invalid slave ID in response
ku8MBInvalidFunction      // Invalid function code in response
ku8MBResponseTimedOut     // Response timeout
ku8MBInvalidCRC           // Invalid CRC in response
```

## Examples

### Basic Usage
```cpp
#include <ModbusMaster_GigaR1.h>

ModbusMaster_GigaR1 node;

void setup() {
  Serial1.begin(9600);
  node.begin(1, Serial1);
}

void loop() {
  uint8_t result = node.readHoldingRegisters(0, 1);
  
  if (result == node.ku8MBSuccess) {
    uint16_t value = node.getResponseBuffer(0);
    Serial.println(value);
  }
  
  delay(1000);
}
```

### RS485 Communication
```cpp
#include <ModbusMaster_GigaR1.h>

#define DE_PIN 2

ModbusMaster_GigaR1 node;

void setup() {
  pinMode(DE_PIN, OUTPUT);
  Serial1.begin(9600);
  node.begin(1, Serial1);
  
  // Set RS485 control functions
  node.preTransmission([]() {
    digitalWrite(DE_PIN, HIGH);
    delay(1);
  });
  
  node.postTransmission([]() {
    delay(1);
    digitalWrite(DE_PIN, LOW);
  });
}

void loop() {
  uint8_t result = node.readHoldingRegisters(0, 10);
  
  if (result == node.ku8MBSuccess) {
    for (int i = 0; i < 10; i++) {
      Serial.print(node.getResponseBuffer(i));
      Serial.print(" ");
    }
    Serial.println();
  }
  
  delay(1000);
}
```

### Writing Data
```cpp
#include <ModbusMaster_GigaR1.h>

ModbusMaster_GigaR1 node;

void setup() {
  Serial1.begin(9600);
  node.begin(1, Serial1);
}

void loop() {
  // Write single register
  uint8_t result = node.writeSingleRegister(0, 12345);
  
  if (result == node.ku8MBSuccess) {
    Serial.println("Write successful");
  }
  
  delay(1000);
}
```

## Giga R1 Optimizations

This library includes several optimizations specifically for the Arduino Giga R1:

- **Enhanced Timing**: Optimized inter-frame delays and byte timeouts
- **Retry Mechanism**: Automatic retry with exponential backoff
- **Improved Reliability**: Better handling of serial communication timing
- **Reduced Latency**: Optimized for the Giga R1's faster processor

## Troubleshooting

### Common Issues

1. **No Response**: Check slave ID, baud rate, and wiring
2. **CRC Errors**: Verify cable quality and length
3. **Timeout Errors**: Increase timeout values or check slave device
4. **RS485 Issues**: Ensure proper DE/RE pin control

### Debug Mode

Enable debug mode by uncommenting in the header file:
```cpp
#define __MODBUSMASTER_DEBUG__ (1)
```

This will output debug signals on pins 4 and 5.

## License

This library is licensed under the Apache License, Version 2.0.

## Credits

Original library by Doc Walker (2009-2016)
Modified for Arduino Giga R1 compatibility

## Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.
