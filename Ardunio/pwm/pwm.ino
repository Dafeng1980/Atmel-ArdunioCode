const byte led = 0;
const byte wave = 13;


void setup() {

  TCCR1B = TCCR1B&B11111000|B00000010;
  
  pinMode(led, OUTPUT);
  pinMode(wave,OUTPUT);
  // put your setup code here, to run once:
  analogWrite(wave, 128);
}

void loop() {

  digitalWrite(led,HIGH);
  delay(1500);
  digitalWrite(led,LOW);
  delay(1500);
  // put your main code here, to run repeatedly:

}
