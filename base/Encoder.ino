
#include <ESP32Encoder.h>


// ENCODER VALUES FOR INTERRUPT
//volatile bool encoder_direction = false; // true is cw false is ccw
int encoder_ticks = 0;
//volatile int reverse_ticks = 0;

// DEAL WITH REVERSE TRAVEL AS AN ERROR
//int max_reverse_ticks = 2000;
//bool reverse_flag = false;

// VALUES FOR DISTANCE TRAVELED

float ticks_per_rev = 1200; // ticks/rev
float distance_per_rev = 13 * 3.1415 / 12; // ft/rev
float rev_per_transit = feet_per_transit / distance_per_rev; // One way travel
int ticks_per_transit = ticks_per_rev * rev_per_transit;
int ticks_per_stop_tol = ticks_per_rev * (stop_tolerance / distance_per_rev);
float ft_per_tick = distance_per_rev / ticks_per_rev;



int pre_stop_slowdown_ticks = pre_stop_slowdown / ft_per_tick;
int post_stop_slowdown_ticks = post_stop_slowdown / ft_per_tick;


// VARIABLES FOR CURRENT TRAVEL SPEED
unsigned long prev_time = micros();
unsigned long current_time;
volatile int ellapsed_encoder_ticks = 0;
float ellapsed_time; 
float bull_wheel_diam = 14/12; //ft
bool load_complete = false;
int which_pod = 1;


void setup_encoder(){
  ESP32Encoder::useInternalWeakPullResistors=UP;
  encoder.attachHalfQuad(encoderA_pin, encoderB_pin);   
  encoder.setCount(0);
}

//void calculate_motor_speed(){
//    current_time = micros();
//    // THESE IF STATEMENTS ENSURE NO SHENANIGANS WITH STARTING AND STOPPING 
//    if (current_time <= prev_time){
//      speed_current = 0.0;
//      prev_time = current_time;
//      return;
//    }
//    if (ellapsed_encoder_ticks == 0){
//      speed_current = 0.0;
//      ellapsed_encoder_ticks = 0;
//      prev_time = current_time;
//      return;
//    }
//    ellapsed_time = abs(current_time - prev_time); // microseconds
//    if (ellapsed_time <= 1){
//      speed_current = 0.0;
//      ellapsed_encoder_ticks = 0;
//      prev_time = current_time;
//      return;
//    }
//    
//    float tps = ellapsed_encoder_ticks / (ellapsed_time/1000000); // Ticks per second
//    float rps = (tps/ticks_per_rev);
//    rpm = rps * 60;
//    unfiltered_speed_current = rps * 3.1415 * bull_wheel_diam; // ft/s
//    speed_current = exp_filter(unfiltered_speed_current);
//    prev_time = current_time;
//    ellapsed_encoder_ticks = 0;
//}

// USE THIS DELAY IF delay DOESN't SEEM TO DELAY
//void better_delay(unsigned long delay_time){
//  //noInterrupts();
//    unsigned long start_time = millis();
//    unsigned long t2 = millis();
//    while (abs(t2-start_time) < delay_time){
//      t2 = millis();
//      //Serial.println(t2);
//      //Serial.println(start_time);
//    }
//   // interrupts();
//}

void slowdown_motor(){
  in_slowdown = true;
  motor_running = true;
  pwm = slowdown_pwm;
}


// slow phases
// 0 -> post_stop_slowdown ticks
// pre_stop_slowdown -> ticks_per_transits

void handle_pod_location(){
  // IF AT CORRECT LOCATION TRANSMIT SIGNAL TO OPEN
  encoder_ticks = encoder.getCount();
  Serial.print("Encoder Ticks: ");
  Serial.println(encoder_ticks);
  if (!run_with_encoder){
    in_slowdown = false;
    return;
  }

  if (encoder_ticks <= post_stop_slowdown_ticks){
    slowdown_motor();
  }
  else if (encoder_ticks >= pre_stop_slowdown_ticks and encoder_ticks < ticks_per_transit) // ENTERING SLOWDOWN
  {
    slowdown_motor();
  }
  else if (encoder_ticks >= ticks_per_transit) // LOADING/UNLOADING // NEEDS THE LOAD_COMPLETE BECAUSE IT WOULD TRIGGER TWICE
  {   
    stop_motor();
    in_slowdown = false;    // DISABLEs SPEED CONTROL
    if (!run_with_pods){
      delay(5000);
    }else{
      transmitData(which_pod);
      if (which_pod == 1){
        which_pod = 2;
      }else{
        which_pod = 1;
    }
   }
    get_speed_value();
    encoder_ticks = 0;
    encoder.setCount(0);
  }
  else{
    in_slowdown = false;
    get_speed_value();
  }

}
