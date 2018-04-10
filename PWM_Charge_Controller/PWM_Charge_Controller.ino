
  /*
   * Based on tutorial from Julian Ilett 
   * https://www.youtube.com/watch?v=D826h-YQun4
   */

void setup() {
  Serial.begin(9600);     //Enable serial monitor line

////////////////////////////////
//
//    Set up the PWM inverting pins
//
///////////////////////////////

  TCCR2A = TCCR2A | 0x30; //This sets bits 4&5 to 1 which puts the time 2 
                          //Into inverting mode.
                          //Timer 2 controls pins 11 and 3. 11 Being on A, and 3 on B
  pinMode(11,OUTPUT);
  pinMode(3,OUTPUT);
///////////////////////////////////////
  
  analogWrite(11,127);
  analogWrite(3,127);

}

void loop() {


}
