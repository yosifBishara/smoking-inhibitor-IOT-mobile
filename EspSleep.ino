#define MICRO_SECOND_FACTOR 1000000

int sleepDuration = 0;
bool timerConfiged = false;

void setSleepDuration(int duration) {
  //sleepDuration=0 by default, should set it to a different value
  if (duration == 0) {
    Serial.println("Set sleep duration to a value that isn't zero");
    return;
  }
  sleepDuration = duration;
}

void configSleepDurationWithTimer() {
  if (sleepDuration == 0) {
    Serial.println("set sleep duration before timer config");
    return;
  } 
  esp_sleep_enable_timer_wakeup(sleepDuration*MICRO_SECOND_FACTOR);
  timerConfiged = true;
}

void goToSleep() {
  if (!timerConfiged) {
    Serial.println("config timer before going to sleep");
    return;
  }
  esp_deep_sleep_start();
  return;
}
