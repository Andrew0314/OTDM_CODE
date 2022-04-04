
void setup_input(){
  pinMode(forward_pin, INPUT_PULLUP);
  pinMode(reverse_pin, INPUT_PULLUP);
}



// Get potentiometer speed value
int get_speed_value(){
  int pot_value = analogRead(pot_pin);
  int pwm = map(pot_value, 0,1023,0,255); // Map input to valid PWM
  int R = map(pwm,0,255,255,0);

  RGB_LED(R,pwm,0);
  return pwm;
}


// Get direction based on switch
int get_direction(){
  int forward_switch = digitalRead(forward_pin);
  int reverse_switch = digitalRead(reverse_pin);
  int dir;

  if (forward_switch && reverse_switch){  // Don't run motor
    dir = 0;
  }
  else if (forward_switch && !reverse_switch){ // Run Forward
    dir = 1;
  }
  else if (!forward_switch && reverse_switch){ // Run Reverse
    dir = -1;
  }
  return dir;
}
