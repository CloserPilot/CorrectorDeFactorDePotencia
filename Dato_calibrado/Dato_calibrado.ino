//         (Vi-offset)rms  (V_suma)rms
// Irms = -------------- = ----------
//               m              m


//      lectura*5V
// Vx = ---------- = lectura*0.004882
//        1024

const int OFFSET = 510;//offset       "Calibracion_offset"
const float M = 0.07234; //Sensibilidad "Calibracion_sensibilidad"

const float ADC_ = 0.004882;   //Relacion de transformacion del ADC 


float Iaux;

void setup(){
  Serial.begin(115200);
}

void loop(){
  Iaux = (analogRead(A0)-OFFSET)*ADC_/M; // Vaux = (Vi-offset) [V]
  Serial.println(Iaux);
}
