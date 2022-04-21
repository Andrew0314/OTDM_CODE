#include <heltec.h>


#include <esp_now.h>
#include <WiFi.h>

#include <Wire.h>


#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// REPLACE WITH THE MAC Address of your receiver 
uint8_t broadcastAddress[] = {0xC8, 0xC9, 0xA3, 0xD4, 0x3C, 0x74}; 


// Variable to store if sending data was successful
String success;

//Structure example to send data
//Must match the receiver structure
typedef struct msg {
  int podNum;
  int openSessimy;
  int ready2go;
} msg;

// Create a msg called BME280Readings to hold sensor readings
msg pod_msg = {0,0,0};

// Create a msg to hold incoming sensor readings
msg incomingReadings= {0,0,0};

esp_now_peer_info_t peerInfo;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
}
 int LED_PIN = 14;
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  

  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
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
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
  pinMode(LED_PIN,OUTPUT);
}
 bool sent = false;
void loop() {
  if (incomingReadings.ready2go){
    digitalWrite(LED_PIN,HIGH);
    Serial.println("READY");
    incomingReadings.ready2go = 0;
  }
  else{
   // digitalWrite(LED_PIN,LOW);
  }
  if (!sent){
      pod_msg.openSessimy = 1;
  pod_msg.ready2go = 0;
  pod_msg.podNum = 1;
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &pod_msg, sizeof(pod_msg));
  delay(2000);
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");

}
sent = true;
  }
  

}
