//         (Vi-offset)rms  (V_suma)rms
// Irms = -------------- = ----------
//               m              m


//      lectura*5V
// Vx = ---------- = lectura*0.004882
//        1024


float Vaux; //lectura del puerto A0
float Irms; 
float Ipico;
float Isuma;
float Iaux; 
int contador;
const float raiz_2 = 1.4142;

const int offset = 511;//offset       "Calibracion_offset"
const float m = 0.185; //Sensibilidad "Calibracion_sensibilidad"

const float ADC_ = 0.004882;  //Relacion de transformacion del ADC 

int t_muestreo = 1000;
unsigned long t_ini;


void setup(){
  Serial.begin (9600);
}

void loop(){
  Isuma = 0;
  contador = 0;
  t_ini = millis();

  while( (millis()-t_ini) < t_muestreo){
    Vaux = (analogRead(A0)-offset)*ADC_; // Vaux = (Vi-offset) [V]
    Iaux = Vaux/m;   
    Isuma += pow(Iaux, 2);  // Isuma += [(Vi-offset)/m] ^2
    contador++;
  }

  Irms = sqrt(Isuma/contador);
  Ipico = Irms*raiz_2;
   Serial.print("Irms: ");
   Serial.print(Irms,2);
   Serial.print("  Ipico: ");
   Serial.println(Ipico);
}
