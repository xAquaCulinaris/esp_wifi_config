#include <Arduino.h>

typedef struct wifi_cresedentials {
  char ssid[32];
  char pass[32];

} wifi_cresedentials;

void setup_espnow();
void send_data(String ssid, String password);
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus);