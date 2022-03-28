// CONFIG PARAMS
const int OPEN_CLOSE_DELAY = 5000;

// LED PINS
int receive_led_blue = 7;
int send_led_red = 5;
int running_led_green = 6;

// RGB PINS
int R_pin = 35;
int G_pin = 33;
int B_pin = 34; 

// IR REMOTE PIN
const int REMOTE_PIN = 28;

// MOTOR PINS
const int R_EN = 40;
const int L_EN = 41;
const int L_PWM = 45;
const int R_PWM = 46;

// POTENTIOMETER PIN
#define pot_pin A0

// DIRECTION SWITCH PINS
int forward_pin = 22;
int reverse_pin = 24;

// RF PINS
int CE = 9;
int CSN = 10;

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
int pwm, dir;   // FINAL VALUES


// PREVIOUS BASE READINGS
int old_motor_pwm_base, old_dir_base; 


void setup() {
  Serial.begin(115200);
  setup_remote(REMOTE_PIN);
  setup_RF();
  setup_encoder();
  setup_LED();
  setup_input();
}

void loop() {
  print_pod_status();
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
  int motor_pwm_base = get_speed_value();
  int cur_dir = get_direction();
  
  // If base command changes, execute and overwrite remote
  if (motor_pwm_base != old_motor_pwm_base){ 
    pwm = motor_pwm_base;    
  }

  if (cur_dir != old_dir_base){
    dir = cur_dir;
  }
  
  old_motor_pwm_base = motor_pwm_base;
  old_dir_base = cur_dir;

  // IF PODS ARE CLOSED AND READY RUN MOTOR AT DIRECTION AND PWM
  if (pod1.ready2go && pod2.ready2go){
    run_motor(dir,pwm);    
  }else{
    stop_motor();
  }
  delay(200);
}
