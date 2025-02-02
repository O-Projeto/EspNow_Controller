// No arquivo main.cpp:
#include "ESPReceiver.h"
#include "H_bridge_TB6612.hpp"
#include <Arduino.h>

// Pinos do motor
#define AIN1 25
#define BIN1 27
#define AIN2 33
#define BIN2 14
#define PWMA 32
#define PWMB 13
#define STBY 26

// Instância global da classe
EspNowReceiver *EspNowReceiver::instance = nullptr;

// Cria o objeto do receptor
EspNowReceiver receiver;

// Objeto Motores
Motor rightMotor = Motor(AIN1, AIN2, PWMA, STBY, 1, 10);
Motor leftMotor = Motor(BIN1, BIN2, PWMB, STBY, 2, 10);

int linear = 0;
int angular = 0;

int left_vel = 0;
int right_vel = 0;

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

        linear = data.joy_data.x;
        angular = data.joy_data.y;

        right_vel = (linear - angular);
        left_vel = (linear + angular);
    }
    else
    {
        linear = 0;
        angular = 0;
    }
    leftMotor.drive(left_vel);
    rightMotor.drive(right_vel);
}
