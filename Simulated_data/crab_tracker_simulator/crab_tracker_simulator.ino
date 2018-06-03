 /*
*Author: John Lund 
*Crab Tracker Simulator
*
*            |a
*            |
*            |
*            |   1m
*  d ------------------b
*            |
*            |
*            |
*            |c
*
*
*Simulates sensor signal arriving at sensors designated
*as shown above.
*
*Angle is angle from the x-axis (b sensor) and distance
*is distance from the origin (0,0)
*
*Signals for a, b, c, and d on pins D2,D3,D4,D5 respectively.
*Magnitude pulse on pin D6 corresponding to sensor a (D2)
*/

#include <math.h>

#define MINIMUM_PING_DUR_US 1000
#define STEP_SIZE_US 100
#define MINIMUM_DELAY_US 10000
#define STEP_SIZE_DELAY_US 100
#define MAX_ID 499
#define SPEED_OF_SOUND 1520
#define MAX_DISTANCE 200
#define MAX_DEPTH 20
#define DISTANCE_DELAY_MULT 100

//volatile long crabID=-1;

struct pulse_event{
  uint32_t event_time;
  int pin;
  int pin_state;
};

void bubbleSort(pulse_event* sortArray, int arrayLen){
  bool sorted = false;
  pulse_event tempStorage;

  while(!sorted){
    sorted = true;
    for(int x = 0; x <arrayLen-2; x++){
      if(sortArray[x].event_time > sortArray[x+1].event_time){
        sorted = false;
        //Serial.println("BUBBLE!");
        tempStorage = sortArray[x];
        sortArray[x] = sortArray[x+1];
        sortArray[x+1] = tempStorage;
      }
    }
  }
}

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  Serial.begin(9600);
  randomSeed(analogRead(0)+analogRead(1)+analogRead(2));
}


