#define THRESHOLD 550
#define FIVETEEN_SECONDS 15000

const int SensorPin = 36;       // 'S' Signal pin connected to A0
int Signal = 0;
int beatCount = 0;
unsigned int startTime = 0;
boolean counted = false;

int getPBM() {
  startTime = millis();
  while (millis() < (startTime + FIVETEEN_SECONDS)) {
    Signal = analogRead(SensorPin);
    if ((Signal > THRESHOLD) && (counted == false)) {
      beatCount++;
      delay(50);
      counted = true; 
    } else if (Signal <= THRESHOLD) {
      counted = false;
    }
  }

  Serial.print("BPM = ");
  Serial.println(beatCount*4);
  return (beatCount*4);
  beatCount = 0;
  counted = false;
  
  
}
