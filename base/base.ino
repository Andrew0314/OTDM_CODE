#include <AutoPID.h>

// CONFIG PARAMS
const int OPEN_CLOSE_DELAY = 5000;
bool test_door_open = true;
bool run_with_encoder = false;
bool run_with_pid = false;
bool run_with_pods = false;
bool run_with_slowdown = false;

// LED PINS
int receive_led_blue = 8;
int send_led_red = 9;
int running_led_green = 10;

// RGB PINS
int R_pin = 11;
int G_pin = 12;
int B_pin = 13; 

// IR REMOTE PIN
const int REMOTE_PIN = 22;

// MOTOR PINS
const int R_EN = 42;
const int L_EN = 43;
const int L_PWM = 41;
const int R_PWM = 40;

// POTENTIOMETER PIN
#define pot_pin A0

// DIRECTION SWITCH PINS
int forward_pin = 26;
int reverse_pin = 28;

// RF PINS
int CE = 5;
int CSN = 4;

// INTERRUPT PINS

int encoderA_pin = 18;
int encoderB_pin = 19;
int rf_int_pin = 3;

// RF MESSAGES TO PODS
struct msg{
  bool openSesimy;
  bool ready2go;
};

// INDIVIDUAL POD MESSAGE
msg pod1 = {0,1};
msg pod2 = {0,1};

// OUTPUT FROM REMOTE
struct remote_output{
  bool new_data = false;
  bool do_pod_stuff = false;
  int  motor_pwm_remote = 0;
  int  dir_remote = 0;
  int  openSesimy = 0;
  int  pod_number = 0;  
};

// MOTOR STATUS
bool motor_running = false;
double pwm;   // FINAL VALUES
int dir;

// PID values
double speed_setpoint;
double speed_current;

double motor_deadband = 60;
double max_speed = 2; // ft/s
double slowdown_tolerance = 3; // ft
double slowdown_speed = .25; // ft/s
bool in_slowdown = false;
double kp = 1;
double ki = 1;
double kd = 1;

int pid_timestep = 200;

AutoPID pid(&speed_current, &speed_setpoint, &pwm, motor_deadband, 255.0, kp,ki,kd);

// PREVIOUS BASE READINGS
double old_motor_pwm_base, old_dir_base; 


void setup() {
  Serial.begin(115200);
  setup_remote(REMOTE_PIN);
  setup_RF();
  setup_encoder();
  setup_LED();
  setup_input();
  setup_pid();
  all_lights();
  delay(2000);
  all_lights_off();
}

void loop() {
  print_pod_status();
  //print_pot();
//  // REMOTE INPUTS
//  remote_output remote_vals = get_remote_input();
// 
//  if (remote_vals.new_data){  // If there is new data from the remote set values
//   
//    if (remote_vals.motor_pwm_remote != 0){
//      pwm = remote_vals.motor_pwm_remote;        
//    }
//    if (    remote_vals.dir_remote != 0){
//      dir = remote_vals.dir_remote;      
//    }
//    
//    // TRANSMIT POD OPEN AND CLOSE
//    if (remote_vals.do_pod_stuff){
//      stop_motor();
//      if (remote_vals.pod_number == 1){
//        pod1.openSesimy = remote_vals.openSesimy;
//        transmitData(1);
//      }else if (remote_vals.pod_number == 2){
//        pod2.openSesimy = remote_vals.openSesimy;  
//        transmitData(2);
//      }
//      delay(OPEN_CLOSE_DELAY);
//    }
// }
    
  // BASE CONTROL TAKES PRIORITY OVER REMOTE
  get_speed_value();
  int cur_dir = get_direction();
  //run_motor(cur_dir,motor_pwm_base);
  // If base command changes, execute and overwrite remote
  if (speed_setpoint != old_motor_pwm_base){ 
    pwm = speed_setpoint;    
  }

  if (cur_dir != old_dir_base){
    dir = cur_dir;
  }

  
  old_motor_pwm_base = speed_setpoint;
  old_dir_base = cur_dir;
  
  assign_motor_pwm();
  if (test_door_open){
    stop_motor();
    for (int i = 0; i < 3; i++){
      pod1.openSesimy = 1;
      transmitData(1);
      pod2.openSesimy = 1;
      transmitData(2);
      delay(30000);          
    }
  }else{
    if (run_with_pods){
      // IF PODS ARE CLOSED AND READY RUN MOTOR AT DIRECTION AND PWM
      if (pod1.ready2go){// && pod2.ready2go){
        run_motor(dir,pwm);    
      }else{
        stop_motor();
      }
    }else{
      run_motor(dir,pwm);
    }
  delay(200);
  }
}
