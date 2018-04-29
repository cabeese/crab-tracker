

// Test code for pulse generator, makes only one pin high 
// TEENSY generated pulses
// use someone else's code to recreate delayMicrosecond functionality to see if that results in accurate deltas

//I stole this from the internet, cite source.
//static inline void delaymicroseconds(uint32_t) __attribute__((always_inline, unused));
//static inline void delaymicroseconds(uint32_t usec){
////  #if F_CPU == 96000000
////    uint32_t n = usec << 5;
////  #elif F_CPU == 48000000
////    uint32_t n = usec << 4;
////  #elif F_CPU == 24000000
////    uint32_t n = usec << 3;
////  #endif
//    uint32_t n = usec << 6;
//    if (usec == 0) return;
//    asm volatile(
//        "L_ %=_delaymicroseconds:"      "\n\t"
//        "subs   %0, #1"                 "\n\t"  
//        "bne  L_%=_delaymicroseconds"   "\n"
//        : "+r" (n) :
//        );
//}
volatile int i;
volatile int j;
volatile uint32_t n;
void setup() {
  // put your setup code here, to run once:

  
  // sets pins  3,4 D pins to output, 0 and 1 are RX & TX (not used)
  DDRD = 0B00011000;
  // pin 7 is also not used  
  // like pinMode without the function call overhead

  // sets all pins to low
 // like digital write
  PORTD = 0B00000000;

  
  delay(30000);
  


}

void loop() {

  //Serial.println(F_CPU);


   for(i=1; i<1002; i++){
    for (j=0; j<10; j++){
        PORTD = 0B00001000;
        n =  i *  40;
                __asm__ volatile(
                  //doesn't work as this calls the built in function
        "L_%=_delayMicroseconds:"    "\n\t"
        #if F_CPU < 24000000
        "nop"         "\n\t"
        #endif
        "subs   %0, #1"       "\n\t"
        "bne    L_%=_delayMicroseconds"   "\n"
            : "+r" (n) :
          );
//        delayMicroseconds(i);
        PORTD = 0B00011000;
        delayMicroseconds(50);
        PORTD = 0B00000000;
        delayMicroseconds(3000);
    }

// this is definitley generating the pattern that I think it is
      PORTD = 0B00001000;
      delayMicroseconds(2000);
      PORTD = 0B00011000;
      delayMicroseconds(500);
      PORTD = 0B00000000;
      delay(3000);
  
    
   }
  


}
