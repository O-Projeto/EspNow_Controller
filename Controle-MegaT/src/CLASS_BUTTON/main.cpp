#include "ButtonManager.h"
#include "config.h"

// Defina os pinos conectados aos botões
int buttonPins[] = {PIN_UP, PIN_DOWN, PIN_LEFT, PIN_RIGHT};
const int numButtons = sizeof(buttonPins) / sizeof(buttonPins[0]);

ButtonManager buttonManager(buttonPins, numButtons);

void setup()
{
    Serial.begin(115200);
    buttonManager.begin(); // Inicializa os botões
}

void loop()
{
    buttonManager.update(); // Atualiza os estados dos botões

    for (int i = 0; i < numButtons; i++)
    {
        if (buttonManager.isPressed(i))
        {
            Serial.print("Botão ");
            Serial.print(i);
            Serial.println(" pressionado!");
        }
    }

    delay(10); // Pequeno atraso para reduzir o uso da CPU
}