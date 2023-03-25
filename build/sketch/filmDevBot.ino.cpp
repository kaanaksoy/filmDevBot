#include <Arduino.h>
#line 1 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
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
#define ENC_SW 5 // Encoder Button
#define ENC_DT 6 // Encoder Out A
#define ENC_CLK 7 // Encoder Out B
//Motor Driver Pins
#define MOT_IN1 8 // Agitate Motor 1
#define MOT_IN2 9 // Agitate Motor 2
#define MOT_IN3 10 // Vibrate Motor 1
#define MOT_IN4 11 // Vibrate Motor 2

bool agitateDirectionFlag = true; //Flag used to switch motor direction on every agitation.

#line 37 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
void setup();
#line 45 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
void loop();
#line 52 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
void agitate(int duration);
#line 74 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
void vibrate();
#line 37 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
void setup() {
  // initialize motor output pins.
  pinMode(MOT_IN1, OUTPUT);
  pinMode(MOT_IN2, OUTPUT);
  pinMode(MOT_IN3, OUTPUT);
  pinMode(MOT_IN4, OUTPUT);
}
// the loop function runs over and over again forever
void loop() {
  agitate(1000);
  delay(1000);
  vibrate();
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
  delay(2000);
  analogWrite(MOT_IN3, 0);
  analogWrite(MOT_IN4, 0);
  }
}
