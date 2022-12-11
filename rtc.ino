/*
 * Connections
 * connect SDA of the RTC with A4
 * connect SCL of the RTC with A5
 */
 
#include "RTClib.h"

#define MIN_FACTOR 60
#define HOUR_FACTOR 3600
#define DAY_FACTOR 86400

RTC_DS3231 rtc;

char daysOfTheWeek[7][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

int Day; 
int Month;
int Year; 
int Secs;
int Minutes;
int Hours;

String dofweek; // day of week

String myDate; 
String myTime; 

// for the 16x2 LCD

#define rs 9 
#define en 8 
#define d4 7 
#define d5 6  
#define d6 5 
#define d7 4 

void rtc_setup () {
  
  delay(3000); // wait for console opening

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
  
  // Comment out below lines once you set the date & time.
    // Following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
    // Following line sets the RTC with an explicit date & time
    // for example to set January 27 2017 at 12:56 you would call:
    // rtc.adjust(DateTime(2017, 1, 27, 12, 56, 0));
  }
}

void rtc_get_date_time (String *dt, String* tm) {
  DateTime now = rtc.now();     
  Day = now.day(); 
  Month = now.month(); 
  Year = now.year();
  Secs = now.second(); 
  Hours = now.hour(); 
  Minutes = now.minute(); 
  dofweek = daysOfTheWeek[now.dayOfTheWeek()];  
  
  myDate = myDate + Day + "/" + Month + "/" + Year ; 
  myTime = myTime + Hours +":"+ Minutes ; 
  // send to serial monitor
  Serial.println(dofweek); 
  Serial.println(myDate); 
  Serial.println(myTime);
  *dt = myDate;
  *tm = myTime;
  myDate = "";   
  myTime = ""; 
  delay(1000);
}

void rtc_save_next_wakeup(int sleep_duration2) {
  int sleep_duration = sleep_duration2;
  int daysToAdd = 0, hoursToAdd = 0, minutesToAdd = 0;
  while (sleep_duration / DAY_FACTOR) { 
    daysToAdd += 1; 
    sleep_duration = (int)(sleep_duration / DAY_FACTOR);
  }
  while (sleep_duration / HOUR_FACTOR) { 
    hoursToAdd += 1; 
    sleep_duration = (int)(sleep_duration / HOUR_FACTOR); 
  }
  while (sleep_duration / MIN_FACTOR) { 
    minutesToAdd += 1; 
    sleep_duration = (int)(sleep_duration / MIN_FACTOR);
  }
  DateTime now = rtc.now();
  DateTime next_wakeup(now + TimeSpan((int16_t)daysToAdd, (int8_t)hoursToAdd, (int8_t)minutesToAdd, 0));
  Day = next_wakeup.day(); 
  Month = next_wakeup.month(); 
  Year = next_wakeup.year();
  Hours = next_wakeup.hour(); 
  Minutes = next_wakeup.minute();
  String nextDate = (String)Day + "/" + (String)Month + "/" + (String)Year ; 
  String nextTime = (String)Hours +":"+ (String)Minutes ; 
  String next_wakeup_params = nextDate + "-" + nextTime;
  Serial.println("Esp Should Wake Up at " + next_wakeup_params);
  firestore_save_next_wakeup(next_wakeup_params, sleep_duration2);
}

boolean verify_wakeup_time(String expectedWakeup) {
  String sleepYear = "" , sleepMonth = "", sleepDay = "", sleepHour = "", sleepMin = "";
  int i = 0;
  for (; i < expectedWakeup.length() ; i++) {
    if (expectedWakeup[i] == '/') break;
    sleepDay += expectedWakeup[i];
  }
  i++;
  for (; i < expectedWakeup.length() ; i++) {
    if (expectedWakeup[i] == '/') break;
    sleepMonth += expectedWakeup[i];
  }
  i++;
  for (; i < expectedWakeup.length() ; i++) {
    if (expectedWakeup[i] == '-') break;
    sleepYear += expectedWakeup[i];
  }
  i++;
  for (; i < expectedWakeup.length() ; i++) {
    if (expectedWakeup[i] == ':') break;
    sleepHour += expectedWakeup[i];
  }
  i++;
  for (; i < expectedWakeup.length() ; i++) {
    sleepMin += expectedWakeup[i];
  }

  Serial.println("ESP Must Wake Up at " + sleepDay + "/" + sleepMonth + "/" + sleepYear + "  -  " + sleepHour + ":" + sleepMin);
  DateTime expectedTime((uint16_t)(sleepYear.toInt()), (uint8_t)(sleepMonth.toInt()), (uint8_t)(sleepDay.toInt()), (uint8_t)(sleepHour.toInt()), (uint8_t)(sleepMin.toInt()));
  DateTime now = rtc.now();
  Serial.print("Now Time is ");
  Serial.print(now.day()); Serial.print("/"); Serial.print(now.month()); Serial.print("/"); Serial.print(now.year()); Serial.print(" - "); Serial.print(now.hour()); Serial.print(":"); Serial.println(now.minute());
  if ((now.year() == expectedTime.year()) &&
      (now.month() == expectedTime.month()) &&
      (now.day() == expectedTime.day()) &&
      (now.hour() == expectedTime.hour()) &&
      ((now.minute() == expectedTime.minute()) || (now.minute() == expectedTime.minute() - 1) || (now.minute() == expectedTime.minute() + 1))) {
        return true;
      }
  return false;
}
