#define F_CPU 8000000
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define SET_OUTPUT(P,B) { DDR##P |= _BV(B); }
#define SET_INPUT(P,B) { DDR##P &= ~_BV(B); }

#define SET_PIN(P,B) { PORT##P |= _BV(B); }
#define CLR_PIN(P,B) { PORT##P &= ~_BV(B); }

#define SET_DIN()   SET_PIN(A,0)
#define CLR_DIN()   CLR_PIN(A,0)

#define SET_CLK()   SET_PIN(A,2)
#define CLR_CLK()   CLR_PIN(A,2)

#define SET_LOAD()   SET_PIN(A,1)
#define CLR_LOAD()   CLR_PIN(A,1)

#define GET_PIN(P,B) ((PIN##P & _BV(B)) != 0)

void init();
void display();

void init()
{
  // init pins
  SET_OUTPUT(A,0);
  SET_OUTPUT(A,1);
  SET_OUTPUT(A,2);
  // init clock
  TCCR1A = 0x00;  // WGM1 = 0, CM = 0
  TCCR1B = 0x03;  // CS = clk/8
  TIMSK1 = 0x01;  // overflow interrupt enabled
  sei();
}

volatile int update = 1;
volatile int val = 0;

const int digits[] = {
  0b01111011, // 0
  0b01100000, // 1
  0b01010111, // 2
  0b01110110, // 3
  0b01101100, // 4
  0b00111110, // 5
  0b00111111, // 6
  0b01110000, // 7
  0b01111111, // 8
  0b01111100, // 9
  0b01111101, // a
  0b00101111, // b
  0b00000111, // c
  0b01100111, // d
  0b00011111, // e
  0b00011101  // f
};

void sendCommand(uint8_t address, uint8_t data);

enum {
  CMD_DIGIT_0 = 0x01,
  // ...
  CMD_DECODE_MODE = 0x09,
  CMD_INTENSITY = 0x0A,
  CMD_SCAN_LIMIT = 0x0B,
  CMD_SHUTDOWN_MODE = 0x0C,
  // ...
  CMD_DISPLAY_TEST = 0x0F
};

int main( void )
{
  init();
  sendCommand(CMD_DISPLAY_TEST,0x00);
  sendCommand(CMD_DECODE_MODE,0xff); // turn on decoding for all segments
  sendCommand(CMD_INTENSITY,0xff);
  sendCommand(CMD_SCAN_LIMIT,0x03); // one digit
  sendCommand(CMD_DIGIT_0,0xff); // one digit
  sendCommand(CMD_DIGIT_0+1,0x0f);
  sendCommand(CMD_DIGIT_0+2,0x0f);
  sendCommand(CMD_DIGIT_0+3,0x0f);
  sendCommand(CMD_SHUTDOWN_MODE,0x01);
  while (1) {
    display();
      _delay_ms(1000);
  }
  return 0;
}

void loadWord(uint16_t word) {
  int i = 16;
  CLR_CLK();
  CLR_LOAD();
  while (i != 0) {
    i--;
    _delay_us(2);
    CLR_CLK();
    if ( (word & (0x01<<i)) != 0 ) {
      SET_DIN();
    } else {
      CLR_DIN();
    }
    _delay_us(2);
    SET_CLK();
  }
  _delay_us(2);
  SET_LOAD();
}

void sendCommand(uint8_t address, uint8_t data) {
  loadWord( (address << 8) | data );
}

void display() {
  val++;
  val = val % 16;
  sendCommand(CMD_DIGIT_0, val);
}

ISR(TIM1_OVF_vect) {
}

