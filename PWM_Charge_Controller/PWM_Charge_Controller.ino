

#define DEBUG TRUE  //Used to trigger debug loops and communications etc. Set to false for prodcution mode
#define WAIT_TIME 2000 //Used in testing PWM waveform generation

//These define the resistor values used in the voltage sensing potentiameters, this will be scaled to 1-5V
#define SOLARPOT_HIGHSIDE 820
#define SOLARPOT_LOWSIDE  220
#define BATTPOT_HIHGSIDE  680
#define BATTPOT_LOWSIDE   220

//These are the PWM pins for the charge controller
//They come from timer 2 on the Arduino
#define CHARGEPUMP_PWM_A 11 //Pin generating the charge pump PWM
#define CHARGEPUMP_PWM_B 3  //Pin generating the inverse charge pump PWM
#define CHARGEWAVEFORM 8    //This is the pin generating the charge waveform   

//These are constants used to configure the algorithm
#define CHARGE_LOW  0     //Returned when max charge is required
#define CHARGE_LOW_VOLTAGE 12.5
#define CHARGE_HIGH 1  //This is the voltage at fully charged
#define CHARGE_HIGH_VOLTAGE 14.0

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


//////////////////////////////////////////////////
//    Set up the PWM inverting pins and frequency
/////////////////////////////////////////////////

  ChargePumpPWM::ChargePumpPWM (int OutputA, int OutputB)
    {
      OutA = OutputA;
      OutB = OutputB;

      TCCR2A = TCCR2A | 0x30; //This sets bits 4&5 to 1 which puts the time 2 
                          //Into inverting mode.
                          //Timer 2 controls pins 11 and 3. 11 Being on A, and 3 on B
      TCCR2B = TCCR2B & 0xF8 | 0x1; // Max frequency, of abou 30Khz.
      pinMode(OutA,OUTPUT);
      pinMode(OutB,OUTPUT);  
    };

    void ChargePumpPWM::On (void)
    {
       analogWrite(OutA,117);  //Less than (127) 50% duty cycle for non overlapping
       analogWrite(OutB,137);
    }

     void  ChargePumpPWM::Off (void)
    {
       analogWrite(OutA,0);  //Sets output low
        analogWrite(OutB,255);  //Sets output low (NB It's inverting)
    }
///////////////////////////////////////////////////////////////////////////////////////////////
//END Class ChargePumpPWM
//////////////////////////////////////////////////////////////////////////////////////////////


///GLOBALS
 ChargePumpPWM Mosfet_Highside (CHARGEPUMP_PWM_A,CHARGEPUMP_PWM_B);



void setup() {

#ifdef DEBUG
  
      Serial.begin(9600);     //Enable serial monitor line
      
#endif
  
  pinMode(CHARGEWAVEFORM,OUTPUT);

}

void loop() {

  digitalWrite(CHARGEWAVEFORM,HIGH);
  delay(WAIT_TIME);
  digitalWrite(CHARGEWAVEFORM,LOW);
  delay(WAIT_TIME);

}

void WakeUp()
{
    //Startup the Charge Pump PWM signal
    Mosfet_Highside.On();
}


