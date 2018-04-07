// send output on only one pin for a time in milliseconds determined by signal lenght
// then make all pins low for a delay in milliseconds determined by wait
// can change which pin will go high by changing value in PIND register
unsigned int signal_length = 100; 

unsigned int wait = 200; 

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
  

// make pin 4 high  
  PORTD = 0B00010000;
  delay(signal_length);
  PORTD = 0B00000000;
  delay(wait);

}
