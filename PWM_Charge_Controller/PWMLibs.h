//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Charge pump class handles the initialisation of the charge pump
//  Sets the registers etc.
//  Based on tutorial from Julian Ilett 
//  https://www.youtube.com/watch?v=D826h-YQun4
//
//////////////////////////////////////////////////////////////////////////////////////////////////

class ChargePumpPWM {

  public:
        ChargePumpPWM (int, int);
        void On (void);
        void Off (void);
  private:
        int OutA;
        int OutB;
};


