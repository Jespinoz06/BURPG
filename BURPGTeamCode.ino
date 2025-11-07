#include "HX711.h" 
//#define calibration_factor <- still need to find the value for this :(
const int LOADCELL_DOUT_PIN = 4;
const int LOADCELL_SCK_PIN = 5;
const int BUTTON = 9;
const int SOLENOID = 3;
const int EMATCH = 2;
HX711 loadcell;


void setup() {
loadcell.begin(LOADCELL_DOUT_PIN, SCK);
//loadcell.set_scale(calibration_factor);
loadcell.tare(); // zeroing our scale
pinMode(SOLENOID, INPUT); //initializing our digital inputs and mosfets
pinMode(EMATCH, INPUT);
pinMode(BUTTON, OUTPUT);
digitalWrite(SOLENOID, LOW); //set the states of our mosfets to be LOW
digitalWrite(EMATCH, LOW);

}

void loop() {
  // put your main code here, to run repeatedly:

}
