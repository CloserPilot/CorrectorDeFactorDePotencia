// Filtro
const float EMA_ALPHA = 0.7;    //Constante alpha
float EMA_LP = 0.0;
boolean filtro = true;

//Conversión ADC
const int OFFSET = 509;//offset       "Calibracion_offset"
const float M = 0.180; //Sensibilidad "Calibracion_sensibilidad"
const float ADC_ = 5.0/1024.0;   //Relacion de transformacion del ADC = 5V/1024
const float CONST_ADC = ADC_/M;  // = ADC/m

//Datos aux
float I_crudo = 0.0;
float I_filtrado = 0.0;

//Gráficas
const int EJE_SUPERIOR = 5;
const int EJE_INFERIOR = -5;

//Opciones seriales
const int OP_VALORES = 0;
const int OP_GRAFICA = 1;
const int OP_GRAFICA_AMBOS = 2;

const int OP_ACTIVA_FILTRO = 9;
const int OP_DESACTIVA_FILTRO = 8;
int opcion = 10;

void setup(){
  Serial.begin(115200);
}

void loop(){
  LEE_SERIAL();   
  I_crudo = (analogRead(A0)-OFFSET)*CONST_ADC; // I_crudo = (Vi-offset)*ADC/M [V]
  filtro? I_filtrado = EMALowPassFilter(I_crudo) : I_filtrado = I_crudo;


  switch (opcion) {
    case OP_VALORES:
      
    break;
    
    case OP_GRAFICA:
      IMPRIME_GRAFICA();
    break;

    case OP_GRAFICA_AMBOS:
      IMPRIME_GRAFICA_AMBOS();
    break;

    case OP_ACTIVA_FILTRO:
      filtro = true;
      IMPRIME_GRAFICA_AMBOS();
    break;

    case OP_DESACTIVA_FILTRO:
      filtro = false;
      IMPRIME_GRAFICA_AMBOS();

    
    default:
    break;
   }
}


void LEE_SERIAL(){
  if(Serial.available()){
    opcion = Serial.read()-48;
  }
}

float EMALowPassFilter(float value){
   EMA_LP = EMA_ALPHA * value + (1 - EMA_ALPHA) * EMA_LP;
   return EMA_LP;
}

void IMPRIME_GRAFICA(){
  Serial.print(I_filtrado);
  Serial.print(",");
  Serial.print(EJE_SUPERIOR);
  Serial.print(",");
  Serial.println(EJE_INFERIOR);
}

void IMPRIME_GRAFICA_AMBOS(){
  Serial.print(I_crudo);
  Serial.print(",");
  Serial.print(I_filtrado);
  Serial.print(",");
  Serial.print(EJE_SUPERIOR);
  Serial.print(",");
  Serial.println(EJE_INFERIOR);
}
