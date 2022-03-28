
// ENCODER VALUES FOR INTERRUPT
volatile bool encoder_direction = false; // true is cw false is ccw
volatile int encoder_ticks = 0;
volatile int reverse_ticks = 0;

// DEAL WITH REVERSE TRAVEL AS AN ERROR
int max_reverse_ticks = 2000;
bool reverse_flag = false;

// VALUES FOR DISTANCE TRAVELED
float ticks_per_rev = 600;
float distance_per_rev = 14 * 3.1415 / 12; // feet
float rev_per_transit = 5 / distance_per_rev;
int ticks_per_transit = ticks_per_rev * rev_per_transit;


void setup_encoder(){
   pinMode(2,INPUT_PULLUP);
   pinMode(3,INPUT_PULLUP);
   attachInterrupt(digitalPinToInterrupt(2), encoderA, RISING);
   attachInterrupt(digitalPinToInterrupt(3), encoderB, RISING);
}

void encoderA(){
  // INCREMENT CW ENCODER COUNTS
  if (!digitalRead(3) && digitalRead(2)){
    encoder_direction = true;
    encoder_ticks ++;
    reverse_ticks = 0;  // Reset reverse ticks because we have moved forward
  }

  // IF AT CORRECT LOCATION TRANSMIT SIGNAL TO OPEN
  if (encoder_ticks == ticks_per_transit){
    pod1.openSesimy = 1;
    transmitData(1);
    stop_motor();
    
  }else if (encoder_ticks == ticks_per_transit * 2){
    pod2.openSesimy = 1;
    transmitData(2);
    stop_motor();
    encoder_ticks = 0;      // After full loop reset encoder distance
  }
  
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
