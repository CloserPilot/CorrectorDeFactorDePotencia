class Filtro{
  private:
    float Lp = 0.0;
    float ALPHA;
    float AmpliPot = 1.0;
  
  public:
    //Constructor
    Filtro(const float ALPHA){
      this->ALPHA = ALPHA;
    }

    float Filtrado(float value){
      return Lp = AmpliPot*ALPHA * value + (1 - ALPHA*AmpliPot) * Lp;
    }

    void AjustePot(float value){
      AmpliPot = value;
    }
};
