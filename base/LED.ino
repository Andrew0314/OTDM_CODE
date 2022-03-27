


void setup_LED(){
   pinMode(blue, OUTPUT);
   pinMode(green, OUTPUT);
   pinMode(red, OUTPUT);
}

void flashlight(int x){
      digitalWrite(x, HIGH);
      delay (200);
      digitalWrite(x, LOW);
      delay(200);
      digitalWrite(x, HIGH);
      delay (200);
      digitalWrite(x, LOW);
      delay(200);

}



void RGB_LED(){
  
}
