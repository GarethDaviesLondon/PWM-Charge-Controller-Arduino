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

///////////////////////////////////////////////////////////////////////////////////////////////
//END Class ChargePumpPWM
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
//
//  Class for Charging PWM waveform control. This implements the various waveforms based on 
//  Requirements for PWM function of the charge waveform.
//
/////////////////////////////////////////////////////////////////////////////////////////////

class PWM {
  private:
        int state;  //This provides a record of it's current charge configuration
        void ImplementWaveForm (int desiredState);
  public:
        void chargeFull (void);
        void chargeOff (void);
        void chargeTrickle (int targetVoltage, int currentVoltage);
        void Suspend (void);
        void UnSuspend (void);
        bool isTrickle(void);
        bool isOff(void);
        bool isFull(void);
};

///////////////////////////////////////////////////////////////////////////////////////////////
//END Class PWM
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
//
//  Class for sensing voltages from Arduino
//  Understands the voltage divider resistors to give a scaled output
//  Note yet implemented is a read under PWM conditions for the battery
/////////////////////////////////////////////////////////////////////////////////////////////


class VoltageSensor {

   private:
          int Highside;
          int Lowside;
          int Readpin;
          float FullScaleR;
          float AcrossLowR;
          float LowRangeConvRatio;
          float FullRangeConvRatio;
          bool PossOver;
          float LowR;
          float FullScaledReading;
          int ADReading;

          void takeReading(void);
   public:
          float volts (void);
          bool PossOverVolts (void);
          float LowReading (void);
          int ADValue (void);
          VoltageSensor (int , float ,int , int );
};

