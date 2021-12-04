#include "Sensor.h"
#include "math.h"
////////////////////////
////                ////
////    SENSORES    ////
////                ////
////////////////////////
//Para los sensores
const float RESOLUCION      = 1024.0; //2^Nbits
const float VOLTAJE_MAXIMO  = 5.0;
const float FILTRO_ALPHA    = 0.86;    //Constante alpha del filtro

Sensor*     SensorCorriente;
const float I_OFFSET        = 516;
const float I_PENDIENTE     = 0.180;
float       I_Dato;           

Sensor*     SensorVoltaje;  
const float V_OFFSET        = 515;
const float V_PENDIENTE     = 0.03308;
float       V_Dato;  
        

////////////////////////
////                ////
////   RMS y FP     ////
////                ////
////////////////////////
float         Irms;
float         Vrms;

float         Preal;
float         Paparente;
float         FP;
float         phi;

float         RMS_contador;

const int     RMS_t_mues  = 1000;
unsigned long RMS_t_ini;


////////////////////////
////                ////
////      MENU      ////
////                ////
////////////////////////
const int OP_RMS_FP     = 80; //'P'
const int OP_GRAFICA    = 71; //'G'
const int OP_COTA       = 67; //'C'
const int OP_AMPLI_POT  = 65; //'A'
const int OP_RESET      = 82; //'R'
const int OP_DELAY      = 68; //'D'
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
float       Cota_Max      = 0;
float       Cota_Min      = 0;
const int   MULTIPLICADOR = 10;  
const int   NO_COTA       = 0;
const int   SI_COTA       = 1;
int         Retraso       = 0;
const int   RETRASO       = 100;
boolean     Activa_Retraso= false; 

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
    
    case OP_RMS_FP:
      OBTENER_RMS_FP();
      IMPRIME_INFO();
    break;
    
    case OP_GRAFICA:
      APAGA_CAMBIOS();
      MAPEA_SENSORES();
      IMPRIME_GRAFICA(NO_COTA);
    break;

    case OP_COTA:
      CAMBIA_COTA();
      MAPEA_SENSORES();
      IMPRIME_GRAFICA(SI_COTA);
    break;
    
    case OP_AMPLI_POT:
      CAMBIA_AMPLITUD();
      MAPEA_SENSORES();
      IMPRIME_GRAFICA(NO_COTA);
    break;

    case OP_DELAY:
      Activa_Retraso?Retraso = 0: Retraso = RETRASO;
      Activa_Retraso = !Activa_Retraso;
      OP_opcion = OP_GRAFICA;
    break;
    
    case OP_RESET:
      RESET();
      OP_opcion = OP_GRAFICA;
    break;
  }
}




////////////////////////////////////
////                            ////
////     Valores RMS y FP       ////
////                            ////
////////////////////////////////////
void OBTENER_RMS_FP(){
  Irms = Vrms = RMS_contador = 0;
  Preal = 1.0;
  
  RMS_t_ini = millis();

  while( (millis()-RMS_t_ini) < RMS_t_mues){
    MAPEA_SENSORES();
    
    Irms += pow(I_Dato, 2);   // Irms += [(Vi-offset)/m] ^2
    Vrms += pow(V_Dato, 2);   // Vrms += [(Vi-offset)/m] ^2

    Preal += I_Dato*V_Dato;   //  Preal += I(t)*V(t)

    RMS_contador++;
  }

  Irms = sqrt(Irms/RMS_contador);
  Vrms = sqrt(Vrms/RMS_contador);

  Preal = Preal/RMS_contador;
  Paparente = Vrms*Irms;
  FP = Preal/Paparente;
  phi = acos(FP)*360/(2*PI);
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
////        Apaga Cambios       ////
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
  Serial.print("Irms = ");
  Serial.print(Irms,2);
  Serial.print("|   Vrms = ");
  Serial.print(Vrms,2);
  Serial.print("|   PReal = ");
  Serial.print(Preal,2);
  Serial.print("|   PAparente = ");
  Serial.print(Paparente,2);
  Serial.print("|   FP = ");
  Serial.print(FP,2);
  Serial.print("|   Angulo = ");
  Serial.println(phi);
}

////////////////////////////////////
////                           /////
////      Imprime Grafica      /////
////                           /////
////////////////////////////////////
void IMPRIME_GRAFICA(int COTA){
  Serial.print(I_Dato,4);
  Serial.print(",");
  
  if(COTA == NO_COTA)
    Serial.println(V_Dato); 
  else{
    Serial.print(V_Dato); 
    Serial.print(",");
    Serial.print(Cota_Max);
    Serial.print(",");
    Serial.println(Cota_Min);  
  }

  delay(Retraso);
}
