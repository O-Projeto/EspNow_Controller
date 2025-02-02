// ESPCommunicator.h
#ifndef ESPCOMMUNICATOR_H
#define ESPCOMMUNICATOR_H

#include <esp_now.h>
#include <WiFi.h>
#include "espdata.h"

class ESPCommunicator
{
private:
    uint8_t peerMAC[6]; // Endereço MAC do peer
    static void (*receiveCallback)(const uint8_t *, const espdata &);

    static void onReceive(const uint8_t *macAddr, const uint8_t *data, int len)
    {
        if (len == sizeof(espdata))
        {
            espdata receivedData;
            memcpy(&receivedData, data, sizeof(espdata));
            if (receiveCallback)
            {
                receiveCallback(macAddr, receivedData);
            }
        }
        else
        {
            Serial.println("Dados recebidos de tamanho incorreto.");
        }
    }

public:
    ESPCommunicator() {}

    void begin(const uint8_t *peerMACAddr)
    {
        memcpy(peerMAC, peerMACAddr, 6);

        WiFi.mode(WIFI_STA);
        if (esp_now_init() != ESP_OK)
        {
            Serial.println("Erro ao inicializar ESP-NOW");
            return;
        }

        esp_now_register_recv_cb(onReceive);

        esp_now_peer_info_t peerInfo = {};
        memcpy(peerInfo.peer_addr, peerMAC, 6);
        peerInfo.channel = 0;
        peerInfo.encrypt = false;

        if (esp_now_add_peer(&peerInfo) != ESP_OK)
        {
            Serial.println("Erro ao adicionar peer");
        }
    }

    void send(const espdata &data)
    {
        esp_err_t result = esp_now_send(peerMAC, (uint8_t *)&data, sizeof(data));
        if (result == ESP_OK)
        {
            Serial.println("Dados enviados com sucesso.");
        }
        else
        {
            Serial.printf("Erro ao enviar dados: %d\n", result);
        }
    }

    void setReceiveCallback(void (*callback)(const uint8_t *, const espdata &))
    {
        receiveCallback = callback;
    }
};

#endif
