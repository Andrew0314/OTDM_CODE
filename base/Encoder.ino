
// ENCODER VALUES FOR INTERRUPT
volatile bool encoder_direction = false; // true is cw false is ccw
volatile int encoder_ticks = 0;
volatile int reverse_ticks = 0;

// DEAL WITH REVERSE TRAVEL AS AN ERROR
int max_reverse_ticks = 2000;
bool reverse_flag = false;

// VALUES FOR DISTANCE TRAVELED
float feet_per_transit = 10.0;
float ticks_per_rev = 1200; // ticks/rev
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
int which_pod = 1;


void setup_encoder(){
   pinMode(encoderA_pin,INPUT_PULLUP);
   pinMode(encoderB_pin,INPUT_PULLUP);
   attachInterrupt(digitalPinToInterrupt(encoderA_pin), encoderA, RISING);
   attachInterrupt(digitalPinToInterrupt(encoderB_pin), encoderB, RISING);
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
  //noInterrupts();
    unsigned long start_time = millis();
    unsigned long t2 = millis();
    while (abs(t2-start_time) < delay_time){
      t2 = millis();
      //Serial.println(t2);
      //Serial.println(start_time);
    }
   // interrupts();
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
      assign_motor_pwm();
  }
}

void handle_pod_location(){
  // IF AT CORRECT LOCATION TRANSMIT SIGNAL TO OPEN
  
  if (!run_with_encoder){
    in_slowdown = false;
    return;
  }
  Serial.println(encoder_ticks);
  if (encoder_ticks <= ticks_per_slowdown_tol){
    slowdown_motor();
    Serial.println("START SLOWDOWN");
  }
  else if (abs(encoder_ticks - ticks_per_transit) <= ticks_per_slowdown_tol) // ENTERING SLOWDOWN
  {
    slowdown_motor();
    Serial.println("STATION SLOWDOWN");
  }
  else if (abs(encoder_ticks - ticks_per_transit) <= ticks_per_stop_tol) // LOADING/UNLOADING // NEEDS THE LOAD_COMPLETE BECAUSE IT WOULD TRIGGER TWICE
  {
        Serial.println("STOP");
    stop_motor();
    in_slowdown = false;    // DISABLEs SPEED CONTROL
    if (!run_with_pods){
      better_delay(5000);
    }else{
      pod1.openSesimy = 1;
      transmitData(which_pod);
      if (which_pod == 1){
        which_pod = 2;
      }else{
        which_pod = 1;
    }
   }
    get_speed_value();
    assign_motor_pwm();   // STARTS MOTOR AGAIN
    encoder_ticks = 0;
  }
  else{
    in_slowdown = false;
    get_speed_value();
    assign_motor_pwm();
  }

}



void encoderA(){
  // INCREMENT CW ENCODER COUNTS

  if (digitalRead(encoderB_pin)){
    encoder_direction = true;
    encoder_ticks ++;
    //Serial.println(encoder_ticks);
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

void encoderB(){

      // INCREMENT CW ENCODER COUNTS
  //calculate_motor_speed();

  if (!digitalRead(encoderA_pin)){
    encoder_direction = true;
    encoder_ticks ++;
    reverse_ticks = 0;  // Reset reverse ticks because we have moved forward


  }else{
    reverse_ticks++;
    encoder_direction = false;
    if (reverse_ticks == max_reverse_ticks){
      reverse_flag = true;
    }
  }


}
