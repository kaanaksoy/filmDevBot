# 1 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
# 2 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino" 2

/*

  Film Development Bot



  Automates your film development using AP Tanks. 



last modified 25 March 2023

  by Kaan Aksoy



*/
# 13 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
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

//Menu option selection marker.
int menu = 1;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize motor output pins.
  pinMode(8 /* Agitate Motor 1*/, 0x1);
  pinMode(9 /* Agitate Motor 2*/, 0x1);
  pinMode(10 /* Vibrate Motor 1*/, 0x1);
  pinMode(11 /* Vibrate Motor 2*/, 0x1);

  // Initialize UI Pins
  pinMode(2, 0x1);
  pinMode(3, 0x1);
  pinMode(4, 0x1);

  pinMode(5 /* Encoder Button*/, 0x0);


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
# 72 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
  if (!digitalRead(5 /* Encoder Button*/)){
    executeAction();
    updateMenu();
    delay(100);
    while (!digitalRead(5 /* Encoder Button*/));
  }
}

// Agitate function controls the agitation motor.
void agitate(int durationSeconds){
  switch (agitateDirectionFlag)
  {
  case true:
    analogWrite(8 /* Agitate Motor 1*/, 255);
    analogWrite(9 /* Agitate Motor 2*/, 0);
    delay(durationSeconds * 1000);
    analogWrite(8 /* Agitate Motor 1*/, 0);
    agitateDirectionFlag = false;
    break;
  case false:
  default:
    analogWrite(8 /* Agitate Motor 1*/, 0);
    analogWrite(9 /* Agitate Motor 2*/, 255);
    delay(durationSeconds * 1000);
    analogWrite(9 /* Agitate Motor 2*/, 0);
    agitateDirectionFlag = true;
    break;
  }
  return;
}

//Simple vibrate function, used to release air bubbles from the emulsion surface.
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
  return;
}

void buzz(int repeatTimes){
  for (int i = 0; i < repeatTimes; i++)
  {
    tone(2, 4000, 125);
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
