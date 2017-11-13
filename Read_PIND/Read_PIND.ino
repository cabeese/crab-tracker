uint8_t prevpinval = PIND;
extern volatile unsigned long timer0_overflow_count;
unsigned long time_elapsed;
int output[255][2];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}



int i = 0;
void loop() {

  uint8_t pinval = PIND;

  //for debugging
//  Serial.println(prevpinval, BIN);
//  Serial.println(pinval, BIN);

  uint8_t xorpins = (prevpinval ^ pinval);
  
//  Serial.println(xorpins, BIN);
//  Serial.println(xorpins >> 1, BIN);
//  Serial.println("times: ");
    time_elapsed = ((timer0_overflow_count << 8) + TCNT0) * 4;
 //Serial.println(((timer0_overflow_count << 8) + TCNT0) * 4); //what is happening here?
 // Serial.println(micros()); //less accurate, has function acall overhead

  
  

  if (xorpins != 0) {
    Serial.println();
    Serial.println("we have input");
//    Serial.print("previous pin value: ");
//    Serial.println(prevpinval, BIN);
//    Serial.print("current pin value: ");
//    Serial.println(pinval, BIN);
//    Serial.println(time_elapsed); 
    output[i][0] = (pinval);
    
    output[i][1] = time_elapsed;

    Serial.println(output[i][0],BIN);
    Serial.println(output[i][1],DEC);
    Serial.println(i);
//    Serial.print("xored value: ");
//    Serial.println(xorpins, BIN);
    i++;
  }

  //Serial.println();
  prevpinval = pinval;
}
