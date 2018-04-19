/*
* 
* 
* Hydrophones a,b,c, d will correspond to pins 3,4,5,6, respectively.
*  Digital pin 7 corresponds to the duration indicator. 
*  test file for pulse collector to test accuracy of time stamps for very small deltas
*  only tests for changes on two pins
*  teensy collected pulses
*
*/

// Masks off digital pins 0, 1, and 2. 
// Most significant bit of register corresponds to dignital pin 7
uint8_t bitMask = B11111000;


uint8_t prevpinval = PIND & bitMask;
uint8_t pinval;
uint8_t xorpins;
extern volatile unsigned long timer0_overflow_count;
unsigned long time_elapsed;
unsigned long prev_time = 0;
unsigned long delta;

unsigned long ts0;
unsigned long ts1;
unsigned long ts2;


// longs are 32 bits, so the array can only hold 210 values
// will likely need to hold fewer values when SPI code is 
// integrated in to the code
unsigned long output[64][2]; 

void setup() {

 //  for debugging  
   Serial.begin(9600);
 
  // sets all D pins to input,
  // may be unnecessary 
  DDRD = 0B00000000;

}

//int i = 0;
void loop() {
 

  // reads high or low value of register at once
  pinval = PIND & bitMask; 
  xorpins = (prevpinval ^ pinval);
  
  // recreates the functionality of the micors() function
  // without the overhead of a function call
  time_elapsed = ((timer0_overflow_count << 8) + TCNT0) * 4;
  //time_elapsed = micros();

  if ((xorpins != 0) && (xorpins & pinval)) {
//      Serial.println(prevpinval, BIN);
//      Serial.println(pinval, BIN);
//      Serial.println(time_elapsed);
    // Serial.println(xorpins & (1 << 3), BIN);
     if (xorpins & (1 << 3)){
      ts0 = time_elapsed;
     }
     if (xorpins & (1 << 4)){
       ts1 = time_elapsed;
       Serial.println(ts1 - ts0); //print delta
       Serial.println();
     }
     
  }
  prevpinval = pinval;
}

