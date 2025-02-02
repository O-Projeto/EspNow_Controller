#include "joy.h"

joy::joy(int pin_x, int pin_y, int pin_sw)
{
    PIN_X = pin_x;
    PIN_Y = pin_y;
    PIN_SW = pin_sw;
}

void joy::begin()
{
    pinMode(PIN_X, INPUT);
    pinMode(PIN_Y, INPUT);
    pinMode(PIN_SW, INPUT);
}

joyData joy::getJoyData()
{
    readX = analogRead(PIN_X);
    readY = analogRead(PIN_Y);

    // TODO add debouncer

    readSw = digitalRead(PIN_SW);
    joy_data.x = deadZoneFilter(rangeConverter(readX));
    joy_data.y = deadZoneFilter(rangeConverter(readY));

    return joy_data;
}

int joy::rangeConverter(int data)
{
    int new_data = map(data, 0, 4096, -_range, _range);
    return new_data;
}

int joy::deadZoneFilter(int data)
{
    if (data > -_deadZone && data < _deadZone)
    {
        data = 0;
    }
    return data;
}
