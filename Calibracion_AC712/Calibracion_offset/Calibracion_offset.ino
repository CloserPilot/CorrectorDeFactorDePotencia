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



void setup() {
  Serial.begin(115200);
}

void loop() {
  SensorVoltaje = 0;
  SensorCorriente = 0;
  contador = 0;
  T_ini = millis();
  
  while(  (millis()-T_ini) < T_MUESTREO){
    SensorCorriente += analogRead(P_CORRIENTE)*CONSTANTE_ADC; //Mapeo del sensor
    SensorVoltaje   += analogRead(P_VOLTAJE)  *CONSTANTE_ADC; //Mapeo del sensor
    contador++;
  }

  SensorCorriente = SensorCorriente/contador;
  SensorVoltaje   = SensorVoltaje/contador;
  IMPRIME_INFO();
}


void IMPRIME_INFO(){
  //Serial.print("Corriente: ");
  //Serial.print(SensorCorriente,3);  //Voltaje a 3 decimales 
  Serial.print(" Corriente int: ");
  Serial.print(SensorCorriente/CONSTANTE_ADC,1);
  
  Serial.print("   |   ");

  //Serial.print("Voltaje: ");
  //Serial.print(SensorVoltaje,3);  //Voltaje a 3 decimales 
  Serial.print(" Voltaje int: ");
  Serial.println(SensorVoltaje/CONSTANTE_ADC,1);
}
