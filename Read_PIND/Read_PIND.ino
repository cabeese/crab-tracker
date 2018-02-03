/*
* 
* 
* Hydrophones a,b,c, d will correspond to pins 3,4,5,6, respectively.
*  Digital pin 7 corresponds to the duration indicator. 
*
*/

// Masks off digital pins 0, 1, and 2. 
// Most significant bit of register corresponds to dignital pin 7
uint8_t bitMask = B11111000;


uint8_t prevpinval = PIND & bitMask;
uint8_t pinval;
uint8_t xorpins;
extern volatile unsigned long timer0_overflow_count;
unsigned long time_elapsed;

// longs are 32 bits, so the array can only hold 210 values
// will likely need to hold fewer values when SPI code is 
// integrated in to the code
unsigned long output[210][2]; 

void setup() {

 //  for debugging  
//   Serial.begin(9600);

  // sets all D pins to input,
  // may be unnecessary 
  DDRD = 0B00000000;

}

int i = 0;
void loop() {

  
  // reads high or low value of register at once
  pinval = PIND & bitMask; 
  xorpins = (prevpinval ^ pinval);
  
  // recreates the functionality of the micors() function
  // without the overhead of a function call
  time_elapsed = ((timer0_overflow_count << 8) + TCNT0) * 4;
  
  if (xorpins != 0) {

//    for debugging: check values of pins before storing    
//    Serial.println(); 
//    Serial.print("previous pin value: ");
//    Serial.println(prevpinval, BIN);
//    Serial.print("current pin value: ");
//    Serial.println(pinval, BIN);
//    Serial.print("time_elaspsed: ");
//    Serial.println(time_elapsed);
//    Serial.print("pinval: ");
//    Serial.println((pinval >> 3), BIN);


    // stores high pins and timestamp
    output[i][0] = (pinval >> 3); 
    output[i][1] = time_elapsed;
    
//    for debugging: confirm that expected values are stored
//    Serial.print("stored pin value: ");
//    Serial.println(output[i][0], BIN);
//    Serial.print("stored time: ");
//    Serial.println(output[i][1], DEC);
//    Serial.print("value of i: ");
//    Serial.println(i);
//    Serial.println();
 
    i++;
  }
  prevpinval = pinval;
}
