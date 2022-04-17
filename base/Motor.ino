

#include "BTS7960.h"

BTS7960 motor(L_EN, R_EN, L_PWM, R_PWM);

void setup_motor(){
  motor.Enable();
}

void run_motor(int dir, int pwm){
  if (dir == prev_dir and abs(pwm - prev_pwm) <= 3){
    return;
  }
   // motor.Enable();
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
  prev_dir = dir;
  prev_pwm = pwm;
}

void stop_motor(){
  motor.Stop();
  //motor.Disable();
  speed_setpoint = 0.0;
}
