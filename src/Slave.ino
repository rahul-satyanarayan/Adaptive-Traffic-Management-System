#include <Wire.h>

// Define total sensors and pin mappings for the 4 directions
const int TOTAL_SENSORS = 12;
const int irPins[TOTAL_SENSORS] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

// Global 16-bit variable to hold all 12 sensor states (2 bytes)
volatile uint16_t packedSensorData = 0;

void setup() {
  // Initialize all 12 pins as inputs with internal pull-ups for Active Low logic
  for (int i = 0; i < TOTAL_SENSORS; i++) {
    pinMode(irPins[i], INPUT_PULLUP);
  }
  
  Wire.begin(0x08);                // Initialize Slave with I2C Address 0x08
  Wire.onRequest(sendSensorData);  // Interrupt handler for Master requests
}

void loop() {
  uint16_t currentPacked = 0;
  
  for (int i = 0; i < TOTAL_SENSORS; i++) {
    // Active Low Logic: Sensor triggers 'LOW' (0) when a vehicle is present.
    // We invert it using '!' so a '1' bit in our variable means "Vehicle Detected".
    if (digitalRead(irPins[i]) == LOW) {
      currentPacked |= (1 << i);
    }
  }
  
  packedSensorData = currentPacked;
  delay(50); // High-speed sampling interval
}

// Automatically runs when Master requests data
void sendSensorData() {
  // Break the 16-bit integer into two 8-bit bytes for I2C transmission
  byte lowByte = packedSensorData & 0xFF;
  byte highByte = (packedSensorData >> 8) & 0xFF;
  
  Wire.write(lowByte);
  Wire.write(highByte);
}