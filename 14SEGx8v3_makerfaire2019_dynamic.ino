#include <SPI.h>
// SPI: 10(SS)、11(MOSI)、(12)(MISO)、13(SCK) 

#define N_DISP 2 // number of 8-digit display unit

const unsigned char digits[] = {
  0x80, //digit0
  0x40, //digit1
  0x20, //digit2
  0x10, //digit3
  0x08, //digit4
  0x04, //digit5
  0x02, //digit6
  0x01, //digit7
};

const unsigned char seg14[] = {
  0x00, 0x00, //Space
  0x40, 0x06, //!
  0x00, 0x00, //"
  0xff, 0xFF, //#
  0x12, 0xED, //$
  0x24, 0x24, //%
  0x29, 0x30, //&
  0x04, 0x00, //'
  0x00, 0x39, //(
  0x00, 0x0F, //)
  0x3f, 0x00, //*
  0x12, 0xC0, //+
  0x20, 0x00, //,
  0x00, 0xC0, //-
  0x40, 0x00, //.
  0x00, 0x00, ///
  0x00, 0x3F, //0
  0x00, 0x06, //1
  0x00, 0xDB, //2
  0x00, 0xCF, //3
  0x00, 0xE6, //4
  0x00, 0xED, //5
  0x00, 0xFD, //6
  0x00, 0x27, //7
  0x00, 0xFF, //8
  0x00, 0xEF, //9
  0x0C, 0x00, //:
  0x21, 0x00, //;
  0x0C, 0x40, //<
  0x00, 0xC8, //=
  0x21, 0x80, //>
  0x50, 0x83, //?
  0x10, 0xBF, //@
  0x24, 0x86, //A
  0x04, 0xFD, //B
  0x00, 0x3B, //C
  0x12, 0x0F, //D
  0x00, 0xF9, //E
  0x00, 0x71, //F
  0x00, 0xBD, //G
  0x00, 0xF6, //H
  0x12, 0x09, //I
  0x00, 0x1E, //J
  0x0C, 0x70, //K
  0x00, 0x38, //L
  0x05, 0x36, //M
  0x09, 0x36, //N
  0x00, 0x3F, //O
  0x00, 0xF3, //P
  0x08, 0x3F, //Q
  0x08, 0xF3, //R
  0x01, 0x9D, //S  0x09, 0x1B, //S
  0x12, 0x01, //T
  0x00, 0x3E, //U
  0x24, 0x30, //V
  0x28, 0x36, //W
  0x2D, 0x00, //X
  0x15, 0x00, //Y
  0x24, 0x09, //Z  0x24, 0x2D, //Z
};

int mask[8*N_DISP];
int target_seg[8*N_DISP];

const char str[] = "MAKER-FAIRE-2019MAENOH! 14SEGBAR";

void setup() {
  //Serial.begin(9600);
  SPI.begin() ;
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.setDataMode(SPI_MODE0) ;
}
void scan14seg(int msg) { // 74HC595 x 3
  int digit;
  int g_ch;
  digitalWrite(SS, 1);
  for ( int ch = 0; ch <=7 ; ch++ ) {
    digit = digits[ch]; // for 5241BS
    for (int disp_id=0; disp_id<N_DISP; disp_id++) {
      g_ch = disp_id*8+ch;
      SPI.transfer(((seg14[(str[ch+(disp_id+msg*N_DISP)*8]-' ')*2]&(mask[g_ch]>>8))|target_seg[g_ch])^0xff); // 0xff for 5241BS
      SPI.transfer(((seg14[(str[ch+(disp_id+msg*N_DISP)*8]-' ')*2+1]&(mask[g_ch]&0xff))|target_seg[g_ch])^0xff); // 0xff for 5241BS
      //SPI.transfer((seg14[(str[ch+disp_id*8]-' ')*2])^0xff); // 0xff for 5241BS
      //SPI.transfer((seg14[(str[ch+disp_id*8]-' ')*2+1])^0xff); // 0xff for 5241BS
      SPI.transfer(digit);
    }
    digitalWrite(SS, 0);
    delay(1);
    digitalWrite(SS, 1);
  }
}

void loop() {
  for (int msg=0; msg<=1; msg++) {
    for ( int g_ch = 0; g_ch <8*N_DISP ; g_ch++ ) { mask[g_ch]=0; target_seg[g_ch]=0; }
    for ( int g_ch = 0; g_ch <8*N_DISP ; g_ch++ ) {
      for (int seg_id=0; seg_id<=15; seg_id++) {
        target_seg[g_ch] = 1<<seg_id;
        mask[g_ch] |= 1<<seg_id ;
        for (int k=0; k<5; k++) scan14seg(msg);
      }
    }
    for (int k=0; k<500; k++) scan14seg(msg);
  }
}
