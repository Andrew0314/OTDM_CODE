
void setup_LED(){
   pinMode(receive_led_blue, OUTPUT);
   pinMode(send_led_red, OUTPUT);
   pinMode(running_led_green, OUTPUT);

   pinMode(R_pin, OUTPUT);
   pinMode(G_pin, OUTPUT);
   pinMode(B_pin, OUTPUT);
}


void RGB_LED(int r, int g, int b){
  analogWrite(R_pin,r);
  analogWrite(G_pin,g);
  analogWrite(B_pin,b);
}
