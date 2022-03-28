#include <SPI.h>
#include "printf.h"
#include "RF24.h"

RF24 radio(CE, CSN); 
uint8_t pod1_send_address[6] = "3Node";
uint8_t pod1_receive_address[6] = "4Node";
uint8_t pod2_send_address[6] = "1Node";
uint8_t pod2_receive_address[6] = "2Node";

void setup_RF(){
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {} // hold in infinite loop
  }
   radio.setPALevel(RF24_PA_LOW);
   radio.openReadingPipe(1, pod1_receive_address); 
   radio.openReadingPipe(2, pod2_receive_address); 
   radio.openWritingPipe(pod1_send_address);
   radio.openWritingPipe(pod2_send_address);
   radio.startListening();  // put radio in TX mode
   radio.maskIRQ(1,1,0);
   attachInterrupt(digitalPinToInterrupt(18), recieveData, FALLING);
   digitalWrite(receive_led_blue,HIGH);
}


void recieveData(){
    
    uint8_t pipe;
    if (radio.available(&pipe)) {             // is there a payload? get the pipe number that recieved it 
      if (pipe == 1){
        radio.read(&pod1, sizeof(msg));            // fetch payload from FIFO
      }
      if (pipe == 2){
        radio.read(&pod2, sizeof(msg));            // fetch payload from FIFO
      }
      flashlight(receive_led_blue);      
    }
}

void transmitData(int pod_number){
  
    digitalWrite(send_led_red,HIGH);
    msg package; 
    if (pod_number == 1){
      radio.openWritingPipe(pod1_send_address);
      package = pod1;
      pod1.ready2go = false;
    }else if ( pod_number == 2){
      radio.openWritingPipe(pod2_send_address);
      package = pod2;
      pod2.ready2go = false;
    }
    radio.stopListening();  // put radio in TX mode
    bool report = radio.write(&package, sizeof(package));      // transmit & save the report
    
    if (report) {
      flashlight(send_led_red);
    } else {
      digitalWrite(send_led_red,LOW);
    }
    radio.startListening();
}
