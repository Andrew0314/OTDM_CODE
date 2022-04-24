#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>

uint8_t base_addr[] = {0x30, 0xC6, 0xF7, 0x14, 0xA5, 0x60}; 

void setup_WIFI(){
  WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
      Serial.println("Error initializing ESP-NOW");
      return;
  }
  
  esp_now_register_send_cb(OnDataSent);
  esp_now_peer_info_t peerInfo;

  // Register peer 1
  memcpy(peerInfo.peer_addr, base_addr, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
}


// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Sending to: ");
  Serial.print(macStr);
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&pod, incomingData, sizeof(pod));
   Serial.print("RECIEVED:\t"); 
  Serial.print("OPEN SESSIMY: ");
  Serial.print(pod.openSessimy);
  Serial.print("\tREADY: ");
  Serial.println(pod.ready2go);
}

void transmitData(){
  
  esp_err_t result;
  result = esp_now_send(base_addr, (uint8_t *) &pod, sizeof(pod));
  Serial.print("Sending to base:\t");
  if (result == ESP_OK){
    Serial.println("Sent with success");
    Serial.print("SENT:\t");
    Serial.print("OPEN SESSIMY: ");
    Serial.print(pod.openSessimy);
    Serial.print("\tREADY: ");
    Serial.println(pod.ready2go);
  }else{
    Serial.println("Error sending the data");
  }

}
