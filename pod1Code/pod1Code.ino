#include <SPI.h>
#include "printf.h"
#include "RF24.h"


RF24 radio(9, 10); 

int L_EN = 4;
int R_EN = 5;
int L_PWM = 6;
int R_PWM = 7;

uint8_t receive_address[6] = "3Node";
uint8_t send_address[6] = "4Node";


struct msg{
  bool openSesimy;
  bool ready2go;
};

msg package = {0,1};


int open_close_delay = 10000;

void setup() {

  Serial.begin(115200);
  while (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
  }
 
  radio.setPALevel(RF24_PA_LOW);
   
  radio.openReadingPipe(1, receive_address); 
  radio.openWritingPipe(send_address);
  radio.startListening();  // put radio in TX mode
  radio.maskIRQ(1,1,0);
  attachInterrupt(digitalPinToInterrupt(2), recieveData, FALLING); // ENSURE INTERRUPT IS WORKING   
}

void loop() {
  if (package.openSesimy){
    //noInterrupts();
    openPod();
    closePod();
    //interrupts();
  }
}

void openPod(){
    run_motor(1,255);
    delay(open_close_delay);
    stop_motor();
}

void closePod(){
    run_motor(-1,255);
    delay(open_close_delay);
    package.openSesimy = false;
    package.ready2go = true;
    stop_motor();
    transmitData();
}


void recieveData(){
    uint8_t pipe;
    if (radio.available(&pipe)) {             // is there a payload? get the pipe number that recieved it
      radio.read(&package, sizeof(msg));            // fetch payload from FIFO
  }
}

void transmitData(){
    radio.stopListening();  // put radio in TX mode

     bool report = radio.write(&package, sizeof(package));      // transmit & save the report


    if (report) {
      Serial.print(F("Transmission successful! "));          // payload was delivered

    } else {
      Serial.println(F("Transmission failed or timed out")); // payload was not delivered

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
