#include "Sensor.h"
////////////////////////
////                ////
////    SENSORES    ////
////                ////
////////////////////////
//Para los sensores
const float RESOLUCION      = 1024.0; //2^Nbits
const float VOLTAJE_MAXIMO  = 5.0;
const float FILTRO_ALPHA    = 0.9;    //Constante alpha del filtro

Sensor*     SensorCorriente;
const float I_OFFSET        = 516.9;
const float I_PENDIENTE     = 0.180;
float       I_Dato;           

Sensor*     SensorVoltaje;  
const float V_OFFSET        = 513;
const float V_PENDIENTE     = 0.03318;
float       V_Dato;  
        

////////////////////////
////                ////
////      RMS       ////
////                ////
////////////////////////
float         Irms;
float         Vrms;
float         RMS_contador;

const int     RMS_t_mues  = 1000;
unsigned long RMS_t_ini;


////////////////////////
////                ////
////      MENU      ////
////                ////
////////////////////////
const int OP_RMS        = 80; //'P'
const int OP_GRAFICA    = 71; //'G'
const int OP_COTA       = 67; //'C'
const int OP_AMPLI_POT  = 65; //'A'
const int OP_RESET      = 82;  //'R'
      int OP_opcion     = 0;  //Default

////////////////////////
////                ////
////   PUERTOS AD   ////
////                ////
////////////////////////
const int P_CORRIENTE = A0;   //A0
const int P_VOLTAJE   = A1;   //A1
const int P_POT1      = A2;   //A2
const int P_POT2      = A3;   //A3
const int P_LED_A     = 9;
const int P_LED_C     = 10;
const int P_LED_R     = 11;


////////////////////////
////                ////
////    Grafica     ////
////                ////
////////////////////////
float       Cota_Max      = 14;
float       Cota_Min      = 0;
const int   MULTIPLICADOR = 10;  


void setup() {
  Serial.begin(115200);
  SensorCorriente = new Sensor(I_OFFSET,I_PENDIENTE,RESOLUCION,VOLTAJE_MAXIMO,FILTRO_ALPHA);
  SensorVoltaje   = new Sensor(V_OFFSET,V_PENDIENTE,RESOLUCION,VOLTAJE_MAXIMO,FILTRO_ALPHA);
  digitalWrite(P_LED_A,LOW);
  digitalWrite(P_LED_C,LOW);
  digitalWrite(P_LED_R,HIGH);
}



void loop() {
  
  if (Serial.available() > 0)
      OP_opcion = Serial.read();

  switch (OP_opcion) {
    
    case OP_RMS:
      OBTENER_RMS();
      IMPRIME_INFO();
    break;
    
    case OP_GRAFICA:
      APAGA_CAMBIOS();
      MAPEA_SENSORES();
      IMPRIME_GRAFICA();
    break;

    case OP_COTA:
      CAMBIA_COTA();
      MAPEA_SENSORES();
      IMPRIME_GRAFICA();
    break;
    
    case OP_AMPLI_POT:
      CAMBIA_AMPLITUD();
      MAPEA_SENSORES();
      IMPRIME_GRAFICA();
    break;
    
    case OP_RESET:
      RESET();
      OP_opcion = OP_GRAFICA;
    break;
  }
}




////////////////////////////////////
////                            ////
////        Valores RMS         ////
////                            ////
////////////////////////////////////
void OBTENER_RMS(){
  RMS_contador = 0;
  RMS_t_ini = millis();

  while( (millis()-RMS_t_ini) < RMS_t_mues){
    MAPEA_SENSORES();
    
    Irms += pow(I_Dato, 2);  // Isuma += [(Vi-offset)/m] ^2
    Vrms += pow(V_Dato, 2);  // Isuma += [(Vi-offset)/m] ^2

    RMS_contador++;
  }

  Irms = sqrt(Irms/RMS_contador);
  Vrms = sqrt(Vrms/RMS_contador);
}


////////////////////////////////////
////                            ////
////       Mapea Sensores       ////
////                            ////
////////////////////////////////////
void MAPEA_SENSORES(){
  I_Dato = SensorCorriente->MapeoYFiltrado(analogRead(P_CORRIENTE));  //MapeoYFiltrado
  V_Dato = SensorVoltaje  ->MapeoYFiltrado(analogRead(P_VOLTAJE));    //MapeoYFiltrado
}

////////////////////////////////////
////                            ////
////         Mapea Cota         ////
////                            ////
////////////////////////////////////
void CAMBIA_COTA(){
  digitalWrite(P_LED_C,HIGH);
  digitalWrite(P_LED_A,LOW);
  Cota_Max = (analogRead(P_POT1))/4;
  Cota_Min = (analogRead(P_POT2))/4*-1;
}

////////////////////////////////////
////                           /////
////      Cambia Amplitud      /////
////                           /////
////////////////////////////////////
void CAMBIA_AMPLITUD(){
  digitalWrite(P_LED_A,HIGH);
  digitalWrite(P_LED_R,LOW);
  digitalWrite(P_LED_C,LOW);
  SensorCorriente->AjusteAmplitud(analogRead(P_POT1)*MULTIPLICADOR);
  SensorVoltaje  ->AjusteAmplitud(analogRead(P_POT2)*MULTIPLICADOR);
}


////////////////////////////////////
////                            ////
////       Mapea Sensores       ////
////                            ////
////////////////////////////////////
void APAGA_CAMBIOS(){
  digitalWrite(P_LED_A,LOW);
  digitalWrite(P_LED_C,LOW);
}


////////////////////////////////////
////                            ////
////           Reset            ////
////                            ////
////////////////////////////////////
void RESET(){
  digitalWrite(P_LED_R,HIGH);
  digitalWrite(P_LED_A,LOW);
  digitalWrite(P_LED_C,LOW);
  SensorCorriente->AjusteAmplitud(RESOLUCION);
  SensorVoltaje  ->AjusteAmplitud(RESOLUCION);
}


////////////////////////////////////
////                           /////
////       Imprime Info        /////
////                           /////
////////////////////////////////////
//Imprime los valores en el monitor serial
void IMPRIME_INFO(){
   Serial.print("Corriente RMS = ");
   Serial.print(Irms,2);
   Serial.print("          Voltaje RMS = ");
   Serial.println(Vrms,2);
}

////////////////////////////////////
////                           /////
////      Imprime Grafica      /////
////                           /////
////////////////////////////////////
void IMPRIME_GRAFICA(){
  Serial.print(I_Dato);
  Serial.print(",");
  Serial.print(V_Dato);
  Serial.print(",");
  Serial.print(Cota_Max);
  Serial.print(",");
  Serial.println(Cota_Min);

}
