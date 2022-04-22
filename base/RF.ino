#include <heltec.h>
#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>


uint8_t pod_1_address[] = {0x34, 0x86, 0x5D, 0x3F, 0xDE, 0x00}; 
uint8_t pod_2_address[] = {0x30, 0xC6, 0xF7, 0x23, 0x87, 0x34}; 

void setup_WIFI(){
  WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
      Serial.println("Error initializing ESP-NOW");
      return;
  }
  
  esp_now_register_send_cb(OnDataSent);
  esp_now_peer_info_t peerInfo_1;
  esp_now_peer_info_t peerInfo_2;

  // Register peer 1
  memcpy(peerInfo_1.peer_addr, pod_1_address, 6);
  peerInfo_1.channel = 0;  
  peerInfo_1.encrypt = false;

    // Register peer 2
  memcpy(peerInfo_2.peer_addr, pod_2_address, 6);
  peerInfo_2.channel = 1;  
  peerInfo_2.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo_1) != ESP_OK){
    Serial.println("Failed to add peer 1");
    return;
  }
    // Add peer        
  if (esp_now_add_peer(&peerInfo_2) != ESP_OK){
    Serial.println("Failed to add peer 2");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
}


// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingMsg, incomingData, sizeof(incomingMsg));
  if (incomingMsg.podNum == 1){
    pod1 = incomingMsg;
    Serial.println("Recieved pod 1 message");
  }else if (incomingMsg.podNum == 2){
    pod2 = incomingMsg;
    Serial.println("Recieved pod 2 message");
  }
}

void transmitData(int pod_number){
    esp_err_t result;
    if (pod_number == 1){
      pod1.openSessimy = true;
      pod1.ready2go = false;
      result = esp_now_send(pod_1_address, (uint8_t *) &pod1, sizeof(pod1));
      Serial.println("Sending to pod 1");
    }else if ( pod_number == 2){
      pod2.openSessimy = true;
      pod2.ready2go = false;
      result = esp_now_send(pod_2_address, (uint8_t *) &pod2, sizeof(pod2));
      Serial.println("Sending to pod 2");
    }
    if (result == ESP_OK){
        Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
}
