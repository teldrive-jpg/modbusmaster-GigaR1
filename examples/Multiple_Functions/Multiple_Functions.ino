/*
  Multiple Functions ModbusMaster_GigaR1 Example
  
  This example demonstrates various Modbus RTU functions using the
  ModbusMaster_GigaR1 library. It shows how to read and write different
  types of data (coils, discrete inputs, holding registers, input registers).
  
  Hardware:
  - Arduino Giga R1
  - RS485 transceiver (optional)
  - Modbus RTU slave device
  
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
  Serial.println("Multiple Functions ModbusMaster_GigaR1 Example");
  
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
  static uint8_t functionIndex = 0;
  
  switch (functionIndex) {
    case 0:
      // Read Coils (Function 0x01)
      Serial.println("--- Reading Coils ---");
      readCoilsExample();
      break;
      
    case 1:
      // Read Discrete Inputs (Function 0x02)
      Serial.println("--- Reading Discrete Inputs ---");
      readDiscreteInputsExample();
      break;
      
    case 2:
      // Read Holding Registers (Function 0x03)
      Serial.println("--- Reading Holding Registers ---");
      readHoldingRegistersExample();
      break;
      
    case 3:
      // Read Input Registers (Function 0x04)
      Serial.println("--- Reading Input Registers ---");
      readInputRegistersExample();
      break;
      
    case 4:
      // Write Single Coil (Function 0x05)
      Serial.println("--- Writing Single Coil ---");
      writeSingleCoilExample();
      break;
      
    case 5:
      // Write Single Register (Function 0x06)
      Serial.println("--- Writing Single Register ---");
      writeSingleRegisterExample();
      break;
      
    case 6:
      // Write Multiple Coils (Function 0x0F)
      Serial.println("--- Writing Multiple Coils ---");
      writeMultipleCoilsExample();
      break;
      
    case 7:
      // Write Multiple Registers (Function 0x10)
      Serial.println("--- Writing Multiple Registers ---");
      writeMultipleRegistersExample();
      break;
      
    case 8:
      // Mask Write Register (Function 0x16)
      Serial.println("--- Mask Write Register ---");
      maskWriteRegisterExample();
      break;
      
    case 9:
      // Read Write Multiple Registers (Function 0x17)
      Serial.println("--- Read Write Multiple Registers ---");
      readWriteMultipleRegistersExample();
      break;
  }
  
  functionIndex = (functionIndex + 1) % 10;
  delay(2000);
}

void readCoilsExample() {
  uint8_t result = node.readCoils(0, 8); // Read 8 coils starting from address 0
  
  if (result == node.ku8MBSuccess) {
    Serial.println("Coils read successfully:");
    for (int i = 0; i < 8; i++) {
      uint16_t coil = node.getResponseBuffer(i / 16);
      bool state = (coil >> (i % 16)) & 0x01;
      Serial.print("Coil ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(state ? "ON" : "OFF");
    }
  } else {
    Serial.print("Read coils failed. Error: 0x");
    Serial.println(result, HEX);
  }
}

void readDiscreteInputsExample() {
  uint8_t result = node.readDiscreteInputs(0, 8); // Read 8 discrete inputs starting from address 0
  
  if (result == node.ku8MBSuccess) {
    Serial.println("Discrete inputs read successfully:");
    for (int i = 0; i < 8; i++) {
      uint16_t input = node.getResponseBuffer(i / 16);
      bool state = (input >> (i % 16)) & 0x01;
      Serial.print("Input ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(state ? "ON" : "OFF");
    }
  } else {
    Serial.print("Read discrete inputs failed. Error: 0x");
    Serial.println(result, HEX);
  }
}

void readHoldingRegistersExample() {
  uint8_t result = node.readHoldingRegisters(0, 5); // Read 5 holding registers starting from address 0
  
  if (result == node.ku8MBSuccess) {
    Serial.println("Holding registers read successfully:");
    for (int i = 0; i < 5; i++) {
      uint16_t value = node.getResponseBuffer(i);
      Serial.print("Register ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(value);
    }
  } else {
    Serial.print("Read holding registers failed. Error: 0x");
    Serial.println(result, HEX);
  }
}

void readInputRegistersExample() {
  uint8_t result = node.readInputRegisters(0, 5); // Read 5 input registers starting from address 0
  
  if (result == node.ku8MBSuccess) {
    Serial.println("Input registers read successfully:");
    for (int i = 0; i < 5; i++) {
      uint16_t value = node.getResponseBuffer(i);
      Serial.print("Register ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(value);
    }
  } else {
    Serial.print("Read input registers failed. Error: 0x");
    Serial.println(result, HEX);
  }
}

void writeSingleCoilExample() {
  static bool coilState = false;
  uint8_t result = node.writeSingleCoil(0, coilState ? 0xFF00 : 0x0000);
  
  if (result == node.ku8MBSuccess) {
    Serial.print("Write single coil successful. State: ");
    Serial.println(coilState ? "ON" : "OFF");
    coilState = !coilState;
  } else {
    Serial.print("Write single coil failed. Error: 0x");
    Serial.println(result, HEX);
  }
}

void writeSingleRegisterExample() {
  static uint16_t value = 0;
  uint8_t result = node.writeSingleRegister(100, value);
  
  if (result == node.ku8MBSuccess) {
    Serial.print("Write single register successful. Value: ");
    Serial.println(value);
    value++;
  } else {
    Serial.print("Write single register failed. Error: 0x");
    Serial.println(result, HEX);
  }
}

void writeMultipleCoilsExample() {
  // Prepare coil data
  node.clearTransmitBuffer();
  node.setTransmitBuffer(0, 0x0001); // First 16 coils: only coil 0 ON
  
  uint8_t result = node.writeMultipleCoils(0, 16); // Write 16 coils starting from address 0
  
  if (result == node.ku8MBSuccess) {
    Serial.println("Write multiple coils successful");
  } else {
    Serial.print("Write multiple coils failed. Error: 0x");
    Serial.println(result, HEX);
  }
}

void writeMultipleRegistersExample() {
  // Prepare register data
  node.clearTransmitBuffer();
  for (int i = 0; i < 5; i++) {
    node.setTransmitBuffer(i, 1000 + i);
  }
  
  uint8_t result = node.writeMultipleRegisters(200, 5); // Write 5 registers starting from address 200
  
  if (result == node.ku8MBSuccess) {
    Serial.println("Write multiple registers successful");
  } else {
    Serial.print("Write multiple registers failed. Error: 0x");
    Serial.println(result, HEX);
  }
}

void maskWriteRegisterExample() {
  uint16_t andMask = 0x00FF; // Clear high byte
  uint16_t orMask = 0x5500;  // Set high byte to 0x55
  
  uint8_t result = node.maskWriteRegister(300, andMask, orMask);
  
  if (result == node.ku8MBSuccess) {
    Serial.println("Mask write register successful");
  } else {
    Serial.print("Mask write register failed. Error: 0x");
    Serial.println(result, HEX);
  }
}

void readWriteMultipleRegistersExample() {
  // Prepare write data
  node.clearTransmitBuffer();
  for (int i = 0; i < 3; i++) {
    node.setTransmitBuffer(i, 2000 + i);
  }
  
  uint8_t result = node.readWriteMultipleRegisters(0, 5, 400, 3);
  // Read 5 registers starting from address 0, write 3 registers starting from address 400
  
  if (result == node.ku8MBSuccess) {
    Serial.println("Read write multiple registers successful:");
    for (int i = 0; i < 5; i++) {
      uint16_t value = node.getResponseBuffer(i);
      Serial.print("Read Register ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(value);
    }
  } else {
    Serial.print("Read write multiple registers failed. Error: 0x");
    Serial.println(result, HEX);
  }
}
