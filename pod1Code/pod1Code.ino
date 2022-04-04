

#include <SPI.h>
#include "printf.h"
#include "RF24.h"

// instantiate an object for the nRF24L01 transceiver
RF24 radio(9, 10); // using pin 7 for the CE pin, and pin 8 for the CSN pin


int L_EN = 4;
int R_EN = 5;
int L_PWM = 6;
int R_PWM = 7;

uint8_t receive_address[6] = "3Node";
uint8_t send_address[6] = "4Node";

int blue = 8;
int red = 7;
int green = 8;

struct msg{
  bool openSesimy;
  bool ready2go;
  //accleronmeter
  //check if pod is finished 
};

msg package = {0,1};




void setup() {

  Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {} // hold in infinite loop
  }else{
    Serial.println("RADIO GOOD");
  }
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);
  delay(4000);
  digitalWrite(LED_BUILTIN,LOW);
  radio.setPALevel(RF24_PA_LOW);
   
   radio.openReadingPipe(1, receive_address); 
   radio.openWritingPipe(send_address);
   radio.startListening();  // put radio in TX mode

   pinMode(blue, OUTPUT);
   pinMode(green, OUTPUT);
   pinMode(red, OUTPUT);


   
    radio.maskIRQ(1,1,0);
    attachInterrupt(digitalPinToInterrupt(2), recieveData, FALLING);

  
   
}

void loop() {

  if (package.openSesimy == 1){
    openPod();
    delay(1000);
    closePod();
  }


}

void openPod(){
    digitalWrite(LED_BUILTIN, HIGH);
    run_motor(1,255);
    delay(10000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
    stop_motor();
}

void closePod(){
    digitalWrite(LED_BUILTIN, HIGH);
    run_motor(-1,255);
    delay(10000);
    digitalWrite(LED_BUILTIN, LOW);
    package.openSesimy = false;
    package.ready2go = true;
    transmitData();
    stop_motor();
    
}


void recieveData(){
    uint8_t pipe;
    if (radio.available(&pipe)) {             // is there a payload? get the pipe number that recieved it
      //uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
      radio.read(&package, sizeof(msg));            // fetch payload from FIFO
      Serial.print(F("Received "));
      //Serial.print(bytes);                    // print the size of the payload
      Serial.print(F(" bytes on pipe "));
      Serial.print(pipe);                     // print the pipe number
      Serial.print(F(": "));
      Serial.println(package.openSesimy);// print the payload's value
      digitalWrite(red,HIGH);
      flashlight(blue); 
          
  }
}

void transmitData(){
    radio.stopListening();  // put radio in TX mode
    unsigned long start_timer = micros();                    // start the timer
     bool report = radio.write(&package, sizeof(package));      // transmit & save the report
    unsigned long end_timer = micros();                      // end the timer

    if (report) {
      Serial.print(F("Transmission successful! "));          // payload was delivered
      Serial.print(F("Time to transmit = "));
      Serial.print(end_timer - start_timer);                 // print the timer result
      Serial.print(F(" us. Sent: "));

      Serial.println(package.openSesimy);                               // print payload sent
                                            // increment float payload
      flashlight(blue);
    } else {
      Serial.println(F("Transmission failed or timed out")); // payload was not delivered
      flashlight(red);
    }
    radio.startListening();
}


void sendflag(){
  
    radio.stopListening();  // put radio in TX mode
    unsigned long start_timer = micros();  
    bool x = true;
    
    // start the timer
    bool report = radio.write(&x, sizeof(x));      // transmit & save the report
    unsigned long end_timer = micros();                      // end the timer

    if (report) {
      Serial.print(F("Transmission successful! "));          // payload was delivered
      Serial.print(F("Time to transmit = "));
      Serial.print(end_timer - start_timer);                 // print the timer result
      Serial.print(F(" us. Sent: "));
      Serial.println(x);                               // print payload sent                                  // increment float payload
      flashlight(blue);
    } else {
      Serial.println(F("Transmission failed or timed out")); // payload was not delivered
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
