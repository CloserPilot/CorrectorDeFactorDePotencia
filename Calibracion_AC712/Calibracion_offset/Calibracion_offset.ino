// 5V -> 1024
// Vx -> lectura

//      lectura*5V
// Vx = ---------- = lectura*0.004882
//        1024

const float ADC_ = 0.004882;

int t_muestreo = 1000;  //Tiempo de muestreo (milis)
unsigned long t_ini;

float v_suma;
int contador;
float vout;
float vout_int;

void setup() {
  Serial.begin(9600);
}

void loop() {
  v_suma = 0;
  contador = 0;
  t_ini = millis();
  
  while(  (millis()-t_ini) < t_muestreo){
    v_suma += analogRead(A0)*ADC_; //Mapeo del sensor
    contador++;
  }

  vout = v_suma/contador;
  vout_int = vout/ADC_;
  Serial.print("Valor de voltaje: ");
  Serial.print(vout,3);  //Voltaje a 3 decimales 
  Serial.print("    Valor int: ");
  Serial.println(vout_int,1);
}
