
void setup_input(){
  pinMode(forward_pin, INPUT_PULLUP);
  pinMode(reverse_pin, INPUT_PULLUP);
}

// MODIFIED MAP DUE TO BUILTIN MAP ONLY WORKING FOR INT
double double_map(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Get potentiometer speed value
void get_speed_value(){
  
  // NO POT INPUT IF POD IS IN SLOWDOWN
  if (in_slowdown){
    speed_setpoint = slowdown_speed;
    return;
  }

  if (!motor_running){
    speed_setpoint = 0.0;
    return;
  }
  int pot_value = analogRead(pot_pin);
  speed_setpoint = double_map(pot_value, 0.0,1023.0,0.0,max_speed); // Map input to valid speed in ft/s

  // HANDLE LED
  int R = map(pot_value,0,1023,255,0);
  int G = map(pot_value, 0,1023, 0, 255);
  RGB_LED(R,G,0);
}


// Get direction based on switch
void get_direction(){
  int forward_switch = digitalRead(forward_pin);
  int reverse_switch = digitalRead(reverse_pin);

  if (forward_switch && reverse_switch){  // Don't run motor
    dir = 0;
  }
  else if (forward_switch && !reverse_switch){ // Run Forward
    dir = 1;
  }
  else if (!forward_switch && reverse_switch){ // Run Reverse
    dir = -1;
  }
}
