#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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
     quiescent Output voltage is factor for VCC that appears at output
     when the current is zero.
     for Bidirectional sensor it is 0.5 x VCC
     for Unidirectional sensor it is 0.12 x VCC
     for model ACS758LCB-050B, the B at the end represents Bidirectional (polarity doesn't matter)
     for model ACS758LCB-100U, the U at the end represents Unidirectional (polarity must match)
      Do not change.
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
const float QOV = quiescent_Output_voltage[model] * VCC; // set quiescent Output voltage for selected model
float voltage;                                           // internal variable for voltage
float cutOff = FACTOR / cutOffLimit;                     // convert current cut off to mV

void setup()
{
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void loop() {
  float measurements = 0;
  for (int i = 0; i < 20; i++) {
    measurements += (5.0 / 1023.0) * analogRead(VIN);
  }


  //float correctionVoltage = -2.501;
  float voltage_raw = measurements / 20; // Read the voltage from sensor
  voltage =  voltage_raw - QOV + 0.018 ;// 0.007 is a value to make voltage zero when there is no current
  float current = voltage / FACTOR;
  // Serial.print("V: ");
  // Serial.print(voltage, 3); // print voltage with 3 decimal places
  Serial.print(current, 5);
  drawOnLED(current);
  // Serial.println(" I"); // print the current with 2 decimal places
  // Serial.println("A");
  delay(2000);
}

void drawOnLED(float current) {
  if (current < 0) {
    current = current * (-1);
  }
  display.clearDisplay();
  display.setCursor(0, 15);
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.print(current);
  display.print(F(" A"));
  display.display();
}
