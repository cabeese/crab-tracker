// Test code for pulse generator, makes only one pin high 
// uno generated pulses
// will generate pulses in a loop incrementing by one millisecond
unsigned int signal_length = 0; // in microseconds
unsigned int delay1 = 50;
unsigned int wait = 0; // in microseconds
unsigned int delay2 = 100;
uint8_t bitMask = 0B01111100;
uint8_t pinval;
uint8_t pind;
       int delta = 2;
 int plength = 50;
 int delays= 100;

void setup() {
  // put your setup code here, to run once:
  
  // sets pins 2,3,4,5,6 D pins to output, 0 and 1 are RX & TX (not used)
  DDRD = 0B01111100;
  // pin 7 is also not used  
  // like pinMode without the function call overhead

  // sets all pins to low
 // like digital write
  PORTD = 0B00000000;

  delay(30000); //10 seconds to switch to the teensy output in the serial monitor
  
//  Serial.begin(9600);

}

void loop() {

 for(int i=1; i<1002; i++){
    for (int j=0; j<1000; j++){

      PORTD = 0B00000100;
    //  delayMicroseconds(2); // when delta = 1 teensy off by +1; d=2 off by +1
//              if (--2 == 0)
//                return;
//
//        // the following loop takes a quarter of a microsecond (4 cycles)
//        // per iteration, so execute it four times for each microsecond of
//        // delay requested.
//        2 <<= 2;
//
//        // account for the time taken in the preceeding commands.
//        2 -= 2;

        // busy wait
        __asm__ __volatile__ (
                "1: sbiw %0,1" "\n\t" // 2 cycles
                "brne 1b" : "=w" (delta) : "0" (delta) // 2 cycles
        );
      PORTD = 0B00001100;
   //   delayMicroseconds(50); // when pulse length = 50 teensy off by +2; d=2 length = 51
//             if (--50 == 0)
//                return;
//
//        // the following loop takes a quarter of a microsecond (4 cycles)
//        // per iteration, so execute it four times for each microsecond of
//        // delay requested.
//        50 <<= 2;
//
//        // account for the time taken in the preceeding commands.
//        50 -= 2;

        // busy wait
        __asm__ __volatile__ (
                "1: sbiw %0,1" "\n\t" // 2 cycles
                "brne 1b" : "=w" (plength) : "0" (plength) // 2 cycles
        );
      PORTD = 0B00000000; 
        __asm__ __volatile__ (
      "1: sbiw %0,1" "\n\t" // 2 cycles
      "brne 1b" : "=w" (delays) : "0" (delays) // 2 cycles
);



   //   delay(100); // do nothing for a bit so the collector has some time to print values, without the delay the values printed by the timing collector do not even remotely correspond with the value of i 
    }

//    This configuration of the inner for loop does not fix the timing issue
//     PORTD = 0B00000100;
//      delayMicroseconds(i);
//      PORTD = 0B00001100;
//      delay(1000); // do nothing for 1 second
//      PORTD = 0B00000000;
//       delayMicroseconds(50);

// trying to make a delta that will be easy to spot
// so we know when i has incremented
//  PORTD = 0B00000100;
//  delayMicroseconds(2000);
//  PORTD = 0B00001100;
//  delayMicroseconds(50);
//  PORTD = 0B00000000;
//  delay(100);
  
  
    
  }

//  exit(1);
  
  
//
}
