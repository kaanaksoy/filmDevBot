# 1 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
/*

  Film Development Bot



  Automates your film development using AP Tanks. 



last modified 25 March 2023

  by Kaan Aksoy



*/
# 11 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
/* ---- PIN DEFINITIONS ---- */

// UI Pins




//Encoder Pins 
/* 

  (Using Encoder Breakout Board with pullip resistors. 

  If you dont have pullup resistors, you can enable the built in ones.

*/

//Motor Driver Pins (Using Mini L298 Motor Driver Board)





//Flag used to switch motor direction on every agitation.
bool agitateDirectionFlag = true;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize motor output pins.
  pinMode(8 /* Agitate Motor 1*/, 0x1);
  pinMode(9 /* Agitate Motor 2*/, 0x1);
  pinMode(10 /* Vibrate Motor 1*/, 0x1);
  pinMode(11 /* Vibrate Motor 2*/, 0x1);
}
// the loop function runs over and over again forever
void loop() {
  vibrate();
}

// Agitate function controls the agitation motor.
void agitate(int duration){
  switch (agitateDirectionFlag)
  {
  case true:
    analogWrite(8 /* Agitate Motor 1*/, 255);
    analogWrite(9 /* Agitate Motor 2*/, 0);
    delay(duration);
    analogWrite(8 /* Agitate Motor 1*/, 0);
    agitateDirectionFlag = false;
    break;
  case false:
  default:
    analogWrite(8 /* Agitate Motor 1*/, 0);
    analogWrite(9 /* Agitate Motor 2*/, 255);
    delay(duration);
    analogWrite(9 /* Agitate Motor 2*/, 0);
    agitateDirectionFlag = true;
    break;
  }
}

//Simple vibrate function, used to release air bubbles fromt the emulsion surface.
void vibrate(){
  for (int i = 0; i < 4; i++)
  {
  analogWrite(10 /* Vibrate Motor 1*/, 255);
  analogWrite(11 /* Vibrate Motor 2*/, 0);
  delay(1000);
  analogWrite(10 /* Vibrate Motor 1*/, 0);
  analogWrite(11 /* Vibrate Motor 2*/, 0);
  delay(500);
  }
}
