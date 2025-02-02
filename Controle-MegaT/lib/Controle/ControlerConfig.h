
#include "ButtonManager.h"
#include "joy.h"
#include "espdata.h"
#include "config.h"

joy joystickEsquerdo(JOY_X, JOY_Y, 33);
joyData joystickEsquerdoData;
// Defina os pinos conectados aos botões
int buttonPins[] = {PIN_UP, PIN_DOWN, PIN_LEFT, PIN_RIGHT};
const int numButtons = sizeof(buttonPins) / sizeof(buttonPins[0]);

ButtonManager buttonManager(buttonPins, numButtons);

espdata data2send;

void debug()
{

    Serial.print("Botão|");

    Serial.print(" up: ");
    Serial.print(data2send.arrow_data.up);
    Serial.print(" down: ");
    Serial.print(data2send.arrow_data.down);
    Serial.print(" left: ");
    Serial.print(data2send.arrow_data.left);
    Serial.print(" right: ");
    Serial.print(data2send.arrow_data.right);

    Serial.print("| JOY| X : ");
    Serial.print(data2send.joy_data.x);
    Serial.print(" | Y : ");
    Serial.print(data2send.joy_data.y);
    Serial.print(" | sw: ");
    Serial.print(data2send.joy_data.sw);
    Serial.println("");
}

void fill_struct(ButtonManager &buttonManager, joyData &joystickEsquerdoData)
{

    data2send.joy_data.x = joystickEsquerdoData.x;
    data2send.joy_data.y = joystickEsquerdoData.y;
    data2send.joy_data.sw = 0;
    data2send.arrow_data.up = buttonManager.isPressed(0);
    data2send.arrow_data.down = buttonManager.isPressed(1);
    data2send.arrow_data.left = buttonManager.isPressed(2);
    data2send.arrow_data.right = buttonManager.isPressed(3);
}

void ControlerBegin()
{

    buttonManager.begin(); // Inicializa os botões
    joystickEsquerdo.begin();
}

void getControlerData()
{
    joystickEsquerdoData = joystickEsquerdo.getJoyData();
    buttonManager.update();
    fill_struct(buttonManager, joystickEsquerdoData);
}
