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
msg pod = {2,0,1};

int open_close_delay = 6000;

int dir = 1;
int sp = 0;

void setup() {
  Serial.begin(115200);
  setup_WIFI();
  setup_motor();
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
    run_motor(-1,255);
    delay(open_close_delay);
    stop_motor();
}

void closePod(){
    run_motor(1,255);
    delay(open_close_delay);
    pod.ready2go = true;
    pod.openSessimy = false;
    stop_motor();
    transmitData();
}

void all_combinations(){
  Serial.println(10);
  run_motor(1,0);
  delay(6000);
  Serial.println("-10");
  run_motor(-1,0);
  delay(6000);
  Serial.println("1255");
  run_motor(1,255);
  delay(6000);
  Serial.println("-1255");
  run_motor(-1,255);
  delay(6000);
}


void debug_hbridge(){
  
  //Serial.print("DIR: ");
  //Serial.println(dir);
  //Serial.println(pwm);
    
   
    if(Serial.available()>0){
    if(dir = 1){
      dir = -1;
    }else if (dir = -1){
      dir = 1;
    }
    sp=Serial.parseInt();
    run_motor(1, sp);

    Serial.print("data: ");
    Serial.println(sp);
   
  
  }
}
