#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>

int SENSOR_PIN = 25;
volatile bool motion = false;
uint8_t receiverAddress[] = {0x7c, 0x9e, 0xbd, 0xe3, 0x68, 0x8c};
const char* SSID = "ZTE-HDCTCY";

void IRAM_ATTR detected()
{
  motion = true;
}

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

int32_t getWiFiChannel(const char *ssid) {
  if (int32_t n = WiFi.scanNetworks()) {
    for (uint8_t i=0; i<n; i++) {
      if (!strcmp(ssid, WiFi.SSID(i).c_str())) {
        return WiFi.channel(i);
      }
    }
  }
  return 0;
}

void setup()
{

  // --- ESP-NOW initialization
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  int32_t channel = getWiFiChannel(SSID);
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Transmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // ---

  // --- Sensor initialization
  pinMode(SENSOR_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), detected, RISING);
  Serial.begin(115200);
  // ---
}

void loop()
{
  if (motion)
  {
    uint8_t data = 1;
    esp_err_t result = esp_now_send(receiverAddress, &data, sizeof(data));
    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
    motion = false;
  }
  delay(1000);
}
