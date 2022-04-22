#include <heltec.h>
#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>

int L_EN = 4;
int R_EN = 5;
int L_PWM = 6;
int R_PWM = 7;


struct msg{
  int podNum;
  bool openSessimy;
  bool ready2go;
};

// INDIVIDUAL POD MESSAGE
msg pod = {2,0,1};

int open_close_delay = 10000;

int disp_led = 8;

void setup() {
  pinMode(disp_led,OUTPUT);
  Serial.begin(115200);
  setup_WIFI();
}

void loop() {
  if (pod.openSessimy){
    openPod();
    closePod();
  }else{
    stop_motor();
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
    pod.ready2go = true;
    pod.openSessimy = false;
    stop_motor();
    transmitData();
}
