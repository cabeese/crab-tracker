// recreate crab 22 using register manipulations and nops
// built specifically for 16MHz nano
// with one cpu cycle lasting 62.5 nanoseconds

volatile uint8_t i = 0;
////volatile int i = 0;
////volatile uint8_t j = 0;
//volatile uint8_t k = 0;

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
    
    for(i=0; i<4; i++){
//      for(j=0; j<22; j++){ //21 creates a time of 3.19 ms 22 creates a time of 3.34 ms
//        for(k=0; k<200; k++){
          asm("nop \n");
        
//        }
//      }
    }
//    PORTD = 0B00000000;
////    for(i=0; i<4; i++){
//      for(j=0; j<21; j++){
//        for(k=0; k<200; k++){
//          asm("nop \n");
//        
//        }
//      }
////    }

//    for(i=0; i<195200; i++){
//      asm("nop \n");
//    } 
//    PORTD = 0B00001000;
//    for(i=0; i<51200; i++){
//      asm("nop \n");
//    }
//    delay(20);
    PORTD = 0B00000000;
    for(i=0; i<4; i++){

          asm("nop \n");
        
    }
//    delay(100);
  }
}
