#include "HX711.h" 
#include <SPI.h>
#include <SD.h>
//for the sd card we only care about the setup; after just make a function that reads the code for when
// our load cell is ready to take measurements
//#define calibration_factor <- still need to find the value for this :(
const int loadcell_dout_pin = 4;
const int loadcell_sck_pin = 5;
const int button_pin = 9; // change formatting to buttonPin so on and so on
const int solenoid_pin = 3;
const int ematch_pin = 2;
const int sd_cs = 10;

const int threshold; // needs to be found through testing
const int ematch_pulse = 250; //ms
const int solenoid_safety_time = 10000; //ms ~10 ms
bool solenoid_open = false;
bool ematchIgnited = false;

unsigned long solenoid_start_time = 0;
unsigned long ematch_start_time = 0;

HX711 loadcell;
File dataFile;

void setup() {
Serial.begin(9600);

// Beggining to instantiate our loadcell
loadcell.begin(loadcell_dout_pin, loadcell_sck_pin);
//loadcell.set_scale(calibration_factor);
loadcell.tare(); // zeroing our scale

// IO directions
  pinMode(solenoid_pin, OUTPUT);
  pinMode(ematch_pin, OUTPUT);
  pinMode(button_pin, INPUT); 

// ensure outputs start LOW
digitalWrite(solenoid_pin, LOW);
digitalWrite(ematch_pin, LOW);

// takes in input from whether button is pressed or not

dataFile = SD.open("datalog.csv", FILE_WRITE);
    if (dataFile) {
      dataFile.println("Time(ms),Force(RAW)");
      dataFile.close();
    }
}





void loop() {
  unsigned long current_millis = millis();
  int buttonval = digitalRead(button_pin);
  //continouly recording and storing load cell data
  long reading = loadcell.read();
  logData(reading,current_millis); 

  //turn on ematch when button pressed & ematch not already ignited
  if(buttonval == HIGH && !ematchIgnited){
    igniteEmatch();
  }

  if (ematchIgnited && (current_millis - ematch_start_time >= ematch_pulse)) {
      digitalWrite(ematch_pin, LOW);
      ematchIgnited = false;
      Serial.println("E-match pulse completed");
    }

  else {
    Serial.println("Ematch already ignited >:( ");
  }

  if(reading >= threshold && !solenoid_open ) //open solenoid when a combustion is guranteed/actually good
  {
    openSolenoid();
  }
  else if (reading < threshold && solenoid_open) //close solenoid when not prodcucing enough force
  {
    digitalWrite(solenoid_pin, LOW);
    solenoid_open = false;
  }
  else{
    Serial.println(":(");
  }
  if((solenoid_open && current_millis - solenoid_start_time) >= solenoid_safety_time) //auto close the solenoid if open for too long
  {
    digitalWrite(solenoid_pin, LOW);
    solenoid_open = false;
  }

}


void openSolenoid(){
  digitalWrite(solenoid_pin, HIGH);
  solenoid_open = true;
  solenoid_start_time = millis();

}

void igniteEmatch(){
  digitalWrite(ematch_pin, HIGH);
  ematchIgnited = true;
  ematch_start_time = millis();
}

void logData( long forceReading,unsigned long millis){
      dataFile = SD.open("datalog.csv", FILE_WRITE);
      if (dataFile) {
        dataFile.println(millis);
        dataFile.print(",");
        dataFile.print(forceReading);
        dataFile.println();
        dataFile.close();
      }

}
