#include <OneWire.h> // Dallas Semi Temperature sensor
#include <ShiftDisplay.h> // 74HC595 Shift Registers for IEE FFD21 segment displays

OneWire ds(6); // Dallas Semi Sensor Pin

int latchPin = 4;
int clockPin = 3;
int dataPin = 2;

byte digit[] = {
  0x3F, // 0
  0x06, // 1
  0x5B, // 2
  0x4F, // 3
  0x66, // 4
  0x6D, // 5
  0x7D, // 6
  0x07, // 7
  0x7F, // 8
  0x6F  // 9
};

int d = 1000; //delay time
int dig1; // Digit One
int dig2; // Digit Two
int tempPin = 6; // Dallas Semi Sensor Pin
float temperature = 0;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  dig1 = 11;
  dig2 = 11;
  dispTemp();
  delay(d); // Delay for time defined as d
  analogReference(INTERNAL); // For Temp
}

void loop() {
  temp();
}

void temp() {
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
  int Temp;
  if (!ds.search(addr)) {
    ds.reset_search();
    return;
  }
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);
  delay(1000);
  present = ds.reset();
  ds.select(addr);
  ds.write(0xBE);

  for (i = 0; i < 9; i++) {
    data[i] = ds.read();

  }
  Temp = (data[1] << 8) + data[0]; // Take the two bytes from the response relating to temperature
  Temp = Temp >> 4; // Divide by 16 to get pure Celcius readout
  Temp = Temp * 1.8 + 32; // Math for Ferenheight - comment this line out to get Celcius

  if (Temp < 10) {
    dig1 = 10; // Show nothing
    dig2 = Temp; // Ones
    dispTemp();
  } else {
    dig1 = Temp / 10; // First Digit Tens
    dig2 = Temp - dig1 * 10; // Second Digit Ones
    dispTemp(); // Programm to run
  }
  delay(d);
}

void dispTemp() { // Controls the FFD21 displays with 74HC595 shift registers
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digit[dig1]); // Ones
  shiftOut(dataPin, clockPin, MSBFIRST, digit[dig2]); // Tens
  digitalWrite(latchPin, HIGH);
}
