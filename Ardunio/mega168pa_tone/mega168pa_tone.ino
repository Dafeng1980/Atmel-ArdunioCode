void setup() {
  // put your setup code here, to run once:

}
const uint8_t scale[] PROGMEM = {239,226,213,201,190,179,169,160,151,142,134,127};

void note (int n, int octave) {
  DDRD = DDRD | 1<<DDD3;                     // PD3 (Arduino D3) as output
  TCCR2A = 0<<COM2A0 | 1<<COM2B0 | 2<<WGM20; // Toggle OC2B on match
  int prescaler = 9 - (octave + n/12);
  if (prescaler<3 || prescaler>6) prescaler = 0;
  OCR2A = pgm_read_byte(&scale[n % 12]) - 1;
  TCCR2B = 0<<WGM22 | prescaler<<CS20;
}
void loop() {
  note(0, 4);
  delay(1000);
  // put your main code here, to run repeatedly:

}
