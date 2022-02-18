float rms=0; // varible para la media de cuadrados
float Vout = 0;  //Lectura del puerto
float sum_volt = 0;
int n = 0;
float offset = 2.505;


int N_ciclos = 10;  //Numero de ciclos para realizar el valor RMS
float frecuencia = 60.0;
unsigned long t_ini;  //Variable para controlar el tiempo de muestreo
unsigned long tiempo;
float aux_tiempo;


void setup(){
  Serial.begin (9600);
  aux_tiempo = (float)((1/frecuencia)*N_ciclos*1000);
  tiempo = (unsigned long)(aux_tiempo);
  Serial.println(aux_tiempo);
}

void loop(){
  t_ini = millis();
  sum_volt = 0;
  n = 0;

  while((millis()-t_ini) < tiempo){
    Vout = analogRead(A0)*(5.0/1023.0);
    sum_volt += pow( Vout -offset , 2);  //Suma de los cuadrados
    n++;
  }
 
  rms = sqrt(sum_volt/n);
  Serial.print("Valor rms: ");
  Serial.println(rms,4);
  delay(500);
}
