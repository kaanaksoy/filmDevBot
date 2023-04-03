#line 1 "/Users/kaan/Documents/Arduino/filmDevBot/filmDevHelpers.h"
/*
  --- agitate | Film Development Helper Functions ---
  
  Runs the agitate motor for the amount of time provided, 
  each time in a different direction.
*/
void agitate(int durationSeconds, int MOT_IN1, int MOT_IN2){
    bool agitateDirectionFlag = true;
  switch (agitateDirectionFlag)
  {
  case true:
    analogWrite(MOT_IN1, 255);
    analogWrite(MOT_IN2, 0);
    delay(durationSeconds * 1000);

    agitateDirectionFlag = false;
    break;
  case false:
  default:
    analogWrite(MOT_IN1, 0);
    analogWrite(MOT_IN2, 255);
    delay(durationSeconds * 1000);
    agitateDirectionFlag = true;
    break;
  }

  analogWrite(MOT_IN1, 0);
  analogWrite(MOT_IN2, 0);
  
  return;
}

/*
  --- vibrate | Film Development Helper Functions ---
  Simple vibrate function, used to release air bubbles from the emulsion surface.
*/
void vibrate(int MOT_IN3, int MOT_IN4){
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