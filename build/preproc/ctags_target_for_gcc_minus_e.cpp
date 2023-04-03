# 1 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"

# 3 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino" 2
# 4 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino" 2


# 7 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino" 2
# 8 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino" 2
# 9 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino" 2

# 11 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino" 2
# 12 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino" 2

/*

  Film Development Bot

  - Automates your film development using AP Tanks. 

*/
# 19 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
/* ---- DEFINITIONS ---- */


// UI Pins



//Encoder Pins 
/* 

  (Using Encoder Breakout Board with pullip resistors. 

  If you dont have pullup resistors, you can enable the built in ones.

*/

// Motor Driver Pins 
// (Using Mini L298 Motor Driver Board)
# 45 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
/* MENU Definitions */

// Define menu options
const char gMenuColor[] 
# 48 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino" 3
                       __attribute__((__progmem__)) 
# 48 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
                               = {"      Color     "};
const char gMenuC41[] 
# 49 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino" 3
                     __attribute__((__progmem__)) 
# 49 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
                             = {"       C-41     "};
const char gMenuE6[] 
# 50 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino" 3
                    __attribute__((__progmem__)) 
# 50 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
                            = {"       E-6      "};
const char gMenuBW[] 
# 51 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino" 3
                    __attribute__((__progmem__)) 
# 51 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
                            = {"       B&W      "};
const char gMenuBWCustom[] 
# 52 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino" 3
                          __attribute__((__progmem__)) 
# 52 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
                                  = {"      Custom    "};



// Define menu functionIDs
enum MenuFID {
  MenuColor,
  MenuC41,
  MenuE6,
  MenuBW,
  MenuBWCustom
};

// Define Encoder Navigation types
enum EncoderInputType {
  EncoderNone,
  EncoderLeft,
  EncoderRight,
  EncoderEnter,
  EncoderExit
};

//create menu instance
CMBMenu<5> gMenu;

// create LCD Instance
LCD_I2C gLCD(0x27, 16, 2); //Default LCD Address. Change for your module.


Encoder gEncoder(3 /* Encoder In A*/, 2 /* Encoder In B*/);
int gEncoderPrevPos = 0;
bool buttonActive = false;
bool longPressActive = false;

long buttonTimer = 0;
long longPressTime = 500;

// ------------ SETUP -------------
void setup(){


  Serial.begin(9600);


  // Initialize motor control pins.
  pinMode(8 /* Agitate Motor 1*/, 0x1);
  pinMode(9 /* Agitate Motor 2*/, 0x1);
  pinMode(10 /* Vibrate Motor 1*/, 0x1);
  pinMode(11 /* Vibrate Motor 2*/, 0x1);

  // Initialize UI pins.
  pinMode(5, 0x1);
  pinMode(7, 0x1);

  // Initialize encoder button.
  pinMode(4 /* Encoder Button*/, 0x0);

  gLCD.begin();
  gLCD.backlight();

  // Add Nodes to the menu
  gMenu.addNode(0, gMenuColor, MenuColor);
  gMenu.addNode(1, gMenuC41, MenuC41);
  gMenu.addNode(1, gMenuE6, MenuE6);

  gMenu.addNode(0, gMenuBW, MenuBW);
  gMenu.addNode(1, gMenuBWCustom, MenuBWCustom);

  // Build & Print Menu
  const char* info;
  gMenu.buildMenu(info);
  gMenu.printMenu();
  // Print Current Menu Entry
  printMenuEntry(info);
}

// ------------ LOOP -------------
void loop(){
  int fid = 0; //Function ID

  // Info text from menu
  const char* info;

  bool layerChanged = false; // Should navigate layers?

  EncoderInputType key = getKey(); // Determine pressed key.

  // Call menu methods based on key selection
  switch (key)
  {
  case EncoderExit:
    gMenu.exit();
    break;
  case EncoderEnter:
    gMenu.enter(layerChanged);
    break;
  case EncoderLeft:
    gMenu.left();
    break;
  case EncoderRight:
    gMenu.right();
  default:
    break;
  }

  /* 

    Print/update the menu when the key is pressed.

    get the current function ID.

  */
# 161 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
  if (EncoderNone != key)
  {
    fid = gMenu.getInfo(info);
    printMenuEntry(info);
  }

  // Do action regarding fid 
  if ((0 != fid) && (EncoderEnter == key) && (!layerChanged)){
    switch (fid)
    {
    case MenuC41:
      ColorC41();
      break;
    case MenuE6:
      ColorE6();
      break;
    case MenuBWCustom:
      BWCustom();
      break;
    default:
      break;
    }
  }
}

