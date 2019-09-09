void setup() {
  pinMode(7,OUTPUT);
   pinMode(13,OUTPUT);
  // put your setup code here, to run once:
delay(30);
 digitalWrite(13,HIGH);
 delay(50);
}

void loop() 
{
  digitalWrite(7,HIGH);
  //delayMicroseconds(10);
 // delay(0.1);
  digitalWrite(7,LOW);
  //delayMicroseconds(10);
  //delay(0.1);
  
}
