#include <Arduino.h>

#define VIN A0         // define the Arduino pin A0 as voltage input (V in)
const float VCC = 5.0; // supply voltage 5V or 3.3V. If using PCB, set to 5V only.
const int model = 0;   // enter the model (see below)

float cutOffLimit = 00.1; // reading cutt off current. 1.00 is 1 Amper

float sensitivity[] = {
    40.0, // for ACS758LCB-050B
    60.0, // for ACS758LCB-050U
    20.0, // for ACS758LCB-100B
    40.0, // for ACS758LCB-100U
    13.3, // for ACS758KCB-150B
    16.7, // for ACS758KCB-150U
    10.0, // for ACS758ECB-200B
    20.0, // for ACS758ECB-200U
};

/*         
 *   quiescent Output voltage is factor for VCC that appears at output       
 *   when the current is zero. 
 *   for Bidirectional sensor it is 0.5 x VCC
 *   for Unidirectional sensor it is 0.12 x VCC
 *   for model ACS758LCB-050B, the B at the end represents Bidirectional (polarity doesn't matter)
 *   for model ACS758LCB-100U, the U at the end represents Unidirectional (polarity must match)
 *    Do not change.
 */
float quiescent_Output_voltage[] = {
    0.5,  // for ACS758LCB-050B
    0.12, // for ACS758LCB-050U
    0.5,  // for ACS758LCB-100B
    0.12, // for ACS758LCB-100U
    0.5,  // for ACS758KCB-150B
    0.12, // for ACS758KCB-150U
    0.5,  // for ACS758ECB-200B
    0.12, // for ACS758ECB-200U
};
const float FACTOR = sensitivity[model] / 1000;          // set sensitivity for selected model
// const float QOV = quiescent_Output_voltage[model] * VCC; // set quiescent Output voltage for selected model
float voltage;                                           // internal variable for voltage
float cutOff = FACTOR / cutOffLimit;                     // convert current cut off to mV

void setup()
{
  Serial.begin(9600);
  Serial.println("Hello World");
  // put your setup code here, to run once:
}

void loop()
{
  float measurements = 0;
  for(int i = 0; i<100;i++) {
    measurements += (5.0 / 1023.0) * analogRead(VIN);
  }


  float correctionVoltage = -2.493;
  float voltage_raw = measurements / 100; // Read the voltage from sensor
  voltage = voltage_raw + correctionVoltage;                  // 0.007 is a value to make voltage zero when there is no current
  float current = voltage / FACTOR;
  Serial.print("V: ");
  Serial.print(voltage, 3); // print voltage with 3 decimal places
  Serial.print("V, I: ");
  Serial.print(current, 2); // print the current with 2 decimal places
  Serial.println("A");
  delay(500);
}
