#define unchar  uint8_t
#define unint   uint16_t

// Const Define 
#define  DMX_CHANNEL_NUMBER   512

#define  WAIT_RECEIVE_DMX     0
#define  DMX_SYNC_OK          1
#define  RECEIVE_DMX_DATA     2
#define  DMX_RECEIVE_OK       3 

// Buffer Define 
unchar dmx_receive_buf[520];

// Variable Define 
unchar dmx_receive_course;
unint  dmx_receive_point;

// Function Body *********************************************************/
void INIT_Device_Initial(void)
{
//...

// SeriesPortInitial:  
// Baud:250kbps@16MHz, No Parity
// Data Struct: StartBit(0) - D0~D7 - RB8 - StopBit(1),  (0: Lo-Level, 1: Hi_Level)
UCSR1A = 0x00;
UCSR1C = 0x86;  // ASYNC Communication, No Parity 
UBRR1L = 0x03;  // set baud rate lo
UBRR1H = 0x00;  // set baud rate hi
UCSR1B = 0x94;  // Enable RXCIE,RXEN

dmx_receive_course = WAIT_RECEIVE_DMX;

//...
}

/* Interrupt Serve Sub-Program *******************************************/
SIGNAL(SIG_USART1_RECV)
{
unchar tmp_rb8;
unchar tmp_receive_data;

//ReadSeriesRegister:
tmp_rb8          = UCSR1B;     
tmp_receive_data = UDR1; 

tmp_rb8 &= 0x02;
if(tmp_rb8 == 0)
  {
// STEP 1: Check DMX Frame Sync
   dmx_receive_course = DMX_SYNC_OK;
   return;
  }

switch(dmx_receive_course)
  {
   case DMX_SYNC_OK:
// STEP 2: FirstDataCheck: TB8 = 1 & Data = 0x00
        if(tmp_receive_data == 0x00)
         {
          dmx_receive_course = RECEIVE_DMX_DATA;
          dmx_receive_point  = 0;
         }
        break;
   case RECEIVE_DMX_DATA:
// STEP 3: NormalReceiveDMXData:
        if(dmx_receive_point < DMX_CHANNEL_NUMBER)
         {
          dmx_receive_buf[dmx_receive_point] = tmp_receive_data;
          dmx_receive_point ++;
         }
        else
         {
          dmx_receive_course = DMX_RECEIVE_OK;
         }
        break;
   default: break;
  }
}
