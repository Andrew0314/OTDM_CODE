
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
