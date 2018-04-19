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
      delayMicroseconds(i);
      PORTD = 0B00001100;
      delayMicroseconds(50);
      PORTD = 0B00000000; 
      delay(100); // do nothing for a bit so the collector has some time to print values, without the delay the values printed by the timing collector do not even remotely correspond with the value of i 
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
  PORTD = 0B00000100;
  delayMicroseconds(2000);
  PORTD = 0B00001100;
  delayMicroseconds(50);
  PORTD = 0B00000000;
  delay(100);
  
  
    
  }

  exit(1);
  
  
//
}
