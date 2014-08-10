//init serial
#define HWSERIAL Serial1


//measure capacitance on density selector controller
#define analogPin      A0          // analog pin for measuring capacitor voltage
#define chargePin      11         // pin to charge the capacitor - connected to one end of the charging resistor
#define dischargePin   12         // pin to discharge the capacitor
#define resistorValue  44000.0F   // change this to whatever resistor value you are using
                                  // F formatter tells compliler it's a floating point value

unsigned long startTime;
unsigned long elapsedTime;
float microFarads;                // floating point variable to preserve precision, make calculations
//float nanoFarads;
int test;

///forkstuff
#define ForkPin1      A1          // analog pin for fork



void setup(){
  pinMode(chargePin, OUTPUT);     // set chargePin to output
  digitalWrite(chargePin, LOW);  
  Serial.begin(9600);             // initialize serial transmission for debugging
  HWSERIAL.begin(9600);
  test = 0;  
}

void loop(){
  int incomingByte;
//  	if (Serial.available() > 0) {
//		incomingByte = Serial.read();
//		Serial.print("USB received: ");
//		Serial.println(incomingByte, DEC);
//                HWSERIAL.print("USB received:");
//                HWSERIAL.println(incomingByte, DEC);
//	}
//	if (HWSERIAL.available() > 0) {
//		incomingByte = HWSERIAL.read();
//		Serial.print("UART received: ");
//		Serial.println(incomingByte, DEC);
//                HWSERIAL.print("UART received:");
//                HWSERIAL.println(incomingByte, DEC);
//	}
//  
//  Serial.print("density = ");
//  Serial.println(getDensitySelector());
  delay(100);
//  HWSERIAL.println(getForks());
  HWSERIAL.println(test++);
  Serial.println(test++);
  //forks
  
  
} 

int getForks(){
  return analogRead(ForkPin1);
}


int getDensitySelector(){  //red wire from densityselector to GND (yes GND)  red/black wire to chargepin+ 44K ohm
  digitalWrite(chargePin, HIGH);  // set chargePin HIGH and capacitor charging
  startTime = micros();
  while(analogRead(analogPin) < 648){       // 647 is 63.2% of 1023, which corresponds to full-scale voltage 
  }
  elapsedTime= micros() - startTime;
  microFarads = ((float)elapsedTime / resistorValue) * 1000;   

  /* dicharge the capacitor  */
  digitalWrite(chargePin, LOW);             // set charge pin to  LOW 
  pinMode(dischargePin, OUTPUT);            // set discharge pin to output 
  digitalWrite(dischargePin, LOW);          // set discharge pin LOW 
  while(analogRead(analogPin) > 0){         // wait until capacitor is completely discharged
  }

  pinMode(dischargePin, INPUT);            // set discharge pin back to input
//  return 111;


  if (microFarads < 5) {
    return 1;
  } else if (microFarads >= 5 && microFarads <= 6.5) {
    return 2;
  } else if (microFarads > 6.5 && microFarads <= 8) {
    return 3;
  } else if (microFarads > 8 && microFarads <= 10) {
    return 4;
  } else if (microFarads > 10 && microFarads <= 13) {
    return 5;
  } else if (microFarads > 13 && microFarads <= 16) {
    return 6;
  } else if (microFarads > 16 && microFarads <= 21) {
    return 7;
  } else if (microFarads > 21 && microFarads <= 26) {
    return 8;
  } else if (microFarads > 26 && microFarads <= 32) {
    return 9;
  } else if (microFarads > 32 && microFarads <= 42) {
    return 10;
  } else if (microFarads > 42 && microFarads <= 52) {
    return 11;
  } else if (microFarads > 52 && microFarads <= 60) {
    return 12;
  } else if (microFarads > 60 && microFarads <= 100){
    return 13;
  } else if (microFarads > 100 && microFarads <= 125){
    return 14;
  } else if (microFarads > 125 && microFarads <= 168){
    return 15;
  } else if (microFarads > 168 && microFarads <= 200){
    return 16;
  } else if (microFarads > 200 && microFarads <= 250){
    return 17;
  } else if (microFarads > 250 && microFarads <= 300){
    return 18;
  } else if (microFarads > 300 && microFarads <= 350){
    return 19;
  }

}
