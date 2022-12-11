#include <Servo.h>
static const int servoPin = 25;

Servo servo1;

void servoMotorSetup() {
    servo1.attach(servoPin);
}

void servoMotorTwist(int degrees) {
    for(int posDegrees = 0; posDegrees <= degrees; posDegrees+=10) {
        servo1.write(posDegrees);
        Serial.println(posDegrees);
        delay(50);
    }
}
