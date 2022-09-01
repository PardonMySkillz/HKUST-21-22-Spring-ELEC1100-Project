/*
  ELEC1100 Your Lab#06 & Project Template

  To program the car tracking the white line on a dark mat

  Your name: WONG HOI CHUN
  
*/

// assign meaningful names to those pins that will be used

const int pinL_Sensor = A5;      //pin A5: left sensor 
const int pinB_Sensor = A4;      //pin A4: bumper sensor
const int pinR_Sensor = A3;      //pin A3: right sensor 

const int pinL_PWM = 9;          //pin D9: left motor speed
const int pinL_DIR = 10;         //pin D10: left motor direction

const int pinR_PWM = 11;         //pin D11: right motor speed
const int pinR_DIR = 12;         //pin D12: right motor direction

//define variables to be used in script

int bumperSensor = 1;  // not sensing white
int leftSensor = 1;    // not sensing white
int rightSensor = 1;   // not sensing white

int countBumper = 0;   // bumper not triggered yet
int countLeftSplit = 0; // number of left turns at split (max = 3)

int already_paused = 0; 

// the setup function runs once when you press reset or power the board

void setup ()
{
  // define pins as input and output
  pinMode(pinB_Sensor, INPUT);
  pinMode(pinL_Sensor, INPUT);
  pinMode(pinR_Sensor, INPUT);
  
  pinMode(pinL_DIR, OUTPUT);
  pinMode(pinR_DIR, OUTPUT);
  
  pinMode(pinL_PWM, OUTPUT);
  pinMode(pinR_PWM, OUTPUT);
  
  // initialize output pins
  digitalWrite(pinL_DIR, HIGH);   //forward direction    
  digitalWrite(pinR_DIR, HIGH);   //forward direction
  analogWrite(pinL_PWM, 0);    //stop at the start position 
  analogWrite(pinR_PWM, 0);    //stop at the start position 
}

// the loop function runs over and over again forever

void loop() {

  // Arduino is reading the sensor measurements
  bumperSensor = digitalRead(pinB_Sensor);
  leftSensor = digitalRead(pinL_Sensor);
  rightSensor = digitalRead(pinR_Sensor);
  
  // Car is reset at the start position
  if ( bumperSensor && countBumper == 0 ) {
    analogWrite(pinL_PWM, 0);
    analogWrite(pinR_PWM, 0);
  }

  // Bumper Sensor is triggered at the start position
  else if ( !bumperSensor && countBumper == 0) {
    analogWrite(pinL_PWM, 150);
    analogWrite(pinR_PWM, 150);
    countBumper = countBumper + 1;
    delay(350);     //to let the car leave the start position to avoid sensors miscount 00 at the start position 
  }
  
  // Car is tracking on the white line
  else if ( bumperSensor && countBumper == 1 && countLeftSplit != 3 ) 
  { 
    if ( !leftSensor && !rightSensor ) {     // Hit the Y junction
      
      while ( !rightSensor ) {        // Left turn until right sensor sense black
        leftSensor = digitalRead(pinL_Sensor);
        rightSensor = digitalRead(pinR_Sensor);
        analogWrite(pinL_PWM, 150);
        analogWrite(pinR_PWM, 230);
        digitalWrite(pinL_DIR, LOW);
        digitalWrite(pinR_DIR, HIGH); 
        }
      countLeftSplit += 1;
    }
    
    else if ( !leftSensor && rightSensor ) {
        analogWrite(pinL_PWM, 150);
        analogWrite(pinR_PWM, 190);
        digitalWrite(pinL_DIR, LOW);
        digitalWrite(pinR_DIR, HIGH);  
      }
    
    else if ( leftSensor && !rightSensor ) {
        analogWrite(pinL_PWM, 150);
        analogWrite(pinR_PWM, 150);
        digitalWrite(pinL_DIR, HIGH);
        digitalWrite(pinR_DIR, LOW);  
      }
    
    else if ( leftSensor && rightSensor ) {
        analogWrite(pinL_PWM, 150);
        analogWrite(pinR_PWM, 150);
        digitalWrite(pinL_DIR, HIGH);
        digitalWrite(pinR_DIR, HIGH);
      }
  }
  
  //finish navigating the 3rd split, stop for 1 second
  else if ( countBumper == 1 && countLeftSplit == 3 && already_paused == 0) {
    analogWrite(pinL_PWM, 0);
    analogWrite(pinR_PWM, 0);
    digitalWrite(pinL_DIR, HIGH);
    digitalWrite(pinR_DIR, HIGH);
    delay(2000);                // stop for one second
    
    already_paused = 1;    // transistion to right angle turning mode
  }
  
  else if ( bumperSensor && countBumper == 1 && countLeftSplit == 3 && already_paused == 1) {  
    if ( leftSensor && !rightSensor ) {
      analogWrite(pinL_PWM, 190);
      analogWrite(pinR_PWM, 140);
      digitalWrite(pinL_DIR, HIGH);
      digitalWrite(pinR_DIR, LOW);
      }
    else if ( !leftSensor && rightSensor ) {

      analogWrite(pinL_PWM, 140);
      analogWrite(pinR_PWM, 190);
      digitalWrite(pinL_DIR, LOW);
      digitalWrite(pinR_DIR, HIGH);
      }
    else if ( leftSensor && rightSensor ) {
      
      analogWrite(pinL_PWM, 140);
      analogWrite(pinR_PWM, 140);
      digitalWrite(pinL_DIR, HIGH);
      digitalWrite(pinR_DIR, HIGH);
    }
  }
  
  // Car is hitting the white wall
  else if ( !bumperSensor && countBumper == 1) {
    countBumper = countBumper + 1;
    analogWrite(pinL_PWM, 255);
    analogWrite(pinR_PWM, 255);
    digitalWrite(pinL_DIR, LOW);
    digitalWrite(pinR_DIR, LOW);  
    delay(750);     // to let the car keep moving backward for 45 cm
  }
  else if ( bumperSensor && countBumper == 2) {
    analogWrite(pinL_PWM, 0);
    analogWrite(pinR_PWM, 0);
  }
  
  }
