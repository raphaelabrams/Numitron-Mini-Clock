#include "DS1302.h"

#include "WProgram.h"
void writeSegments(byte segments[]);
void setup();
void loop();
const static int CLOCK_CE   = 5;  // pin D5 = 5
const static int CLOCK_IO   = 6;  // pin D6 = 6
const static int CLOCK_SCLK = 7;  // pin D7 = 7

const static int SR_IN      = 14; // pin C0 = 14
const static int SR_G       = 15; // pin C1 = 15
const static int SR_RCK     = 16; // pin C2 = 16
const static int SR_SCK     = 18; // pin C4 = 18
const static int SR_SCLR    = 17; // pin C3 = 17

const static int DEBUG_LED  = 19; // pin C5 = 19

// Define our DS1302 clock
DS1302 clock(CLOCK_CE,CLOCK_IO,CLOCK_SCLK);

const static int TUBE_COUNT = 4;

void writeSegments(byte segments[]) {
  // clear latch
  digitalWrite(SR_RCK,LOW);
  for (int i = 0; i < TUBE_COUNT; i++) {
    shiftOut(SR_IN,SR_SCK,LSBFIRST,segments[i]);
  }
  // latch data
  digitalWrite(SR_RCK,HIGH);
}

const int digits[] = {
  0b10000100, // 0
  0b10011111, // 1
  0b10101000, // 2
  0b10001001, // 3
  0b10010011, // 4  
  0b11000001, // 5
  0b11000000, // 6
  0b10001111, // 7
  0b10000000, // 8
  0b10000011, // 9
  0b10000010, // a
  0b11010000, // b
  0b11111000, // c
  0b10011000, // d
  0b11100000, // e
  0b11100010  // f
};

void setup() {
    pinMode(SR_IN,OUTPUT);
    pinMode(SR_G,OUTPUT);
    pinMode(SR_RCK,OUTPUT);
    pinMode(SR_SCK,OUTPUT);
    pinMode(SR_SCLR,OUTPUT);
    
    digitalWrite(SR_G,LOW);
    digitalWrite(SR_SCLR,HIGH);

}

void loop() {
  byte data[4];
  data[0]=digits[4];
  data[1]=digits[5];
  data[2]=digits[6];
  data[3]=digits[7];
  writeSegments(data);    
}

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