void loop() {
  //first we have to pick a random position for the crab.  Randomize in cartesian coordinates
  //calculate in cartesian coordinates, then convert to polar for presentation.

  long crabx = random(MAX_DISTANCE*2)-MAX_DISTANCE;
  long craby = random(MAX_DISTANCE*2)-MAX_DISTANCE;
  long crabz = random(0, MAX_DEPTH);
//
// Serial.println(crabx);
//  Serial.println(craby);
//   Serial.println(crabz);
   
//  long crabDistA = sqrt(sq(1-craby)+sq(crabx)+sq(crabz));
//  long crabDistB = sqrt(sq(craby)+sq(1-crabx)+sq(crabz));
//  long crabDistC = sqrt(sq(-1-craby)+sq(crabx)+sq(crabz));
//  long crabDistD = sqrt(sq(craby)+sq(-1-crabx)+sq(crabz));


// altered the locations of the hydrophones to correspond to the hydrophone configuration used the triangulation algorithm
  float crabDistA = sqrt(sq(craby-1)+sq(crabx-1)+sq(crabz));
  float crabDistB = sqrt(sq(craby-1)+sq(crabx+1)+sq(crabz));
  float crabDistC = sqrt(sq(1+craby)+sq(crabx+1)+sq(crabz));
  float crabDistD = sqrt(sq(craby+1)+sq(crabx-1)+sq(crabz));

//   Serial.println(crabDistA);
//   Serial.println(crabDistB);
//   Serial.println(crabDistC);
//   Serial.println(crabDistD);
  //calculate polar coordinates of the crab
  float crabR = sqrt(sq(craby)+sq(crabx));
  float crabTheta = atan2((float)craby, (float)crabx)*180.0/3.1416;

  //randomly pick crab ID

  long crabID = random(0,MAX_ID);
//   crabID +=1; 
//   if (crabID == 1000){
//      crabID=0;
//    }
  
  //print positions to screen
  Serial.print("Crab ID: ");
  Serial.println(crabID);
  Serial.print("Crab Distance: ");
  Serial.print(crabR);
  Serial.println(" m");
  Serial.print("Crab Angle: ");
  Serial.print(crabTheta);
  Serial.println(" degrees");
  Serial.print("Crab Depth: ");
  Serial.print(crabz);
  Serial.println(" m");
  Serial.println();

  //calculate pulse delay wave based on crabID all values in microseconds
  //from document https://github.com/cabeese/crab-tracker/blob/6-uid-encoding-documentation/doc/TransmissionProtocol/TransmissionProtocol.pdf
  long firstPulseRise = 0;
  long firstPulseFall = MINIMUM_PING_DUR_US + crabID*STEP_SIZE_US;
  long secondPulseRise = firstPulseFall + MINIMUM_DELAY_US+crabID*STEP_SIZE_DELAY_US;
  long secondPulseFall = secondPulseRise + MINIMUM_PING_DUR_US + crabID*STEP_SIZE_US;

  //now each detector will see the four pulse events we just calculated, but delayed based on
  //the distance from the crab to each receiver.
  //calculate that distance delay and add the pulse delays to create four signal events
  //for each receiver.  Record those signal events in an array, then sort the array by
  //time and follow each of the corresponding pulse event behaviors with the
  //microcontroller.  All time values in microseconds.

  pulse_event pulseTrain[16]; //four pulse events for each of the fours sensors plus two pulse events for the distance signal

  pulseTrain[0] = {(uint32_t)((1000000*crabDistA)/SPEED_OF_SOUND + firstPulseRise), 2, HIGH};
  pulseTrain[1] = {(uint32_t)((1000000*crabDistA)/SPEED_OF_SOUND + firstPulseFall), 2, LOW};  
  pulseTrain[2] = {(uint32_t)((1000000*crabDistA)/SPEED_OF_SOUND + secondPulseRise), 2, HIGH};
  pulseTrain[3] = {(uint32_t)((1000000*crabDistA)/SPEED_OF_SOUND + secondPulseFall), 2, LOW};

  pulseTrain[4] = {(uint32_t)((1000000*crabDistB)/SPEED_OF_SOUND + firstPulseRise), 3, HIGH};
  pulseTrain[5] = {(uint32_t)((1000000*crabDistB)/SPEED_OF_SOUND + firstPulseFall), 3, LOW};  
  pulseTrain[6] = {(uint32_t)((1000000*crabDistB)/SPEED_OF_SOUND + secondPulseRise), 3, HIGH};
  pulseTrain[7] = {(uint32_t)((1000000*crabDistB)/SPEED_OF_SOUND + secondPulseFall), 3, LOW};

  pulseTrain[8] = {(uint32_t)((1000000*crabDistC)/SPEED_OF_SOUND + firstPulseRise), 4, HIGH};
  pulseTrain[9] = {(uint32_t)((1000000*crabDistC)/SPEED_OF_SOUND + firstPulseFall), 4, LOW};  
  pulseTrain[10] = {(uint32_t)((1000000*crabDistC)/SPEED_OF_SOUND + secondPulseRise), 4, HIGH};
  pulseTrain[11] = {(uint32_t)((1000000*crabDistC)/SPEED_OF_SOUND + secondPulseFall), 4, LOW};
  
  pulseTrain[12] = {(uint32_t)((1000000*crabDistD)/SPEED_OF_SOUND + firstPulseRise), 5, HIGH};
  pulseTrain[13] = {(uint32_t)((1000000*crabDistD)/SPEED_OF_SOUND + firstPulseFall), 5, LOW};  
  pulseTrain[14] = {(uint32_t)((1000000*crabDistD)/SPEED_OF_SOUND + secondPulseRise), 5, HIGH};
  pulseTrain[15] = {(uint32_t)((1000000*crabDistD)/SPEED_OF_SOUND + secondPulseFall), 5, LOW};

  pulseTrain[16] = {(uint32_t)((1000000*crabDistA)/SPEED_OF_SOUND + firstPulseRise), 6, HIGH};
  pulseTrain[17] = {(uint32_t)((1000000*crabDistA)/SPEED_OF_SOUND + firstPulseFall + 2*MAX_DISTANCE * DISTANCE_DELAY_MULT/(crabDistA+1)), 6, LOW};

  bubbleSort(pulseTrain, 18); //yeah, it's a bubble sort.  My bad, but the sort isn't time critical while I am.

  unsigned long int startTime = micros();

  for(int x = 0; x<18;x++){
    while(micros()-startTime < pulseTrain[x].event_time); //wait until the next event time has arrived

    digitalWrite(pulseTrain[x].pin, pulseTrain[x].pin_state);
  }

  delay(5000); //let's do it all again in 5 seconds.  Maybe too fast?  Random?
  
}

