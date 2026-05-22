#include <Arduino.h>

#define TFT_BL 15

void setup()
{
    pinMode(TFT_BL, OUTPUT);

    digitalWrite(TFT_BL, HIGH);

    delay(1000);
}

void loop()
{
}