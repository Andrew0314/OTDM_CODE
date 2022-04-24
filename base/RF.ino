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
  esp_now_peer_info_t peerInfo;

  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // Register peer 1
  memcpy(peerInfo.peer_addr, pod_1_address, 6);

  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer 1");
    return;
  }
    memcpy(peerInfo.peer_addr, pod_2_address, 6);
    // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer 2");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
}


// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status_sent) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Sending to: ");
  Serial.print(macStr);
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status_sent == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  Serial.print("INCOMING DATA: ");
  memcpy(&incomingMsg, incomingData, sizeof(incomingMsg));
    Serial.print(incomingMsg.podNum);
    Serial.print("\tReady: ");
    Serial.print(incomingMsg.ready2go);
    Serial.print("\tOpen: ");
    Serial.println(incomingMsg.openSessimy);    
  if (incomingMsg.podNum == 1){
    pod1.podNum = incomingMsg.podNum;
    pod1.openSessimy = incomingMsg.openSessimy;
    pod1.ready2go = incomingMsg.ready2go;
    Serial.println("Recieved pod 1 message");
  }else if (incomingMsg.podNum == 2){
    pod2.podNum = incomingMsg.podNum;
    pod2.openSessimy = incomingMsg.openSessimy;
    pod2.ready2go = incomingMsg.ready2go;
    Serial.println("Recieved pod 2 message");
  }
}

void transmitData(int pod_number){
    esp_err_t result;
    if (pod_number == 1){
      pod1.openSessimy = true;
      pod1.ready2go = false;
      result = esp_now_send(pod_1_address, (uint8_t *) &pod1, sizeof(pod1));
      Serial.print("Attempting sending to pod 1: ");
    }else if ( pod_number == 2){
      pod2.openSessimy = true;
      pod2.ready2go = false;
      result = esp_now_send(pod_2_address, (uint8_t *) &pod2, sizeof(pod2));
      Serial.print("Attempting sending to pod 2: ");
    }
    if (result == ESP_OK){
        Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
}
