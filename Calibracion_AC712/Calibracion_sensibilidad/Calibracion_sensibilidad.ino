////////////////////////
/////              /////
/////   SENSORES   /////
/////              /////
////////////////////////
const float   RESOLUCION      = 1024.0;
const float   VOLTAJE_MAXIMO  = 5.0;
const float   CONSTANTE_ADC   = VOLTAJE_MAXIMO/RESOLUCION;

float         SensorVoltaje;
float         SensorCorriente;
float         Vrms;
float         Irms;
float         V_OFFSET        = 514.2;
float         I_OFFSET        = 511.5;

const int     T_MUESTREO      = 1000;  //Tiempo de muestreo (milis)
unsigned long T_ini;
int           contador;

////////////////////////
////                ////
////   PUERTOS AD   ////
////                ////
////////////////////////
const int P_CORRIENTE = A0;   //A0
const int P_VOLTAJE   = A1;   //A1


void setup(){
  Serial.begin (115200);
}

void loop(){
  Vrms = 0.0;
  Irms = 0.0;
  SensorCorriente = 0.0;
  contador = 0;
  
  T_ini = millis();

  while( (millis()-T_ini) < T_MUESTREO){
    SensorCorriente = (analogRead(P_CORRIENTE)-I_OFFSET)*CONSTANTE_ADC; // Vaux = (Vi-offset) [V]
    SensorVoltaje   = (analogRead(P_VOLTAJE)  -V_OFFSET)*CONSTANTE_ADC; // Vaux = (Vi-offset) [V]
    
    Irms += pow(SensorCorriente , 2);  // Irms  += (Vi-offset)^2
    Vrms += pow(SensorVoltaje   , 2);  // Vsuma += (Vi-offset)^2
    contador++;
  }

  Irms = sqrt(Irms/contador);
  Vrms = sqrt(Vrms/contador);
  IMPRIME_INFO();
}


void IMPRIME_INFO(){
  //Serial.print("Corriente: ");
  //Serial.print(SensorCorriente,3);  //Voltaje a 3 decimales 
  Serial.print(" Corriente: ");
  Serial.print(Irms,3);
  
  Serial.print("   |   ");

  //Serial.print("Voltaje: ");
  //Serial.print(SensorVoltaje,3);  //Voltaje a 3 decimales 
  Serial.print(" Voltaje: ");
  Serial.println(Vrms,3);
}
