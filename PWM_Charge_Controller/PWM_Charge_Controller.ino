
#define DEBUG   //Used to trigger debug loops and communications etc. Set to false for prodcution mode

#include <Arduino.h>
#include "PWM_Charge_Controller.h"
#include "PWMLibs.h"

///GLOBALS
ChargePumpPWM Mosfet_Gate_Driver (CHARGEPUMP_PWM_A,CHARGEPUMP_PWM_B); //Definitions of pins are found in PWM_Charge_Controller.h
VoltageSensor VBat (A0,BATTPOT_HIHGSIDE,BATTPOT_LOWSIDE);
VoltageSensor VSolar(A1,SOLARPOT_HIGHSIDE,SOLARPOT_LOWSIDE);
ChargePWM Charger(CHARGEWAVEFORM);

float BatVoltage;
float SolarVoltage;

///////////////////////////////////////////////////
////////////////MAIN ARDUINO CODE SKETCH ///////////
///////////////////////////////////////////////////

void setup() {

#ifdef DEBUG
      Serial.begin(9600);     //Enable serial monitor line
      Serial.println("Debug Enabled");
#endif

  WakeUp();
}

void loop() 
{  

 //Suspend all PWM Activity and take a voltage reading, then turn on againg
 bool Hysteresis = false;
 float target = 13.5;
 float HystGap = 0.5;
 Charger.Suspend();
 BatVoltage = VBat.volts();
 SolarVoltage = VSolar.volts();
 Charger.Resume();

 //Decide mode of charge based on battery voltage;

//If the battery is more than 1.5V below target voltage then give it the beans.
 if (BatVoltage <= (target -1.5)) Charger.chargeHardOn();

 //If the battery is at max voltage then stop charge and set Hysteresis mode on
 if (BatVoltage >= target) {
          Charger.chargeOff();
          Hysteresis = true;
 }
 

//This gives it a PWM waveform to handle charging on the final bit
//Implements a Hysteresis so it backs off for a bit once the battery is fully charged
//Helps to prevent oscillations
 
 if (BatVoltage > (target-1.5)) 
 {
  if (Hysteresis == true)
  {
    //If Hystersis then we've gone up to voltage so
    //turn off until there's a bit of a drop.
    //Then raise up to voltage again.
    
    #ifdef DEBUG
      Serial.print("Hysteresis Mode");
    #endif
     if (BatVoltage < (target-HystGap))
     {
      Charger.chargeTrickle(target-BatVoltage);
      Hysteresis = false;
 #ifdef DEBUG   
      Serial.println("Hysteresis BackOn");
 #endif
     }
     else
     {
    #ifdef DEBUG
      Serial.println("Hysteresis Skip");
    #endif
    Charger.chargeOff();
     }
  } else {
    //No Hysteresis so keep on charging
    if (BatVoltage < (target))
     {
      Charger.chargeTrickle(target-BatVoltage);
#ifdef DEBUG   
      Serial.println("No Hysteresis and below target voltage");
 #endif
     }

  }
 }

 //This governs the time of the charge cycle.
 delay(WAIT_TIME);  //Do that for a while before trying again.
}

void WakeUp()
//This starts up the controller ready to work.
{
    //Startup the Charge Pump PWM signal
    Mosfet_Gate_Driver.On();
}

void Sleep()
//This puts the Arduino into a low power mode, shuts of the charging circuit.
{
    Mosfet_Gate_Driver.Off();
}




