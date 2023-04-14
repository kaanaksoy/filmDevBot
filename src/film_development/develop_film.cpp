#include "develop_film.hpp"

namespace DevelopFilm
{
    // --- ColorC41 | Film Development Functions ---
    void ColorC41()
    {
        DEBUG_PRINT("ColorC41(): Started now.")
        StateManager::setState(StateManager::DEVELOPING);

        int8_t pushPullValue = 0;
        uint16_t duration;

        Display::readyDisplay();
        Display::gLCD.print("C-41 Development");
        SystemEncoder::EncoderInputType command = SystemEncoder::EncoderNone;
        while (command != SystemEncoder::EncoderEnter)
        {
            Display::gLCD.setCursor(0, 1);
            // manually adding sign & usign abs allows the text to not expand and shrink when it becomes positive or negative.
            Display::gLCD.print(
                String("Psh/Pll: ") +
                (0 > pushPullValue ? "-" : "+") +
                String(abs(pushPullValue)));
            command = SystemEncoder::getCommand(command); // Determine command.
            switch (command)
            {
            case SystemEncoder::EncoderLeft:
                pushPullValue--;
                if (pushPullValue < MAX_PULL)
                    pushPullValue = MAX_PULL;
                break;
            case SystemEncoder::EncoderRight:
                pushPullValue++;
                if (pushPullValue > MAX_PUSH)
                    pushPullValue = MAX_PUSH;
                break;
            case SystemEncoder::EncoderExit:
                StateManager::setState(StateManager::IDLE);
                return;
            case SystemEncoder::EncoderNone:
            default:
                break;
            }
            delay(400);
        }

        Display::readyDisplay();

        switch (pushPullValue)
        {
        case -1:
            duration = PULL_ONE_DEV_DUR;
            Display::gLCD.print("C-41, -1 Pull");
            break;
        case 0:
            duration = STD_DEV_DUR;
            Display::gLCD.print("C-41, +0 Push");
            break;
        case 1:
            duration = PUSH_ONE_DEV_DUR;
            Display::gLCD.print("C-41, +1 Push");
            break;
        case 2:
            duration = PUSH_TWO_DEV_DUR;
            Display::gLCD.print("C-41, +2 Push");
            break;
        case 3:
            duration = PUSH_THR_DEV_DUR;
            Display::gLCD.print("C-41, +3 Push");
            break;
        default:
            break;
        }

        Display::gLCD.setCursor(0, 1);
        Display::gLCD.setCursor(3, 1);
        Display::gLCD.write(ENTER_ICON_ADDR);
        Display::gLCD.setCursor(5, 1);
        Display::gLCD.print("to start...");

        switch (SystemEncoder::encoderAwaitConfirm())
        {
        case SystemEncoder::EncoderEnter:
            /* code */
            break;
        case SystemEncoder::EncoderExit:
            StateManager::setState(StateManager::IDLE);
            return;
        default:
            break;
        }

        Display::readyDisplay();
        Display::gLCD.print("Running.");
        Display::gLCD.setCursor(11, 0);
        Display::gLCD.write(TANK_TEMP_ICON_ADDR);

        develop(duration, 10, 6, 30);
        DEBUG_PRINT("ColorC41(): Just finished developing");

        Display::readyDisplay();

        Display::gLCD.print("Dev Finished");
        Display::gLCD.setCursor(0, 1);
        Display::gLCD.createChar(ENTER_ICON_ADDR, Icons::enterCustomChar);
        Display::gLCD.setCursor(0, 1);
        Display::gLCD.write(ENTER_ICON_ADDR);
        Display::gLCD.setCursor(2, 1);
        Display::gLCD.print("to strt fixing");

        DEBUG_PRINT("ColorC41(): Starting Fixing.")

        switch (SystemEncoder::encoderAwaitConfirm())
        {
        case SystemEncoder::EncoderEnter:
            /* code */
            break;
        case SystemEncoder::EncoderExit:
            StateManager::setState(StateManager::IDLE);
            return;
        default:
            break;
        }

        Display::readyDisplay();
        Display::gLCD.print("Fixing...");
        Display::gLCD.setCursor(11, 0);
        Display::gLCD.write(TANK_TEMP_ICON_ADDR);
        fix(FIXING_DUR);

        Display::readyDisplay();
        Display::gLCD.print("Process Finished");
        Display::gLCD.setCursor(0, 1);
        Display::gLCD.createChar(ENTER_ICON_ADDR, Icons::enterCustomChar);
        Display::gLCD.setCursor(3, 1);
        Display::gLCD.write(ENTER_ICON_ADDR);
        Display::gLCD.setCursor(5, 1);
        Display::gLCD.print("to exit...");

        switch (SystemEncoder::encoderAwaitConfirm())
        {
        case SystemEncoder::EncoderEnter:
            /* code */
            break;
        case SystemEncoder::EncoderExit:
            StateManager::setState(StateManager::IDLE);
            return;
        default:
            break;
        }

        StateManager::setState(StateManager::IDLE);
        return;
    }

