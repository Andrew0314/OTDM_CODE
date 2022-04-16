

#include "BTS7960.h"

BTS7960 motor(L_EN, R_EN, L_PWM, R_PWM);

void run_motor(int dir, int pwm){
  motor.Enable();
  if (dir == 1){
    motor.TurnLeft(pwm);
    motor_running = true;
    digitalWrite(running_led_green, HIGH);
  }else if (dir == -1){
    motor.TurnRight(pwm);
    motor_running = true;
    flashlight(running_led_green);
  }
  else if (dir == 0){
    stop_motor();
    digitalWrite(running_led_green, LOW);
  }
}

void stop_motor(){
  motor.Stop();
  motor.Disable();
  speed_setpoint = 0.0;
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
