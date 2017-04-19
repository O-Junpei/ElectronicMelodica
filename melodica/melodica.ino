const int C_Btn = 0;
int C_Btn_State = 0;
const int D_Btn = 2;
int D_Btn_State = 2;
const int E_Btn = 4;
int E_Btn_State = 4;

const int Speaker = 9;

void setup () {
  Serial.begin(9600);          /* 9600 bpsで接続 */

  //Inputモードでプルアップ抵抗を有効に
  pinMode(C_Btn, INPUT_PULLUP); 
  pinMode(D_Btn, INPUT_PULLUP); 
  pinMode(E_Btn, INPUT_PULLUP); 

  pinMode(Speaker,OUTPUT);
}
 
void loop () {
  Serial.println("Hello world!!"); /* Hello world!! を表示 */

  C_Btn_State = digitalRead(C_Btn);
  D_Btn_State = digitalRead(D_Btn);
  E_Btn_State = digitalRead(E_Btn);

  //C
  if (C_Btn_State == LOW) {
    Serial.println("C_ON"); 
  } 
  else {
    Serial.println("C_OFF"); 
  }
  
  //D
  if (D_Btn_State == LOW) {
    Serial.println("D_ON"); 
  } 
  else {
    Serial.println("D_OFF"); 
  }

  //E
  if (E_Btn_State == LOW) {
    Serial.println("E_ON"); 
  } 
  else {
    Serial.println("E_OFF"); 
  }
    //noTone(PINNO);

      //tone(PINNO, 440); 

  delay(1000);
}

