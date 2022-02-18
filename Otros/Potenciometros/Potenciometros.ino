void setup() {
  Serial.begin(115200);
}

int aux;
int aux2;
void loop() {
  aux = analogRead(A2);
  Serial.print("Potenciometro A2 = ");
  Serial.print(aux);
  aux2 = analogRead(A3);
  Serial.print("               Potenciometro A3 = ");
  Serial.println(aux2);
}
