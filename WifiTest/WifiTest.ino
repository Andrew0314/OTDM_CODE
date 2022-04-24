
#include <esp_now.h>
#include <WiFi.h>

#include <Wire.h>

//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels


// REPLACE WITH THE MAC Address of your receiver 
uint8_t broadcastAddress[] = {0x30, 0xC6, 0xF7, 0x14, 0xA5, 0x60}; 

// Define variables to store BME280 readings to be sent



String success;

//Structure example to send data
//Must match the receiver structure
typedef struct msg {
  int podNum;
  int openSessimy;
  int ready2go;
} msg;

// Create a msg called BME280Readings to hold sensor readings
msg pod_msg;

// Create a msg to hold incoming sensor readings
msg incomingReadings;

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
 int led_pin = 12;
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  pinMode(led_pin,OUTPUT);


  // Set device as a Wi-Fi Station
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
}
 
void loop() {

 if (incomingReadings.openSessimy == 1){
    digitalWrite(led_pin,HIGH);
      incomingReadings.openSessimy = 0;
      pod_msg.openSessimy = 0;
      pod_msg.ready2go = 1;
      pod_msg.podNum = 1;
      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &pod_msg, sizeof(pod_msg));
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  //updateDisplay();
  delay(10000);

}else{
    digitalWrite(led_pin,LOW);
}
  // Send message via ESP-NOW
}
   


//
//void updateDisplay(){
//  // Display Readings on OLED Display
//  display.clearDisplay();
//  display.setTextSize(1);
//  display.setTextColor(WHITE);
//  display.setCursor(0, 0);
//  display.println("INCOMING READINGS");
//  display.setCursor(0, 15);
//  display.print("Temperature: ");
//  display.print(incomingTemp);
//  display.cp437(true);
//  display.write(248);
//  display.print("C");
//  display.setCursor(0, 25);
//  display.print("Humidity: ");
//  display.print(incomingHum);
//  display.print("%");
//  display.setCursor(0, 35);
//  display.print("Pressure: ");
//  display.print(incomingPres);
//  display.print("hPa");
//  display.setCursor(0, 56);
//  display.print(success);
//  display.display();
//  
//  // Display Readings in Serial Monitor
//  Serial.println("INCOMING READINGS");
//  Serial.print("Temperature: ");
//  Serial.print(incomingReadings.temp);
//  Serial.println(" ºC");
//  Serial.print("Humidity: ");
//  Serial.print(incomingReadings.hum);
//  Serial.println(" %");
//  Serial.print("Pressure: ");
//  Serial.print(incomingReadings.pres);
//  Serial.println(" hPa");
//  Serial.println();
//}