// --- printMenuEntry | Menu Functions ---
void printMenuEntry(const char* funcInfo){
  String infoStr;
  MBHelper::stringFromPgm(funcInfo, infoStr);

  gLCD.clear();
  gLCD.setCursor(0,0);
  gLCD.print(infoStr);

  gLCD.createChar(1,leftArrowChar);
  gLCD.setCursor(6, 1);
  gLCD.write(1);

  gLCD.createChar(2, enterChar);
  gLCD.setCursor(8,1);
  gLCD.write(2);

  gLCD.createChar(3, rightArrowChar);
  gLCD.setCursor(10,1);
  gLCD.write(3);

  gLCD.createChar(4, exitChar);
  gLCD.setCursor(14,1);
  gLCD.write(4);


}

// --- getKey | Menu Navigation Function ---
EncoderInputType getKey(){
  EncoderInputType key = EncoderNone;



  if (gEncoderPrevPos > gEncoder.read())
  {
    key = EncoderLeft;
  } else if (gEncoderPrevPos < gEncoder.read())
  {
    key = EncoderRight;
  }

  if (digitalRead(4 /* Encoder Button*/) == 0x0)
  {
    if (buttonActive == false)
    {
      buttonActive = true;
      buttonTimer = millis();
    }
    if ((millis() - buttonTimer > longPressTime) && (longPressActive == false))
    {
      longPressActive = true;
      key = EncoderExit;
      delay(250);
    }
  } else
  {
    if (buttonActive == true)
    {
      if (longPressActive == true)
      {
        longPressActive = false;
      } else
      {
        key = EncoderEnter;
        delay(100);
      }
      buttonActive = false;
    }

  }

  gEncoderPrevPos = gEncoder.read();
  return key;
}

