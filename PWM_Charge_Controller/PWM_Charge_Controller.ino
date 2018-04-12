
#include <Arduino.h>
#include "PWM_Charge_Controller.h"
#include "PWMLibs.h"

///GLOBALS
 ChargePumpPWM Mosfet_Highside (CHARGEPUMP_PWM_A,CHARGEPUMP_PWM_B); //Definitions of pins are found in PWM_Charge_Controller.h


void WakeUp()
//This starts up the controller ready to work.
{
    //Startup the Charge Pump PWM signal
    Mosfet_Highside.On();
}

void Sleep()
//This puts the Arduino into a low power mode, shuts of the charging circuit.
{
    Mosfet_Highside.Off();
}



///////////////////////////////////////////////////
////////////////MAIN ARDUINO CODE SKETCH ///////////
///////////////////////////////////////////////////

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




