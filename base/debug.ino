
void print_pot(){
  Serial.print("Potentiometer Reading: ");
  Serial.println(analogRead(pot_pin));
    Serial.println();
      Serial.println();
        Serial.println();
          Serial.println();
}


void plot_rpm(){
  Serial.print(speed_current);
  Serial.print(" ");
  Serial.println(unfiltered_speed_current);
  Serial.println();
  Serial.println();
  Serial.println();
}
void print_motor_speed(){
  Serial.print("Motor Speed: ");
  Serial.println(speed_current);
  Serial.print("RPM: ");
  Serial.println(rpm);
  Serial.print("Motor Speed Setpoint: ");
  Serial.println(speed_setpoint); 
  Serial.println();
  Serial.println();
  Serial.println();
}

void print_pod_location(int pod_number){
  
  float distance_traveled = encoder_ticks * ft_per_tick; // ft
  if (pod_number == 1){
    Serial.println("POD 1:");
    if (abs(distance_traveled - 0.0) <= stop_tolerance){
      Serial.println("Loading");
    }
    else if (distance_traveled < feet_per_transit){
      Serial.print(distance_traveled);
      Serial.println(" ft moving up");
    }
    else if (abs(distance_traveled - feet_per_transit) <= stop_tolerance){
      Serial.println("Unloading!");
    }
    else if ( distance_traveled > feet_per_transit){
      Serial.print(distance_traveled - feet_per_transit);
      Serial.println(" ft moving down");
    }
  }else   if (pod_number == 2){
    Serial.println("POD 2:");
    if (abs(distance_traveled - feet_per_transit) <= stop_tolerance){
      Serial.println("Unloading");
    }
    else if (distance_traveled < feet_per_transit * 2){
      Serial.print(distance_traveled);
      Serial.println(" ft moving up");
    }
    else if (abs(distance_traveled - 2 * feet_per_transit) <= stop_tolerance){
      Serial.println("Loading!");
    }
    else if ( distance_traveled > 2* feet_per_transit){
      Serial.print(distance_traveled - 2*feet_per_transit);
      Serial.println(" ft moving down");
    }
  }
  Serial.print("Pod Slowdown: ");
  Serial.println(in_slowdown);
  
      Serial.println();
        Serial.println();
                Serial.println();
}

void print_pid(){
  Serial.print("PWM");
  Serial.println(pwm);
  Serial.println();
  Serial.println();
  Serial.println();
}
void print_motor_dir(){
  Serial.print("Motor Direction: ");
  Serial.println(dir);
  Serial.println();
  Serial.println();
    Serial.println();
}

void debug_encoder(){
  Serial.print("Encoder Ticks: ");
  Serial.println(encoder_ticks);
  Serial.print("Reverse Ticks: ");
  Serial.println(reverse_ticks);
  Serial.println();
  Serial.println();
    Serial.println();
}
void print_pod_status(int pod){
  if (pod == 1){
    Serial.print("Pod: ");
    Serial.print(pod1.podNum);
    Serial.print("\tReady: ");
    Serial.print(pod1.ready2go);
    Serial.print("\tOpen: ");
    Serial.println(pod1.openSessimy);    
  }else if(pod ==2){
    Serial.print("Pod: ");
    Serial.print(pod2.podNum);
    Serial.print("\tReady: ");
    Serial.print(pod2.ready2go);
    Serial.print("\tOpen: ");
    Serial.println(pod2.openSessimy);    
  }
  Serial.println();
  Serial.println();
  Serial.println();
      
}

//void print_msg(msg pod){
//  Serial.print("Pod: ");
//  Serial.print(pod.podNum);
//  Serial.print("\tReady: ");
//  Serial.print(pod.ready2go);
//  Serial.print("\tOpen: ");
//  Serial.println(pod.openSessimy);    
//}

void rgb(){
  RGB_LED(0,0,255);
  delay(1000);
  RGB_LED(0,255,0);
  delay(1000);
  RGB_LED(255,0,0);
  delay(1000);
  RGB_LED(0,0,0);
  
  
}

void all_lights(){

  digitalWrite(receive_led_blue, HIGH);
  digitalWrite(send_led_red, HIGH);
  digitalWrite(running_led_green, HIGH);
  digitalWrite(receive_led_blue, HIGH);
//  analogWrite(R_pin,255);
//  analogWrite(G_pin,255);
//  analogWrite(B_pin,255);

}

void all_lights_off(){

  digitalWrite(receive_led_blue, LOW);
  digitalWrite(send_led_red, LOW);
  digitalWrite(running_led_green, LOW);
//  analogWrite(R_pin,0);
//  analogWrite(G_pin,0);
//  analogWrite(B_pin,0);

}