// --- ColorC41 | Film Development Functions ---
void ColorC41(){

  int8_t pushPullValue = 0;
  bool confirmSelection = false;
  uint16_t duration;

  gLCD.clear();
  gLCD.setCursor(0,0);
  gLCD.println("C-41 Development");


  while (!confirmSelection)
  {
    EncoderInputType key = getKey(); // Determine pressed key.
    switch (key)
    {
    case EncoderLeft:
      pushPullValue--;
      break;
    case EncoderRight:
      pushPullValue++;
      break;
    case EncoderExit:
      return;
    case EncoderEnter:
      confirmSelection = true;
    default:
      break;
    }
    if (pushPullValue > 3)
    {
      pushPullValue = 3;
    }
    else if (pushPullValue < -1)
    {
      pushPullValue = -1;
    }
    gLCD.setCursor(0,1);
    gLCD.println(String("Push/Pull: ") + String(pushPullValue) + String(" stp"));
    delay(150);
  }
  confirmSelection = false;
  buzz(((pushPullValue)>0?(pushPullValue):-(pushPullValue)));
  gLCD.clear();
  gLCD.setCursor(0,0);

  switch (pushPullValue)
  {
  case 0:
    duration = 210;
    gLCD.print("C-41, +0 Push");
    break;
  case 1:
    duration = 273;
    gLCD.print("C-41, +1 Push");
    break;
  case 2:
    duration = 368;
    gLCD.print("C-41, +2 Push");
    break;
  case 3:
    duration = 525;
    gLCD.print("C-41, +3 Push");
    break;
  case -1:
    duration = 165;
    gLCD.print("C-41, -1 Pull");
  default:
    break;
  }

  gLCD.setCursor(0,1);
  gLCD.createChar(2, enterChar);
  gLCD.setCursor(3,1);
  gLCD.write(2);
  gLCD.setCursor(5, 1);
  gLCD.print("to start...");

  while (!confirmSelection)
  {
    EncoderInputType key = getKey();
    switch (key)
    {
    case EncoderEnter:
      confirmSelection = true;
      break;
    case EncoderExit:
      return;
    default:
      break;
    }
    buzz(1);
  }
  confirmSelection = false;

  gLCD.clear();
  gLCD.print("Developing...");
  develop(duration, 10, 6, 30);

  gLCD.clear();
  gLCD.print("Development Finished");
  gLCD.setCursor(0,1);
  gLCD.createChar(2, enterChar);
  gLCD.setCursor(3,1);
  gLCD.write(2);
  gLCD.setCursor(5, 1);
  gLCD.print("to start fixing...");
  gLCD.clear();

  while (!confirmSelection)
  {
    EncoderInputType key = getKey();
    switch (key)
    {
    case EncoderEnter:
      confirmSelection = true;
      break;
    case EncoderExit:
      return;
    default:
      break;
    }
    buzz(1);
  }
  confirmSelection = false;

  gLCD.print("Fixing...");
  fix(480);

  gLCD.clear();
  gLCD.setCursor(0,0);
  gLCD.print("Process Finished");
    gLCD.setCursor(0,1);
  gLCD.createChar(2, enterChar);
  gLCD.setCursor(3,1);
  gLCD.write(2);
  gLCD.setCursor(5, 1);
  gLCD.print("to start...");
  gLCD.clear();

  while (!confirmSelection)
  {
    EncoderInputType key = getKey();
    switch (key)
    {
    case EncoderEnter:
      confirmSelection = true;
      break;
    case EncoderExit:
      return;
    default:
      break;
    }
    buzz(1);
  }
  confirmSelection = false;
  return;
}

void ColorE6(){
  return;
}

void BWCustom(){
  return;
}

/*

  --- buzz | UI Helper Functions ---

  Simple buzzer control, used to simplify code.

*/
# 434 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
void buzz(int repeatTimes){

    tone(5, 4000, 125);
  }

/*

  --- develop | Film Development Functions ---

  Develop function that handles the development process.

*/
# 443 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
void develop(uint16_t devDurationSeconds,
              uint8_t firstAgitationDurationSeconds,
              uint8_t agitationDurationSeconds,
              uint16_t agitateEveryDurationSeconds){
  uint8_t totalCycles = (devDurationSeconds - firstAgitationDurationSeconds) / (agitationDurationSeconds + agitateEveryDurationSeconds);
  uint8_t padding = (devDurationSeconds - firstAgitationDurationSeconds) % (agitationDurationSeconds + agitateEveryDurationSeconds);

  digitalWrite(7, 0x1);
  agitate(firstAgitationDurationSeconds, 8 /* Agitate Motor 1*/, 9 /* Agitate Motor 2*/);
  vibrate(10 /* Vibrate Motor 1*/, 11 /* Vibrate Motor 2*/);
  for (uint8_t cycleCount = 0; cycleCount < totalCycles; cycleCount++)
  {
    delay(agitateEveryDurationSeconds * 1000);
    agitate(agitationDurationSeconds, 8 /* Agitate Motor 1*/, 9 /* Agitate Motor 2*/);
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
/*

  --- fix | Film Development Functions ---

  fix function handles the Fixing Step of the process. vales in seconds. 

  */
# 473 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevBot.ino"
 void fix(uint8_t fixingDurationSeconds){
  uint8_t totalCycles = fixingDurationSeconds * 2;
  digitalWrite(7, 0x1);
  vibrate(10 /* Vibrate Motor 1*/, 11 /* Vibrate Motor 2*/);
  for (uint8_t cycleCount = 0; cycleCount < totalCycles; cycleCount++)
  {
    agitate(15, 8 /* Agitate Motor 1*/, 9 /* Agitate Motor 2*/);
    delay(15 * 1000);
  }
  buzz(6);
  digitalWrite(7, 0x0);
  return;

}