    void ColorE6()
    {
        StateManager::setState(StateManager::DEVELOPING);
        // StatusLED::blink(5);
        StateManager::setState(StateManager::IDLE);
        return;
    }

    void BWCustom()
    {
        StateManager::setState(StateManager::DEVELOPING);
        StateManager::setState(StateManager::IDLE);
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
        DEBUG_PRINT("Agitation started.");

        bool agitateDirectionFlag = true;
        bool motorRunning = false;
        unsigned long motorStartMillis;

        switch (agitateDirectionFlag)
        {
        case true:
            analogWrite(AGITATE_MOT_1, 255);
            analogWrite(AGITATE_MOT_2, 0);
            agitateDirectionFlag = false;
            break;
        case false:
        default:
            analogWrite(AGITATE_MOT_1, 0);
            analogWrite(AGITATE_MOT_2, 255);
            agitateDirectionFlag = true;
            break;
        }

        motorStartMillis = StateManager::State.currentMillis;
        motorRunning = true;

        while (motorRunning)
        {
            if (StateManager::State.currentMillis > motorStartMillis + (duration * 1000))
            {
                analogWrite(AGITATE_MOT_1, 0);
                analogWrite(AGITATE_MOT_2, 0);
                motorRunning = false;
            }
            TempSensors::requestTankTemp();
            MenuUI::printTempReadings(TempSensors::getTankTemp());
        }
        DEBUG_PRINT("Agitate Finished.")
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
        DEBUG_PRINT("Vibrate started.");
        unsigned long motorStartTime;
        bool motorRunning;

        for (int count = 0; count < 4; count++)
        {
            DEBUG_PRINT(String("Vibrated ") + count + String(" time(s)."));
            motorStartTime = StateManager::State.currentMillis;
            analogWrite(VIBRATE_MOT_1, 255);
            analogWrite(VIBRATE_MOT_2, 0);
            motorRunning = true;

            while (motorRunning)
            {
                if (StateManager::State.currentMillis > motorStartTime + 1000)
                {
                    analogWrite(VIBRATE_MOT_1, 0);
                    analogWrite(VIBRATE_MOT_2, 0);
                    motorRunning = false;
                }
                TempSensors::requestTankTemp();
                MenuUI::printTempReadings(TempSensors::getTankTemp());
            }
            motorRunning = StateManager::State.currentMillis;
            while (!motorRunning)
            {
                if (StateManager::State.currentMillis > motorRunning + 500)
                    motorRunning = true;
                TempSensors::requestTankTemp();
                MenuUI::printTempReadings(TempSensors::getTankTemp());
            }
        }
        analogWrite(VIBRATE_MOT_2, 0);
        analogWrite(VIBRATE_MOT_1, 0);
        DEBUG_PRINT("Vibrate Ended");
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
        DEBUG_PRINT("develop(): started.");
        digitalWrite(RED_LED_PIN, HIGH);

        agitate(firstAgitationDurationSeconds);
        vibrate();

        DEBUG_PRINT("develop(): completed initial cycle.")
        for (uint8_t cycleCount = 0; cycleCount < totalCycles; cycleCount++)
        {
            DEBUG_PRINT(String("develop(): Cycle ") + cycleCount + String("started. "));

            startTime = StateManager::State.currentMillis;

            while (StateManager::State.currentMillis < startTime + (agitateEveryDurationSeconds * 1000))
            {
                TempSensors::requestTankTemp();
                MenuUI::printTempReadings(TempSensors::getTankTemp());
                delay(250);
            }
            agitate(agitationDurationSeconds);
            DEBUG_PRINT("develop(): just agitated.")

            if (cycleCount + 2 >= totalCycles)
                Utils::buzz(3);

            startTime = StateManager::State.currentMillis;
            while (StateManager::State.currentMillis < startTime + (padding * 1000))
            {
                TempSensors::requestTankTemp();
                MenuUI::printTempReadings(TempSensors::getTankTemp());
                delay(250);
            }
            Utils::buzz(5);
        }
        digitalWrite(RED_LED_PIN, LOW);
        DEBUG_PRINT("develop(): ending now.");
        return;
    }
    /*
      --- fix | Film Development Functions ---
      fix function handles the Fixing Step of the process. vales in seconds.
      */
    void fix(uint8_t fixingDurationSeconds)
    {
        DEBUG_PRINT("fix(): started.");

        digitalWrite(RED_LED_PIN, HIGH);
        DEBUG_PRINT("develop(): calling develop now.");
        develop(FIXING_DUR, 10, 7, 30);

        Utils::buzz(6);
        digitalWrite(RED_LED_PIN, LOW);
        DEBUG_PRINT("develop(): ending now.");
        return;
    }
} // namespace DevelopFilm
