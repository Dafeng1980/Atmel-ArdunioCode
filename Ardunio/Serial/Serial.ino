void setup() {
  // put your setup code here, to run once:
//Serial.begin(9600);








pinMode(0, OUTPUT);
digitalWrite(0, LOW);
pinMode(3, OUTPUT);
digitalWrite(3, LOW);

delay(1500);

digitalWrite(0, HIGH);
//digitalWrite(3, HIGH);



}

void loop() {
  
  digitalWrite(0,LOW);
  delay(350);
  digitalWrite(0,HIGH);
  delay(550);
  // if (Serial.available())
   //  Serial.write(Serial.read());
    
  // put your main code here, to run repeatedly:

}
