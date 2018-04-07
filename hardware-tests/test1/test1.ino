// This test file sets pins 2 through 6 high, one at a time with a delay between each pin so that no two pins will be high at the same time
// time period for which each pin is high as well as the time inbetween each pin's signal is in microseconds and is the same for each pin


unsigned int signal_length = 100; // in microseconds

unsigned int wait = 200; // in microseconds

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
  
//  Serial.begin(9600);

}

void loop() {
  
  //make pin 2 high
  PORTD = 0B00000100;
  delayMicroseconds(signal_length); 
  PORTD = 0B00000000;
  delayMicroseconds(wait);

  // make pin 3 high
  PORTD = 0B00001000;
  delayMicroseconds(signal_length);
  PORTD = 0B00000000;
  delayMicroseconds(wait);

// make pin 4 high  
  PORTD = 0B00010000;
  delayMicroseconds(signal_length);
  PORTD = 0B00000000;
  delayMicroseconds(wait);


  //make pin 5 high
 
  PORTD = 0B00100000;
  delayMicroseconds(signal_length);
  PORTD = 0B00000000;
  delayMicroseconds(wait);

  //make pin 6 high
  PORTD = 0B01000000;
  delayMicroseconds(signal_length);
  PORTD = 0B00000000;
 // delay(wait);

  delay(20000);

}
