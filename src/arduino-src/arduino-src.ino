/*******************************************************************************
Crab Tracker - Arduino Code

Reads in data from the pins (attached to Hydrophones) and transfers that data
via SPI to another device.

Data is sent in a specific sequence with SPI. Each time a pin state changes, we
store a new set of pin values along with a timestamp for when the change
occurred. The `pinval` is an 8-bit integer (though only the lower 5 bits
correspond to actual receivers/hydrophones). The timestamp is a 32-bit `long`.
By default, SPI only sends 8 bits at a time. For simplicity's sake, we will only
ever send 8 bits at a time. This means that we must send the data in chunks,
like so:
    [pinvals, timestamp_part1, timestamp_part2, timestamp_part3, timestamp_part4]
where each `timestamp_part` is only 8 of the 32 bits in the integer. The low-order
(least significant) bits are sent first, followed by the next 8 bits, and then
eventually the highest order bits.
*******************************************************************************/

struct state {
  byte pinvals;
  unsigned long timestamp;
} state;

const int  BB_LEN = 64; /* number of items in the bounded buffer */
const byte SPI_RESET = 0x1;
const byte SPI_ECHO_REQUEST = 0x2;   /* Send response next time */
const byte SPI_ECHO_RESPONSE = 0x77; /* Response to send */


/* ======================= PIN_D Variables ======================= */
// Masks off digital pins 0, 1, and 2.
// Most significant bit of register corresponds to digital pin 7
uint8_t bitMask = B01111000;

/*
 * Pin values and corresponding timestamps are stored in this 2D array,
 * which is treated like a bounded buffer. As pin values are read in,
 * the 'end' is moved ahead. When data is sent to the SPI master device,
 * the 'beginning' is moved ahead, so long as it doesn't overtake 'end'.
 *
 * Note that 'end' may overtake 'beginning' if data is being detected
 * more rapidly than it is being sent by SPI; this may result in loss
 * of data! Similarly, if SPI is polled too quickly, redundant data may
 * be sent as the 'beginning' will not be advanced until new data is
 * available.
 */
struct state output[BB_LEN];
int bb_beg = 0; /* Start of bounded buffer. SPI reads here */
/* TODO - currently, the first block of data will be sent via SPI
 *  before it's actually valid (immediately, in fact).
 *  Figure out some clean way to prevent this until output[0]
 *  actually has something valid in it.
 *  Could just set it manually to be all zeroes, but then it might
 *  get overwritten (perhaps simultaneously - race condition!)
 *  when first data blip comes in.
 */
int bb_end = 0; /*   End of bounded buffer. PIN_D code writes here*/


volatile uint8_t prevpinval = PIND & bitMask;
volatile uint8_t pinval;
volatile uint8_t xorpins;
//extern volatile unsigned long timer0_overflow_count;

// Noah timing code
volatile unsigned long tmr1_overflow = 0;
volatile unsigned long time_elapsed;

/* ======================== SPI Variables ======================== */
byte marker = 0; /* Index into `long` timestamp in output array */
byte send_pinvals = 1; /* Send 'pinvals' first, so initialize to 1 */
byte flag; /* For Pi -> Arduino messages, such as 'reset' */



/**
 * This function runs once when the board boots.
 * Initial configuration is done here.
 */
void setup (void) {

  TCCR1A = 0;// set registers to 0  
  TCCR1B = 0;  
  TCCR1C = 0;  

  //TIMSK0 = 0; Disables timer0, I think
  TCCR0A = 0;
  TCCR0B = 0;
  
//  TCNT1 = 0; 

  /* Timer 1 is a 16-bit timer. */
//   TIMSK1 |= _BV(TOIE1); /* Enable overflow interrupt */

   TCCR1B = 1;// start timer 1 , this may not be the best place to start the timer

  /* SPI Setup */
  pinMode(MISO, OUTPUT); /* Set "Master In/Slave Out" pin as output */
  SPCR |= _BV(SPE); /* Set 'enable' bit of SPI config register */
  
  /* PIN_D Setup - Sets all D pins to input */
  DDRD = 0b00000000;

  /* Initialize all entries in the buffer to something we can notice.
   * Idealy/eventually, we will not need to do this.
   */
  for (int i=0; i<BB_LEN; i++){
    output[i].pinvals = 0B11111111;
    output[i].timestamp = 0;
  }
//   TCCR1B = 1;// start timer 1 , this may not be the best place to start the timer

}

