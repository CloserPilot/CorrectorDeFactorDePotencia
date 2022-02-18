#include "Filtro.h"

class Sensor : private Filtro{
  private:
    float OFFSET;         //Offset
    float PENDIENTE;      //Pendiente
    float RESOLUCION;     //Resolucion ~1024
    float CONSTANTE_ADC;  //Constante adicional para el ADC = VolMax/Resolucion


    float DatoCrudo;              
    float DatoFiltrado;           
    float RelacionPot = 1.0;  //Valor entre 0 y 1 para variar la amplitud final de la señal con el potenciometro


  public:
    Sensor(const float OFFSET, const float PENDIENTE, const float RESOLUCION, const float VoltajeMaximo, const float ALPHA):Filtro(ALPHA){
      this->OFFSET = OFFSET;
      this->PENDIENTE = PENDIENTE;
      this->RESOLUCION = RESOLUCION;

      CONSTANTE_ADC = VoltajeMaximo/RESOLUCION;
    }

    float MapeoYFiltrado(int value){
      DatoCrudo = ( float(value)-OFFSET ) /PENDIENTE; //X = (Vi-offset)/m
      DatoCrudo *= CONSTANTE_ADC;                     //X = (Vi-offset)/m * [ConstanteADC]
      DatoCrudo *= RelacionPot;                       //X * Potenciometro

      DatoFiltrado = Filtrado(DatoCrudo);
      return DatoFiltrado;
    }

    void AjusteAmplitud(int value){
      RelacionPot = float(value)/RESOLUCION;
    }

    void AjusteFiltro(int value){
      AjustePot(float(value)/RESOLUCION);
    }
};
