int rl_ch = 3;
int gl_ch = 4;
int bl_ch = 5;

void setup_LED(){
   pinMode(receive_led_blue, OUTPUT);
   pinMode(send_led_red, OUTPUT);
   pinMode(running_led_green, OUTPUT);

  ledcAttachPin(R_pin, rl_ch); // assign RGB led pins to channels
  ledcAttachPin(G_pin, gl_ch);
  ledcAttachPin(B_pin, bl_ch);

  ledcSetup(rl_ch, 12000, 8); // 12 kHz PWM, 8-bit resolution
  ledcSetup(gl_ch, 12000, 8);
  ledcSetup(bl_ch, 12000, 8);

}

void RGB_LED(int r, int g, int b){
  ledcWrite(rl_ch,r);
  ledcWrite(gl_ch,g);
  ledcWrite(bl_ch,b);
}
