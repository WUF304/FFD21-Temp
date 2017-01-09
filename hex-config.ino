#include <ShiftDisplay.h>
#define LATCH 4
#define CLK 3
#define DATA 2

//This is the hex value of each number stored in an array by index num
byte digitOne[10]= {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10};
byte digitTwo[10]= {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10};

int i;

void setup(){
 
  pinMode(LATCH, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(DATA, OUTPUT);
 
}

void loop(){
 
  for(int i=0; i<10; i++){
    for(int j=0; j<10; j++){
      digitalWrite(LATCH, LOW);
      shiftOut(DATA, CLK, MSBFIRST, ~digitTwo[i]); // digitTwo
      shiftOut(DATA, CLK, MSBFIRST, ~digitOne[j]); // digitOne
      digitalWrite(LATCH, HIGH);
      delay(1000);
    }
  }
}
