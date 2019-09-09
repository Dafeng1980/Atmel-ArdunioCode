#include <avr/wdt.h>
enum period_t
{
  SLEEP_15MS,
  SLEEP_30MS,
  SLEEP_60MS,
  SLEEP_120MS,
  SLEEP_250MS,
  SLEEP_500MS,
  SLEEP_1S,
  SLEEP_2S,
  SLEEP_4S,
  SLEEP_8S,
  SLEEP_FOREVER
};

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}


void PowerDown(period_t period)
{
    ADCSRA &= ~(1 << ADEN);

  if (period != SLEEP_FOREVER)
  {
    wdt_enable(period);
    WDTCSR |= (1 << WDIE);
  }
      set_sleep_mode(SLEEP_MODE_PWR_DOWN); 
      cli();        
      sleep_enable();   
      sleep_bod_disable(); 
      sei();        
      sleep_cpu();      
      sleep_disable();    
      sei();        
  ADCSRA |= (1 << ADEN);
}
