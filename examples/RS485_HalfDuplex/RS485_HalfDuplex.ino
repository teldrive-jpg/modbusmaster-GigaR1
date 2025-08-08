/*
  RS485 Half-Duplex ModbusMaster_GigaR1 Example
  
  This example demonstrates Modbus RTU communication over RS485 using
  the ModbusMaster_GigaR1 library. It shows how to properly control
  the RS485 transceiver for half-duplex communication.
  
  Hardware:
  - Arduino Giga R1
  - RS485 transceiver (e.g., MAX485, SN75176)
  - Modbus RTU slave device
  
  Connections:
  - Arduino TX -> RS485 DI (Driver Input)
  - Arduino RX -> RS485 RO (Receiver Output)
  - Arduino Pin 2 -> RS485 DE (Driver Enable)
  - Arduino Pin 3 -> RS485 RE (Receiver Enable) - optional
  - RS485 A+ -> Slave A+
  - RS485 B- -> Slave B-
  - Common ground between Arduino and slave
  
  Created: 2024
  Modified for Arduino Giga R1 compatibility
*/

#include <ModbusMaster_GigaR1.h>

// Create ModbusMaster object
ModbusMaster_GigaR1 node;

// RS485 control pins
#define DE_PIN 2    // Driver Enable
#define RE_PIN 3    // Receiver Enable (optional, can be tied to DE)

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);
  Serial.println("RS485 Half-Duplex ModbusMaster_GigaR1 Example");
  
  // Initialize RS485 control pins
  pinMode(DE_PIN, OUTPUT);
  pinMode(RE_PIN, OUTPUT);
  digitalWrite(DE_PIN, LOW);
  digitalWrite(RE_PIN, LOW);
  
  // Initialize Modbus communication
  Serial1.begin(9600);
  node.begin(1, Serial1); // Slave ID = 1
  
  // Set RS485 control functions for transmission
  node.preTransmission([]() {
    digitalWrite(DE_PIN, HIGH);  // Enable driver
    digitalWrite(RE_PIN, HIGH);  // Disable receiver (optional)
    delay(1); // Allow time for signal to stabilize
  });
  
  node.postTransmission([]() {
    delay(1); // Allow time for transmission to complete
    digitalWrite(DE_PIN, LOW);   // Disable driver
    digitalWrite(RE_PIN, LOW);   // Enable receiver
  });
  
  Serial.println("RS485 ModbusMaster initialized");
}

void loop() {
  static uint16_t counter = 0;
  
  // Read holding registers
  uint8_t result = node.readHoldingRegisters(0, 5);
  
  if (result == node.ku8MBSuccess) {
    Serial.println("Read successful:");
    
    // Print received values
    for (int i = 0; i < 5; i++) {
      uint16_t value = node.getResponseBuffer(i);
      Serial.print("Register ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(value);
    }
    
    // Write a counter value to register 100
    result = node.writeSingleRegister(100, counter);
    
    if (result == node.ku8MBSuccess) {
      Serial.print("Write successful. Counter: ");
      Serial.println(counter);
      counter++;
    } else {
      Serial.print("Write failed. Error code: 0x");
      Serial.println(result, HEX);
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
  delay(3000); // Wait 3 seconds before next transaction
}
