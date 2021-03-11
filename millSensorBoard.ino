


#include <Wire.h>
int half_revolutions[3];
int rpm[8];
byte AD[5];
byte freq[3];
unsigned long lastmillis[4];

int byteSending = 1;
int toTransfer = 0;
int Shift = toTransfer;
int mask = 0xFF;
unsigned char toSend = 0;


 
void setup() {
  pinMode(18, INPUT_PULLUP);
  pinMode(19, INPUT_PULLUP);
  pinMode(20, INPUT_PULLUP);    //address bits
  pinMode(21, INPUT_PULLUP);

  
  pinMode(22, OUTPUT);   //LED
//pinMode(SDA, OUTPUT);
//pinMode(SCL, OUTPUT);

  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);  
  pinMode(12, INPUT_PULLUP);   //frequency bits 
  pinMode(13, INPUT_PULLUP);   
  pinMode(14, INPUT_PULLUP);   
  pinMode(15, INPUT_PULLUP);   

  pinMode(0, INPUT_PULLUP);   
  pinMode(1, INPUT_PULLUP);   
  pinMode(3, INPUT_PULLUP);   //analog/digital
  pinMode(4, INPUT_PULLUP);   
  pinMode(29, INPUT_PULLUP);   

  if (digitalRead(0)) { pinMode(24, INPUT_PULLUP); AD[0] = 1;  }
  if (digitalRead(1)) { pinMode(25, INPUT_PULLUP); AD[1] = 1;  }
  if (digitalRead(3)) { pinMode(26, INPUT_PULLUP); AD[2] = 1;  }    
  if (digitalRead(4)) { pinMode(27, INPUT_PULLUP); AD[3] = 1;  }
  if (digitalRead(29)) { pinMode(28, INPUT_PULLUP); AD[4] = 1; } 

byte address = digitalRead(21) + digitalRead(20)*2 + digitalRead(19)*4 + digitalRead(18)*8;   //determine I2C address from jumpers
address = ~address - 240;


freq[0] = digitalRead(9) + digitalRead(8)*2;
freq[0] = (~freq[0] - 251);

freq[1] = digitalRead(13) + digitalRead(12)*2;    //determine tach sample rate from jumpers
freq[1] = (~freq[1] - 251);

freq[2] = digitalRead(15) + digitalRead(14)*2;
freq[2] = (~freq[2] - 251);

  digitalWrite(22, HIGH);  //LED on


  Wire.begin(address);  // join i2c bus with address 
  Wire.onRequest(requestEvent); // register event
  attachInterrupt(0, rpm_fan0, FALLING);
  attachInterrupt(1, rpm_fan1, FALLING);
  attachInterrupt(2, rpm_fan2, FALLING);

//     Serial.begin(9600); 
     digitalWrite(10, HIGH);   //pullup resistor for sensor
     digitalWrite(11, HIGH);   //pullup resistor for sensor
     digitalWrite(2, HIGH);   //pullup resistor for sensor


  

}








void loop() {
 //   digitalWrite(SDA, 1);
 //   digitalWrite(SCL, 1);

  if (millis() - lastmillis[0] == freq[0]*1000) { //Uptade every one second, this will be equal to reading frecuency (Hz).

    detachInterrupt(0);//Disable interrupt when calculating

    rpm[0] = half_revolutions[0]; // Convert frecuency to RPM, note: this works for one interruption per full rotation. For two interrups per full rotation use half_revolutions * 30.

    half_revolutions[0] = 0; // Restart the RPM counter

    
    lastmillis[0] = millis(); // Uptade lasmillis
    attachInterrupt(0, rpm_fan0, FALLING); //enable interrupt


  }


    if (millis() - lastmillis[1] == freq[1]*1000) { //Uptade every one second, this will be equal to reading frecuency (Hz).

     detachInterrupt(1);//Disable interrupt when calculating
    rpm[1] = half_revolutions[1];

    half_revolutions[1] = 0; // Restart the RPM counter
    
    
    lastmillis[1] = millis(); // Uptade lasmillis
    attachInterrupt(1, rpm_fan1, FALLING); //enable interrupt

  }

    if (millis() - lastmillis[2] == freq[2]*1000) { //Uptade every one second, this will be equal to reading frecuency (Hz).

     detachInterrupt(2);//Disable interrupt when calculating
    rpm[2] = half_revolutions[2];

    half_revolutions[2] = 0; // Restart the RPM counter
    
    
    lastmillis[2] = millis(); // Uptade lasmillis
    attachInterrupt(2, rpm_fan2, FALLING); //enable interrupt

  }  


 if (millis() - lastmillis[3] == 1000) {

    if (AD[0]) rpm[3] = !digitalRead(24);
    else rpm[3] = analogRead(0);

    if (AD[1]) rpm[4] = !digitalRead(25);
    else rpm[4] = analogRead(1);
  
    if (AD[2]) rpm[5] = !digitalRead(26);
    else rpm[5] = analogRead(2);

    if (AD[3]) rpm[6] = !digitalRead(27);
    else rpm[6] = analogRead(3);

    if (AD[4]) rpm[7] = !digitalRead(28);
    else rpm[7] = analogRead(4);
    
    
    lastmillis[3] = millis(); // Uptade lasmillis
 }

}






















void requestEvent() {

  String str; 
  for (int y=0; y < 8; y++) {

    str = str + String(rpm[y]) + ","; //place RPM reading into string
   
    
  } 
  char b[40];
  str.toCharArray(b,40); 
  Wire.write(b);  //write chars to i2c
  
}






 void rpm_fan0(){
  half_revolutions[0]++;

 }

 void rpm_fan1(){
  half_revolutions[1]++;

 }

  void rpm_fan2(){
  half_revolutions[2]++;

 }