/**
 * The main loop. This code runs indefinitely while the device
 * is powered on. It handles SPI communication and input pin
 * reading/change detection.
 */
void loop (void){
  while(1){
  /* ================= SPI TRANSFER =================
   * SPSR = SPI Status Register
   * SPDR = SPI Data Register
   * SPIF = SPI End of Transmission Flag
   * 
   * When the SPIF flag is set, this indicates that a byte has
   * been received. At this point, we can load a new value into
   * the data register. This will be sent to the master.
   * 
   * We send the timestamp in 4 chunks, as it is 32 bytes and
   * we only send 8 bytes at a time.
   */
	if((SPSR & (1 << SPIF)) != 0){
    flag = SPDR;
    if( (flag & SPI_ECHO_REQUEST) == flag){
      /* Next time, send echo request. This is a connection test */
      SPDR = SPI_ECHO_RESPONSE;
    } else {
      if( (flag & SPI_RESET) == flag){
        /* In case the devices get out of sync, we can send the 'RESET'
         * flag, causing us to send the 5-transmission sequence starting
         * at the beginning (with the pinvals). Could be useful if the Pi
         * is restarted without the Arduino resetting its counters.
         * NOTE: this is received while the current data is simultaneously
         * transmitted, meaning that the effects of this flag won't take
         * place until the next byte is transmitted.
         */
        marker = 0;
        send_pinvals = 1;
      }

      if(send_pinvals){
        /* Send the pin values */
        SPDR = output[bb_beg].pinvals;
        output[bb_beg].pinvals = (1 << 7) | output[bb_beg].pinvals;
        send_pinvals = 0;
      } else {
        /* Send the timestamp, 8 bits at a time */
        SPDR = output[bb_beg].timestamp >> (8 * marker);
        marker++;

        if(marker > 3){
          marker = 0;
          send_pinvals = 1;

          /* ================ Body of bb_advance_beg() ================ */
          /* The next entry isn't ready for us - don't move our pointer */
          if(bb_beg == bb_end || bb_beg == bb_end-1 || (bb_beg+1==BB_LEN && bb_end == 0)){}
          else {
            /* Otherwise, we can increment a little more */
            bb_beg++;
            if(bb_beg >= BB_LEN) bb_beg = 0;
            if(bb_beg == BB_LEN){
              /* If bb_end is at 0, we still can't move */
              bb_beg = bb_end == 0 ? BB_LEN - 1 : 0;
            }
          }
          /* ================== End bb_advance_beg() ================== */
        }
      }
    }
	}


  /* Read PIN_D values
   * reads high or low value of register at once
   * Hydrophones a,b,c,d will correspond to pins 3,4,5,6, respectively.
   * Digital pin 7 corresponds to the duration indicator.
   */
  pinval = PIND & bitMask;
  xorpins = (prevpinval ^ pinval);

  if (xorpins != 0) {
    // recreates the functionality of the micors() function
    // without the overhead of a function call
//    time_elapsed = ((timer0_overflow_count << 8) + TCNT0) * 4;

    TCCR1B = 0;    //stop the timer 
    unsigned int temp_timer = TCNT1;  //store passed ticks  
    TCCR1B = 1; // restart the timer
    
    unsigned long ticks = (((tmr1_overflow+1) << 16) | (unsigned long)temp_timer);  
    
    
//    unsigned long ticks = ((((unsigned long)tmr1_overflow - 1) << 16) | (unsigned long)temp_timer) - 4;  
//    

//    timestamp = (tmr1_overflow << 16) | TCNT1;
    time_elapsed = ticks;

    // stores high pins and timestamp
    output[bb_end].pinvals = (pinval >> 3) & 0b00011111;
    output[bb_end].timestamp = time_elapsed;
    
    /* ================ Body of bb_advance_end() ================ */
    bb_end++;
    if(bb_end >= BB_LEN) bb_end = 0;
    /* ================== End bb_advance_end() ================== */
  }
  prevpinval = pinval;

  }

}
ISR(TIMER1_OVF_vec){
      tmr1_overflow++;
}


