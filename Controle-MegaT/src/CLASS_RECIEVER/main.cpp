// No arquivo main.cpp:
#include "ESPReceiver.h"
#include <Arduino.h>

// Instância global da classe
EspNowReceiver *EspNowReceiver::instance = nullptr;

// Cria o objeto do receptor
EspNowReceiver receiver;

void setup()
{
    // Inicializa o monitor serial
    Serial.begin(115200);

    // Define a instância estática para o callback
    EspNowReceiver::instance = &receiver;

    // Inicializa o receptor ESP-NOW
    if (!receiver.begin())
    {
        Serial.println("Falha ao iniciar o ESP-NOW");
        while (true)
            ;
    }
}

void loop()
{
    // Verifica se há novos dados
    if (receiver.hasData())
    {
        espdata data = receiver.getData();

        // Imprime os dados recebidos no Serial Monitor
        Serial.print("| up: ");
        Serial.print(data.arrow_data.up);
        Serial.print("| down: ");
        Serial.print(data.arrow_data.down);
        Serial.print("| left: ");
        Serial.print(data.arrow_data.left);
        Serial.print("| right: ");
        Serial.print(data.arrow_data.right);
        Serial.print("| JOY:X :  ");
        Serial.print(data.joy_data.x);
        Serial.print(":Y:  ");
        Serial.print(data.joy_data.y);
        Serial.print(":SW:  ");
        Serial.print(data.joy_data.sw);
        Serial.print("--Bat  ");
        Serial.print(data.battery);
        Serial.println("");
    }
}
