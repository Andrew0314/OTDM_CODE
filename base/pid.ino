//#include <AutoPID.h>


void setup_pid(){
//  pid.setTimeStep(pid_timestep);
}
void assign_motor_pwm(){
  // IF NO PID GO OFF POT INPUT
  if (!run_with_pid){
    pwm = double_map(speed_setpoint,0.0,max_speed,0.0,255.0);
  }else{
 //   pid.run();
  }
}
