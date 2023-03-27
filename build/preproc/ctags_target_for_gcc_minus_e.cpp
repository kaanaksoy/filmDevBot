# 1 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
# 2 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino" 2
# 3 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino" 2

/*

  Film Development Bot



  Automates your film development using AP Tanks. 



last modified 25 March 2023

  by Kaan Aksoy



*/
# 14 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
/* ---- PIN DEFINITIONS ---- */

LCD_I2C lcd(0x27); //Default LCD Address. Change for your module.


// UI Pins




//Encoder Pins 
/* 

  (Using Encoder Breakout Board with pullip resistors. 

  If you dont have pullup resistors, you can enable the built in ones.

*/
Encoder encoder(3 /* Encoder In A*/, 4 /* Encoder In B*/);

//Motor Driver Pins (Using Mini L298 Motor Driver Board)





//Flag used to switch motor direction on every agitation.
bool agitateDirectionFlag = true;

//Menu option selection marker.
int menu = 1;
int buttonPressed = false;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize motor output pins.
  pinMode(8 /* Agitate Motor 1*/, 0x1);
  pinMode(9 /* Agitate Motor 2*/, 0x1);
  pinMode(10 /* Vibrate Motor 1*/, 0x1);
  pinMode(11 /* Vibrate Motor 2*/, 0x1);

  // Initialize UI Pins.
  pinMode(5, 0x1);
  pinMode(6, 0x1);
  pinMode(7, 0x1);

  // Initialize Encoder Button.
  pinMode(2 /* Encoder Button*/, 0x2);
  attachInterrupt(((2) == 2 ? 0 : ((2) == 3 ? 1 : -1)), buttonStateChange, 2);

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
  if (!digitalRead(2 /* Encoder Button*/)){
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
    tone(5, 4000, 125);
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
    lcd.print("  Film Dev Bot  ");
    lcd.setCursor(0, 1);
    lcd.print(">C-41     B&W   ");
    break;
  case 2:
    lcd.clear();
    lcd.print("  Film Dev Bot  ");
    lcd.setCursor(0, 1);
    lcd.print(" C-41    >B&W   ");
    break;
  case 3:
    lcd.clear();
    lcd.print("  Film Dev Bot  ");
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
    !digitalRead(2 /* Encoder Button*/)? flag = false : false;
  }
  flag = true;
  buzz(((pushPullValue)>0?(pushPullValue):-(pushPullValue)));

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
    digitalWrite(7, 0x1); // turn the LED on (HIGH is the voltage level)
    delay(500); // wait for a second
    digitalWrite(7, 0x0); // turn the LED off by making the voltage LOW
    delay(500);
    !digitalRead(2 /* Encoder Button*/)? flag = false : false;
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
    !digitalRead(2 /* Encoder Button*/)? flag = false : false;
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
    !digitalRead(2 /* Encoder Button*/)? flag = false : false;
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

  digitalWrite(7, 0x1);
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
  digitalWrite(7, 0x0);

  return;
}

//fix function handles the Fixing Step of the process. vales in seconds. 
void fix(uint8_t fixingDuration){
  uint8_t totalCycles = fixingDuration * 2;
  digitalWrite(7, 0x1);
  vibrate();
  for (uint8_t cycleCount = 0; cycleCount < totalCycles; cycleCount++)
  {
    agitate(15);
    delay(15 * 1000);
  }
  buzz(6);
  digitalWrite(7, 0x0);
  return;

}

void buttonStateChange(){
  buttonPressed = true;
}
