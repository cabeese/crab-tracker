// recreate crab 22 using register manipulations and nops
// built specifically for 16MHz nano
// with one cpu cycle lasting 62.5 nanoseconds
// Now I want to do it with assembly instructions

volatile uint8_t i = 0;



void setup() {
  // set pint 3 to output
  DDRD = 0B00001000;

  //clear PORTD
  PORTD = 0B00000000;

}

void loop() {
  
  while(1){
    PORTD = 0B00001000;
    // registers are only 8 bits
    
    for(i=0; i<51200; i++){
      asm("nop \n");
    }
    PORTD = 0B00000000;
    for(i=0; i<195200; i++){
      asm("nop \n");
    } 
    PORTD = 0B00001000;
    for(i=0; i<51200; i++){
      asm("nop \n");
    }
    delay(100);
  }
}
