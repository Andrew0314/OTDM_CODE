

#include <IRremote.h>

int RECV_PIN;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup_remote(int pin){
  RECV_PIN = pin;
  irrecv = IRrecv(RECV_PIN);
  irrecv.enableIRIn(); // Start the receiver
}


remote_output get_remote_input() {

  remote_output out;
  out.do_pod_stuff = false;
  out.new_data = true;
  char output = '0';
  
  if (irrecv.decode(&results)) {
    switch (results.value){
      case 16753245: // power button
        if (motor_running){
            stop_motor(); // power button
            out.dir_remote = 0;
        }else{
            motor_running = true;
        }
        break;
      case 16761405:
        out.dir_remote = 1; // fast forward button
        break;
      case 16720605:
        out.dir_remote = -1; // rewing button
        break;
      case 16748655:
        out.openSesimy = 0; // up arrow button
        out.do_pod_stuff = true;
        break;
      case 16769055: 
        out.openSesimy = 1;
        out.do_pod_stuff = true; // down arrow button
        break;
      case 16724175:
        out.motor_pwm_remote = (255/4)*1; // 1 button
        break;
      case 16718055:
        out.motor_pwm_remote = (255/4)*2; // 2 button
        break;
      case 16743045:
        out.motor_pwm_remote = (255/4)*3; // 3 button
        break;
      case 16716015:
        out.motor_pwm_remote = (255/4)*4; // 4 button
        break;
      case 16730805:
        out.pod_number = 1;  // 8 button
        out.do_pod_stuff = true;
        break;
      case 16732845: 
        out.pod_number = 2; // 9 button 
        out.do_pod_stuff = true;
        break;
      default:
        out.new_data = false;
        output = '0';
    }
    irrecv.resume(); // Receive the next value
  }
  delay(10);
  return out;
    
  }
