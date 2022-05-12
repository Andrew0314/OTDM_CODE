//#include <AutoPID.h>
#include <heltec.h>
#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <ESP32Encoder.h>

ESP32Encoder encoder;
// CONFIG PARAMS
const int OPEN_CLOSE_DELAY = 20000;
bool test_door_open = false;
bool run_with_encoder = true;
bool run_with_pods = true;
bool run_with_slowdown = true;



// LED PINS
int receive_led_blue = 17;
int send_led_red = 22;
int running_led_green = 23;

// RGB PINS
int R_pin = 5; // 
int G_pin = 15; //
int B_pin = 12; 


// MOTOR PINS
const int L_PWM = 26;
const int R_PWM = 25;

// POTENTIOMETER PIN
#define pot_pin 33

// DIRECTION SWITCH PINS
int forward_pin = 14;
int reverse_pin = 27;


// INTERRUPT PINS

int encoderA_pin = 18;
int encoderB_pin = 19;

// RF MESSAGES TO PODS
struct msg{
  int podNum;
  bool openSessimy;
  bool ready2go;
};

// INDIVIDUAL POD MESSAGE
msg incomingMsg = {0,0,0};
volatile msg pod1 = {1,0,1};
volatile msg pod2 = {2,0,1};


// MOTOR STATUS
bool motor_running = true;
int dir;
float rpm;

// MOTOR PARAMS
int motor_deadband = 10;
int max_speed = 2; // ft/s

// PID VALUES
//double speed_setpoint;
double unfiltered_speed_current;
//double speed_current;
int pwm;   // FINAL VALUES

// PID CONSTANTS
//double kp = 2000;
//double ki = 0;
//double kd = 0;

// RUNNING PARAMETERS
float feet_per_transit = 25.1875;
float pre_stop_slowdown = feet_per_transit - 7.0;
float post_stop_slowdown = 3.0;
int slowdown_pwm = 100; 

int slowdown_steps = 10;
int slowdown_delay = 100;
double stop_tolerance = .1; //ft when to stop
bool in_slowdown = false;



//int pid_timestep = 20;

//AutoPID pid(&speed_current, &speed_setpoint, &pwm, motor_deadband, 255.0, kp,ki,kd);

// PREVIOUS BASE READINGS
double old_motor_pwm_base, old_dir_base; 


int prev_dir = 0;
int prev_pwm = 0;

// LEARNED:
// use better delay in encoder file for delays that don't work

// TODO:
// tune pid
// add ultrasonic/ir sensor for pod location verification
// add gradual slowdown it is very abrupt
// test and implement with pods ie waiting for pod to respond back. (I believe it should work out the box but needs test)

// ONLY HAVE CODE FOR NO PODS TESTED
// TO IMPLEMENT PODS I BELIEVE YOU WILL ONLY NEED TO EDIT HANDLE POD LOCATION FUNCTION IN ENCODER FILE


// PROBLEMS:
// NANO NOT SENDING READY2go back
// BLUE RGB LED DOESN'T WORK THINK IT JUST DOESN'T WORK ON PIN 13

// PWM on ESP32 is different

void setup() {
  Serial.begin(115200);
  setup_WIFI();
  setup_encoder();
  setup_motor();
  setup_input();
  setup_LED();
  delay(1000);



}

void loop() {

  
  // DEBUG PRINT STATMENTS
  //print_pod_status(1);
  //print_pot();
  //print_motor_speed();
  //print_pod_location(1);
  //print_pod_location(2);
  //print_motor_dir();
  //debug_encoder();
  //debug_hbridge(true);
  //plot_rpm();
  //print_pid();
  //calculate_motor_speed();        // Calculates motor speed from encoder to speed_current variable

  
  // LOGIC FOR CONFIG PARAMS 
  if (test_door_open){
    test_door();
  }else{
    get_speed_value();              // Reads potentiometer into speed_setpoint
    get_direction();                // Reads switch
    handle_pod_location();          // Nested if statement to see where pods is and when to stop/slowdown
    if (run_with_pods){
      // IF PODS ARE CLOSED AND READY RUN MOTOR AT DIRECTION AND PWM
      Serial.print("POD! READY @ GO:");
      Serial.println(pod1.ready2go);
      if (pod1.ready2go && pod2.ready2go){
        Serial.println("Running Motor");
        run_motor(dir,pwm);    
      }else{
        stop_motor();
      }
    }else{
      run_motor(dir,pwm);
    }
  }
    //delay(20);
}


void test_door(){
  // WORKING RECIEVE SEEMS THAT NOT USING DIGITAL PIN TO INTERRUPT WORKED
  // POWER SUPPLY STOPPED WORKING WHEN POWERING NRF ONLY COMP WORKS NOW
    
    get_speed_value();              // Reads potentiometer into speed_setpoint
    get_direction();                // Reads switch
    if (dir==0){
      stop_motor();
      
      delay(5000);
   
    pod1.openSessimy = 1;
    pod2.openSessimy = 1;

    
    transmitData(1);
    Serial.println("SENDING:");
    print_pod_status(1);
    while(!pod1.ready2go){ }
    Serial.println("HOPEFULLY RECIEVED:");
    print_pod_status(1);
    if (pod1.ready2go){
      digitalWrite(running_led_green,HIGH);
    }
    delay(2000);
    transmitData(2);
    Serial.println("SENDING:");
    print_pod_status(2);
    while(!pod2.ready2go){ }
    Serial.println("HOPEFULLY RECIEVED:");
    print_pod_status(2);
    if (pod2.ready2go){
      RGB_LED(0,255,0);
    }
    
    delay(10000);
    }
    else{
    run_motor(dir,pwm);
    delay(1000);
    }

}
