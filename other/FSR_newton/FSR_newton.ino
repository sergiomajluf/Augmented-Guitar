/* FSR testing sketch. 
 
 Connect one end of FSR to power, the other end to Analog 0.
 Then connect one end of a 10K resistor from Analog 0 to ground 
 
 For more information see www.ladyada.net/learn/sensors/fsr.html */

int fsrPin = 0; // the FSR and 10K pulldown are connected to a0
int fsrReading; // the analog reading from the FSR resistor divider
int fsrVoltage; // the analog reading converted to voltage
unsigned long fsrResistance; // The voltage converted to resistance, can be very big so make "long"
unsigned long fsrConductance; 
long fsrForce; // Finally, the resistance converted to force

void setup(void) {
  Serial.begin(9600); // We'll send debugging information via the Serial monitor
  delay(2000);
  Serial.println("Analog, mV, Ohms, Conductance, Newtons");
  delay(2000);
}

void loop(void) {
  fsrReading = analogRead(fsrPin); 
  //Serial.print("Analog reading = ");
  //Serial.println(fsrReading);
  Serial.print(millis());
  Serial.print(fsrReading);
  Serial.print(",");

  // analog voltage reading ranges from about 0 to 1023 which maps to 0V to 5V (= 5000mV)
  fsrVoltage = map(fsrReading, 0, 1023, 0, 5000);
  //Serial.print("Voltage reading in mV = ");
  //Serial.println(fsrVoltage); 

  Serial.print(fsrVoltage); 
  Serial.print(",");

  if (fsrVoltage == 0) {
    //Serial.println("No pressure"); 
    Serial.print(0);
    Serial.print(",0,0,");
  } 
  else {
    // The voltage = Vcc * R / (R + FSR) where R = 10K and Vcc = 5V
    // so FSR = ((Vcc - V) * R) / V yay math!
    fsrResistance = 5000 - fsrVoltage; // fsrVoltage is in millivolts so 5V = 5000mV
    fsrResistance *= 10000; // 10K resistor
    fsrResistance /= fsrVoltage;
    //Serial.print("FSR resistance in ohms = ");
    //Serial.println(fsrResistance);
    Serial.print(fsrResistance);
    Serial.print(",");

    fsrConductance = 1000000; // we measure in micromhos so 
    fsrConductance /= fsrResistance;
    //Serial.print("Conductance in microMhos: ");
    //Serial.println(fsrConductance);
    Serial.print(fsrConductance);
    Serial.print(",");

    // Use the two FSR guide graphs to approximate the force
    if (fsrConductance <= 1000) {
      fsrForce = fsrConductance / 80;
      //Serial.print("Force in Newtons: ");
      //Serial.println(fsrForce); 
      Serial.print(fsrForce);
      Serial.print(",");
    } 
    else {
      fsrForce = fsrConductance - 1000;
      fsrForce /= 30;
      //Serial.print("Force in Newtons: ");
      //Serial.println(fsrForce); 
      Serial.print(fsrForce);
      Serial.print(",");
    }
  }
  //Serial.println("--------------------");
  Serial.println();
  delay(1000);
}

