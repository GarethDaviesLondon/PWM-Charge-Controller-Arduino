
#define DELAY 10

void setup() {
  Serial.begin(9600);
  pinMode(1,OUTPUT);
  pinMode(2,OUTPUT);
}

void loop() {

  digitalWrite(2,LOW);
  digitalWrite(1,HIGH); 
  delay(DELAY);
  digitalWrite(1,LOW);
  digitalWrite(2,HIGH);
  delay(DELAY);
}
