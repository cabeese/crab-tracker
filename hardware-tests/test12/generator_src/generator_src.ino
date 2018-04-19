// Test code for pulse generator, makes only one pin high 
// nano generated pulses
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
  delayMicroseconds(1);
  PORTD = 0B00001100;
  delayMicroseconds(5000);
  PORTD = 0B00000000;
  
  delay(3000);

}
