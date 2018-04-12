#include <Arduino.h>
#include "PWMLibs.h"

///////////////////////////////////////////////////////////////////////////////////////////////
// Class ChargePumpPWM
//////////////////////////////////////////////////////////////////////////////////////////////

  ChargePumpPWM::ChargePumpPWM (int OutputA, int OutputB)
    {
      /////////////////////////////////////////////////
      //    Set up the PWM inverting pins and frequency
      /////////////////////////////////////////////////
      OutA = OutputA;
      OutB = OutputB;

      //
      //This bit of code is hardcoded to pins 11 and 3 across the timer.
      //So it  means that passing the pins as parameters doesn't make a lot of sense
      //
      
      TCCR2A = TCCR2A | 0x30; //This sets bits 4&5 to 1 which puts the time 2 
                          //Into inverting mode.
                          //Timer 2 controls pins 11 and 3. 11 Being on A, and 3 on B
      TCCR2B = TCCR2B & 0xF8 | 0x1; // Max frequency, of about 30Khz.

      
      pinMode(OutA,OUTPUT);
      pinMode(OutB,OUTPUT);  
      #ifdef DEBUG
        serial.print("Pump Charge PWM Signal Establised on Pins");
        serial.print(OutA);
        serial.print(" and ");
        serial.println(OutB);
      #endif
    };

    void ChargePumpPWM::On (void)
    {
       analogWrite(OutA,117);  //Less than (127) 50% duty cycle for non overlapping
       analogWrite(OutB,137);
        #ifdef DEBUG
          serial.println("ON: Pump Charge PWM Signal turned on");
        #endif
    }

     void  ChargePumpPWM::Off (void)
    {
       analogWrite(OutA,0);  //Sets output low
       analogWrite(OutB,255);  //Sets output low (NB It's inverting)
       #ifdef DEBUG
          serial.println("OFF: Pump Charge PWM Signal turned OFF");
       #endif
    }
///////////////////////////////////////////////////////////////////////////////////////////////
//END Class ChargePumpPWM
//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////
//
//  VoltageSensor Class
//  for sensing voltages from Arduino
//  Understands the voltage divider resistors to give a scaled output
//  Note yet implemented is a read under PWM conditions for the battery
/////////////////////////////////////////////////////////////////////////////////////////////

VoltageSensor::VoltageSensor (int Pin, float FullV,int HighR, int LowR)
{
  Readpin = Pin;
  Highside = HighR;
  Lowside = LowR;
  FullScale = FullV;
  PossOver = false;

  //This determines calculates the voltage seen across the Potential Divider
  AcrossLowR = (FullScale * Lowside) / (Lowside+Highside);

 

  LowRangeConvRatio = AcrossLowR / 1023;    //This is the ratio that will be needed to convert back to volts from a 0-1023 A-D scale
  FullRangeConvRatio = FullScale /1023;     //This is the ratio for the full voltage

   #ifdef DEBUG
    serial.print("\nVoltage Sensor Initialised : Pin");
    serial.print(Readpin);
    serial.print(" Max Voltage ");
    serial.print(FullScale);
    serial.print(" POT resistor H ");
    serial.print(Highside);
    serial.print(" L ");
    serial.print(Lowside);
    serial.print(" Lowside Conversion ");
    serial.print(LowRangeConvRatio);
    serial.print(" FullS Conversion ");
    seral.println(FullRangeConvRatio);
  #endif

  
   //Check if there is a potential that there will be more than 5 volts on the analogue pin
  if (AcrossLowR > 5)
  {
    PossOver = true;
    #ifdef DEBUG
      serial.println("**** OverVoltage Possible : Warning Flag Set");
    #endif
  }
}

void VoltageSensor::takeReading (void)
{
  ADReading = analogRead(Readpin);
  LowR = ADReading * LowRangeConvRatio; 
  FullScaledReading = ADReading * FullRangeConvRatio;
  
  #ifdef DEBUG
    serial.print("\nVoltage Reading Taken: Pin");
    serial.print(Readpin);
    serial.print(" AD Value ");
    serial.print(ADReading);
    serial.print(" Full V ");
    serial.print(FullScaleReading);
    serial.print(" V across divider ");
    seral.println(LowR);
  #endif
}

bool VoltageSensor::PossOverVolts (void)
{
  return (PossOver);
}

float VoltageSensor::volts (void)
{
  takeReading();
  return (FullScaledReading);
}

float VoltageSensor::LowReading (void)

{
  takeReading();
  return (LowR); 
}

int VoltageSensor::ADValue (void)
{
  takeReading();
  return (ADReading);
}

