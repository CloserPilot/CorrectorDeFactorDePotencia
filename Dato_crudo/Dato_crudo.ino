float voltaje;
float corriente;

void setup() {
  Serial.begin(115200);
}

void loop() {
  voltaje = analogRead(A1);
  Serial.print(analogRead(A1));
  Serial.print(",");
  Serial.println(voltaje);
  delay(100);
  //Serial.print(",");
  //Serial.println(analogRead(A1));

}
