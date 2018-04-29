

// Test code for pulse generator, makes only one pin high 
// TEENSY generated pulses


void setup() {

  
  // sets digital pins  3,4 D pins to output, 0 and 1 are RX & TX (not used)
  DDRD = 0B00011000;
  // pin 7 is also not used  
  // like pinMode without the function call overhead
  // sets all pins to low
  PORTD = 0B00000000;
  delay(30000); // a 30 seconds delay to plug in the micro controller that collects time stamps and do any necessary configurations
  

}

void loop() {

  // i is the number of microseconds between one pin going high and the next pin going high
  // it's the delta that the pulse collector should calculate
   for(int i=1; i<1002; i++){
    
    // j is the number of times we want to send i to see if the calculated delta is correct
    // needs to be configurable and the proper value of j 
    // depends on the distribution of errors in the deltas recorded 
    for (int j=0; j<10; j++){
        PORTD = 0B00001000; // make pin 3 high
        delayMicroseconds(i); // wait i microseconds
        PORTD = 0B00011000; // make both pin 3 and 4 high
        delayMicroseconds(50); // wait 50 microseconds
        PORTD = 0B00000000; // make all pins low
        delay(3000); // wait some delay that will give the timestamp collector enough time to print out the recorded delta value, would prefer this to be configurable so that I can minimize the delay between loops
   
   }

      // after each iteration of i generate the same pin pattern but with  asignificantly larger delay than the greatest value of i
      // this will indicate that i has incremented
      PORTD = 0B00001000;
      delayMicroseconds(2000);
      PORTD = 0B00011000;
      delayMicroseconds(50);
      PORTD = 0B00000000;
      delay(3000); // delay that allows time for the recieving microcontroller to print, also needs to be configurable
    
   }

}
