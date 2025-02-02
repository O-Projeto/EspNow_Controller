#ifndef ESPNOWRECEIVER_H
#define ESPNOWRECEIVER_H

#include <esp_now.h>
#include <WiFi.h>
#include "espdata.h" // Certifique-se de que esta estrutura esteja definida corretamente

class EspNowReceiver
{
private:
    espdata myData;
    bool dataReceived;

    // Método interno para tratar dados recebidos
    void handleDataReceived(const uint8_t *mac, const uint8_t *incomingData, int len)
    {
        memcpy(&myData, incomingData, sizeof(myData));
        dataReceived = true;
    }

public:
    // Construtor
    EspNowReceiver() : dataReceived(false) {}

    // Inicializa o ESP-NOW
    bool begin()
    {
        // Configura o Wi-Fi como Station
        WiFi.mode(WIFI_STA);

        // Inicializa o ESP-NOW
        if (esp_now_init() != ESP_OK)
        {
            Serial.println("Erro ao inicializar ESP-NOW");
            return false;
        }

        // Registra o callback
        esp_now_register_recv_cb(OnDataRecvStatic);
        return true;
    }

    // Retorna os dados mais recentes recebidos
    espdata getData()
    {
        dataReceived = false; // Reseta o status de novos dados recebidos
        return myData;
    }

    // Verifica se novos dados foram recebidos
    bool hasData()
    {
        return dataReceived;
    }

    // Callback estático global para tratar dados recebidos
    static void OnDataRecvStatic(const uint8_t *mac, const uint8_t *incomingData, int len)
    {
        if (instance)
        {
            instance->handleDataReceived(mac, incomingData, len);
        }
    }

    // Ponteiro estático para a instância
    static EspNowReceiver *instance;
};

#endif
