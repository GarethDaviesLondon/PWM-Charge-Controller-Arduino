#include <Arduino.h>
#include "PWMLibs.h"

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



//////////////////////////////////////////////////////////////////////////////////////////////
//
//  Class for sensing voltages from Arduino
//  Understands the voltage divider resistors to give a scaled output
//  Note yet implemented is a read under PWM conditions for the battery
/////////////////////////////////////////////////////////////////////////////////////////////


VoltageSensor::VoltageSensor (int Pin, float FullV,int HighR, int LowR)
{
  Readpin = Pin;
  Highside = HighR;
  Lowside = LowR;
  FullScaleR = FullV;
  PossOver = false;

  //This determines calculates the voltage seen across the Potential Divider
  AcrossLowR = (FullScaleR * Lowside) / (Lowside+Highside);

  //Check if there is a potential that there will be more than 5 volts on the analogue pin
  if (AcrossLowR > 5)
  {
    PossOver = true;
  }

  LowRangeConvRatio = AcrossLowR / 1023;    //This is the ratio that will be needed to convert back to volts from a 0-1023 A-D scale
  FullRangeConvRatio = FullScaleR /1023;     //This is the ratio for the full voltage
  
}

void VoltageSensor::takeReading (void)
{
  ADReading = analogRead(Readpin);
  LowR = ADReading * LowRangeConvRatio; 
  FullScaleR = ADReading * FullRangeConvRatio;
}

bool VoltageSensor::PossOverVolts (void)
{
  return (PossOver);
}

float VoltageSensor::volts (void)
{
  takeReading();
  return (FullScaleR);
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

