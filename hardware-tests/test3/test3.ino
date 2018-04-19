// Test code for pulse generator
// tests deltas between pulses being generated


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
  
  PORTD = 0B00000100; // 2 high
  delayMicroseconds(500); 
  PORTD = 0B00001100; // 2,3 high
  delayMicroseconds(500);
  PORTD = 0B00011100; // 2,3,4 high
  delayMicroseconds(500);
  PORTD = 0B00111100; // 2,3,4,5 high
  delayMicroseconds(1700);
  PORTD = 0B00111000; // 2 low, 3,4,5 high
  delayMicroseconds(500);
  PORTD = 0B00110000; // 2,3 low 4,5 high
  delayMicroseconds(500);
  PORTD = 0B00100000; // 2,3,4 low 5 high
  delayMicroseconds(500);
  PORTD = 0B00000000;  // all low


  delay(20000);

}
