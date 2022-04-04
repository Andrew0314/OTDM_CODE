
// ENCODER VALUES FOR INTERRUPT
volatile bool encoder_direction = false; // true is cw false is ccw
volatile int encoder_ticks = 0;
volatile int reverse_ticks = 0;

// DEAL WITH REVERSE TRAVEL AS AN ERROR
int max_reverse_ticks = 2000;
bool reverse_flag = false;

// VALUES FOR DISTANCE TRAVELED
float feet_per_transit = 21.0;
float ticks_per_rev = 600;
float distance_per_rev = 14 * 3.1415 / 12; // feet
float rev_per_transit = feet_per_transit / distance_per_rev;
int ticks_per_transit = ticks_per_rev * rev_per_transit;
int ticks_per_tolerance = ticks_per_rev * (slowdown_tolerance / distance_per_rev);


// TICKS WHERE SLOWDOWN SHOULD BEGIN
int start_slowdown1 = ticks_per_transit - ticks_per_tolerance;
int stop_slowdown1 = ticks_per_transit + ticks_per_tolerance;
int start_slowdown2 = ticks_per_transit - (2*ticks_per_tolerance);
int stop_slowdown2 = ticks_per_transit + (2*ticks_per_tolerance);

// VARIABLES FOR CURRENT TRAVEL SPEED
unsigned long last_encoder_tick = 0;
unsigned long current_time;
unsigned long ellapsed_time; 

void setup_encoder(){
   pinMode(encoderA_pin,INPUT_PULLUP);
   pinMode(encoderB_pin,INPUT_PULLUP);
   attachInterrupt(digitalPinToInterrupt(encoderA_pin), encoderA, RISING);
   attachInterrupt(digitalPinToInterrupt(encoderB_pin), encoderB, RISING);
}


double calculate_motor_speed(){
  if (last_encoder_tick == 0){
    return 0;
  }else{
    current_time = millis();
    ellapsed_time = (current_time - last_encoder_tick)  * 1000; // seconds
    float rps = (1/ticks_per_rev) / ellapsed_time;
    speed_current = rps * 2 * 3.1415 * 14 /12; // ft/s
    last_encoder_tick = current_time;    
  }
}

void handle_pod_location(){
  // IF AT CORRECT LOCATION TRANSMIT SIGNAL TO OPEN
  if (!run_with_encoder){
    in_slowdown = false;
    return;
  }
  // IF AT SLOWDOWN LOCATION CHANGE SETPOINT AND DISABLE POTENTIOMETER SPEED INPUT
  if (encoder_ticks >= start_slowdown1)
  {
   speed_setpoint = slowdown_speed;
   in_slowdown = true;
  }
  else if (encoder_ticks == ticks_per_transit)
  {
    pod1.openSesimy = 1;
    transmitData(1);
    stop_motor();
  }
  else if(encoder_ticks <= stop_slowdown1)
  {
    speed_setpoint = slowdown_speed;
    in_slowdown = true;
  }
  else if (encoder_ticks >= start_slowdown2)
  {
     speed_setpoint = slowdown_speed;
     in_slowdown = true;
  }
  else if (encoder_ticks == ticks_per_transit * 2)
  {
    pod2.openSesimy = 1;
    transmitData(2);
    stop_motor();
    encoder_ticks = 0;      // After full loop reset encoder distance
  }
  else if(encoder_ticks <= stop_slowdown2)
  {
    speed_setpoint = slowdown_speed;
    in_slowdown = true;    
  }
  else
  {
    in_slowdown = false;
  }
}

void encoderA(){
  // INCREMENT CW ENCODER COUNTS
  calculate_motor_speed();
  if (!digitalRead(3) && digitalRead(2)){
    encoder_direction = true;
    encoder_ticks ++;
    reverse_ticks = 0;  // Reset reverse ticks because we have moved forward
  }
  handle_pod_location();

}

void encoderB(){
    // INCREMENT REVERSE ENCODER COUNT
    if (digitalRead(3) && !digitalRead(2)){
      encoder_direction = false;
      reverse_ticks++;
      if (reverse_ticks == max_reverse_ticks){
        reverse_flag = true;
      }
    }
}
