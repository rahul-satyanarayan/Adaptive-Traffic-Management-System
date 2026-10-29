#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize the 16x2 I2C LCD (Address 0x27)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Arrays for mapping parsed bits back to names
const char* sensorNames[] = {"N1", "N2", "N3", "E1", "E2", "E3", "S1", "S2", "S3", "W1", "W2", "W3"};
bool sensorStates[12] = {false};

unsigned long lastLCDUpdate = 0;
int lcdDisplayPage = 0; // Alternates views to fit all 12 sensors on a 16x2 screen

void setup() {
  Wire.begin();        // Join I2C Bus as Master
  Serial.begin(115200); // Fast baud rate for dense diagnostic stream
  
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("IR Matrix Test");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(1500);
}

void loop() {
  // 1. Request 2 bytes (16-bits) from Slave Nano Hub at address 0x08
  Wire.requestFrom(0x08, 2);
  
  if (Wire.available() == 2) {
    byte lowByte = Wire.read();
    byte highByte = Wire.read();
    
    // Reconstruct the 16-bit integer
    uint16_t packedData = (highByte << 8) | lowByte;
    
    // 2. Unpack the bits back into individual booleans
    for (int i = 0; i < 12; i++) {
      sensorStates[i] = (packedData & (1 << i));
    }
    
    // 3. Output detailed layout to the Serial Monitor
    printSerialDiagnostics();
    
  } else {
    Serial.println("[ERROR] Failed to fetch data stream from Sensor Hub.");
  }

  // 4. Update the 16x2 LCD layout cleanly every 1 second (Avoids flicker)
  if (millis() - lastLCDUpdate >= 1000) {
    lastLCDUpdate = millis();
    updateLCDDiagnostics();
    lcdDisplayPage = !lcdDisplayPage; // Alternate screen pages
  }

  delay(2000); 
}

// Prints a visual matrix grid in the Serial Monitor for rapid hardware troubleshooting
void printSerialDiagnostics() {
  Serial.println("\n--- Real-Time Intersection Density Map ---");
  
  // Print North
  Serial.print("NORTH: ");
  for(int i=0; i<3; i++) { Serial.print(sensorNames[i]); Serial.print(sensorStates[i] ? "[X] " : "[ ] "); }
  Serial.println();
  
  // Print East
  Serial.print("EAST:  ");
  for(int i=3; i<6; i++) { Serial.print(sensorNames[i]); Serial.print(sensorStates[i] ? "[X] " : "[ ] "); }
  Serial.println();
  
  // Print South
  Serial.print("SOUTH: ");
  for(int i=6; i<9; i++) { Serial.print(sensorNames[i]); Serial.print(sensorStates[i] ? "[X] " : "[ ] "); }
  Serial.println();
  
  // Print West
  Serial.print("WEST:  ");
  for(int i=9; i<12; i++) { Serial.print(sensorNames[i]); Serial.print(sensorStates[i] ? "[X] " : "[ ] "); }
  Serial.println("\n-------------------------------------------");
}

// Helper function to render 6 sensors per screen loop on the 16x2 space
void updateLCDDiagnostics() {
  lcd.clear();
  if (lcdDisplayPage == 0) {
    // Page 0: Display North and East states
    lcd.setCursor(0, 0);
    lcd.print("N:");
    lcd.print(sensorStates[0] ? "X " : "- ");
    lcd.print(sensorStates[1] ? "X " : "- ");
    lcd.print(sensorStates[2] ? "X" : "-");
    
    lcd.setCursor(9, 0);
    lcd.print("E:");
    lcd.print(sensorStates[3] ? "X " : "- ");
    lcd.print(sensorStates[4] ? "X " : "- ");
    lcd.print(sensorStates[5] ? "X" : "-");
    
    lcd.setCursor(0, 1);
    lcd.print("[Viewing: N & E]");
  } else {
    // Page 1: Display South and West states
    lcd.setCursor(0, 0);
    lcd.print("S:");
    lcd.print(sensorStates[6] ? "X " : "- ");
    lcd.print(sensorStates[7] ? "X " : "- ");
    lcd.print(sensorStates[8] ? "X" : "-");
    
    lcd.setCursor(9, 0);
    lcd.print("W:");
    lcd.print(sensorStates[9] ? "X " : "- ");
    lcd.print(sensorStates[10] ? "X " : "- ");
    lcd.print(sensorStates[11] ? "X" : "-");
    
    lcd.setCursor(0, 1);
    lcd.print("[Viewing: S & W]");
  }
}