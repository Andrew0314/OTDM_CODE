

#include "BTS7960.h"

BTS7960 motor(L_EN, R_EN, L_PWM, R_PWM);

void setup_motor(){
    motor.Enable();
}

void run_motor(int dir, int pwm){
  if (dir == 1){
    motor.TurnLeft(pwm);
  }else if (dir == -1){
    motor.TurnRight(pwm);
  }
  else if (dir == 0){
    stop_motor();
  }
}

void stop_motor(){
  motor.Stop();
}
