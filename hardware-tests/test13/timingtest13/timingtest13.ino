unsigned long start, finished, elapsed;

void setup() {
  Serial.begin(115200);
}

void loop() {
//  Serial.println("Start...");
  start = micros();
//  Serial.println(start);
  delayMicroseconds(1000);
  finished = micros();
//  Serial.println(finished);
//  Serial.println("Finished");
  elapsed = finished - start;
  Serial.print(elapsed);
//  Serial.println(" microseconds elapsed");
  Serial.println();
}
