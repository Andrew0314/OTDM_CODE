
void print_pot(){
  //Serial.print("SPEED");
  Serial.println(get_speed_value());
}

void print_pod_status(){
  Serial.println("POD1:");
  Serial.print("Ready: ");
  Serial.print(pod1.ready2go);
  Serial.print(" Open: ");
  Serial.print(pod1.openSesimy);
  Serial.println();
  Serial.println("POD2:");
  Serial.print("Ready: ");
  Serial.print(pod2.ready2go);
  Serial.print(" Open: ");
  Serial.print(pod2.openSesimy);
  Serial.println();
  Serial.println();
}

void all_lights(){

  digitalWrite(receive_led_blue, HIGH);
  digitalWrite(send_led_red, HIGH);
  digitalWrite(running_led_green, HIGH);
  digitalWrite(receive_led_blue, HIGH);
  analogWrite(R_pin,255);
  analogWrite(G_pin,255);
  analogWrite(B_pin,255);

}

void all_lights_off(){

  digitalWrite(receive_led_blue, LOW);
  digitalWrite(send_led_red, LOW);
  digitalWrite(running_led_green, LOW);
  analogWrite(R_pin,0);
  analogWrite(G_pin,0);
  analogWrite(B_pin,0);

}
