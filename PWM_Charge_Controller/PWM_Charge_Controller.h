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
