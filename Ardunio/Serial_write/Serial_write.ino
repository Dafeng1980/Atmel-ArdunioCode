int incomingByte = 0;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  delay(500);
}

void loop() {

     if (Serial.available() > 0) {

      incomingByte = Serial.read();
      Serial.print("I received: ");
      Serial.println(incomingByte, HEX);
     }
  // put your main code here, to run repeatedly:

}
