#include <Arduino.h>
#include "joy.h"
#include "config.h"

joy joystickEsquerdo(JOY_X, JOY_Y, 33);
joyData joystickEsquerdoData;

void setup()
{
    Serial.begin(115200);
}

void loop()
{

    joystickEsquerdoData = joystickEsquerdo.getJoyData();

    Serial.print("JOY| X : ");
    Serial.print(joystickEsquerdoData.x);
    Serial.print(" | Y : ");
    Serial.print(joystickEsquerdoData.y);
    Serial.print(" | sw: ");
    Serial.println(joystickEsquerdoData.sw);
}