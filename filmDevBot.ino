/*
  Blink
Turns an LED on for one second, then off for one second, repeatedly.
Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products
modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman
This example code is in the public domain.
https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/
// the setup function runs once when you press reset or power the board

// UI Pins
#define BUZZER 2
#define GREEN_LED 3
#define RED_LED 4
//Encoder Pins
#define ENC_SW 5
#define ENC_DT 6
#define ENC_CLK 7
//Motor Driver Pins
#define MOT_IN1 8
#define MOT_IN2 9
#define MOT_IN3 10
#define MOT_IN4 11

bool agitateDirectionFlag = true; //Flag used to switch motor direction on every agitation.

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

void agitate(int duration){
  switch (agitateDirectionFlag)
  {
  case true:
    analogWrite(MOT_IN1, 255);
    analogWrite(MOT_IN2, 0);
    delay(duration);
    agitateDirectionFlag = false;
    break;
  case false:
  default:
    analogWrite(MOT_IN1, 0);
    analogWrite(MOT_IN2, 255);
    delay(duration);
    agitateDirectionFlag = true;
    break;
  }
}

void vibrate(){
  for (int i = 0; i < 4; i++)
  {
  analogWrite(MOT_IN3, 255);
  analogWrite(MOT_IN4, 0);
  delay(2000);
  analogWrite(MOT_IN3, 0);
  analogWrite(MOT_IN4, 0);
  }
}