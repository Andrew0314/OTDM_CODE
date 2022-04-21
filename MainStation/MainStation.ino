#include <heltec.h>

/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-two-way-communication-esp32/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <esp_now.h>
#include <WiFi.h>

#include <Wire.h>


#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)

//Adafruit_BME280 bme;


//C8:C9:A3:D4:3C:74
// REPLACE WITH THE MAC Address of your receiver 
uint8_t broadcastAddress[] = {0xC8, 0xC9, 0xA3, 0xD4, 0x3C, 0x74}; 

// Define variables to store BME280 readings to be sent


// Define variables to store incoming readings
int incoming_openSessimy;
int incoming_ready2go;
int incoming_podNum;
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
  incoming_openSessimy = incomingReadings.openSessimy;
  incoming_ready2go = incomingReadings.ready2go;
  incoming_podNum = incomingReadings.podNum;
}
 int led_pin = 12;
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  pinMode(led_pin,OUTPUT);

  Heltec.begin(true, false, true);
  Heltec.display->setContrast(255);
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0, 0, "Hello world"); //this is of course one of the main methods, writing text on the screen

Heltec.display->setColor(WHITE); //color to use
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
  pod_msg.openSessimy = 1;
  pod_msg.ready2go = 0;
  pod_msg.podNum = 1;
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &pod_msg, sizeof(pod_msg));
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");

  // Send message via ESP-NOW
}
// updateDisplay();  
}


//void updateDisplay(){
//  // Display Readings on OLED Display
//  display.clearDisplay();
//  display.setTextSize(1);
//  display.setTextColor(WHITE);
//  display.setCursor(0, 0);
//  display.println("INCOMING READINGS");
//  display.setCursor(0, 15);
//  display.print("Pod Number: ");
//  display.print(incoming_podNum);
//  display.cp437(true);
//  display.write(248);
//  display.print("C");
//  display.setCursor(0, 25);
//  display.print("Ready2go: ");
//  display.print(incoming_ready2go);
//  display.print("%");
//  display.setCursor(0, 35);
//  display.print("OpenSessimy: ");
//  display.print(incoming_openSessimy);
//  display.print("hPa");
//  display.setCursor(0, 56);
//  display.print(success);
//  display.display();
//  
//}
