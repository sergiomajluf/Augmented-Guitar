/*

 Logarithmic mapping and Smoothing an Analog input 
 
 http://www.arduino.cc/en/Tutorial/Smoothing
 http://playground.arduino.cc/Main/Fscale
 
 This example code is in the public domain.
 
 */
#include <math.h>

// Number of samples to keep track of. 
const int numReadings = 2;

const int expressionPin01 = A0;
const int expressionPin02 = A1;
const int devState = 1;  // 1 local test , 2 production;

const int controlCmd = 0xB0;     //channel
const int modulationCmd01 = 0x01;  //controller
const int modulationCmd0102 = 0x02;  //controller

const int analogOutPinRed = 11; // Analog output pin that the LED is attached to
const int analogOutPinGreen = 10; // Analog output pin that the LED is attached to

int expressionVal01 = 0;
int expressionVal02 = 0;
int oldVal01, oldVal02 = 0;
int newVal01, newVal02 = 0;



int readings[numReadings];      // the readings from the analog input
int index = 0;                  // the index of the current reading
int total = 0;                  // the running total
float average = 0;                // the average
//int inputPin = A0;
float scaledResult; 

void setup() {
  pinMode(analogOutPinRed, OUTPUT);
  pinMode(analogOutPinGreen, OUTPUT);
  Serial.begin(115200);                   
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;          
  }
}

void loop() {
  // Input mapping
  scaledResult = fscale( 0, 970, 0, 1023, analogRead(expressionPin01), -6.5);


  //Average smoothing
  total= total - readings[index];         
  readings[index] = scaledResult; 
  total= total + readings[index];       
  index = index + 1;                    

  // if we're at the end of the array...
  if (index >= numReadings) {             
    index = 0;
  }  
  average = total / numReadings;         
  //Serial.println(scaledResult, DEC); 


  expressionVal01 = average;
  expressionVal02 = analogRead(expressionPin02);

  newVal01 = map(expressionVal01, 0, 760, 0x00, 0x70); // up to 128
  newVal02 = map(expressionVal02, 0, 1023, 0x00, 0x5F); // up to 95

  analogWrite(analogOutPinRed, newVal01);
  analogWrite(analogOutPinGreen, newVal02);  

  writeModulation(newVal01);
  /*if (newVal01 != oldVal01) {
   writeModulation(newVal01);
   oldVal01 = newVal01;
   }
   
   if (newVal02 != oldVal02) {
   writeModulation02(newVal02);
   oldVal02 = newVal02;
   }
   
   */



  //Serial.println(average);   
  delay(1);        // delay in between reads for stability            
}


// Hello Hairless
void writeModulation(int val)
{
  switch (devState) {
  case 1:

    // devState = 1 for local testing
    /*
    Serial.print("Flex is: ");
     Serial.print(expressionVal01);
     Serial.print("\t FSR is: ");
     */
    Serial.println(average);
    break;
  case 2:
    // devState = 2 for production environment
    Serial.write(controlCmd);
    Serial.write(modulationCmd01);
    Serial.write(val);
    break;
  }



}

void writeModulation02(int val)
{
  //    Serial.write(controlCmd);
  //    Serial.write(modulationCmd0102);
  //    Serial.write(val);
}




// FScale Function
float fscale( float originalMin, float originalMax, float newBegin, float
newEnd, float inputValue, float curve){

  float OriginalRange = 0;
  float NewRange = 0;
  float zeroRefCurVal = 0;
  float normalizedCurVal = 0;
  float rangedValue = 0;
  boolean invFlag = 0;


  // condition curve parameter
  // limit range

  if (curve > 10) curve = 10;
  if (curve < -10) curve = -10;

  curve = (curve * -.1) ; // - invert and scale - this seems more intuitive - postive numbers give more weight to high end on output 
  curve = pow(10, curve); // convert linear scale into lograthimic exponent for other pow function

  /*
   Serial.println(curve * 100, DEC);   // multply by 100 to preserve resolution  
   Serial.println(); 
   */

  // Check for out of range inputValues
  if (inputValue < originalMin) {
    inputValue = originalMin;
  }
  if (inputValue > originalMax) {
    inputValue = originalMax;
  }

  // Zero Refference the values
  OriginalRange = originalMax - originalMin;

  if (newEnd > newBegin){ 
    NewRange = newEnd - newBegin;
  }
  else
  {
    NewRange = newBegin - newEnd; 
    invFlag = 1;
  }

  zeroRefCurVal = inputValue - originalMin;
  normalizedCurVal  =  zeroRefCurVal / OriginalRange;   // normalize to 0 - 1 float

  /*
  Serial.print(OriginalRange, DEC);  
   Serial.print("   ");  
   Serial.print(NewRange, DEC);  
   Serial.print("   ");  
   Serial.println(zeroRefCurVal, DEC);  
   Serial.println();  
   */

  // Check for originalMin > originalMax  - the math for all other cases i.e. negative numbers seems to work out fine 
  if (originalMin > originalMax ) {
    return 0;
  }

  if (invFlag == 0){
    rangedValue =  (pow(normalizedCurVal, curve) * NewRange) + newBegin;

  }
  else     // invert the ranges
  {   
    rangedValue =  newBegin - (pow(normalizedCurVal, curve) * NewRange); 
  }

  return rangedValue;
}









