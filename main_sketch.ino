#include <Wire.h> // This library allows you to communicate with I2C devices.
#include <iostream>
#include "P:/arduino code/my_firestore/firestore.h"
using namespace std;

int CURR_SLEEP_DURATION = 0;
uint16_t tvoc = 0, eco2 = 0;
void setup() {
  Serial.begin(115200);
  sgp_setup();
  sgp_loop(&tvoc, &eco2);
  delay(1000);
  firestore_setup();
  /*
    SETUP SLEEP DURATION
  */
  get_duration_in_seconds_firestore(&CURR_SLEEP_DURATION);
  Serial.print("curr sleep duration is ");
  Serial.println(CURR_SLEEP_DURATION);
  // Config sleep timer for ESP32
  setSleepDuration(CURR_SLEEP_DURATION);
  configSleepDurationWithTimer();
  
  
  mpu_setup();
  screen_setup();
  servoMotorSetup();
  rtc_setup();
}

void loop() {
  // GET EXPECTED WAKEUP AND VERIFY IT
  String expected_wakeup = "";
  firestore_get_expected_wakeup_time(&expected_wakeup);
  if (verify_wakeup_time(expected_wakeup)) {
    /*
      SAMPLE 
    */
    double acc_avg = mpu_loop();
    Serial.print("Acc=");
    Serial.println(acc_avg);
    screen_display("Place Finger");
    int heart_rate = getPBM();
    Serial.print("BPM=");
    Serial.println(heart_rate);
    Serial.print("TVOC = ");
    Serial.println(tvoc);
    Serial.print("eCO2 = ");
    Serial.println(eco2);
    boolean cigarette = false;
    if (acc_avg > 5) {
      screen_display("move Detected");
      screen_display("No Cigarrete");
    } else if ((tvoc > 500) || (eco2 > 1000)) {
      screen_display("High Pollution");
      screen_display("No Cigarrete");
    } else if (!(heart_rate >= 60 && heart_rate <= 100)) {
      screen_display("Unstable BPM");
      screen_display("No Cigarrete");
    } else {
      screen_display("Enjoy");
      servoMotorTwist(70);
      cigarette = true;
    }
  
    /*
      GET CURRENT TIME & DATE
    */
    String now_date = "", now_time = "";
    rtc_get_date_time (&now_date, &now_time);
    firestore_write(acc_avg, heart_rate, (int)tvoc, (int)eco2, now_date, now_time, cigarette);
  
    /*
      SAVE NEXT WAKEUP TIME & DATE
    */
    rtc_save_next_wakeup(CURR_SLEEP_DURATION);
  }
  
  delay(1500);
  goToSleep();

}
