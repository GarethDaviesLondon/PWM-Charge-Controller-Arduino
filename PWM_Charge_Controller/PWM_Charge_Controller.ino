
#define DEBUG   //Used to trigger debug loops and communications etc. Set to false for prodcution mode
#include <Arduino.h>
#include <LowPower.h> //Available from https://github.com/rocketscream/Low-Power
#include "PWM_Charge_Controller.h"
#include "PWMLibs.h"

///////////////////////////////////////////////////////////////
//CONSTANTS SUCH AS TARGET VOLTAGE, RESISTOR POTENTIAL DIVIDERS
//ETC ARE DEFINED IN PWM_Charge_Controller.h
//////////////////////////////////////////////////////////////

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

  doChargeWake();
}

void loop()
{
  ChargeLoop();
  // If we've returned then we're undervoltage detected between Solar and Battery.
  PauseLoop();
}

////////////////////////////////////////////////////////////////
//The Pause loop is called when the solar voltage is too low
//This puts a longer delay between sampling
//And makes calls to reduce power consumption of the board
///////////////////////////////////////////////////////////////

void PauseLoop()
{
  doChargeSleep();

  while (true) // loop until break.
  {
    BatVoltage = VBat.volts();
    SolarVoltage = VSolar.volts();
    
#ifdef DEBUG   
      Serial.print(" Pause Loop: Battery ");
      Serial.print(BatVoltage);
      Serial.print("V Solar ");
      Serial.print(SolarVoltage);
      Serial.println("V");
 #endif
    if (SolarVoltage < BatVoltage)
    {
#ifdef DEBUG
      Serial.println("Solar Voltage Low - Sleeping");
#endif
      doChargeSleep();
    }
    else
    {
      doChargeWake();
      return;
    }
  }
}

////////////////////////////////////////////////////////////////
// The Charge Loop implements the various charging strategies. The program
// Stays in this loop until the solar voltage drops then it returns to the main
// Program control.
// This function takes voltage readings from both solar and battery
// And depending on voltage vs. target (with hysterisis control) it decides
// What instruction to give the PWM waveform controller
/////////////////////////////////////////////////////////////////////////

void ChargeLoop() 
{  
 
 bool Hysteresis = false;
 while (true) // Loop forever.
 {
  
  //Suspend PWM Activity and take a voltage reading, then turn on again
#ifdef DEBUG   
      Serial.print("\nCharge Loop: Prep for Voltage Reading: ");
 #endif
   Charger.Suspend();;
   delay(100);
   BatVoltage = VBat.volts();
   SolarVoltage = VSolar.volts(); 
   Charger.Resume();
   
#ifdef DEBUG   
      Serial.print("Battery ");
      Serial.print(BatVoltage);
      Serial.print("V Solar ");
      Serial.print(SolarVoltage);
      Serial.print("V");
 #endif
   
   if (SolarVoltage < BatVoltage) return; //No Charging is possible;
 
    //Decide mode of charge based on battery voltage;

    //If the battery is more than 1.5V below target voltage then give it the beans.
    if (BatVoltage <= (TARGET -1.5)) 
    {
#ifdef DEBUG   
      Serial.print(" Battery low voltage full charge :");
 #endif
        Charger.chargeHardOn();
    } else {
        //If the battery is at max voltage then stop charge and set Hysteresis mode on
         if (BatVoltage >= TARGET) {
 #ifdef DEBUG   
      Serial.print(" Battery above target voltage ");
      Serial.print(TARGET);
      Serial.print ("V charge off :");
 #endif
             Charger.chargeOff();
             Hysteresis = true;
         } else {
           Hysteresis=doPWMwithHysteresis(Hysteresis);
         }
     }
    //This governs the time of the charge cycle.
    delay(WAIT_TIME);  //Do that for a while before trying again.
  }
}

////////////////////////////////////////////////////////////////
// This is called from the main charge loop
// Seperating this way makes the operation clearer
//
//This calls the PWM waveform to handle charging on the final bit
//
//Implements a Hysteresis so it backs off for a bit once the battery is fully charged
//Helps to prevent oscillations.The difference re-trigger voltage is set as an
//offset from TARGET with the constant HYSTGAP
//
/////////////////////////////////////////////////////////////////////////

bool doPWMwithHysteresis(bool H)
{


  bool Hysteresis = H;
  if (Hysteresis == false)
  {

#ifdef DEBUG   
      Serial.print(" No Hysteresis and below target voltage PWM ");
 #endif
      Charger.chargeTrickle(TARGET-BatVoltage+0.5); //The 0.5 is here to make sure the PWM doesn't fade away before reaching voltage
                                                    //The algorithm goes into Hysterisis mode at target so this is safe.
      return (Hysteresis); //bail back to calling function
  }
    ///ONLY GETS HERE IF THERE IS HYSTERESIS SET
    //If Hystersis then we've gone up to voltage so
    //turn off until there's a bit of a drop.
    //Then raise up to voltage again.
    #ifdef DEBUG
      Serial.print(" Hysteresis Mode");
    #endif
     if (BatVoltage < (TARGET-HYSTGAP))
     {
      Hysteresis = false;
 #ifdef DEBUG  
      Serial.print(" Battery Voltage Below ");
      Serial.print(TARGET-HYSTGAP);
      Serial.print("V Hysteresis has been reset :");
 #endif
       Charger.chargeTrickle(TARGET-BatVoltage);
     }
     else
     {
#ifdef DEBUG
      Serial.print(" Hysteresis means Charge off ");
#endif
          Charger.chargeOff();

     }
     return (Hysteresis);
}

/////////////////////////////////////////////////////////////////////////
//This puts the contoller into a low power mode, shuts of the charging circuit.
/////////////////////////////////////////////////////////////////////////

void doChargeSleep()
{
      pinMode(LED_BUILTIN, OUTPUT);
#ifdef DEBUG
      Serial.println("\nPreparing to sleep ");
      digitalWrite(LED_BUILTIN, LOW); //Turnoff the Board LED
      delay(250);
      digitalWrite(LED_BUILTIN, HIGH); //Turn on the Board LED
      delay(250);
      digitalWrite(LED_BUILTIN, LOW); //Turnoff the Board LED
      delay(250);
#endif
    digitalWrite(LED_BUILTIN, LOW); //Turnoff the Board LED
    Mosfet_Gate_Driver.Off(); //Turn off the charge Pump signals
    Charger.Suspend();        //stop the output PWM signal and save state
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  

}

/////////////////////////////////////////////////////////////////////////
//This wakes the controller up from  low power mode.
///////////////////////////////////////////////////////////////////////

void doChargeWake()
//This starts up the controller ready to work.
{
    
    Mosfet_Gate_Driver.On(); //Startup the Charge Pump PWM signal
    Charger.Resume();        //Go back to whatever it was doing before
#ifdef DEBUG
      Serial.println("\nWaking Up Charger ");
#endif
}






