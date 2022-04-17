
// ENCODER VALUES FOR INTERRUPT
volatile bool encoder_direction = false; // true is cw false is ccw
volatile int encoder_ticks = 0;
volatile int reverse_ticks = 0;

// DEAL WITH REVERSE TRAVEL AS AN ERROR
int max_reverse_ticks = 2000;
bool reverse_flag = false;

// VALUES FOR DISTANCE TRAVELED
float feet_per_transit = 19.0;
float ticks_per_rev = 600; // ticks/rev
float distance_per_rev = 13 * 3.1415 / 12; // ft/rev
float rev_per_transit = feet_per_transit / distance_per_rev; // One way travel
int ticks_per_transit = ticks_per_rev * rev_per_transit;
int ticks_per_slowdown_tol = ticks_per_rev * (slowdown_tolerance / distance_per_rev);
int ticks_per_stop_tol = ticks_per_rev * (stop_tolerance / distance_per_rev);
float ft_per_tick = distance_per_rev / ticks_per_rev;



// TICKS WHERE SLOWDOWN AND STOP SHOULD BEGIN
int start_slowdown1 = ticks_per_transit - ticks_per_slowdown_tol;
int stop_slowdown1 = ticks_per_transit + ticks_per_slowdown_tol;
int start_slowdown2 = (2*ticks_per_transit) - ticks_per_slowdown_tol;
int stop_slowdown2 = (2*ticks_per_transit) + ticks_per_slowdown_tol;
int stop1_ticks = ticks_per_transit - ticks_per_stop_tol;
int stop2_ticks = (2 * ticks_per_transit) - ticks_per_stop_tol;

// VARIABLES FOR CURRENT TRAVEL SPEED
unsigned long prev_time = micros();
unsigned long current_time;
volatile int ellapsed_encoder_ticks = 0;
float ellapsed_time; 
float bull_wheel_diam = 14/12; //ft
bool load_complete = false;

void setup_encoder(){
   pinMode(encoderA_pin,INPUT_PULLUP);
   pinMode(encoderB_pin,INPUT_PULLUP);
   attachInterrupt(digitalPinToInterrupt(encoderA_pin), encoderA, RISING);
//   attachInterrupt(digitalPinToInterrupt(encoderB_pin), encoderB, RISING);
}

void calculate_motor_speed(){
    current_time = micros();
    // THESE IF STATEMENTS ENSURE NO SHENANIGANS WITH STARTING AND STOPPING 
    if (current_time <= prev_time){
      speed_current = 0.0;
      prev_time = current_time;
      return;
    }
    if (ellapsed_encoder_ticks == 0){
      speed_current = 0.0;
      ellapsed_encoder_ticks = 0;
      prev_time = current_time;
      return;
    }
    ellapsed_time = abs(current_time - prev_time); // microseconds
    if (ellapsed_time <= 1){
      speed_current = 0.0;
      ellapsed_encoder_ticks = 0;
      prev_time = current_time;
      return;
    }
    
    float tps = ellapsed_encoder_ticks / (ellapsed_time/1000000); // Ticks per second
    float rps = (tps/ticks_per_rev);
    rpm = rps * 60;
    unfiltered_speed_current = rps * 3.1415 * bull_wheel_diam; // ft/s
    speed_current = exp_filter(unfiltered_speed_current);
    prev_time = current_time;
    ellapsed_encoder_ticks = 0;
}

// USE THIS DELAY IF delay DOESN't SEEM TO DELAY
void better_delay(unsigned long delay_time){
    unsigned long t1 = millis();
    unsigned long t2 = millis();
    while (abs(t2-t1) < delay_time){
      t2 = millis();
    }
}

void slowdown_motor(){
  in_slowdown = true;
  motor_running = true;
  
  if (run_with_incremental_slowdown){
    double slowdown_increment = (speed_setpoint - slowdown_speed ) / slowdown_steps;
    for (int i = 0; i < slowdown_steps; i++){
      speed_setpoint -= slowdown_increment;
      delay(slowdown_delay);
    }
  }else{
      speed_setpoint = slowdown_speed;
  }
}

void handle_pod_location(){
  // IF AT CORRECT LOCATION TRANSMIT SIGNAL TO OPEN
  
  if (!run_with_encoder){
    in_slowdown = false;
    return;
  }
  
  if (encoder_ticks >= start_slowdown1 and encoder_ticks < stop1_ticks) // ENTERING SLOWDOWN
  {
    slowdown_motor();
  }
  else if (abs(encoder_ticks - ticks_per_transit) <= ticks_per_stop_tol and !load_complete) // LOADING/UNLOADING // NEEDS THE LOAD_COMPLETE BECAUSE IT WOULD TRIGGER TWICE
  {
    stop_motor();
    in_slowdown = false;    // DISABLEs SPEED CONTROL
    motor_running = false;  
    pod1.openSesimy = 1;
    load_complete = true; // ONLY RUNS THIS CASE ONCE
    if (!run_with_pods){
      better_delay(5000);
    }else{
      transmitData(1);
    }
    run_motor(dir,pwm);   // STARTS MOTOR AGAIN
  }
  
  else if((encoder_ticks <= stop_slowdown1 and encoder_ticks > ticks_per_transit and motor_running) or load_complete) // EXITING SLOWDOWN
  {
    if (encoder_ticks > ticks_per_transit + ticks_per_stop_tol){ // THIS MAKES SURE ABOVE CASE CAN"T BE CALLED AGAIN AND RESETS LOAD COMPLETE FOR NEXT ROUND
      load_complete = false;      
    }
    slowdown_motor();
  }
  else if (encoder_ticks >= start_slowdown2 and encoder_ticks < stop2_ticks) // ENTERING SLOWDOWN
  {
     slowdown_motor();
  }
  else if (abs(encoder_ticks - (ticks_per_transit * 2)) <= ticks_per_stop_tol and !load_complete) // LOADING/UNLOADING
  {
    stop_motor();
    in_slowdown = false;
    motor_running = false; 
    pod2.openSesimy = 1;
    load_complete = true;
    if (!run_with_pods){
      better_delay(5000);
    }else{
      transmitData(2);
    }
   run_motor(dir,pwm);  
  }
  else if((encoder_ticks <= stop_slowdown2 and encoder_ticks > ticks_per_transit*2 and motor_running) or load_complete) // EXITING SLOWDOWN
  {
    if (encoder_ticks > ticks_per_transit + ticks_per_stop_tol){
      load_complete = false;      
    }
    slowdown_motor();
  }
}



void encoderA(){
  // INCREMENT CW ENCODER COUNTS

  if (digitalRead(encoderB_pin)){
    encoder_direction = true;
    encoder_ticks ++;
    ellapsed_encoder_ticks++;
    reverse_ticks = 0;  // Reset reverse ticks because we have moved forward

  }else{
    reverse_ticks++;
    encoder_direction = false;
    if (reverse_ticks == max_reverse_ticks){
      reverse_flag = true;
    }
  }
}

//void encoderB(){
//
//      // INCREMENT CW ENCODER COUNTS
//  calculate_motor_speed();
//
//  if (!digitalRead(encoderA_pin)){
//    encoder_direction = true;
//    encoder_ticks ++;
//    reverse_ticks = 0;  // Reset reverse ticks because we have moved forward
//  }else{
//    reverse_ticks++;
//    encoder_direction = false;
//    if (reverse_ticks == max_reverse_ticks){
//      reverse_flag = true;
//    }
//  }
//  handle_pod_location();
//
//}
