/*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/
#include <esp_now.h>
#include <WiFi.h>
#include "espdata.h"
#include "ControlerConfig.h"
#include "config.h"
#include "screen.h"
#include <esp_wifi.h>

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xe0, 0x5a, 0x1b, 0xd2, 0x41, 0x84};

// Structure example to send data
// Must match the receiver structure

// Create a struct_message called myData
espdata myData;

esp_now_peer_info_t peerInfo;

screen tela;
int index_vert = 1;
int index_horizontal = 1;
uint8_t baseMac[6];

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    // Serial.print("\r\nLast Packet Send Status:\t");
    // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup()
{
    // Init Serial Monitor
    Serial.begin(115200);
    tela.begin();
    pinMode(2, OUTPUT);

    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Once ESPNow is successfully Init, we will register for Send CB to
    // get the status of Trasnmitted packet
    esp_now_register_send_cb(OnDataSent);

    // Register peer
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    // Add peer
    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("Failed to add peer");
        return;
    }
    esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
}

void loop()
{
    getControlerData();
    myData = data2send;

    if (myData.arrow_data.up)
    {
        index_vert = index_vert - 1;
    }
    if (myData.arrow_data.down)
    {
        index_vert = index_vert + 1;
    }
    if (myData.arrow_data.left)
    {
        index_horizontal = index_horizontal - 1;
    }
    if (myData.arrow_data.right)
    {
        index_horizontal = index_horizontal + 1;
    }

    // Set values to send
    // debug();

    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));

    Serial.print(index_vert);
    Serial.print("  ");
    Serial.println(index_horizontal);

    if (index_horizontal == 1)
        tela.main_menu(index_vert);
    if (index_horizontal == 0)
        tela.test(data2send);
    if (index_horizontal == 2)
    {
        tela.conect_menu(baseMac);
    }
    if (index_horizontal == 3)
    {
        tela.info_screen(25, 75, 32, 42, 66);
    }
    tela.update();

    if (index_horizontal > 3)
    {
        index_horizontal = 0;
    }
    if (index_horizontal < 0)
    {
        index_horizontal = 3;
    }

    if (index_vert > 2)
    {
        index_vert = 0;
    }
    if (index_vert < 0)
    {
        index_vert = 2;
    }

    if (result == ESP_OK)
    {
        digitalWrite(2, HIGH);
    }
    else
    {
        digitalWrite(2, LOW);
    }

    delay(60);
}