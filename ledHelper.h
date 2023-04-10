#pragma once

#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS 1
#define RGB_LED_PIN 12
#define STATUS_LED_BRIGHTNESS 50 // (0-255) Sets max brightness
#define INTERVAL 1000
namespace StatusLED
{
    unsigned long currentMillis;

    CRGB gled[NUM_LEDS];

    enum statusLEDColors {
        Red,
        Green,
        Blue,
        Yellow,
    };

    void initStatusLED(){
        
        FastLED.addLeds<WS2812, RGB_LED_PIN, RGB>(gled, NUM_LEDS);  // GRB ordering is typical
        FastLED.setBrightness(STATUS_LED_BRIGHTNESS);
        
        int hue = 0;
        while(hue != 255)
        {
            gled[0] = CHSV(hue, 255, 255);
            FastLED.show();

            EVERY_N_MILLISECONDS(10) //Rainbow move speed
            {
                hue++;
            }
        }
        gled[0]=CHSV(hue,0,0);
        FastLED.show();
    }

    /* void blinkStatusLED(statusLEDColors color, uint8_t blinkCount){
        bool ledOn = false;
        uint8_t hue;
        uint8_t counter = 0; 
        switch (color)
        {
        case Red:
            hue = 0;
            break;
        case Green:
            hue = 100;
            break;
        case Blue:
            hue = 160;
            break;
        case Yellow:
            hue = 40;
            break;
        default:
            hue = 200; //Pinkish magenta
            break;
        }

        EVERY_N_MILLISECONDS(250){
            if(ledOn)
            {
                leds[0]=CHSV(hue,0,0);
                counter++;
                if (counter >= blinkCount)
                {
                    leds[0]=CHSV(hue,0,0);
                    FastLED.show();
                    return;
                }
                
            }
            else {
                leds[0]=CHSV(hue,0,0);
            }
            ledOn = !ledOn;
            FastLED.show();
        }
    } */
        unsigned long lastUpdateMillis;
        uint8_t blinkCount;

    void blink(int count = 0){

        if (count > 0){
            if (StateManager::State.ledInUse) return;
            blinkCount = count;
            StateManager::State.ledInUse = true;
            //Serial.println("BLink Called");
            return;
        }

        if (blinkCount > 0)
        {
            if(currentMillis - lastUpdateMillis >= INTERVAL){
                //Serial.println("Update LED");
                digitalWrite(RED_LED, !digitalRead(RED_LED)); //Change LED State
                lastUpdateMillis == currentMillis; // Update the last update time.
                blinkCount --; //Subtract from blink count
                // if (blinkCount <=0) State.ledUsed = false;
                return;        
            }
        } else {

            digitalWrite(RED_LED, LOW);
            StateManager::State.ledInUse = false;
            lastUpdateMillis = currentMillis;
        }
        return;
    }
    
} // namespace StatusLED
