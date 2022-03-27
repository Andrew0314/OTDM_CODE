

#include <IRremote.h>

int RECV_PIN;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup_remote(int pin){
  RECV_PIN = pin;
  irrecv = IRrecv(RECV_PIN);
  irrecv.enableIRIn(); // Start the receiver
}


char get_remote_input() {
  char output = '0';
  if (irrecv.decode(&results)) {
    switch (results.value){
      case 16753245:
        output = 'p'; // power button
        break;
      case 16712445:
        output = 's'; // play button
        break;
      case 16761405:
        output = 'f'; // fast forward button
        break;
      case 16720605:
        output = 'b'; // rewing button
        break;
      case 16748655:
        output = 'c'; // up arrow button
        break;
      case 16769055: 
        output = 'o';  // down arrow button
        break;
      case 16724175:
        output = '1'; // 1 button
        break;
      case 16718055:
        output = '2'; // 2 button
        break;
      case 16743045:
        output = '3'; // 3 button
        break;
      case 16716015:
        output = '4'; // 4 button
        break;
      case 16730805:
        output = '8'; // 8 button
        break;
      case 16732845: 
        output = '9'; // 9 button 
        break;
      default:
        output = '0';
    }
    irrecv.resume(); // Receive the next value
  }
  delay(10);
  return output;
}
