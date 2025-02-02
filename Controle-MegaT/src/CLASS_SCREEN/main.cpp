#include <Arduino.h>
#include "espdata.h"
#include "ControlerConfig.h"
#include "config.h"
#include "screen.h"
// Create a struct_message called myData
espdata myData;

screen tela;

void setup()
{
    ControlerBegin();
    tela.begin();
}

void loop()
{
    getControlerData();
    tela.test(data2send);
    tela.update();
}