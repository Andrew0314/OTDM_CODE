

#include "BTS7960.h"

BTS7960 motor(L_EN, R_EN, L_PWM, R_PWM);

void run_motor(int dir, int pwm){
  motor.Enable();
  if (dir == 1){
    motor.TurnLeft(pwm);
      motor_running = true;
  }else if (dir == -1){
    motor.TurnRight(pwm);
      motor_running = true;
  }
  else if (dir == 0){
    stop_motor();
  }
}

void stop_motor(){
  motor.Stop();
  motor.Disable();
  motor_running = false;
}
