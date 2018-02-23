//TODO comment this
// how long does this mean?
//#define signal_length 60000 // in microseconds
// recreates signal for crab 22

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
  delayMicroseconds(1000); 
  PORTD = 0B00001100;
  delayMicroseconds(1000);
  PORTD = 0B00011100;
  delayMicroseconds(1000);
  PORTD = 0B00111100;
  delayMicroseconds(200);
  PORTD = 0B00111000; // 2 low, 3,4,5 high
  delayMicroseconds(1000);
  PORTD = 0B00110000;
  delayMicroseconds(1000);
  PORTD = 0B00100000;
  delayMicroseconds(1000);
  PORTD = 0B00000000;  
  delayMicroseconds(6000);
  PORTD = 0B00000100; 
  delayMicroseconds(1000); 
  PORTD = 0B00001100; 
  delayMicroseconds(1000); 
  PORTD = 0B00011100;    
  delayMicroseconds(1000); 
  PORTD = 0B00111100;
  delayMicroseconds(200); 
  PORTD = 0B00111000; // 2 low, 3,4,5 high
  delayMicroseconds(1000); 
  PORTD = 0B00110000;
  delayMicroseconds(1000); 
  PORTD = 0B00100000; 
  delayMicroseconds(1000);   
  PORTD = 0B00000000;


  delay(20000);

}
