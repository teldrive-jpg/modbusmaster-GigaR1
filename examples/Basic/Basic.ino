/*
  Basic ModbusMaster_GigaR1 Example
  
  This example demonstrates basic Modbus RTU communication using the
  ModbusMaster_GigaR1 library. It reads holding registers from a Modbus slave.
  
  Hardware:
  - Arduino Giga R1
  - RS485 transceiver (optional)
  - Modbus RTU slave device
  
  Connections:
  - Connect TX/RX to your RS485 transceiver or directly to slave device
  - If using RS485, connect DE pin to control transmission direction
  
  Created: 2024
  Modified for Arduino Giga R1 compatibility
*/

#include <ModbusMaster_GigaR1.h>

// Create ModbusMaster object
ModbusMaster_GigaR1 node;

// RS485 control pin (if using RS485 transceiver)
#define DE_PIN 2

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);
  Serial.println("ModbusMaster_GigaR1 Basic Example");
  
  // Initialize RS485 control pin
  pinMode(DE_PIN, OUTPUT);
  digitalWrite(DE_PIN, LOW);
  
  // Initialize Modbus communication
  Serial1.begin(9600);
  node.begin(1, Serial1); // Slave ID = 1
  
  // Set RS485 control functions
  node.preTransmission([]() {
    digitalWrite(DE_PIN, HIGH);
    delay(1);
  });
  
  node.postTransmission([]() {
    delay(1);
    digitalWrite(DE_PIN, LOW);
  });
  
  Serial.println("ModbusMaster initialized");
}

void loop() {
  // Read 10 holding registers starting from address 0
  uint8_t result = node.readHoldingRegisters(0, 10);
  
  if (result == node.ku8MBSuccess) {
    Serial.println("Read successful:");
    
    // Print all received values
    for (int i = 0; i < 10; i++) {
      uint16_t value = node.getResponseBuffer(i);
      Serial.print("Register ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(value);
    }
  } else {
    Serial.print("Read failed. Error code: 0x");
    Serial.println(result, HEX);
    
    // Print specific error messages
    switch (result) {
      case node.ku8MBIllegalFunction:
        Serial.println("Illegal function");
        break;
      case node.ku8MBIllegalDataAddress:
        Serial.println("Illegal data address");
        break;
      case node.ku8MBIllegalDataValue:
        Serial.println("Illegal data value");
        break;
      case node.ku8MBSlaveDeviceFailure:
        Serial.println("Slave device failure");
        break;
      case node.ku8MBInvalidSlaveID:
        Serial.println("Invalid slave ID");
        break;
      case node.ku8MBInvalidFunction:
        Serial.println("Invalid function");
        break;
      case node.ku8MBResponseTimedOut:
        Serial.println("Response timeout");
        break;
      case node.ku8MBInvalidCRC:
        Serial.println("Invalid CRC");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }
  
  Serial.println();
  delay(2000); // Wait 2 seconds before next read
}
