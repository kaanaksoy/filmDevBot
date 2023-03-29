#include <Arduino.h>
#line 1 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
#include <Encoder.h>
#include <LCD_I2C.h>

/*
  Film Development Bot

  Automates your film development using AP Tanks. 

last modified by Kaan Aksoy

*/

/* ---- PIN DEFINITIONS ---- */

LCD_I2C lcd(0x27); //Default LCD Address. Change for your module.
#define TITLE "  Film Dev Bot  "

// UI Pins
#define BUZZER 5
#define GREEN_LED 6
#define RED_LED 7

//Encoder Pins 
/* 
  (Using Encoder Breakout Board with pullip resistors. 
  If you dont have pullup resistors, you can enable the built in ones.
*/
#define ENC_SW 2 // Encoder Button
#define ENC_DT 3 // Encoder In A
#define ENC_CLK 4 // Encoder In B
Encoder encoder(ENC_DT, ENC_CLK);

//Motor Driver Pins (Using Mini L298 Motor Driver Board)
#define MOT_IN1 8 // Agitate Motor 1
#define MOT_IN2 9 // Agitate Motor 2
#define MOT_IN3 10 // Vibrate Motor 1
#define MOT_IN4 11 // Vibrate Motor 2

//Flag used to switch motor direction on every agitation.
bool agitateDirectionFlag = true;

//Menu option selection marker.
int menu = 1;
int buttonPressed = false;

// the setup function runs once when you press reset or power the board
#line 47 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
void setup();
#line 70 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
void loop();
#line 94 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
void agitate(int durationSeconds);
#line 117 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
void vibrate();
#line 130 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
void buzz(int repeatTimes);
#line 140 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
void updateMenu( );
#line 174 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
void executeAction();
#line 194 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
void colorDevelopment();
#line 302 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
void develop(uint16_t devDuration, uint8_t firstAgitationDuration, uint8_t agitationDuration, uint16_t agitateEveryDuration);
#line 330 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
void fix(uint8_t fixingDuration);
#line 345 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
void buttonStateChange();
#line 47 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
void setup() {
  // initialize motor output pins.
  pinMode(MOT_IN1, OUTPUT);
  pinMode(MOT_IN2, OUTPUT);
  pinMode(MOT_IN3, OUTPUT);
  pinMode(MOT_IN4, OUTPUT);

  // Initialize UI Pins.
  pinMode(BUZZER, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // Initialize Encoder Button.
  pinMode(ENC_SW, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), buttonStateChange, FALLING);

  lcd.begin();
  lcd.backlight();
  encoder.write(menu);
  updateMenu();

}
// the loop function runs over and over again forever
void loop() {
  
  if (encoder.read() > menu + 2){
    menu++;
    encoder.write(menu);
    updateMenu();
    delay(100);
  }
  if (encoder.read() < menu - 2){
    menu--;
    encoder.write(menu);
    updateMenu();
    delay(100);
  }
  if (!digitalRead(ENC_SW)){
    executeAction();
    updateMenu();
    encoder.write(menu);
    delay(100);
  }
  delay(100);
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
    lcd.print(TITLE);
    lcd.setCursor(0, 1);
    lcd.print(">C-41     B&W   ");
    break;
  case 2:
    lcd.clear();
    lcd.print(TITLE);
    lcd.setCursor(0, 1);
    lcd.print(" C-41    >B&W   ");
    break;
  case 3:
    lcd.clear();
    lcd.print(TITLE);
    lcd.setCursor(0, 1);
    lcd.print(">Custom   ");
    break;
  case 4:
    menu = 3;
    break;
  default:
    menu = 1;
    break;
  }
}

// Executes action selected via LCD & Encoder
void executeAction() {
  switch (menu) {
    case 1: //C-41
      colorDevelopment();
      //develop();
      //fix();
      buzz(1);
      break;
    case 2: //B&W
      //develop();
      //fix();
      buzz(2);
      break;
    case 3: // Custom
      //develop();
      //fix();
      break;
  }
}

void colorDevelopment(){

  lcd.clear();
  lcd.print("C-41 Development");
  delay(100);
  bool flag = true;
  int8_t pushPullValue = 0;
  uint16_t duration = 0;
  while (flag)
  {

    pushPullValue = encoder.read() / 4;
    if (pushPullValue > 3)
    {
      pushPullValue = 3;
      encoder.write(12); //3 * 4
    } else if (pushPullValue < -1)
    {
      pushPullValue = -1;
      encoder.write(-4);
    }
    
    
    lcd.setCursor(0, 1);
    lcd.print(String("Push/Pull? ") + String(pushPullValue) + String(" stp"));
    delay(100);
    !digitalRead(ENC_SW)? flag = false : false;
  }
  flag = true;
  buzz(abs(pushPullValue));

  lcd.clear();
  lcd.setCursor(0,0);

  switch (pushPullValue)
  {
  case 0:
    duration = 210;
    lcd.print("C-41, +0 Push");
    break;
  case 1:
    duration = 273;
    lcd.print("C-41, +1 Push");
    break;
  case 2:
    duration = 368;
    lcd.print("C-41, +2 Push");
    break;
  case 3:
    duration = 525;
    lcd.print("C-41, +3 Push");
    break;
  case -1:
    duration = 165;
    lcd.print("C-41, -1 Pull");
  default:
    break;
  }

  lcd.setCursor(0,1);
  lcd.print("Hld btn to start");
  delay(100);
  while (flag)
  {
    digitalWrite(RED_LED, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(500);                      // wait for a second
    digitalWrite(RED_LED, LOW);   // turn the LED off by making the voltage LOW
    delay(500); 
    !digitalRead(ENC_SW)? flag = false : false;
  }
  flag = true;

  lcd.clear();
  lcd.print("Developing ...");
  develop(duration, 10, 6, 30);

  lcd.clear();
  lcd.print("Development Finished.");
  lcd.setCursor(0,1);
  lcd.print("Hld btn to fix.");
  while (flag)
  {
    buzz(1);
    delay(100);
    !digitalRead(ENC_SW)? flag = false : false;
  }
  flag = true;
  lcd.print("Fixing ...");
  fix(480);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Process Finished. ");
  lcd.setCursor(0, 1);
  lcd.print("Hld btn to cnfrm");


  while (flag)
  {
    buzz(1);
    delay(100);
    !digitalRead(ENC_SW)? flag = false : false;
  } flag = true;

  encoder.write(menu);
}

//Develop function that handles the development process. all values as seconds
void develop(uint16_t devDuration, 
              uint8_t firstAgitationDuration, 
              uint8_t agitationDuration, 
              uint16_t agitateEveryDuration){
  uint8_t totalCycles = (devDuration - firstAgitationDuration) / (agitationDuration + agitateEveryDuration);
  uint8_t padding = (devDuration - firstAgitationDuration) % (agitationDuration + agitateEveryDuration);    
    
  digitalWrite(RED_LED, HIGH);
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
  digitalWrite(RED_LED, LOW);

  return;
}

//fix function handles the Fixing Step of the process. vales in seconds. 
void fix(uint8_t fixingDuration){
  uint8_t totalCycles = fixingDuration * 2;
  digitalWrite(RED_LED, HIGH);
  vibrate();
  for (uint8_t cycleCount = 0; cycleCount < totalCycles; cycleCount++)
  {
    agitate(15);
    delay(15 * 1000);
  }
  buzz(6);
  digitalWrite(RED_LED, LOW);
  return;
  
}

void buttonStateChange(){
  buttonPressed = true;
}
