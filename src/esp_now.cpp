#include <Arduino.h>
#include "esp_now.h"
#include <espnow.h>

uint8_t broadcastAddress[] = {0x3C, 0x61, 0x05, 0xD0, 0xF3, 0xC4};

wifi_cresedentials credentials;

bool package_received = false;
unsigned long lastTime = 0;
//delay inbetween sending data
unsigned long timerDelay = 2000;

void setup_espnow()
{
    if (esp_now_init() != 0)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    else
    {
        Serial.println("");
        Serial.println("Successfully initalized ESP-Now");
    }

    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    esp_now_register_send_cb(OnDataSent);

    //Register receiver
    esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void send_data(String ssid, String password)
{
    if (package_received == false)
    {
        if ((millis() - lastTime) > timerDelay)
        {
            Serial.println("Trying to send data");
            // Set values to send
            strcpy(credentials.ssid, ssid.c_str());
            strcpy(credentials.pass, password.c_str());

            // Send message via ESP-NOW
            esp_now_send(broadcastAddress, (uint8_t *)&credentials, sizeof(credentials));

            lastTime = millis();
        }
    }
}

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus)
{
    if (sendStatus == 0)
    {
        Serial.println("Package succesfully send");
        package_received = true;
    }
    else
    {
        Serial.println("Sendig package failed");
    }
}