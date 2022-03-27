

#include <SPI.h>
#include "printf.h"
#include "RF24.h"

// instantiate an object for the nRF24L01 transceiver
RF24 radio(9, 10); // using pin 7 for the CE pin, and pin 8 for the CSN pin

volatile bool encoder_direction = false; // true is cw false is ccw
volatile int encoder_ticks = 0;
volatile int reverse_ticks = 0;
int max_reverse_ticks = 2000;
bool reverse_flag = false;

uint8_t pod1_send_address[6] = "3Node";
uint8_t pod1_receive_address[6] = "4Node";
uint8_t pod2_send_address[6] = "1Node";
uint8_t pod2_receive_address[6] = "2Node";

int blue = 8;
int red = 7;
int green = 6;

struct msg{
  bool openSesimy;
  bool ready2go;
  //accleronmeter
  //check if pod is finished 
};

msg pod1 = {0,1};
msg pod2 = {0,1};

#define REMOTE_PIN 28


void setup() {

  Serial.begin(115200);
  setup_remote(REMOTE_PIN);
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

   pinMode(blue, OUTPUT);
   pinMode(green, OUTPUT);
   pinMode(red, OUTPUT);


   
   radio.maskIRQ(1,1,0);
   attachInterrupt(digitalPinToInterrupt(18), recieveData, FALLING);


   pinMode(2,INPUT_PULLUP);
   pinMode(3,INPUT_PULLUP);
   attachInterrupt(digitalPinToInterrupt(2), encoderA, RISING);
   attachInterrupt(digitalPinToInterrupt(3), encoderB, RISING);

  
   
}

void loop() {
  digitalWrite(green,HIGH);
 



  
//  pod1.openSesimy = 1;
//  transmitData(1);
//  delay(1000);
//  pod2.openSesimy = 1;
//  transmitData(2);
//  delay(1000);
//  
//

}



void recieveData(){
    digitalWrite(red,LOW);
    uint8_t pipe;
    if (radio.available(&pipe)) {             // is there a payload? get the pipe number that recieved it 
      if (pipe == 1){
        radio.read(&pod1, sizeof(msg));            // fetch payload from FIFO
      }
      if (pipe == 2){
        radio.read(&pod2, sizeof(msg));            // fetch payload from FIFO
      }

      digitalWrite(red,HIGH);
      flashlight(blue); 
          digitalWrite(green,HIGH);
          
  }
}

void transmitData(int pod_number){
    msg package; 
    if (pod_number == 1){
      radio.openWritingPipe(pod1_send_address);
      package = pod1;
    }else if ( pod_number == 2){
      radio.openWritingPipe(pod2_send_address);
      package = pod2;
    }
    radio.stopListening();  // put radio in TX mode
    bool report = radio.write(&package, sizeof(package));      // transmit & save the report
    
    if (report) {
      flashlight(blue);
    } else {
      flashlight(red);
    }
    radio.startListening();
}


void flashlight(int x){
      digitalWrite(x, HIGH);
      delay (200);
      digitalWrite(x, LOW);
      delay(200);
      digitalWrite(x, HIGH);
      delay (200);
      digitalWrite(x, LOW);
      delay(200);

}


float ticks_per_rev = 600;
float distance_per_rev = 14 * 3.1415 / 12; // feet
float rev_per_transit = 5 / distance_per_rev;
int ticks_per_transit = ticks_per_rev * rev_per_transit;



void encoderA(){
  if (!digitalRead(3) && digitalRead(2)){
    encoder_direction = true;
    encoder_ticks ++;
    reverse_ticks = 0;
  }

  if (encoder_ticks == ticks_per_transit){
    pod1.openSesimy = 1;
    transmitData(1);
    digitalWrite(green,LOW);
    


  }else if (encoder_ticks == ticks_per_transit * 2){

    pod2.openSesimy = 1;
     transmitData(2);
      digitalWrite(green,LOW);
    encoder_ticks = 0;
  }
  
}

void encoderB(){
    if (digitalRead(3) && !digitalRead(2)){
    encoder_direction = false;
    reverse_ticks++;
    if (reverse_ticks == max_reverse_ticks){
      reverse_flag = true;
    }
  }
}
