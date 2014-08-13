//init midi serial
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
#define ForkPin1      A2          // analog pin for fork


//1638 module
#include <TM1638.h>
TM1638 module(8, 9, 7);

//Trellis
#include <Wire.h>
#include "Adafruit_Trellis.h"
#define MOMENTARY 1
#define LATCHING 0
// set the mode here
#define MODE LATCHING 
Adafruit_Trellis matrix0 = Adafruit_Trellis();
Adafruit_Trellis matrix1 = Adafruit_Trellis();
Adafruit_TrellisSet trellis =  Adafruit_TrellisSet(&matrix0, &matrix1);
#define NUMTRELLIS 2
#define numKeys (NUMTRELLIS * 16)
#define INTPIN A1

//char note[4][8] = {
//  {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17},
//  {0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f},
//  {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27},
//  {0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f}
//};

char note[32] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f};



void setup(){
  //trellis
  pinMode(INTPIN, INPUT);
  digitalWrite(INTPIN, HIGH);
  trellis.begin(0x70, 0x71);  // 
    // light up all the LEDs in order
  for (uint8_t i=0; i<numKeys; i++) {
    trellis.setLED(i);
    trellis.writeDisplay();    
    noteOn(0x90, 0x1E, 0x45);
    delay(20);
    noteOn(0x90, 0x1E, 0x00); 
    delay(20);
    
  }
  // then turn them off
  for (uint8_t i=0; i<numKeys; i++) {
    trellis.clrLED(i);
    trellis.writeDisplay();
     
    delay(20);
  }
  //TM1638
  module.setDisplayToHexNumber(0x1234ABCD, 0xF0);
  //capacitancechanger
  pinMode(chargePin, OUTPUT);     // set chargePin to output
  digitalWrite(chargePin, LOW);
  //serial
  Serial.begin(9600);             // initialize serial transmission for debugging
 
  test = 0;
  //MIDI over serial
   HWSERIAL.begin(31250); //midiport
  
}

void loop(){
  getTrellis();
  modulestuff();
//  int incomingByte;
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
//  delay(100);
//  HWSERIAL.println(getForks());
//  HWSERIAL.println(test++);
//  Serial.println(test++);
  //forks
  
  
} 

void modulestuff(){
  byte keys = module.getButtons();
  module.setLEDs(((keys & 0xF0) << 8) | (keys & 0xF));
  
}


int getForks(){
  return analogRead(ForkPin1);
  
}

void getTrellis() {
  delay(30); // 30ms delay is required, dont remove me!
  
  if (MODE == MOMENTARY) {
    // If a button was just pressed or released...
    if (trellis.readSwitches()) {
      // go through every button
      for (uint8_t i=0; i<numKeys; i++) {
	// if it was pressed, turn it on
	if (trellis.justPressed(i)) {
	  Serial.print("v"); Serial.println(i);
	  trellis.setLED(i);
          noteOn(0xb0, note[i], 0x45);

	} 
	// if it was released, turn it off
	if (trellis.justReleased(i)) {
	  Serial.print("^"); Serial.println(i);
          noteOn(0xb0, note[i], 0x00);
	  trellis.clrLED(i);

	}
      }
      // tell the trellis to set the LEDs we requested
      trellis.writeDisplay();
    }
  }

  if (MODE == LATCHING) {
    // If a button was just pressed or released...
    if (trellis.readSwitches()) {
      // go through every button
      for (uint8_t i=0; i<numKeys; i++) {
        // if it was pressed...
	if (trellis.justPressed(i)) {
	  Serial.print("v"); Serial.println(i);
	  // Alternate the LED
	  if (trellis.isLED(i)){
	    trellis.clrLED(i);
            noteOn(0xb0, note[i], 0x00);}
	  else{
	    trellis.setLED(i);
            noteOn(0xb0, note[i], 0x45);}
        } 
      }
      // tell the trellis to set the LEDs we requested
      trellis.writeDisplay();
    }
  }
}


//  plays a MIDI note.  Doesn't check to see that
//  cmd is greater than 127, or that data values are  less than 127:
void noteOn(int cmd, int pitch, int velocity) {
  HWSERIAL.write(cmd);
  HWSERIAL.write(pitch);
  HWSERIAL.write(velocity);
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
