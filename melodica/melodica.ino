#include <usbh_midi.h>
#include <usbhub.h>
#include <SPI.h>

#define OUTPUT_power 0


USB Usb;
USBH_MIDI Midi(&Usb);

int pin = 8;
int melo = 200;
double tones[128] = {8.2, 8.7, 9.2, 9.7, 10.3, 10.9, 11.6, 12.2, 13, 13.8, 14.6, 15.4, 16.4, 17.3, 18.4, 19.4, 20.6, 21.8, 23.1, 24.5, 26, 27.5, 29.1, 30.9, 32.7, 34.6, 36.7, 38.9, 41.2, 43.7, 46.2, 49, 51.9, 55, 58.3, 61.7, 65.4, 69.3, 73.4, 77.8, 82.4, 87.3, 92.5, 98, 103.8, 110, 116.5, 123.5, 130.8, 138.6, 146.8, 155.6, 164.8, 174.6, 185, 196, 207.7, 220, 233.1, 246.9, 261.6, 277.2, 293.7, 311.1, 329.6, 349.2, 370, 392, 415.3, 440, 466.2, 493.9, 523.3, 554.4, 587.3, 622.3, 659.3, 698.5, 740, 784, 830.6, 880, 932.3, 987.8, 1046.5, 1108.7, 1174.7, 1244.5, 1318.5, 1396.9, 1480, 1568, 1661.2, 1760, 1864.7, 1975.5, 2093, 2217.5, 2349.3, 2489, 2637, 2793.8, 2960, 3136, 3322.4, 3520, 3729.3, 3951.1, 4186, 4434.9, 4698.6, 4978, 5274, 5587.7, 5919.9, 6271.9, 6644.9, 7040, 7458.6, 7902.1, 8372, 8869.8, 9397.3, 9956.1, 10548.1, 11175.3, 11839.8, 12543.9};


void setup() {
  Serial.begin(9600);
  pinMode(pin, OUTPUT);

  if (Usb.Init() == -1) {
    while(1);
  }
  delay(200);


}

void loop() {
  Usb.Task();

  if (Usb.getUsbTaskState() == USB_STATE_RUNNING) {
    getTone();
  }
}

void getTone() {
  char buf[20];
  uint8_t bufMidi[64];
  uint16_t rcvd;
  if (Midi.RecvData(&rcvd, bufMidi) == 0 ) {
    
    if (bufMidi[0] == 8) {
            
      //noTone(pin);
    }else if (bufMidi[0] == 9) {
      int note = bufMidi[2];
      tone(pin, tones[note]);
      Serial.println(tones[note]);
    }
  }
}




void set_ss_pin(int val) {
 //ポートB (デジタルピン8から13) 
  //ポートBの３番目は10
  //B00000100 は　論理積で4
  //なのでPORTB |= (4);
  
  //PORTDはデジタルピンの０〜７
  //Pin2はPORTDの３つ目
  // PORTD | (4);
  if(val ==HIGH){
    //PORTB |= (4);
    PORTD = PORTD | (4);
  }else{
    PORTD &= ~(4);
  } 
}

void set_rst_pin(int val) {
    //PORTDはデジタルピンの０〜７
    //Pin2はPORTDの4つ目
    //B00001000 は　論理積で8
    // PORTD | (8);
    if(val ==HIGH) PORTD |= (8);
    else PORTD &= ~(8);
}

void if_write(char addr,unsigned char* data,char num){
  char i;
  char snd;
    set_ss_pin(LOW);
    SPI.transfer(addr);
    for(i=0;i<num;i++){
      SPI.transfer(data[i]);    
    }
    set_ss_pin(HIGH);  
}

void if_s_write(char addr,unsigned char data){
  if_write(addr,&data,1);
}

unsigned char if_s_read(char addr){
  
    unsigned char rcv;
    
    set_ss_pin(LOW);    
    SPI.transfer(0x80|addr);
    rcv = SPI.transfer(0x00);
    set_ss_pin(HIGH);  
    return rcv;  
}

void init_825(void) {
   set_rst_pin(LOW);
   delay(1);
   set_rst_pin(HIGH);
   if_s_write( 0x1D, OUTPUT_power );
   if_s_write( 0x02, 0x0E );
   delay(1);
   if_s_write( 0x00, 0x01 );//CLKEN
   if_s_write( 0x01, 0x00 ); //AKRST
   if_s_write( 0x1A, 0xA3 );
   delay(1);
   if_s_write( 0x1A, 0x00 );
   delay(30);
   if_s_write( 0x02, 0x04 );//AP1,AP3
   delay(1);
   if_s_write( 0x02, 0x00 );
   //add
   if_s_write( 0x19, 0xF0 );//MASTER VOL
   if_s_write( 0x1B, 0x3F );//interpolation
   if_s_write( 0x14, 0x00 );//interpolation
   if_s_write( 0x03, 0x01 );//Analog Gain
   
   if_s_write( 0x08, 0xF6 );
   delay(21);
   if_s_write( 0x08, 0x00 );
   if_s_write( 0x09, 0xF8 );
   if_s_write( 0x0A, 0x00 );
   
   if_s_write( 0x17, 0x40 );//MS_S
   if_s_write( 0x18, 0x00 );
}

void set_tone(void){
  unsigned char tone_data[35] ={
    0x81,//header
    //T_ADR 0
    0x01,0x85,
    0x00,0x7F,0xF4,0xBB,0x00,0x10,0x40,
    0x00,0xAF,0xA0,0x0E,0x03,0x10,0x40,
    0x00,0x2F,0xF3,0x9B,0x00,0x20,0x41,
    0x00,0xAF,0xA0,0x0E,0x01,0x10,0x40,
    0x80,0x03,0x81,0x80,
  };
  
   if_s_write( 0x08, 0xF6 );
   delay(1);
   if_s_write( 0x08, 0x00 );
  
   if_write( 0x07, &tone_data[0], 35 );//write to FIFO
}

void set_ch(void){
   if_s_write( 0x0F, 0x30 );// keyon = 0
   if_s_write( 0x10, 0x71 );// chvol
   if_s_write( 0x11, 0x00 );// XVB
   if_s_write( 0x12, 0x08 );// FRAC
   if_s_write( 0x13, 0x00 );// FRAC  
}

void keyon(unsigned char fnumh, unsigned char fnuml){
   if_s_write( 0x0B, 0x00 );//voice num
   if_s_write( 0x0C, 0x54 );//vovol
   if_s_write( 0x0D, fnumh );//fnum
   if_s_write( 0x0E, fnuml );//fnum
   if_s_write( 0x0F, 0x40 );//keyon = 1  
}

void keyoff(void){
   if_s_write( 0x0F, 0x00 );//keyon = 0
}

void tone_play(){
  
    keyon(0x14,0x65);
  delay(500);
  keyoff();
  delay(200);
  keyon(0x1c,0x11);
  delay(500);
  keyoff();
  delay(200);
  keyon(0x1c,0x42);
  delay(500);
  keyoff();
  delay(200);
  keyon(0x1c,0x5d);
  delay(500);
  keyoff();
  delay(200);
  keyon(0x24,0x17);
  delay(500);
  keyoff();
  delay(200);
  
  }
