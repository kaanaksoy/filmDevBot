/*
  Film Development Bot

  Automates your film development using AP Tanks. 

last modified 25 March 2023
  by Kaan Aksoy

*/

/* ---- PIN DEFINITIONS ---- */

// UI Pins
#define BUZZER 2
#define GREEN_LED 3
#define RED_LED 4

//Encoder Pins 
/* 
  (Using Encoder Breakout Board with pullip resistors. 
  If you dont have pullup resistors, you can enable the built in ones.
*/
#define ENC_SW 5 // Encoder Button
#define ENC_DT 6 // Encoder Out A
#define ENC_CLK 7 // Encoder Out B

//Motor Driver Pins (Using Mini L298 Motor Driver Board)
#define MOT_IN1 8 // Agitate Motor 1
#define MOT_IN2 9 // Agitate Motor 2
#define MOT_IN3 10 // Vibrate Motor 1
#define MOT_IN4 11 // Vibrate Motor 2

//Flag used to switch motor direction on every agitation.
bool agitateDirectionFlag = true; 

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize motor output pins.
  pinMode(MOT_IN1, OUTPUT);
  pinMode(MOT_IN2, OUTPUT);
  pinMode(MOT_IN3, OUTPUT);
  pinMode(MOT_IN4, OUTPUT);
}
// the loop function runs over and over again forever
void loop() {
}

// Agitate function controls the agitation motor.
void agitate(int duration){
  switch (agitateDirectionFlag)
  {
  case true:
    analogWrite(MOT_IN1, 255);
    analogWrite(MOT_IN2, 0);
    delay(duration);
    analogWrite(MOT_IN1, 0);
    agitateDirectionFlag = false;
    break;
  case false:
  default:
    analogWrite(MOT_IN1, 0);
    analogWrite(MOT_IN2, 255);
    delay(duration);
    analogWrite(MOT_IN2, 0);
    agitateDirectionFlag = true;
    break;
  }
}

//Simple vibrate function, used to release air bubbles fromt the emulsion surface.
void vibrate(){
  for (int i = 0; i < 4; i++)
  {
  analogWrite(MOT_IN3, 255);
  analogWrite(MOT_IN4, 0);
  delay(1000);
  analogWrite(MOT_IN3, 0);
  analogWrite(MOT_IN4, 0);
  delay(500);
  }
}