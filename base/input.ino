
void setup_input(){
  pinMode(forward_pin, INPUT_PULLUP);
  pinMode(reverse_pin, INPUT_PULLUP);
}



// Get potentiometer speed value

double double_map(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void get_speed_value(){
  if (in_slowdown){
    return;
  }
  int pot_value = analogRead(pot_pin);
  
  speed_setpoint = double_map(pot_value, 0.0,1023.0,0.0,max_speed); // Map input to valid speed in ft/s

  // HANDLE LED
  int R = map(pot_value,0,1023,255,0);
  RGB_LED(R,pwm,0);
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
