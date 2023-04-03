/*
  --- agitate | Film Development Helper Functions ---
  dur: Agitate duration in seconds
  motorPin1: Motor Control Pin 1
  motorPin2: Motor Control Pin 2
  
  Runs the agitate motor for the amount of time provided, 
  each time in a different direction.
*/
void agitate(int dur, int motorPin1, int motorPin2){
    bool agitateDirectionFlag = true;
  switch (agitateDirectionFlag)
  {
  case true:
    analogWrite(motorPin1, 255);
    analogWrite(motorPin2, 0);
    delay(dur * 1000);

    agitateDirectionFlag = false;
    break;
  case false:
  default:
    analogWrite(motorPin1, 0);
    analogWrite(motorPin2, 255);
    delay(dur * 1000);
    agitateDirectionFlag = true;
    break;
  }

  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, 0);
  
  return;
}

/*
  --- vibrate | Film Development Helper Functions ---
  motorPin1: Motor Control Pin 1
  motorPin2: Motor Control Pin 2

  Simple vibrate function, used to release air bubbles from the emulsion surface.
*/
void vibrate(int motorPin1, int motorPin2){
  for (int i = 0; i < 4; i++)
  {
  analogWrite(motorPin1, 255);
  analogWrite(motorPin2, 0);
  delay(1000);
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, 0);
  delay(500);
  }
  return;
}