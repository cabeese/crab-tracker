///*
//* 
//* 
//* Hydrophones a,b,c, d will correspond to pins 3,4,5,6, respectively.
//*  Digital pin 7 corresponds to the duration indicator. 
//*  test file for pulse collector to test accuracy of time stamps for very small deltas
//*  only tests for changes on two pins
//*  nano collected pulses. We will count clock cycles!!!
//*  this does ... something
//*  
//*  
//*  
//*/
//
//// Masks off digital pins 0, 1, and 2. 
//// Most significant bit of register corresponds to dignital pin 7
//uint8_t bitMask = B00011000;
//
//
//volatile uint8_t prevpinval = PIND & bitMask;
//volatile uint8_t pinval;
//volatile uint8_t xorpins;
//
//// source: https://www.reddit.com/r/arduino/comments/1q1chr/using_timer1_to_count_clock_cycles/
//volatile unsigned int overflows = 0;
//
//volatile int i=0;
////volatile unsigned int output1[32];
//volatile uint16_t output1[32];
////volatile uint16_t count0 = NULL;
////volatile uint16_t count1 = NULL;
//
//
//
//
//
//void setup() {
//
//  // disabling timer0
//  TCCR0A = 0;
//  TCCR0B = 0;
//
//  TCCR1A = 0;// set registers to 0  
//  TCCR1B = 0;  
//  TCCR1C = 0;  
//  TCNT1 = 0;    //set the counter to 0, just in case  
//  TIMSK1 = _BV(TOIE1); //enable interrupt  
////  TCCR1B = 1;// start timer 1  
//
//
//
// //  for debugging, possible the cause of this entire headache
//   Serial.begin(19200);
//  
// 
//  // sets  D pins 3 and 4 to input,
//
//  DDRD = 0B11100111;
//  
//
//
////  delay(3000); // when 3,000 clock cycles are more accurate for first 4 deltas than when there is no delay in setup loop (delta is then 19 clock cycles)
////  delay(1000); // when delay 1,000 clock cycles are just as accurate. Why does a delay in the setup loop make a difference?
//
//
//}
//
////probably wrong one for this 
//ISR(TIM1_OVF_vect)   
//{  
//  overflows++;  
//}//end ISR  
//
//void loop() {
// while(1){
////  TCCR1B = 1;// start timer 1  
//  // reads high or low value of register at once
//  pinval = PIND & bitMask; 
//  xorpins = (prevpinval ^ pinval);
//  
//  if (xorpins & pinval) { 
//
//     if (xorpins & B00001000){  
//
////        TCCR1B = 1;// start timer 1  
////        TCCR1B = 0;    //stop the timer   
////        count0 = TCNT1;
//        TCCR1B = 1;// start timer 1  
//
//
//     }
//
//     if (xorpins & B00010000){ 
//
//        TCCR1B = 0;    //stop the timer   
////        count1 = TCNT1;
////        TCCR1B = 1;// start timer 1  
//
////        volatile uint16_t temp_timer = TCNT1;  //store passed ticks 
//        
////        output1[i] = temp_timer;
////          output1[i] = TCNT1;
//
//        // no idea why this would be needed but it's here
////        unsigned long ticks = ((((unsigned long)overflows - 1) << 16) | (unsigned long)temp_timer) - 4;
//        unsigned long ticks = ((((unsigned long)overflows - 1) << 16) | (unsigned long)TCNT1) - 4;
//        TCNT1 = 0; 
////        output1[i] = ticks/(F_CPU / 1000000.0);
//       
//        i++;
////        count0 = NULL;
////        count1 = NULL;
//
//     }
//  }
//
//   
//  prevpinval = pinval;
//
// 
//  if (i==32){
////    Serial.println("i==32");
//    for (int j=0; j<32; j++){
//      
//// this approach results in smallest deltas of 30 clock cycles, which is what I want
//      Serial.println(output1[j]);
//    }
//    Serial.println();
//    i=0;
//   }
// }  
//}

// Masks off digital pins 0, 1, and 2. 
// Most significant bit of register corresponds to dignital pin 7
uint8_t bitMask = B00011000;


volatile uint8_t prevpinval = PIND & bitMask;
volatile uint8_t pinval;
volatile uint8_t xorpins;

// source: https://www.reddit.com/r/arduino/comments/1q1chr/using_timer1_to_count_clock_cycles/
volatile unsigned int overflows = 0;

volatile int i=0;
//volatile unsigned int output1[32];
volatile uint16_t output1[32];
//volatile uint16_t count0 = NULL;
//volatile uint16_t count1 = NULL;





void setup() {
   //  for debugging, possible the cause of this entire headache
   Serial.begin(19200);
  // sets  D pins 3 and 4 to input,

  DDRD = 0B11100111;
  

  // disabling timer0
  TCCR0A = 0;
  TCCR0B = 0;

  TCCR1A = 0;// set registers to 0  
  TCCR1B = 0;  
  TCCR1C = 0;  
  TCNT1 = 0;    //set the counter to 0, just in case  
  TIMSK1 = _BV(TOIE1); //enable interrupt  




  
 



//  delay(3000); // when 3,000 clock cycles are more accurate for first 4 deltas than when there is no delay in setup loop (delta is then 19 clock cycles)
//  delay(1000); // when delay 1,000 clock cycles are just as accurate. Why does a delay in the setup loop make a difference?


}

//probably wrong one for this 
ISR(TIM1_OVF_vect)   
{  
  overflows++;  
}//end ISR  

void loop() {
 while(1){

  while(!(PIND & B00001000)){  
     }
  TCCR1B = 1;// start timer 1  
  while(!(PIND & B00010000)){ 
     }
   TCCR1B = 0;    //stop the timer   
   unsigned int temp_timer = TCNT1;  //store passed ticks  
   
   unsigned long ticks = ((((unsigned long)overflows - 1) << 16) | (unsigned long)temp_timer) - 4;
   Serial.println(ticks); 

////  TCCR1B = 1;// start timer 1  
//  // reads high or low value of register at once
//  pinval = PIND & bitMask; 
//  xorpins = (prevpinval ^ pinval);
//  
//  if (xorpins & pinval) { 
//
//     if (xorpins & B00001000){  
//
////        TCCR1B = 1;// start timer 1  
////        TCCR1B = 0;    //stop the timer   
////        count0 = TCNT1;
//        TCCR1B = 1;// start timer 1  
//
//
//     }
//
//     if (xorpins & B00010000){ 
//
//        TCCR1B = 0;    //stop the timer   
////        count1 = TCNT1;
////        TCCR1B = 1;// start timer 1  
//
////        volatile uint16_t temp_timer = TCNT1;  //store passed ticks 
//        
////        output1[i] = temp_timer;
////          output1[i] = TCNT1;
//
//        // no idea why this would be needed but it's here
////        unsigned long ticks = ((((unsigned long)overflows - 1) << 16) | (unsigned long)temp_timer) - 4;
//        unsigned long ticks = ((((unsigned long)overflows - 1) << 16) | (unsigned long)TCNT1) - 4;
//        TCNT1 = 0; 
////        output1[i] = ticks/(F_CPU / 1000000.0);
//       
//        i++;
////        count0 = NULL;
////        count1 = NULL;
//
//     }
//  }
//
//   
//  prevpinval = pinval;
//
// 
//  if (i==32){
////    Serial.println("i==32");
//    for (int j=0; j<32; j++){
//      
//// this approach results in smallest deltas of 30 clock cycles, which is what I want
//      Serial.println(output1[j]);
//    }
//    Serial.println();
//    i=0;
//   }
 }  
}
