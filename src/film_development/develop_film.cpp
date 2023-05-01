#include "develop_film.hpp"

namespace DevelopFilm
{
    // --- ColorC41 | Film Development Functions ---
    void ColorC41()
    {
        DEBUG_PRINT(F("C41Strtnow"));
        StateManager::setOperationState(OperationStateType::DEVELOPING);

        int8_t pushPullValue = 0;
        uint16_t duration;

        Display::readyDisplay();
        sprintf_P(tmpStr, PSTR("%S %S"), p_C41, p_dev);
        Display::gLCD.print(tmpStr);
        EncoderInputType command = EncoderNone;

        while (command != EncoderEnter)
        {
            command = SystemEncoder::getCommand(command); // Determine command.
            Display::gLCD.setCursor(0, 1);

            // manually adding sign & usign abs allows the text to not expand and shrink when it becomes positive or negative.
            sprintf_P(tmpStr, PSTR("%S/%S: %s%d"), p_psh, p_pll, (0 > pushPullValue ? "-" : "+"), abs(pushPullValue));
            Display::gLCD.print(tmpStr);

            switch (command)
            {
            case EncoderEnter:
                tone(BUZZER_PIN, 4000, 125);
                break;
            case EncoderLeft:
                pushPullValue--;
                if (pushPullValue < MAX_PULL)
                    pushPullValue = MAX_PULL;
                break;
            case EncoderRight:
                pushPullValue++;
                if (pushPullValue > MAX_PUSH)
                    pushPullValue = MAX_PUSH;
                break;
            case EncoderExit:
                break;
                StateManager::setOperationState(OperationStateType::IDLE);
                return;
            case EncoderNone:
            default:
                break;
            }
        }
        delay(1);

        Display::readyDisplay();

        sprintf_P(tmpStr, PSTR("%S, %s%d %S"),
                  p_C41,
                  (0 > pushPullValue ? "-" : "+"),
                  abs(pushPullValue),
                  (0 > pushPullValue ? p_pll : p_pll));
        Display::gLCD.print(tmpStr);

        Display::gLCD.setCursor(0, 1);
        Display::gLCD.setCursor(3, 1);
        Display::gLCD.write(ENTER_ICON_ADDR);
        Display::gLCD.setCursor(5, 1);
        sprintf_P(tmpStr, PSTR("%S"), p_toStrt);
        Display::gLCD.print(tmpStr);

        switch (SystemEncoder::encoderAwaitConfirm())
        {
        case EncoderEnter:
            /* code */
            break;
        case EncoderExit:
            StateManager::setOperationState(OperationStateType::IDLE);
            return;
        default:
            break;
        }

        Display::readyDisplay();
        sprintf_P(tmpStr, PSTR("%S, %S"), p_running, p_dev);
        Display::gLCD.print(tmpStr);
        Display::gLCD.setCursor(11, 0);
        Display::gLCD.write(TANK_TEMP_ICON_ADDR);

        develop(duration, 10, 6, 30);
        DEBUG_PRINT(F("C41findev"));

        Display::readyDisplay();

        sprintf_P(tmpStr, PSTR("%S"), p_finsihed);
        Display::gLCD.print(tmpStr);
        Display::gLCD.setCursor(0, 1);
        Display::gLCD.write(ENTER_ICON_ADDR);
        Display::gLCD.setCursor(2, 1);
        sprintf_P(tmpStr, "%S %S", p_toStrt, p_fixing);
        Display::gLCD.print(tmpStr);

        DEBUG_PRINT(F("C41StrtFix"));

        switch (SystemEncoder::encoderAwaitConfirm())
        {
        case EncoderEnter:
            /* code */
            break;
        case EncoderExit:
            StateManager::setOperationState(OperationStateType::IDLE);
            return;
        default:
            break;
        }

        Display::readyDisplay();
        sprintf_P(tmpStr, PSTR("%S"), p_fixing);
        Display::gLCD.print(tmpStr);
        Display::gLCD.setCursor(11, 0);
        Display::gLCD.write(TANK_TEMP_ICON_ADDR);
        fix(FIXING_DUR);
        Display::readyDisplay();
        sprintf_P(tmpStr, PSTR("%S"), p_finsihed);
        Display::gLCD.print(tmpStr);
        Display::gLCD.setCursor(3, 1);
        Display::gLCD.write(ENTER_ICON_ADDR);
        Display::gLCD.setCursor(5, 1);
        sprintf_P(tmpStr, PSTR("%S"), p_toExit);
        Display::gLCD.print(tmpStr);

        switch (SystemEncoder::encoderAwaitConfirm())
        {
        case EncoderEnter:
            /* code */
            break;
        case EncoderExit:
            StateManager::setOperationState(OperationStateType::IDLE);
            return;
        default:
            break;
        }

        StateManager::setOperationState(OperationStateType::IDLE);
        return;
    }

    void ColorE6()
    {
        StateManager::setOperationState(OperationStateType::DEVELOPING);
        // StatusLED::blink(5);
        StateManager::setOperationState(OperationStateType::IDLE);
        return;
    }

    void BWCustom()
    {
        StateManager::setOperationState(OperationStateType::DEVELOPING);
        StateManager::setOperationState(OperationStateType::IDLE);
        return;
    }

