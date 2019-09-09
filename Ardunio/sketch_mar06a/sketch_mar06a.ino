
void setup() {
   Serial.begin(9600,SERIAL_8N1);
  // put your setup code here, to run once:
 delay(1000);
}

void loop() {

  Serial.write(0x0e);
   delay(10);
  Serial.print(210);
  delay(10);

}
