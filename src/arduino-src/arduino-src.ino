/*******************************************************************************
Crab Tracker - Arduino Code

Reads in data from the pins (attached to Hydrophones) and transfers that data
via SPI to another device.
*******************************************************************************/

/* ======================= PIN_D Variables ======================= */
// Masks off digital pins 0, 1, and 2.
// Most significant bit of register corresponds to digital pin 7
uint8_t bitMask = B11111000;
int output_start; /* Read out for SPI */
int output_end; /* Store here when reading pins */

/* longs are 32 bits, so the array can only hold 210 values
   will likely need to hold fewer values when SPI code is
   integrated in to the code */
unsigned long output[64][2];


uint8_t prevpinval = PIND & bitMask;
uint8_t pinval;
uint8_t xorpins;
extern volatile unsigned long timer0_overflow_count;
unsigned long time_elapsed;

/* ======================== SPI Variables ======================== */
unsigned char hello[] = {'H','e','l','l','o',' ',
						 'R','a','s','p','i','\n'};
byte marker = 0;

uint8_t tmp_state           = 123;
unsigned long tmp_timestamp = 123456789;

/***************************************************************
 Setup SPI in slave mode (1) define MISO pin as output (2) set
 enable bit of the SPI configuration register
****************************************************************/
void setup (void) {
    /* SPI Setup - Sets pin mode on MISO and does...? */
	pinMode(MISO, OUTPUT);
	SPCR |= _BV(SPE);

    /* PIN_D Setup - Sets all D pins to input; may be unnecessary */
    DDRD = 0B00000000;
}

/***************************************************************
 Loop until the SPI End of Transmission Flag (SPIF) is set
 indicating a byte has been received.  When a byte is
 received, load the next byte in the Hello[] array into SPDR
 to be transmitted to the Raspberry Pi, and increment the marker.
 If the end of the Hell0[] array has been reached, reset
 marker to 0.
****************************************************************/
void loop (void){
  /* SPI TRANSFER */
	if((SPSR & (1 << SPIF)) != 0){
//		SPDR = hello[marker];
    SPDR = tmp_state;
		marker++;

		if(marker > sizeof(hello)){
			marker = 0;
		}
	}


  /* Read PIN_D values */
  // reads high or low value of register at once
  pinval = PIND & bitMask;
  xorpins = (prevpinval ^ pinval);
  
  // recreates the functionality of the micors() function
  // without the overhead of a function call
  time_elapsed = ((timer0_overflow_count << 8) + TCNT0) * 4;
  
  if (xorpins != 0) {
    // stores high pins and timestamp
    output[i][0] = (pinval >> 3);
    output[i][1] = time_elapsed;
    
    count++;
  }
  prevpinval = pinval;

}
/******************************************************************************/
/*
*
*
* Hydrophones a,b,c, d will correspond to pins 3,4,5,6, respectively.
*  Digital pin 7 corresponds to the duration indicator.
*
*/



// int i = 0;
// void loop() {
//
//
// }
