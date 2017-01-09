#include <OneWire.h> 
#include <ShiftDisplay.h>

OneWire ds(6); // sensor pin

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
int dig1;
int dig2;
int tempPin = 6; // LM35 Pin
float temperature = 0;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  dig1 = 11;
  dig2 = 11;
  dispTemp();
  delay(d); //wait a d
  analogReference(INTERNAL); //for Temp
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
  Temp = (data[1] << 8) + data[0]; //take the two bytes from the response relating to temperature
  Temp = Temp >> 4; //divide by 16 to get pure celcius readout
  Temp = Temp * 1.8 + 32; // comment this line out to get celcius

  if (Temp < 10) {
    dig1 = 10; //show nothing
    dig2 = Temp; //ones
    dispTemp();
  } else {
    dig1 = Temp / 10; //first digit tens
    dig2 = Temp - dig1 * 10; // second digit ones
    dispTemp(); // programm to run
  }
  delay(d);
}

void dispTemp() { //controls the display with shift registers
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digit[dig1]); //ones
  shiftOut(dataPin, clockPin, MSBFIRST, digit[dig2]); //tens
  digitalWrite(latchPin, HIGH);
}
