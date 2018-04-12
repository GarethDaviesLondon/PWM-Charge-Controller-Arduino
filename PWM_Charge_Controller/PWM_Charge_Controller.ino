
#define DEBUG   //Used to trigger debug loops and communications etc. Set to false for prodcution mode

#include <Arduino.h>
#include "PWM_Charge_Controller.h"
#include "PWMLibs.h"

///GLOBALS
ChargePumpPWM Mosfet_Gate_Driver (CHARGEPUMP_PWM_A,CHARGEPUMP_PWM_B); //Definitions of pins are found in PWM_Charge_Controller.h
VoltageSensor VBat (A0,5.0,680,220);
VoltageSensor VSolar(A1,10.0,680,220);
ChargePWM Charger(8);

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

  
 VBat.volts();
 delay(WAIT_TIME);
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




