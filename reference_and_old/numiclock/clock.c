#define F_CPU 16000000
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define SET_OUTPUT(P,B) { DDR##P |= _BV(B); }
#define SET_INPUT(P,B) { DDR##P &= ~_BV(B); }

#define SET_PIN(P,B) { PORT##P |= _BV(B); }
#define CLR_PIN(P,B) { PORT##P &= ~_BV(B); }

#define SET_DATA()   SET_PIN(C,0)
#define CLR_DATA()   CLR_PIN(C,0)

#define SET_CLK()   SET_PIN(C,4)
#define CLR_CLK()   CLR_PIN(C,4)

#define SET_LATCH()   SET_PIN(C,2)
#define CLR_LATCH()   CLR_PIN(C,2)

#define SET_CLK_IO()   SET_PIN(D,6)
#define CLR_CLK_IO()   CLR_PIN(D,6)

#define CLK_IO_OUT()   SET_OUTPUT(D,6)
#define CLK_IO_IN()    SET_INPUT(D,6)

#define SET_CLK_CLK()   SET_PIN(D,7)
#define CLR_CLK_CLK()   CLR_PIN(D,7)

#define SET_CLK_CE()   SET_PIN(D,5)
#define CLR_CLK_CE()   CLR_PIN(D,5)

#define GET_PIN(P,B) ((PIN##P & _BV(B)) != 0)

#define GET_CLK_IO()   GET_PIN(D,6)


void init();
void display();

void clock_send(uint8_t byte) {
  CLK_IO_OUT();
  for (int i = 0; i < 8; i++) {
    CLR_CLK_CLK();
    if ((byte & 0x01) != 0) {
      SET_CLK_IO();
    } else {
      CLR_CLK_IO();
    }
    SET_CLK_CLK();
    byte >>= 1;
  }
}

uint8_t clock_recv() {
  CLK_IO_IN();
  uint8_t byte = 0;
  for (int i = 0; i < 8; i++) {
    CLR_CLK_CLK();
    SET_CLK_CLK();
    if (GET_CLK_IO()) {
      byte |= _BV(i);
    }
  }
  return byte;
}

void clock_write_cmd(uint8_t address, uint8_t data) {
  uint8_t cmd_byte = 0b10000000 | ((address & 0b00011111) << 1);
  CLR_CLK_CLK();
  SET_CLK_CE();
  clock_send(cmd_byte);
  clock_send(data);
  CLR_CLK_CE();
}

uint8_t clock_read_cmd(uint8_t address) {
  uint8_t cmd_byte = 0b10000001 | ((address & 0b00011111) << 1);
  CLR_CLK_CLK();
  SET_CLK_CE();
  clock_send(cmd_byte);
  uint8_t data = clock_recv();
  CLR_CLK_CE();
  return data;
}

void init()
{
  // init pins
  SET_OUTPUT(C,0);
  SET_OUTPUT(C,1);
  SET_OUTPUT(C,2);
  SET_OUTPUT(C,3);
  SET_OUTPUT(C,4);
  SET_OUTPUT(C,5);
  SET_OUTPUT(D,5);
  SET_OUTPUT(D,6);
  SET_OUTPUT(D,7);
  CLR_PIN(C,0); // G low
  SET_PIN(C,3); // SCLR high
  SET_PIN(C,5); // Debug on
  clock_write_cmd(7,0);
  clock_write_cmd(0,0); // reset seconds
  clock_write_cmd(1,0); // reset minutes
  // init clock.  Gives us about 15 updates per second,
  // plenty for our purposes.
  TCCR1A = 0x00;  // WGM1 = 0, CM = 0
  TCCR1B = 0x02;  // CS = clk/8
  TIMSK1 = 0x01;  // overflow interrupt enabled
  sei();
}

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

int main( void )
{
  init();
  while (1) {
    sleep_mode();
  }
  return 0;
}

void writeByte(uint8_t bits) {
  for (int i = 0; i < 8; i++) {
    CLR_CLK();
    if ( bits & (1<<i) ) {
      CLR_DATA();
    } else {
      SET_DATA();
    }
    SET_CLK();
  }
}


void display() {
  int seconds = clock_read_cmd(0);
  int minutes = clock_read_cmd(1);
  CLR_LATCH();
  int bits;
  bits = digits[(minutes>>4)&0x0f];
  writeByte(bits);
  bits = digits[minutes&0x0f];
  writeByte(bits);
  bits = digits[(seconds>>4)&0x0f];
  writeByte(bits);
  bits = digits[seconds&0x0f];
  writeByte(bits);
  SET_LATCH();
}

ISR(TIMER1_OVF_vect) {
  display();
}

