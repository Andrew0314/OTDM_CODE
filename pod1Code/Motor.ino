

int L_CH = 0;
int R_CH = 1;

void setup_motor(){
//  Enable();
  ledcAttachPin(L_PWM, L_CH); 
  ledcAttachPin(R_PWM, R_CH);
  ledcSetup(L_CH, 1000, 8);
  ledcSetup(R_CH, 1000, 8);
 // pinMode(L_EN, OUTPUT);
  //pinMode(R_EN, OUTPUT);
//    pinMode(_R_PWM, OUTPUT);
//  pinMode(_L_PWM, OUTPUT);

  
}

void run_motor(int dir, int pwm){

  if (dir == 1){
    TurnLeft(pwm);
  }else if (dir == -1){
    TurnRight(pwm);

  }
  else if (dir == 0){
    stop_motor();

  }
}

void stop_motor(){
  Stop();
}












void TurnRight(uint8_t pwm){
  ledcWrite(L_CH, 0);
  delayMicroseconds(100);
  ledcWrite(R_CH, pwm);
//     analogWrite(_L_PWM, 0);
//   delayMicroseconds(100);
//     analogWrite(_R_PWM, pwm);
}

void TurnLeft(uint8_t pwm){
   ledcWrite(R_CH, 0);
  delayMicroseconds(100);
  ledcWrite(L_CH, pwm);
//     analogWrite(_R_PWM, 0);
//   delayMicroseconds(100);
//     analogWrite(_L_PWM, pwm);
}

//void Enable(){
//  digitalWrite(L_EN,1);
//  if(R_EN != 0) digitalWrite(R_EN, HIGH);  
//}
//
//void Disable(){
//  digitalWrite(L_EN,0);
//  if(R_EN != 0) digitalWrite(R_EN, LOW);
//}

void Stop(){
  ledcWrite(R_CH, 0);
  ledcWrite(L_CH, 0);
//  analogWrite(_L_PWM, LOW);
//  analogWrite(_R_PWM, HIGH);
}
