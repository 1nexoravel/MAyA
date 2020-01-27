#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;
String espaco = " ";

void datahora_setup(){
#ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif
  rtc.begin();

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

void datahora_loop(){
   DateTime time = rtc.now();
   tempo = (time.timestamp(DateTime::TIMESTAMP_DATE)+espaco+time.timestamp(DateTime::TIMESTAMP_TIME));
}
