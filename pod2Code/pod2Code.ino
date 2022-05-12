#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
//const int R_EN = 27;
//const int L_EN = 14;
const int L_PWM = 18;
const int R_PWM = 19;
const int WAIT_FOR_LOAD = 2000;

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


// USE THIS DELAY IF delay DOESN't SEEM TO DELAY
void better_delay(unsigned long delay_time){
  noInterrupts();
    unsigned long start_time = millis();
    unsigned long t2 = millis();
    while ((t2-start_time) < delay_time){
      t2 = millis();
      //Serial.println(t2);
      //Serial.println(start_time);
    }
  interrupts();
}

void setup() {
  Serial.begin(115200);
  setup_WIFI();
  setup_motor();
}

void loop() {

  if (pod.openSessimy){
    openPod();
    Serial.println("Opened");
    better_delay(1000);
    closePod();
    Serial.println("Closed");
  }
}

void openPod(){
    Serial.println("Opening");
    run_motor(-1,255);
    better_delay(open_close_delay);
    stop_motor();
}

void closePod(){
    Serial.println("Closing");
    run_motor(1,255);
    better_delay(open_close_delay);
    pod.openSessimy = false;
    pod.ready2go = true;
    stop_motor();
    better_delay(WAIT_FOR_LOAD);
    transmitData();
}

//void all_combinations(){
//  Serial.println(10);
//  run_motor(1,0);
//  delay(6000);
//  Serial.println("-10");
//  run_motor(-1,0);
//  delay(6000);
//  Serial.println("1255");
//  run_motor(1,255);
//  delay(6000);
//  Serial.println("-1255");
//  run_motor(-1,255);
//  delay(6000);
//}
//
//
//void debug_hbridge(){
//  
//  //Serial.print("DIR: ");
//  //Serial.println(dir);
//  //Serial.println(pwm);
//    
//   
//    if(Serial.available()>0){
//    if(dir = 1){
//      dir = -1;
//    }else if (dir = -1){
//      dir = 1;
//    }
//    sp=Serial.parseInt();
//    run_motor(1, sp);
//
//    Serial.print("data: ");
//    Serial.println(sp);
//   
//  
//  }
//}
