// Filtro
const float EMA_ALPHA = 0.7;    //Constante alpha
float EMA_LP = 0.0;

//Conversión ADC
const int OFFSET = 511;//offset       "Calibracion_offset"
const float M = 0.180; //Sensibilidad "Calibracion_sensibilidad"
const float ADC_ = 0.004882;   //Relacion de transformacion del ADC = 5V/1024
const float CONST_ADC = 0.027;  // = ADC/m

//Datos aux
float I_crudo = 0.0;
float I_filtrado = 0.0;

//Gráficas
const int EJE_SUPERIOR = 5;
const int EJE_INFERIOR = -5;
const int OP_CRUDO = 0;
const int OP_FILTRO = 1;
int opcion = 2;

void setup(){
  Serial.begin(115200);
}

void loop(){
  if(Serial.available()){
    opcion = Serial.read()-48;
  }
  
  I_crudo = (analogRead(A0)-OFFSET)*CONST_ADC; // I_crudo = (Vi-offset)*ADC/M [V]
  I_filtrado = EMALowPassFilter(I_crudo);     //
  IMPRIME_INFO();
}




float EMALowPassFilter(float value){
   EMA_LP = EMA_ALPHA * value + (1 - EMA_ALPHA) * EMA_LP;
   return EMA_LP;
}



void IMPRIME_INFO(){
  switch (opcion) {
  case OP_CRUDO:
    Serial.print(I_crudo);
    break;
  case OP_FILTRO:
    Serial.print(I_filtrado);
    break;
  default:
    Serial.print(I_crudo);
    Serial.print(",");
    Serial.print(I_filtrado);
    break;
  }

  Serial.print(",");
  Serial.print(EJE_SUPERIOR);
  Serial.print(",");
  Serial.println(EJE_INFERIOR);
}
