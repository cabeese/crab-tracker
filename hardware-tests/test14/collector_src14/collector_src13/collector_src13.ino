/*
* 
* 
* Hydrophones a,b,c, d will correspond to pins 3,4,5,6, respectively.
*  Digital pin 7 corresponds to the duration indicator. 
*  test file for pulse collector to test accuracy of time stamps for very small deltas
*  only tests for changes on two pins
*  tTEENSY collected pulses
*  
*  
*/

// Masks off digital pins 0, 1, and 2. 
// Most significant bit of register corresponds to dignital pin 7
uint8_t bitMask = B00011000;


uint8_t prevpinval = PIND & bitMask;
uint8_t pinval;
uint8_t xorpins;
//extern volatile unsigned long timer0_overflow_count;
//volatile unsigned long time_elapsed;
 unsigned long ts0 = 0;
 unsigned long ts1 = 0;
//long double time_elapsed;
//long double period = 1.0/120.0;
double time_elapsed;
double period = 1.0L/120.0L;
//float time_elapsed;
//float period = 1.0/120.0;
int i=0;
double output1[64];

// 
//elapsedMicros timer_us;
//volatile unsigned long cycles; 


// longs are 32 bits, so the array can only hold 210 values
// will likely need to hold fewer values when SPI code is 
// integrated in to the code
//unsigned long output[64][2]; 

void setup() {

 //  for debugging  
   Serial.begin(38400);
  
 
  // sets  D pins 3 and 4 to input,

  DDRD = 0B11100111;
  ARM_DEMCR |= ARM_DEMCR_TRCENA;
  ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;


}

void loop() {
 
//  noInterrupts(); // just to see what would happen
  // reads high or low value of register at once
  pinval = PIND & bitMask; 
  xorpins = (prevpinval ^ pinval);
  
  // recreates the functionality of the micors() function
  // without the overhead of a function call
//  time_elapsed = ((timer0_overflow_count << 8) + TCNT0) * 4;
 // time_elapsed = micros();

//  if ((xorpins != 0) && (xorpins & pinval)) { // need to figure this out and why this logic was chosen
  if (xorpins  && (xorpins & pinval)) { // I bet I can make this logic shorter
    
     if (xorpins & (1 << 3)){    
        ts0 = ARM_DWT_CYCCNT;
     }
     
     if (xorpins & (1 << 4)){ 
       ts1 = ARM_DWT_CYCCNT;
       
//       Serial.println(ts0);
//       Serial.println(ts1);
//        time_elapsed = period * (ts1 - ts0); 
       output1[i] = ts1-ts0;
        i++;
//        Serial.println(ts1 - ts0);
//        Serial.println(output1[i]);
//       Serial.println(time_elapsed, 7); //print delta
       // extra print statements increase the number of clock cycles this loop takes
//         Serial.println();
     }
  }

   
  prevpinval = pinval;
//  interrupts();
 
  if (i==64){
    Serial.println("i is 64");
    
    for (int j=0; j<64; j++){
      time_elapsed = period * output1[j];
      Serial.println(time_elapsed, 7);
    }
    Serial.println();
//    exit(1);
    i=-1;
   }

}

