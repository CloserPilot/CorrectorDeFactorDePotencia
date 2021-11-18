//     (Vi-offset)rms    (V_suma)rms
// m = -------------- = -------
//         Irms           Irms


//      lectura*5V
// Vx = ---------- = lectura*0.004882
//        1024

const int OFFSET = 510;
const float ADC_ = 0.004882;  //Relacion de transformacion del ADC 
const int T_MUESTREO = 500;   //Tiempo para calcular rms

float v_rms; 
float v_suma;
float v_aux; 
int contador;

unsigned long t_ini;

void setup(){
  Serial.begin (115200);
}

void loop(){
  v_suma = 0;
  contador = 0;
  t_ini = millis();

  while( (millis()-t_ini) < T_MUESTREO){
    v_aux = (analogRead(A0)-OFFSET)*ADC_; // Vaux = (Vi-offset) [V]
    v_suma += pow(v_aux, 2);  // Vsuma += (Vi-offset)^2
    contador++;
  }

  v_rms = sqrt(v_suma/contador);
  Serial.print("Valor rms: ");
  Serial.println(v_rms,3);
}
