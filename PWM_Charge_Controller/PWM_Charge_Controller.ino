
  /*
   * Based on tutorial from Julian Ilett 
   * https://www.youtube.com/watch?v=D826h-YQun4
   */

//These are the PWM pins for the charge controller
//They come from timer 2 on the Arduino
#define CHARGEPUMP_PWM_A 11 //Pin generating the charge pump PWM
#define CHARGEPUMP_PWM_B 3  //Pin generating the inverse charge pump PWM
#define CHARGEWAVEFORM 8    //This is the pin generating the charge waveform


void setup() {
  Serial.begin(9600);     //Enable serial monitor line

////////////////////////////////
//
//    Set up the PWM inverting pins and frequency
//
///////////////////////////////

  TCCR2A = TCCR2A | 0x30; //This sets bits 4&5 to 1 which puts the time 2 
                          //Into inverting mode.
                          //Timer 2 controls pins 11 and 3. 11 Being on A, and 3 on B
  TCCR2B = TCCR2B & 0xF8 | 0x1; // Max frequency, of abou 30Khz.
  
  
  pinMode(CHARGEPUMP_PWM_A,OUTPUT);
  pinMode(CHARGEPUMP_PWM_B,OUTPUT);  
  analogWrite(CHARGEPUMP_PWM_A,117);  //Less than (127) 50% duty cycle for non overlapping
  analogWrite(CHARGEPUMP_PWM_B,137);
////////

  pinMode(CHARGEWAVEFORM,OUTPUT);

}

void loop() {

  digitalWrite(CHARGEWAVEFORM,HIGH);
  delay(500);
  digitalWrite(CHARGEWAVEFORM,LOW);
  delay(500);

}
