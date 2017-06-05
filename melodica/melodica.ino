#define C4  262
#define D4  294
#define E4  330

#include <MozziGuts.h>
#include <Oscil.h> // oscillator template
#include <tables/sin2048_int8.h> // sine table for oscillator

Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> cSin(SIN2048_DATA);
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> dSin(SIN2048_DATA);
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> eSin(SIN2048_DATA);

// use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 64 // powers of 2 please

const int C_Btn = 0;
int C_Btn_State = 0;
const int D_Btn = 2;
int D_Btn_State = 2;
const int E_Btn = 4;
int E_Btn_State = 4;

int gain_cSin;
int gain_dSin;
int gain_eSin;

int pitch_cSin;
int pitch_dSin;
int pitch_eSin;


void setup(){

  Serial.begin(9600);          // 9600 bpsで接続 
  startMozzi(CONTROL_RATE); // set a control rate of 64 (powers of 2 please)

  //Inputモードでプルアップ抵抗を有効に
  pinMode(C_Btn, INPUT_PULLUP); 
  pinMode(D_Btn, INPUT_PULLUP); 
  pinMode(E_Btn, INPUT_PULLUP); 

  //pinMode(Speaker,OUTPUT);
  //aSin.setFreq(440); // set the frequency
}
void updateControl(){
  // put changing controls in here
  int X,Y,Z;

  X = C4;
  Y = D4;
  Z = E4;
  
    cSin.setFreq(X); 
    dSin.setFreq(Y); 
    eSin.setFreq(Z); 
    gain_cSin = 255;
    gain_dSin = 255;
    gain_eSin = 255;
    gain_cSin*=0.9;
    gain_dSin*=0.9;
    gain_eSin*=0.9; 
}
int updateAudio(){

  if (C_Btn_State == LOW && D_Btn_State == LOW && E_Btn_State == LOW) {
    return (((cSin.next()*gain_cSin)>>8) + ((dSin.next()*gain_dSin)>>8) + ((eSin.next()*gain_eSin)>>8))/3;
  } 
  else if (C_Btn_State == LOW && D_Btn_State == LOW) {
    return (((cSin.next()*gain_cSin)>>8) + ((dSin.next()*gain_dSin)>>8))/2;
  }  
  else if (C_Btn_State == LOW && E_Btn_State == LOW) {
    return (((cSin.next()*gain_cSin)>>8) + ((eSin.next()*gain_eSin)>>8))/2;
  }  
  else if (D_Btn_State == LOW && E_Btn_State == LOW) {
    return (((dSin.next()*gain_dSin)>>8) + ((eSin.next()*gain_eSin)>>8))/2;
  }
  else if (C_Btn_State == LOW) {
    return ((cSin.next()*gain_cSin)>>8); 
  } 
  else if (D_Btn_State == LOW) {
    return ((dSin.next()*gain_dSin)>>8); 
  }  
  else if (E_Btn_State == LOW) {
    return ((eSin.next()*gain_eSin)>>8); 
  }

  return 0;
  //return (((cSin.next()*gain_cSin)>>8) + ((dSin.next()*gain_dSin)>>8) + ((eSin.next()*gain_eSin)>>8))/3;  
}
void loop(){
  audioHook(); // required here

  C_Btn_State = digitalRead(C_Btn);
  D_Btn_State = digitalRead(D_Btn);
  E_Btn_State = digitalRead(E_Btn);

  //C
  if (C_Btn_State == LOW) {
    //Serial.println("C_ON"); 
  } 
  else {
    //Serial.println("C_OFF"); 
  }
  
  //D
  if (D_Btn_State == LOW) {
    //Serial.println("D_ON"); 
  } 
  else {
    //Serial.println("D_OFF"); 
  }

  //E
  if (E_Btn_State == LOW) {
    //Serial.println("E_ON"); 
  } 
  else {
    //Serial.println("E_OFF"); 
  }
}
