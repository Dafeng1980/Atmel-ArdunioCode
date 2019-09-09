 int light = 1;
 int led = 6;
 int Recevied = 0;
 int light_state = 0;
void setup() {
 Serial1.begin(9600);
 pinMode(light, OUTPUT);
 pinMode(led, OUTPUT);
 
}

void loop() {
  if (Serial1.available()>0)
  {
  Recevied = Serial1.read();
  }
  // LIGHT////
  if (light_state == 0 && Recevied == '1')
  {
        digitalWrite(light,HIGH);
        Recevied = 0;
        light_state = 1;
}
if (light_state == 1 && Recevied == '1')
{
  digitalWrite(light,LOW);
  Recevied = 0;
  light_state = 0;
}
if (Recevied == '8')
{
  digitalWrite(led,HIGH);
  }
  if (Recevied == '9')
  {
    digitalWrite(led,LOW);
}
}

