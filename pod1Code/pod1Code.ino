#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>

//const int R_EN = 27;
//const int L_EN = 14;
const int L_PWM = 18;
const int R_PWM = 19;

struct msg{
  int podNum;
  bool openSessimy;
  bool ready2go;
};

// INDIVIDUAL POD MESSAGE
msg pod = {1,0,1};


int open_close_delay = 6000;

void setup() {
  Serial.begin(115200);
  setup_WIFI();
  setup_motor();
}

void loop() {
  if (pod.openSessimy){
    openPod();
    closePod();
  }
}

void openPod(){
    run_motor(-1,255);
    delay(open_close_delay);
    stop_motor();
}

void closePod(){
    run_motor(1,255);
    delay(open_close_delay);
    pod.openSessimy = false;
    pod.ready2go = true;
    stop_motor();
    transmitData();
}