    /*
  --- agitate | Film Development Helper Functions ---
  duration: Agitate duration in seconds
  AGITATE_MOT_1: Motor Control Pin 1
  AGITATE_MOT_2: Motor Control Pin 2

  Runs the agitate motor for the amount of time provided,
  each time in a different direction.
*/
    void agitate(int duration)
    {
        DEBUG_PRINT("Agitstrt");

        static bool agitateDirectionFlag = true;
        bool motorRunning = false;
        unsigned long motorStartMillis;

        switch (agitateDirectionFlag)
        {
        case true:
            for (int pwmVal = 0; pwmVal < 256; pwmVal += 5)
            {
                analogWrite(AGITATE_MOT_1, pwmVal);
                analogWrite(AGITATE_MOT_2, 0);
                delay(5);
            }

            break;
        case false:
        default:
            for (int pwmVal = 0; pwmVal < 256; pwmVal += 5)
            {
                analogWrite(AGITATE_MOT_1, 0);
                analogWrite(AGITATE_MOT_2, pwmVal);
                delay(5);
            }
            break;
        }

        motorStartMillis = State.currentMillis;
        motorRunning = true;

        while (motorRunning)
        {
            if (State.currentMillis > motorStartMillis + (duration * 1000))
            {
                analogWrite(AGITATE_MOT_1, 0);
                analogWrite(AGITATE_MOT_2, 0);
                motorRunning = false;
            }
            TempSensors::requestTankTemp();
            MenuUI::printTempReadings(TempSensors::getTankTemp());
        }
        agitateDirectionFlag = !agitateDirectionFlag;
        return;
    }

    /*
    --- vibrate | Film Development Helper Functions ---
    AGITATE_MOT_1: Motor Control Pin 1
    AGITATE_MOT_2: Motor Control Pin 2

    Simple vibrate function, used to release air bubbles from the emulsion surface.
  */
    void vibrate()
    {
        unsigned long motorStartTime;
        bool motorRunning;

        for (int count = 0; count < 4; count++)
        {
            DEBUG_PRINT(String("Vibrated ") + count + String(" time(s)."));
            motorStartTime = State.currentMillis;
            analogWrite(VIBRATE_MOT_1, 255);
            analogWrite(VIBRATE_MOT_2, 0);
            motorRunning = true;

            while (motorRunning)
            {
                if (State.currentMillis > motorStartTime + 1000)
                {
                    analogWrite(VIBRATE_MOT_1, 0);
                    analogWrite(VIBRATE_MOT_2, 0);
                    motorRunning = false;
                }
                TempSensors::requestTankTemp();
                MenuUI::printTempReadings(TempSensors::getTankTemp());
            }
            motorRunning = State.currentMillis;
            while (!motorRunning)
            {
                if (State.currentMillis > motorRunning + 500)
                    motorRunning = true;
                TempSensors::requestTankTemp();
                MenuUI::printTempReadings(TempSensors::getTankTemp());
            }
        }
        analogWrite(VIBRATE_MOT_2, 0);
        analogWrite(VIBRATE_MOT_1, 0);
        return;
    }

    /*
      --- develop | Film Development Functions ---
      Develop function that handles the development process.
    */
    void develop(uint16_t devDurationSeconds,
                 uint8_t firstAgitationDurationSeconds,
                 uint8_t agitationDurationSeconds,
                 uint16_t agitateEveryDurationSeconds)
    {

        uint8_t totalCycles = (devDurationSeconds - firstAgitationDurationSeconds) /
                              (agitationDurationSeconds + agitateEveryDurationSeconds);
        uint8_t padding = (devDurationSeconds - firstAgitationDurationSeconds) %
                          (agitationDurationSeconds + agitateEveryDurationSeconds);

        unsigned long startTime;
        digitalWrite(RED_LED_PIN, HIGH);

        agitate(firstAgitationDurationSeconds);
        vibrate();

        for (uint8_t cycleCount = 0; cycleCount < totalCycles; cycleCount++)
        {
            startTime = State.currentMillis;

            while (State.currentMillis < startTime + (agitateEveryDurationSeconds * 1000))
            {
                TempSensors::requestTankTemp();
                MenuUI::printTempReadings(TempSensors::getTankTemp());
                delay(250);
            }
            agitate(agitationDurationSeconds);

            if (cycleCount + 2 >= totalCycles)
                tone(BUZZER_PIN, 4000, 125);

            startTime = State.currentMillis;
            while (State.currentMillis < startTime + (padding * 1000))
            {
                TempSensors::requestTankTemp();
                MenuUI::printTempReadings(TempSensors::getTankTemp());
                delay(250);
            }
            tone(BUZZER_PIN, 4000, 125);
        }
        digitalWrite(RED_LED_PIN, LOW);
        return;
    }
    /*
      --- fix | Film Development Functions ---
      fix function handles the Fixing Step of the process. vales in seconds.
      */
    void fix(uint8_t fixingDurationSeconds)
    {
        digitalWrite(RED_LED_PIN, HIGH);
        develop(FIXING_DUR, 10, 7, 30);

        tone(BUZZER_PIN, 4000, 125);
        digitalWrite(RED_LED_PIN, LOW);
        return;
    }
} // namespace DevelopFilm
