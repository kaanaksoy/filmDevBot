# 1 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
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
# 19 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
// the setup function runs once when you press reset or power the board

// UI Pins



//Encoder Pins



//Motor Driver Pins





bool agitateDirectionFlag = true; //Flag used to switch motor direction on every agitation.

void setup() {
  // initialize motor output pins.
  pinMode(8 /* Agitate Motor 1*/, 0x1);
  pinMode(9 /* Agitate Motor 2*/, 0x1);
  pinMode(10 /* Vibrate Motor 1*/, 0x1);
  pinMode(11 /* Vibrate Motor 2*/, 0x1);
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
  delay(2000);
  analogWrite(10 /* Vibrate Motor 1*/, 0);
  analogWrite(11 /* Vibrate Motor 2*/, 0);
  }
}
