#include "DS1302.h"
#include <avr/interrupt.h>

const static int CLOCK_CE   = 5;  // pin D5 = 5
const static int CLOCK_IO   = 6;  // pin D6 = 6
const static int CLOCK_SCLK = 7;  // pin D7 = 7

const static int SR_IN      = 14; // pin C0 = 14
const static int SR_G       = 15; // pin C1 = 15
const static int SR_RCK     = 16; // pin C2 = 16
const static int SR_SCK     = 18; // pin C4 = 18
const static int SR_SCLR    = 17; // pin C3 = 17

const static int DEBUG_LED  = 19; // pin C5 = 19

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

void writeNumbers(byte numbers[]) {
  byte converted[4];
  for(int i = 0; i < TUBE_COUNT; i++) {
    converted[i] = digits[numbers[i]];
  }
  writeSegments(converted);
}

// Define our DS1302 clock
DS1302 clock(CLOCK_CE,CLOCK_IO,CLOCK_SCLK);

void setup() {
    pinMode(SR_IN,OUTPUT);
    pinMode(SR_G,OUTPUT);
    pinMode(SR_RCK,OUTPUT);
    pinMode(SR_SCK,OUTPUT);
    pinMode(SR_SCLR,OUTPUT);

    pinMode(DEBUG_LED,OUTPUT);
    
    digitalWrite(DEBUG_LED,LOW);
    digitalWrite(SR_G,LOW);
    digitalWrite(SR_SCLR,HIGH);

  // Use clock 2 as a somewhat pokey PWM for the G channel,
  // since we didn't choose a pin that handles PWM.
  // We run clock two in normal mode with overflow and output compare
  // A register matches on.
  // The prescaler is at 1/64, giving us PWM at 1KHz; more than enough
  // for controlling an incandescent bulb.
  TCCR2A = 0x00;
  TCCR2B = 0x04;
  TIMSK2 = 0x03;
  OCR2A  = 0x80;
  
    clock.halt(false);
}

int upswing = 0;
int brightness = 64;

void loop() {
  byte data[4];
  uint8_t seconds = clock.seconds();
  uint8_t minutes = clock.minutes();
  data[3]=seconds%10;
  data[2]=(seconds/10)%10;
  data[1]=minutes%10;
  data[0]=(minutes/10)%10;
  writeNumbers(data);
  delay(100);
  if (upswing) {
    brightness++;
    if (brightness > 230) { upswing = 0; }
  } else {
    brightness--;
    if (brightness < 64) { upswing = 1; }
  }
  OCR2A = brightness;

}

// Timer interrupts
ISR(TIMER2_COMPA_vect) {
  digitalWrite(SR_G,HIGH);
}

ISR(TIMER2_OVF_vect) {
  digitalWrite(SR_G,LOW);
}
