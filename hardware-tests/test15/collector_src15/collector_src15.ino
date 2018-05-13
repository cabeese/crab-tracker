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


volatile uint8_t prevpinval = PIND & bitMask;
volatile uint8_t pinval;
volatile uint8_t xorpins;

//long double time_elapsed;
//long double period = 1.0/120.0;

volatile double time_elapsed;
volatile double period = 1.0L/120.0L;

//float time_elapsed;
//float period = 1.0/120.0;

volatile int i=0;
volatile uint32_t output1[32];
//volatile uint32_t output1[64][2];


// another appraoch to counting clock cycles
volatile uint32_t count0 = NULL;
volatile uint32_t count1 = NULL;
//
//#define CPU_RESET_CYCLECOUNTER    do { ARM_DEMCR |= ARM_DEMCR_TRCENA;          \
//                                       ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA; \
//                                       ARM_DWT_CYCCNT = 0; } while(0)

// addresses of registers
//volatile uint32_t *DWT_CONTROL = (uint32_t *)0xE0001000;
//volatile uint32_t *DWT_CYCCNT = (uint32_t *)0xE0001004; 
//volatile uint32_t *DEMCR = (uint32_t *)0xE000EDFC; 







// 
//elapsedMicros timer_us;
//volatile unsigned long cycles; 


// longs are 32 bits, so the array can only hold 210 values
// will likely need to hold fewer values when SPI code is 
// integrated in to the code
//unsigned long output[64][2]; 

// copy write protects expression of idea

void setup() {
//  noInterrupts();

// interrupt when early pin goes high and cpature the clock


 //  for debugging  
   Serial.begin(38400);
  
 
  // sets  D pins 3 and 4 to input,

  DDRD = 0B11100111;
  

////  //set up to help with counting clock cycles
//  ARM_DEMCR |= ARM_DEMCR_TRCENA;
//  ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;
  // when this setup code is before the delay: how does this change the value of the deltas?


//  // enable the use DWT
//  *DEMCR = *DEMCR | 0x01000000;
//  
//  // Reset cycle counter
//  *DWT_CYCCNT = 0; 
//  
//  // enable cycle counter
//  *DWT_CONTROL = *DWT_CONTROL | 1 ; 



//  delay(3000); // when 3,000 clock cycles are more accurate for first 4 deltas than when there is no delay in setup loop (delta is then 19 clock cycles)
  delay(1000); // when delay 1,000 clock cycles are just as accurate. Why does a delay in the setup loop make a difference?

//  //set up to help with counting clock cycles
// setup after delay doesn't seem to make a differnce
  ARM_DEMCR |= ARM_DEMCR_TRCENA;
  ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;
}

void loop() {
// while(1){

  // reads high or low value of register at once
  pinval = PIND & bitMask; 
  xorpins = (prevpinval ^ pinval);
  
  // recreates the functionality of the micors() function
  // without the overhead of a function call

 // time_elapsed = micros();

//  if ((xorpins != 0) && (xorpins & pinval)) { // need to figure this out and why this logic was chosen
 // no need to compare thevalue of xorpins with zero, if there has been a change it won't be zero


 // by setting the count variables to null after use the different if statements only change by 1 clock cycle
  //there is a 10 clock cylce difference between these two options (for very small deltas)
//  if (xorpins  && (xorpins & pinval)) { // I bet I can make this logic shorter //28 clock cylces 28 clock cyles
  if (xorpins & pinval) { // 38 clock cycles //29 clock cycles

    
//     if (xorpins & (1 << 3)){  \
// trigger an interrupt when the pin changes state? 
     if (xorpins & B00001000){  
//        cli();   // this makes things worse
        count0 = ARM_DWT_CYCCNT;

//        CPU_RESET_CYCLECOUNTER;
//        sei();

        // number of cycles stored in count variable
//        count0 = *DWT_CYCCNT;
     }

//     if (xorpins & (1 << 4)){ 
//     else if (xorpins & B00010000){ // this makes it very very wrong, but why?
     if (xorpins & B00010000){ 
//       cli();
       count1 = ARM_DWT_CYCCNT;

//       output1[i]=ARM_DWT_CYCCNT;
//       sei();
//       count1 = *DWT_CYCCNT;
       
//       Serial.println(count0);
//       Serial.println(count1);
//       Serial.println();
//        time_elapsed = period * (ts1 - ts0); 



        output1[i] = count1-count0;
//
//       output1[i][0] = count0;
//       output1[i][1] = count1;
       
        count1 = NULL;
        count0 = NULL;

       
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
 
  if (i==32){
//        interrupts();
//    noInterrupts();
//    cli();
    for (int j=0; j<64; j++){
//      time_elapsed = period * output1[j];
//      Serial.println(time_elapsed, 7);

// this approach results in smallest deltas of 30 clock cycles, which is what I want
      Serial.println(output1[j]);

// This approach results in deltas of 21 clock cycles
//      Serial.println(output1[j][0]);
//      Serial.println(output1[j][1]);
//      Serial.println();
      
    }
    Serial.println();
//    exit(1);
    i=0;
//    sei();
//    interrupts();
   }
// }
}
