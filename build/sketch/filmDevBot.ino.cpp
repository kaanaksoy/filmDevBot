#include <Arduino.h>
#line 1 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
#include <Encoder.h>

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

//Menu option selection marker.
int menu = 1;

// the setup function runs once when you press reset or power the board
#line 42 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
void setup();
#line 59 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
void loop();
#line 81 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
void agitate(int durationSeconds);
#line 104 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
void vibrate();
#line 117 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
void buzz(int repeatTimes);
#line 127 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
void updateMenu( );
#line 164 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
void executeAction();
#line 182 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
void develop(uint16_t devDuration, uint8_t firstAgitationDuration, uint8_t agitationDuration, uint16_t agitateEveryDuration);
#line 207 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
void fix(uint8_t fixingDuration);
#line 42 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
void setup() {
  // initialize motor output pins.
  pinMode(MOT_IN1, OUTPUT);
  pinMode(MOT_IN2, OUTPUT);
  pinMode(MOT_IN3, OUTPUT);
  pinMode(MOT_IN4, OUTPUT);

  // Initialize UI Pins
  pinMode(BUZZER, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  pinMode(ENC_SW, INPUT);


}
// the loop function runs over and over again forever
void loop() {
  /* if (!digitalRead(downButton)){
    menu++;
    updateMenu();
    delay(100);
    while (!digitalRead(downButton));
  }
  if (!digitalRead(upButton)){
    menu--;
    updateMenu();
    delay(100);
    while(!digitalRead(upButton));
  } */
  if (!digitalRead(ENC_SW)){
    executeAction();
    updateMenu();
    delay(100);
    while (!digitalRead(ENC_SW));
  }
}

// Agitate function controls the agitation motor.
void agitate(int durationSeconds){
  switch (agitateDirectionFlag)
  {
  case true:
    analogWrite(MOT_IN1, 255);
    analogWrite(MOT_IN2, 0);
    delay(durationSeconds * 1000);
    analogWrite(MOT_IN1, 0);
    agitateDirectionFlag = false;
    break;
  case false:
  default:
    analogWrite(MOT_IN1, 0);
    analogWrite(MOT_IN2, 255);
    delay(durationSeconds * 1000);
    analogWrite(MOT_IN2, 0);
    agitateDirectionFlag = true;
    break;
  }
  return;
}

//Simple vibrate function, used to release air bubbles from the emulsion surface.
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
  return;
}

void buzz(int repeatTimes){
  for (int i = 0; i < repeatTimes; i++)
  {
    tone(BUZZER, 4000, 125);
    delay(250);
  }
  
}

//Displays the Menu Options on the LCD based on selection via Encoder
void updateMenu( ){
  switch (menu)
  {
  case 0:
    menu = 1;
    break;
  case 1:
    lcd.clear();
    lcd.print(">Option 1");
    lcd.setCursor(0, 1);
    lcd.print(" Option 2");
    break;
  case 2:
    lcd.clear();
    lcd.print(" Option 1");
    lcd.setCursor(0, 1);
    lcd.print(">Option 2");
    break;
  case 3:
    lcd.clear();
    lcd.print(">Option 3");
    lcd.setCursor(0, 1);
    lcd.print(" Option 4");
    break;
  case 4:
    lcd.clear();
    lcd.print(" Option 3");
    lcd.setCursor(0, 1);
    lcd.print(">Option 4");
    break;
  case 5:
    menu = 4;
    break;
  }
}

// Executes action selected via LCD & Encoder
void executeAction() {
  switch (menu) {
    case 1: //C-41 Standard
      develop();
      break;
    case 2: //C-41 Push 1 Stop
      develop();
      break;
    case 3: // C-41 Push 2 Stops
      develop();
      break;
    case 4: // C-41 Push 3 Stops
      develop();
      break;
  }
}

//Develop function that handles the development process. all values as seconds
void develop(uint16_t devDuration, 
              uint8_t firstAgitationDuration, 
              uint8_t agitationDuration, 
              uint16_t agitateEveryDuration){
  uint8_t totalCycles = (devDuration - firstAgitationDuration) / (agitationDuration + agitateEveryDuration);
  uint8_t padding = (devDuration - firstAgitationDuration) % (agitationDuration + agitateEveryDuration);

  agitate(firstAgitationDuration);
  vibrate();
  for (uint8_t cycleCount = 0; cycleCount < totalCycles; cycleCount++)
  {
    delay(agitateEveryDuration * 1000);
    agitate(agitationDuration);
    if (cycleCount + 2 >= totalCycles)
    {
      buzz(3);
    }
    delay(padding * 1000);
    buzz(5);
    
  }
  return;
}

//fix function handles the Fixing Step of the process. vales in seconds. 
void fix(uint8_t fixingDuration){
  uint8_t totalCycles = fixingDuration * 2;
  vibrate();
  for (uint8_t cycleCount = 0; cycleCount < totalCycles; cycleCount++)
  {
    agitate(15);
    delay(15 * 1000);
  }
  buzz(6);
  return;
  
}
