// CONFIG PARAMS
const int OPEN_CLOSE_DELAY = 5000;

// LED PINS
int blue = 8;
int red = 7;
int green = 6;

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
int forward_pin = 31;
int reverse_pin = 32;

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
  int motor_pwm_remote = 0;
  int dir_remote = 0;
  int openSesimy = 0;
  int pod_number = 0;  
};

// MOTOR STATUS
bool motor_running = false;
int pwm, dir;   // FINAL VALUES

void setup() {

  Serial.begin(115200);
  setup_remote(REMOTE_PIN);
  setup_RF();
  setup_encoder();
  setup_LED();
}

void loop() {
  // PREVIOUS BASE READINGS
  int old_motor_pwm_base, old_dir_base; 
  
  // REMOTE INPUTS
  remote_output remote_vals = get_remote_input();
 
  if (remote_vals.new_data){  // If there is new data from the remote set values
   
    if (remote_vals.motor_pwm_remote != 0){
      pwm = remote_vals.motor_pwm_remote;        
    }
    if (    remote_vals.dir_remote != 0){
      dir = remote_vals.dir_remote;      
    }
    
    // TRANSMIT POD OPEN AND CLOSE
    if (remote_vals.do_pod_stuff){
      stop_motor();
      if (remote_vals.pod_number == 1){
        pod1.openSesimy = remote_vals.openSesimy;
        transmitData(1);
      }else if (remote_vals.pod_number == 2){
        pod2.openSesimy = remote_vals.openSesimy;  
        transmitData(2);
      }
      delay(OPEN_CLOSE_DELAY);
    }
 }

  // BASE CONTROL TAKES PRIORITY OVER REMOTE
  int motor_pwm_base = get_speed_value();
  int dir_base = get_direction();
  
  // If base command changes, execute and overwrite remote
  if (motor_pwm_base != old_motor_pwm_base){ 
    pwm = motor_pwm_base;    
  }

  if (dir_base != old_dir_base){
    dir = dir_base;
  }
  
  old_motor_pwm_base = motor_pwm_base;
  old_dir_base = dir_base;
}
