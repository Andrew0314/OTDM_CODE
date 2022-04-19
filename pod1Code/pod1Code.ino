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

volatile msg package = {0,1};


int open_close_delay = 8000;

void setup() {
  setup_motor();
  Serial.begin(115200);
  while (!radio.begin()) {
    Serial.println("FUCK");
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
    openPod();
    closePod();
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
    msg temp_package;
    if (radio.available(&pipe)) {             // is there a payload? get the pipe number that recieved it
      radio.read(&temp_package, sizeof(msg));            // fetch payload from FIFO
      package.openSesimy = temp_package.openSesimy;
      package.ready2go = temp_package.ready2go;
      radio.stopListening();
  }
}

void transmitData(){
    msg temp_package;
    temp_package.openSesimy = package.openSesimy;
    temp_package.ready2go = package.ready2go;
    bool report = radio.write(&temp_package, sizeof(package));      // transmit & save the report
    radio.startListening();
}
