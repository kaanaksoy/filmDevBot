#include "develop_film.hpp"

namespace DevelopFilm
{
    void Custom()
    {
        return;
    }
    // Func for C41 Dev.
    void ColorC41()
    {        StateManager::setOperationState(INDEVELOPMENU);

        int8_t pushPullValue = 0; // var to store push pull value of process.
        uint16_t duration;

        /* ----------------- Ask for desired push pull val from user ---------------- */
        Display::readyDisplay();

        sprintf_P(tmpStr, PSTR("%S %S"), p_C41, p_dev);
        Display::gLCD.print(tmpStr);
        EncoderInputType command = EncoderNone;

        // Blocking func because we need to wait for a response from the user.
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
                StateManager::setOperationState(IDLE);
                return;
            case EncoderNone:
            default:
                break;
            }
        }
        /* ----------------------- Confirm the push pull value ---------------------- */
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

        switch (pushPullValue)
        {
        case 1:
            duration = PUSH_ONE_DEV_DUR;
            break;
        case 2:
            duration = PUSH_TWO_DEV_DUR;
            break;
        case 3:
            duration = PUSH_THR_DEV_DUR;
            break;
        case -1:
            duration = PULL_ONE_DEV_DUR;
            break;
        case 0:
        default:
            duration = STD_DEV_DUR;
            break;
        }
        switch (SystemEncoder::encoderAwaitConfirm())
        {
        case EncoderEnter:
            tone(BUZZER_PIN, 4000, 125);
            break;
        case EncoderExit:
            StateManager::setOperationState(IDLE);
            return;
        default:
            break;
        }

        /* -------------------------- Start Dev and Fixing -------------------------- */
        Display::readyDisplay();
        Display::gLCD.setCursor(0,1);
        sprintf_P(tmpStr, PSTR("%S, %S"), p_running, p_dev);
        Display::gLCD.print(tmpStr);
        Display::gLCD.setCursor(11, 0);
        Display::gLCD.write(TANK_TEMP_ICON_ADDR);

        develop(duration, STD_FRST_AGITATE_DUR, AGITATE_DUR,
                STD_AGITATE_EVERY_DUR, FIXING_DUR, STD_FRST_AGITATE_DUR,
                AGITATE_DUR, STD_AGITATE_EVERY_DUR);
        // develop(duration, STD_FRST_AGITATE_DUR, AGITATE_DUR,
        //         STD_AGITATE_EVERY_DUR, FIXING_DUR, STD_FRST_AGITATE_DUR,
        //         11, STD_AGITATE_EVERY_DUR);

        return;
    }
    /*
          --- develop | Film Development Functions ---
          Develop function that handles the development process.
        */
    int develop(int devDurSec, int fstAgitDurSec, int agitDurSec,
                int agitEvryDurSec, int fixDurSec, int fstFixAgitDurSec,
                int fixAgitDurSec, int fixAgitEveryDurSec)
    {
        // parameters of the current development.
        static int devCyclesRemaining;
        static unsigned int devPadding;
        static int devAgitationInterval; // agitEveryDurSec * 1000
        static int devAgitDur; // agitDurSec

        static int fixCyclesRemaining;
        static unsigned int fixPadding;
        static int fixAgitationInterval; // fixAgitEveryDurSec * 1000
        static int fixAgitDur; // fixAgitDursec
        static int firstFixAgitDur; // firstFixAgitDurSec


        // flags used to track where in the development process we are.
        static bool firstDevAgitateComplete;
        static bool firstFixAgitateComplete;
        static bool mainDevAgitateComplete;
        static bool mainFixAgitateComplete;
        // used to check if we need to check if agitate is finished.
        static bool agitateRunning;
        int agitateStatus; // In order to see check the status of agitate.

        // used to track how long the current process has been going for.
        static unsigned long lastOperationMillis;

        static unsigned int totalProgressSteps; // max num of steps for progressbar.

        switch (State.currentState)
        {
        case INDEVELOPMENU:
            // We were called for the first time, set params and start first agitate.
            StateManager::setOperationState(DEVELOPING);

            // Calculate the necessary parameters.
            devCyclesRemaining = (devDurSec - fstAgitDurSec) / (agitDurSec + agitEvryDurSec);
            devPadding = ((devDurSec - fstAgitDurSec) % (agitDurSec + agitEvryDurSec)) * 1000;
            devAgitationInterval = agitEvryDurSec * 1000; // Calculate and store the other params
            devAgitDur = agitDurSec;

            fixCyclesRemaining = (fixDurSec - fstFixAgitDurSec) / (fixAgitDurSec + fixAgitEveryDurSec);
            fixPadding = ((fixDurSec - fstFixAgitDurSec) % (fixAgitDurSec + fixAgitEveryDurSec)) * 1000;
            fixAgitationInterval = fixAgitEveryDurSec * 1000;
            fixAgitDur = fixAgitDurSec;
            firstFixAgitDur = fstFixAgitDurSec;
            

            // Set our tracking flags, since we just started.
            firstDevAgitateComplete = false;
            firstFixAgitateComplete = false;
            mainDevAgitateComplete = false;
            mainFixAgitateComplete = false;
            agitateRunning = false;

            //Set the progressBar var
            totalProgressSteps = devCyclesRemaining + 2; // +1 for first agitate + 1 for padding
            /* ----------------------------- Dev Operations ----------------------------- */

            // Start the first agitation.
            Indicators::progressBarLEDs(); //Start the progressBar
            agitate(fstAgitDurSec);
            agitateRunning = true;
            return 0;
            break;
        case DEVELOPING: // Develop was called because we were running.
            if (!firstDevAgitateComplete) // We need to check if we completed the first agitation before we move on.
            {
                agitateStatus = agitate();
                if (agitateStatus == 1)
                {
                    lastOperationMillis = millis(); // Set the last operation time.
                    firstDevAgitateComplete = true;

                    /* Calculate the number of steps completed (-1 is to subtract the now completed first Agitate step)
                    multiply that with the num of leds per steps. */
                    Indicators::progressBarLEDs(((totalProgressSteps - devCyclesRemaining - 1) * (Indicators::calculateProgressBarStep(totalProgressSteps))));
                    //Indicators::progressBarLEDs(9);
                    
                    agitateRunning = false;
                    return 0;
                }
                else if (agitateStatus = 0)
                {
                    return 0;
                } else 
                {
                    DEBUG_TRACE();
                    return -1;
                }
            }
            // Since we finished the first agitation and are not waiting on an another agitation, lets find out what we need to do.
            else if (!mainDevAgitateComplete) // If we are in the main agitation cycles.
            {
                // Since we completed the first agitation, lets first check if we are waiting for an agitatation.
                if (agitateRunning)
                {
                    agitateStatus = agitate();
                    if (agitateStatus == 1)
                    {
                        lastOperationMillis = millis();
                        firstDevAgitateComplete = true;
                        agitateRunning = false;
                        /* Calculate the number of steps completed (-1 is to subtract the completed first Agitate step)
                        multiply that with the num of leds per steps. */
                        Indicators::progressBarLEDs((totalProgressSteps - devCyclesRemaining - 1) * 
                                                    (Indicators::calculateProgressBarStep(totalProgressSteps)));
                        return 0;
                    }
                    else if (agitateStatus = 0)
                    {
                        return 0;
                    } else 
                    {
                        DEBUG_TRACE();
                        return -1;
                    }
                }
                else if (devCyclesRemaining > 0)
                {
                    if (millis() - lastOperationMillis >= devAgitationInterval)
                    {
                        if (devCyclesRemaining == 2)
                            tone(BUZZER_PIN, 4000, 2000);
                        agitateRunning = true;
                        devCyclesRemaining--;
                        agitate(devAgitDur);
                        return 0;
                    }
                    else
                        return 0;
                }
                else if (devCyclesRemaining <= 0)// If we dont have any cycles left, we should do the padding wait.
                {
                    if (millis() - lastOperationMillis >= devPadding)
                    {
                        tone(BUZZER_PIN, 4000, 1000);
                        mainDevAgitateComplete = true;

                        Indicators::progressBarLEDs(100); // Set the progressbar to 100%.
                        Display::readyDisplay();

                        sprintf_P(tmpStr, PSTR("%S %S"),p_dev , p_finsihed);
                        Display::gLCD.print(tmpStr);
                        Display::gLCD.setCursor(0, 1);
                        Display::gLCD.write(ENTER_ICON_ADDR);
                        Display::gLCD.setCursor(1, 1);
                        sprintf_P(tmpStr, PSTR(" %S"), p_toExit);
                        Display::gLCD.print(tmpStr);

                        delay(25);
                        Indicators::progressBarLEDs(); // Turn off the progressbar
                        Indicators::blinkLEDs(CRGB::Green, BLINK_FAST_INTERVAL, Indicators::IndicatorParamType::START);
                        Indicators::buzz(BUZZER_FAST_INTERVAL, Indicators::IndicatorParamType::START);
                        switch (SystemEncoder::encoderAwaitConfirm())
                        {
                        case EncoderEnter:
                            /* code */
                            break;
                        case EncoderExit:
                        default:
                            break;
                        }

                        Indicators::blinkLEDs(CRGB::Green, BLINK_FAST_INTERVAL, Indicators::IndicatorParamType::STOP);
                        Indicators::buzz(BUZZER_FAST_INTERVAL, Indicators::IndicatorParamType::STOP);
                        Indicators::progressBarLEDs(); //Restart the progressbar

                        /* ---------------------------- Fixing operations --------------------------- */

                        Display::readyDisplay();
                        Display::gLCD.setCursor(0,1);
                        sprintf_P(tmpStr, PSTR("%S"), p_fixing);
                        Display::gLCD.print(tmpStr);
                        Display::gLCD.setCursor(11, 0);
                        Display::gLCD.write(TANK_TEMP_ICON_ADDR);

                        totalProgressSteps = fixCyclesRemaining + 2; // +1 for first agitate + 1 for padding
                        agitateRunning = true;
                        Indicators::progressBarLEDs();
                        agitate(firstFixAgitDur);
                    
                        return 0;
                    }
                    else
                        return 0;
                } else return -1;
            }
            else if (!firstFixAgitateComplete) // Lets see if the first fix agitate is complete. (Started as the last step of dev)
            {
                agitateStatus = agitate();
                if (agitateStatus == 1)
                {
                    
                    lastOperationMillis = millis();
                    firstFixAgitateComplete = true;
                    agitateRunning = false;
                    // TODO PROGRESSBAR STEP
                    /* Calculate the number of steps completed (-1 is to subtract the now completed first Agitate step)
                    multiply that with the num of leds per steps. */
                    Indicators::progressBarLEDs((totalProgressSteps - fixCyclesRemaining - 1) * 
                                                (Indicators::calculateProgressBarStep(totalProgressSteps)));
                    return 0;
                }
                else if (agitateStatus = 0)
                {
                    return 0;
                } else 
                {
                    DEBUG_TRACE();
                    return -1;
                }
            }
            // Since we finished the first agitation and are not waiting on an another agitation, lets find out what we need to do.
            else if (!mainFixAgitateComplete) // Since were fixing now, lets do the fixing things
            {
                // Since we completed the first agitation, lets first check if we are waiting for an agitatation.
                if (agitateRunning)
                {
                    agitateStatus = agitate();
                    if (agitateStatus == 1)
                    {
                        lastOperationMillis = millis();
                        firstDevAgitateComplete = true;
                        agitateRunning = false;
                        /* Calculate the number of steps completed (-1 is to subtract the now completed first Agitate step)
                        multiply that with the num of leds per steps. */
                        Indicators::progressBarLEDs((totalProgressSteps - fixCyclesRemaining - 1) * 
                                                    (Indicators::calculateProgressBarStep(totalProgressSteps)));
                        return 0;
                    }
                    else if (agitateStatus = 0)
                    {
                        return 0;
                    } else 
                    {
                        DEBUG_TRACE();
                        return -1;
                    }
                }
                else if (fixCyclesRemaining > 0)
                {
                    if (millis() - lastOperationMillis >= fixAgitationInterval)
                    {
                        if (fixCyclesRemaining == 2)
                            tone(BUZZER_PIN, 4000, 1000);
                        agitateRunning = true;
                        fixCyclesRemaining--;
                        agitate(fixAgitDur);
                        return 0;
                    }
                    else
                        return 0;
                }
                else if (fixCyclesRemaining <= 0)
                {
                    if (millis() - lastOperationMillis >= fixPadding)
                    {
                        tone(BUZZER_PIN, 4000, 125);
                        mainFixAgitateComplete = true;

                        Indicators::progressBarLEDs(100); // Set progressbar to 100%
                        Display::readyDisplay();

                        sprintf_P(tmpStr, PSTR("%S %S"),p_fixing , p_finsihed);
                        Display::gLCD.print(tmpStr);
                        Display::gLCD.setCursor(0, 1);
                        Display::gLCD.write(ENTER_ICON_ADDR);
                        Display::gLCD.setCursor(1, 1);
                        sprintf_P(tmpStr, PSTR(" %S"), p_toExit);
                        Display::gLCD.print(tmpStr);

                        delay(25); // To make sure the 100% progressbar is visible.
                        Indicators::progressBarLEDs();
                        Indicators::blinkLEDs(CRGB::Green, BLINK_FAST_INTERVAL, Indicators::IndicatorParamType::START);
                        Indicators::buzz(BUZZER_FAST_INTERVAL, Indicators::IndicatorParamType::START);
                        switch (SystemEncoder::encoderAwaitConfirm())
                        {
                        case EncoderEnter:
                            Indicators::blinkLEDs(CRGB::Green, BLINK_FAST_INTERVAL, Indicators::IndicatorParamType::STOP);
                            Indicators::buzz(BUZZER_FAST_INTERVAL, Indicators::IndicatorParamType::STOP);
                            StateManager::setOperationState(IDLE);
                            return 1;
                            break;
                        case EncoderExit:
                        default:
                            break;
                        }

                    }
                    else
                        return 0;
                }
            }
            break;
        // If we get here with a state mismatch, we have a problem.
        case IDLE:
        case MONITORING:
        default:
            DEBUG_PRINT("Develop called unexpectedly");
            return -1;
            break;
        }
        DEBUG_PRINT("Develop falled through");
        return -1;
    }
    /*
        --- agitate | Film Development Helper Functions ---
        duration: Agitate duration in seconds
        AGITATE_MOT_1: Motor Control Pin 1
        AGITATE_MOT_2: Motor Control Pin 2

        Runs the agitate motor for the amount of time provided,
        each time in a different direction.
    */
    int agitate(uint16_t duration)
    {
        DEBUG_PRINT("Agit Called");
        DEBUG_PRINT(duration);

        static bool agitateDirectionFlag = true;
        static bool motorRunning;
        static unsigned long motorStartMillis;
        static uint16_t motorToRunFor;

        switch (duration != 0)
        {
        case true:
            DEBUG_PRINT("Started Motors");
            if (!motorRunning)
            {
                motorStartMillis = millis();
                motorToRunFor = duration * 1000;
                motorRunning = true;
                if (agitateDirectionFlag)
                {
                    for (int pwmVal = 0; pwmVal < 256; pwmVal += 5)
                    {
                        analogWrite(AGITATE_MOT_1, pwmVal);
                        analogWrite(AGITATE_MOT_2, 0);
                        delay(5);
                    }
                }
                else
                {
                    for (int pwmVal = 0; pwmVal < 256; pwmVal += 5)
                    {
                        analogWrite(AGITATE_MOT_1, 0);
                        analogWrite(AGITATE_MOT_2, pwmVal);
                        delay(5);
                    }
                }
                return 0;
            } else
            {
                DEBUG_PRINT("Agitate: Error, multiple calls to agitate different durations");
                return -1;
            }
            break;
        case false:
            DEBUG_PRINT("Agit check");
            if (millis() - motorStartMillis >= motorToRunFor)
            {
                DEBUG_PRINT("Stopping Mots");
                analogWrite(AGITATE_MOT_1, 0);
                analogWrite(AGITATE_MOT_2, 0);
                motorRunning = false;
                agitateDirectionFlag = !agitateDirectionFlag;
                return 1;
            } else return 0;
            break;
        default:
            break;
        }
        return -1;
    }

} // namespace DevelopFilm2
