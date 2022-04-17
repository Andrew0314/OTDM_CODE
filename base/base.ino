#include <AutoPID.h>

// CONFIG PARAMS
const int OPEN_CLOSE_DELAY = 20000;
bool test_door_open = false;
bool run_with_encoder = true;
bool run_with_pid = false;
bool run_with_pods = false;
bool run_with_slowdown = true;
bool run_with_incremental_slowdown = false;

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
const int L_PWM = 7;
const int R_PWM = 6;

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
int rf_int_pin = 1; // WANT TO SHOOT MYSELF THIS MEANS 3!!!!!!!!!!!!!

// RF MESSAGES TO PODS
struct msg{
  bool openSesimy;
  bool ready2go;
};

// INDIVIDUAL POD MESSAGE
volatile msg pod1 = {0,1};
volatile msg pod2 = {0,1};

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
bool motor_running = true;
int dir;
float rpm;

// MOTOR PARAMS
double motor_deadband = 10;
double max_speed = 2; // ft/s

// PID VALUES
double speed_setpoint;
double unfiltered_speed_current;
double speed_current;
double pwm;   // FINAL VALUES

// PID CONSTANTS
double kp = 2000;
double ki = 0;
double kd = 0;

// RUNNING PARAMETERS
double slowdown_tolerance = 2; // ft  when to slow down to slower speed
double slowdown_speed = .75; // ft/s
int slowdown_steps = 10;
int slowdown_delay = 100;
double stop_tolerance = .1; //ft when to stop
bool in_slowdown = false;



int pid_timestep = 20;

AutoPID pid(&speed_current, &speed_setpoint, &pwm, motor_deadband, 255.0, kp,ki,kd);

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

void setup() {
  Serial.begin(115200);
  //setup_remote(REMOTE_PIN);
  //setup_RF();
  setup_encoder();
  setup_motor();
  setup_LED();
  setup_input();
  //setup_pid();
  all_lights();
  rgb();
  delay(2000);
  all_lights_off();

}

void loop() {
  // DEBUG PRINT STATMENTS
  //print_pod_status(1);
  //print_pot();
  //print_motor_speed();
  print_pod_location(1);
  //print_pod_location(2);
  //print_motor_dir();
  //debug_encoder();
  //plot_rpm();
  //print_pid();
  //calculate_motor_speed();        // Calculates motor speed from encoder to speed_current variable
  get_speed_value();              // Reads potentiometer into speed_setpoint
  get_direction();                // Reads switch
  assign_motor_pwm();             // Assigns pwm variable with setpoint depending on if PID is enabled
  handle_pod_location();          // Nested if statement to see where pods is and when to stop/slowdown
  
  // LOGIC FOR CONFIG PARAMS 
  if (test_door_open){
    test_door();
  }else{
    if (run_with_pods){
      // IF PODS ARE CLOSED AND READY RUN MOTOR AT DIRECTION AND PWM
      if (pod1.ready2go){// && pod2.ready2go){
        
        //speed_setpoint = slowdown_speed;
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
    stop_motor();
    //for (int i = 0; i < 3; i++){
      print_pod_status(1);
      print_pod_status(2);
      display_pod_ready();
      delay(1000);
      pod1.openSesimy = 1;
      transmitData(1);
      pod2.openSesimy = 1;
      transmitData(2);
      Serial.println("AFTER SENT:");
      print_pod_status(1);
      print_pod_status(2);
      display_pod_ready();
      delay(30000);
      Serial.println("AFTER RECIEVE HOPEFULLY:");
      print_pod_status(1);
      print_pod_status(2);
      display_pod_ready();
      delay(30000);
      //better_delay(60000);         
      
     
    //}
}


void display_pod_ready(){
  if (pod1.ready2go){
    digitalWrite(running_led_green,HIGH);
  }else{
    digitalWrite(running_led_green,LOW);
  }
  
  if (pod2.ready2go){
    RGB_LED(0,255,0);
  }else{
    RGB_LED(0,0,0);
  }
}
