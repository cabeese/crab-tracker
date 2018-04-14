// Test code for pulse generator, makes only one pin high 
unsigned int signal_length = 0; // in microseconds
unsigned int delay1 = 50;
unsigned int wait = 0; // in microseconds
unsigned int delay2 = 100;
uint8_t bitMask = 0B01111100;
uint8_t pinval;
uint8_t pind;

void setup() {
  // put your setup code here, to run once:

  
  // sets pins 2,3,4,5,6 D pins to output, 0 and 1 are RX & TX (not used)
  DDRD = 0B01111100;
  // pin 7 is also not used  
  // like pinMode without the function call overhead

  // sets all pins to low
 // like digital write
  PORTD = 0B00000000;
  signal_length = delay1;
  wait = delay2;
  
//  Serial.begin(9600);

}

void loop() {
  
  PORTD = 0B00000100;
  delayMicroseconds(5000);
//  PORTD = 0B00001100;
//  delayMicroseconds(4000);
//  PORTD = 0B00001000;
//  delayMicroseconds(1000);
  PORTD = 0B00000000;
  
  delay(3000);
//  delayMicroseconds(wait);

////  microDelay(wait);
//    wait <<= 2;
//  // account for the time taken in the preceeding commands.
//  wait -= 2;
//
//  // busy wait
//  __asm__ __volatile__ (
//      "1: sbiw %0,1" "\n\t" // 2 cycles
//      "brne 1b" : "=w" (wait) : "0" (wait) // 2 cycles
//);
//  wait = delay2;





//  // make pin 3 high
//  PORTD = 0B00001000;
//  // sleep
////  delayMicroseconds(signal_length);
// // microDelay(signal_length);
//    signal_length <<= 2;
//  // account for the time taken in the preceeding commands.
//  signal_length -= 2;
//  
//  // busy wait
//  __asm__ __volatile__ (
//      "1: sbiw %0,1" "\n\t" // 2 cycles
//      "brne 1b" : "=w" (signal_length) : "0" (signal_length) // 2 cycles
//);
//  signal_length = delay1;
//  
//  // pin 3 low
//  PORTD = 0B00000000;
//  // wait
//  //delayMicroseconds(wait);
////    microDelay(wait);
//        wait <<= 2;
//  // account for the time taken in the preceeding commands.
//  wait -= 2;
//
//  // busy wait
//  __asm__ __volatile__ (
//      "1: sbiw %0,1" "\n\t" // 2 cycles
//      "brne 1b" : "=w" (wait) : "0" (wait) // 2 cycles
//  );
//    wait = delay2;
//
////  // make pin 4 high
// PORTD = 0B00010000;
//  // sleep
//// delayMicroseconds(signal_length);
//// microDelay(signal_length);
//   signal_length <<= 2;
//  // account for the time taken in the preceeding commands.
//  signal_length -= 2;
//
//  // busy wait
//  __asm__ __volatile__ (
//      "1: sbiw %0,1" "\n\t" // 2 cycles
//      "brne 1b" : "=w" (signal_length) : "0" (signal_length) // 2 cycles
//);
//  signal_length = delay1;
//   // make all pins low
//  PORTD = 0B00000000;
//  // wait
// // delayMicroseconds(wait);
////  microDelay(wait);
//      wait <<= 2;
//  // account for the time taken in the preceeding commands.
//  wait -= 2;
//
//  // busy wait
//  __asm__ __volatile__ (
//      "1: sbiw %0,1" "\n\t" // 2 cycles
//      "brne 1b" : "=w" (wait) : "0" (wait) // 2 cycles
//  );
//  wait = delay2;
//  // make pin 5 high
//  PORTD = 0B00100000;
// // sleep
////  delayMicroseconds(signal_length);
////  microDelay(signal_length);
//    signal_length <<= 2;
//  // account for the time taken in the preceeding commands.
//  signal_length -= 2;
//
//  // busy wait
//  __asm__ __volatile__ (
//      "1: sbiw %0,1" "\n\t" // 2 cycles
//      "brne 1b" : "=w" (signal_length) : "0" (signal_length) // 2 cycles
//);
//  signal_length = delay1;
//  PORTD = 0B00000000;
//  // wait
// // delayMicroseconds(wait);
////   microDelay(wait);
//       wait <<= 2;
//  // account for the time taken in the preceeding commands.
//  wait -= 2;
//
//  // busy wait
//  __asm__ __volatile__ (
//      "1: sbiw %0,1" "\n\t" // 2 cycles
//      "brne 1b" : "=w" (wait) : "0" (wait) // 2 cycles
//);  
//  wait = delay2;
//  // make pin 6 high
//  PORTD = 0B01000000;
// // sleep
////  delayMicroseconds(signal_length);
////  microDelay(signal_length);
//    signal_length <<= 2;
//  // account for the time taken in the preceeding commands.
//  signal_length -= 2;
//
//  // busy wait
//  __asm__ __volatile__ (
//      "1: sbiw %0,1" "\n\t" // 2 cycles
//      "brne 1b" : "=w" (signal_length) : "0" (signal_length) // 2 cycles
//);
//  signal_length = delay1;
//  PORTD = 0B00000000;
// // delay(signal_length);
////  // wait
//  //delayMicroseconds(wait);
////  microDelay(wait);
//       wait <<= 2;
//  // account for the time taken in the preceeding commands.
//  wait -= 2;
//
//  // busy wait
//  __asm__ __volatile__ (
//      "1: sbiw %0,1" "\n\t" // 2 cycles
//      "brne 1b" : "=w" (wait) : "0" (wait) // 2 cycles
//  );
//  wait = delay2;
}
