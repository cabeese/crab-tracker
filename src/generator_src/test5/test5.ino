
uint8_t j = 0;
#define DELAY_720_NS asm("nop\n");
#define DELAY_29800_NS for(j=0; j<246; j++){ asm("nop"); }
uint32_t _k = 0;
#define DELAY_100_US for(_k=0; _k<840; _k++){ asm("nop"); }
#define DELAY_1_MS for(_k=0; _k<8066; _k++){ asm("nop"); }


void setup() {
  // put your setup code here, to run once:

  
  // sets pins 3 pins to output, 0 and 1 are RX & TX (not used)
  DDRD = 0B001111000;
  

  // sets all pins to low
 // like digital write
  PORTD = 0B00000000;
  
//  Serial.begin(9600);

}

void loop() {

  while(1){
//    PORTD |= 1<<3;
//
//    DELAY_29800_NS

    PORTD |= 1<<4;

    DELAY_100_US
    
//    DELAY_100_US
//    DELAY_100_US
// 
//    delayMicroseconds(300);
    PORTD = 0;

//    DELAY_100_US
//    DELAY_100_US
//    DELAY_100_US
    delay(2000);
    
  }


}
