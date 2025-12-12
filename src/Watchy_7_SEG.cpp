#include "Watchy_7_SEG.h"
#include <MoonRise.h>
#include <ctime>
#include "settings.h"
#include <TimeLib.h>

#ifdef ARDUINO_ESP32S3_DEV
#define ACTIVE_LOW 0
#else
#define ACTIVE_LOW 1
#endif


const int DISMODE_SECS = 0;
const int DISMODE_MOON = 1;
const int DISMODE_SUN = 2;
const int DISMODE_WEATHER = 3;
const uint8_t WEATHER_ICON_WIDTH = 48;
const uint8_t WEATHER_ICON_HEIGHT = 32;

RTC_DATA_ATTR bool firstWeatherNotDone = true;

RTC_DATA_ATTR bool DARKMODE = false;
RTC_DATA_ATTR int DISPMODE = DISMODE_MOON;
RTC_DATA_ATTR long LASTMEMTIME = 0;

RTC_DATA_ATTR bool WAS_NTP_UPDATE = false;

moonPhaser moonP;

const uint8_t *const NUM_BITMAPS[] = {num_0, num_1, num_2, num_3, num_4,
                                     num_5, num_6, num_7, num_8, num_9};

struct {
  float voltage;
  int8_t level;
}

const BatteryLevels[] = {
    {4.1, 37},
    {4.0, 33},
    {3.9, 28},
    {3.8, 23},
    {3.7, 18},
    {3.6, 13},
    {3.5, 8},
    {3.4, 4},
    {3.3, 2},
    {3.2, 0}
};

const size_t NUM_LEVELS = sizeof(BatteryLevels) / sizeof(BatteryLevels[0]);

const char ddayStr0[] PROGMEM = "Err";
const char ddayStr1[] PROGMEM = WEEKDAY_SUN;
const char ddayStr2[] PROGMEM = WEEKDAY_MON;
const char ddayStr3[] PROGMEM = WEEKDAY_TUS;
const char ddayStr4[] PROGMEM = WEEKDAY_WEN;
const char ddayStr5[] PROGMEM = WEEKDAY_THU;
const char ddayStr6[] PROGMEM = WEEKDAY_FRI;
const char ddayStr7[] PROGMEM = WEEKDAY_SAT;


const PROGMEM char *const PROGMEM ddayNames_P[] = {ddayStr0, ddayStr1, ddayStr2,
                                                     ddayStr3, ddayStr4, ddayStr5,
                                                     ddayStr6, ddayStr7};

const char dmonthStr0[] PROGMEM = "Err";
const char dmonthStr1[] PROGMEM = MONTH_JAN;
const char dmonthStr2[] PROGMEM = MONTH_FEB;
const char dmonthStr3[] PROGMEM = MONTH_MAR;
const char dmonthStr4[] PROGMEM = MONTH_APR;
const char dmonthStr5[] PROGMEM = MONTH_MAY;
const char dmonthStr6[] PROGMEM = MONTH_JUN;
const char dmonthStr7[] PROGMEM = MONTH_JUL;
const char dmonthStr8[] PROGMEM = MONTH_AUG;
const char dmonthStr9[] PROGMEM = MONTH_SEP;
const char dmonthStr10[] PROGMEM = MONTH_OCT;
const char dmonthStr11[] PROGMEM = MONTH_NOV;
const char dmonthStr12[] PROGMEM = MONTH_DEC;

const PROGMEM char *const PROGMEM dmonthNames_P[] = {
    dmonthStr0, dmonthStr1, dmonthStr2, dmonthStr3, dmonthStr4,
    dmonthStr5, dmonthStr6, dmonthStr7, dmonthStr8, dmonthStr9,
    dmonthStr10, dmonthStr11, dmonthStr12};

    
int SunCurve[190] = {
      0,0, 1,0, 2,1, 3,1, 4,2, 5,2, 6,6, 7,7, 8,8, 9,9, 9,10, 10,11,
      10,12, 11,13, 11,14, 12,15, 12,16, 13,17, 13,18, 13,19, 14,20,
      14,21, 14,22, 15,23, 15,24, 16,25, 16,26, 17,27, 17,28, 18,29,
      18,30, 19,31, 19,32, 19,33, 20,34, 20,35, 21,36, 21,37, 22,38,
      22,39, 23,40, 24,41, 25,42, 26,43, 27,44, 28,44, 
      29,44, 30,44, 32,44, 33,44,
      34,44, 35,43, 36,42, 37,41, 38,40, 39,39, 39,38, 40,37, 40,36,
      41,35, 41,34, 42,33, 42,32, 42,31, 43,30, 43,29, 44,28, 44,27,
      45,26, 45,25, 46,24, 46,23, 47,22, 47,21, 47,20, 48,19, 48,18,
      48,17, 49,16, 49,15, 50,14, 50,13, 51,12, 51,11, 52,10, 52,9,
      53,8, 54,7, 55,6, 56,5, 57,2, 58,2, 59,1, 60,1, 61,0 
  };
  

void Watchy7SEG::handleButtonPress() {
  if (guiState == WATCHFACE_STATE) {
    uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();
    if (wakeupBit & DOWN_BTN_MASK) {
      display.init(0, false);
      int rememberDM = DISPMODE;
      DISPMODE = DISMODE_SECS;
      RTC.read(currentTime);
      drawWatchFace();
      const int16_t clearX = 115;
      const int16_t clearY = 66;
      const int16_t clearW = 37;
      const int16_t clearH = 25;
      if (DISPLAYTYPE == 1) {
        display.firstPage();
        display.setPartialWindow(clearX, clearY, clearW, clearH);
      } else {
        display.display(false);
      }
      display.fillRect(clearX, clearY, clearW, clearH, DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
      guiState = APP_STATE;
      bool timeout = false;
      long lastTimeout = millis();
      long startTimeout = millis();
      RTC.read(currentTime);
      int oldSec = currentTime.Second;
      while (!timeout) {
        if (millis() - lastTimeout > 100) {
          RTC.read(currentTime);
          if (currentTime.Second != oldSec) {
            oldSec = currentTime.Second;
            drawSeconds();
            lastTimeout = millis();
            if (digitalRead(DOWN_BTN_PIN) == HIGH) {
              if (connectWiFi()) if (connectWiFi()) syncNTP();
            }
            if (digitalRead(UP_BTN_PIN) == HIGH) {
              timeout = true;
              LASTMEMTIME = 0;
              Serial.print("LASTMEMTIME: ");
              Serial.println(LASTMEMTIME);
            }
            if (digitalRead(BACK_BTN_PIN) == HIGH) {
              timeout = true;
              time_t unixSeconds = makeTime(currentTime);
              LASTMEMTIME = (long)unixSeconds;
              Serial.print("LASTMEMTIME: ");
              Serial.println(LASTMEMTIME);
            }
            if (digitalRead(MENU_BTN_PIN) == HIGH) {
              timeout = true;
            }
          }
        }
        if (millis() - startTimeout > 30000) {
          timeout = true;
        }
      }
      DISPMODE = rememberDM;
      guiState = WATCHFACE_STATE;
      RTC.read(currentTime);
      showWatchFace(true);
      return;
    }

    if (wakeupBit & UP_BTN_MASK) {
      DISPMODE++;
      if (DISPMODE > DISMODE_WEATHER)
        DISPMODE = DISMODE_MOON;
      if (DISPMODE == DISMODE_WEATHER)
        firstWeatherNotDone = true;
      RTC.read(currentTime);
      showWatchFace(true);
      display.display(false);
      return;
    }
    if (wakeupBit & BACK_BTN_MASK) {
      DARKMODE = !DARKMODE;
      RTC.read(currentTime);
      showWatchFace(true);
      return;
    }
    if (wakeupBit & MENU_BTN_MASK) {
      Watchy::handleButtonPress();
      return;
    }
  } else {
    Watchy::handleButtonPress();
  }
  return;
}


void Watchy7SEG::drawSeason() {
  int year = currentTime.Year + 1970;
  int32_t month = currentTime.Month;
  int32_t day = currentTime.Day;
  float lat = GET_LATITUDE((LOC)); 
  Season current_season = WatchyDusk2Dawn::getCurrentSeason(year, month, day, lat);
  uint16_t color = DARKMODE ? GxEPD_WHITE : GxEPD_BLACK;
  switch(current_season) {
      case SPRING:
          display.drawBitmap(109, 74, spring, 5, 22, color);
          break;
      case SUMMER:
          display.drawBitmap(109, 74, summer, 5, 27, color);
          break;
      case AUTUMN:
          display.drawBitmap(109, 74, autumn, 5, 26, color);
          break;
      case WINTER:
          display.drawBitmap(109, 74, winter, 5, 24, color);
          break;
  }
  
}

void Watchy7SEG::drawTimeDigits(int hour, int minute, int x_start, int y_start) {
  uint16_t color = DARKMODE ? GxEPD_WHITE : GxEPD_BLACK;
  int h1 = hour / 10;
  int h2 = hour % 10;
  int m1 = minute / 10;
  int m2 = minute % 10;
  if (h1 != 0 || hour >= 10) {
    display.drawBitmap(x_start, y_start, NUM_BITMAPS[h1], 3, 5, color);
  }
  display.drawBitmap(x_start + 4, y_start, NUM_BITMAPS[h2], 3, 5, color);
  display.drawBitmap(x_start + 8, y_start, num_dots, 3, 5, color);
  display.drawBitmap(x_start + 12, y_start, NUM_BITMAPS[m1], 3, 5, color);
  display.drawBitmap(x_start + 16, y_start, NUM_BITMAPS[m2], 3, 5, color);
}

void Watchy7SEG::drawWatchFace() {
  //Serial.print("drawWatchFace");
  if (currentTime.Hour == 3 && currentTime.Minute == 0) {
    if (!WAS_NTP_UPDATE) {
      if (connectWiFi()) syncNTP();
      WAS_NTP_UPDATE = true;
    }
  } else {
    WAS_NTP_UPDATE = false;
  }
  display.fillScreen(DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
  display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
  drawBgrd();
  drawTime();
  drawDate();
  drawSteps();
  drawBattery();

  display.drawBitmap(118, 168, WIFI_CONFIGURED ? wifi : wifioff, 25, 18, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
  switch (DISPMODE) {
  case DISMODE_SECS:
    drawMemTime();
    drawSeconds();
    break;
  case DISMODE_MOON:
    drawMoon();
    drawMoonTimes();
    drawSeason();
    break;
  case DISMODE_SUN:
    drawSun();
    drawSunTimes();
    drawSeason();
    break;
  case DISMODE_WEATHER:
    drawWeather();
    drawSeason();
    break;
  }
}

void Watchy7SEG::drawTime() {
  const unsigned char *digitBitmaps[] = {fd_0, fd_1, fd_2, fd_3, fd_4,
                                         fd_5, fd_6, fd_7, fd_8, fd_9};
  const int x = 34;
  const int y = 63;

  int hours = currentTime.Hour;
  int minutes = currentTime.Minute;
  int color = DARKMODE ? GxEPD_WHITE : GxEPD_BLACK;
  if (HOUR24 == false) {
    display.fillRect (x, y, 25, 9, DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
    if (hours >= 12) {
      display.drawBitmap (x, y, pm, 25, 9, color);
    } else {
      display.drawBitmap (x, y, am, 25, 9, color);
    }
  }
  if (HOUR24 == false) {
    if (hours > 12) {
      hours -= 12;
    } else if (hours == 0) {
      hours = 12;
    }
  }
  int hourTens = hours / 10;
  int hourOnes = hours % 10;
  int minuteTens = minutes / 10;
  int minuteOnes = minutes % 10;
  if (hourTens > 0) {
    display.drawBitmap(11, 5, digitBitmaps[hourTens], 33, 53, color);
  }
  display.drawBitmap(55, 5, digitBitmaps[hourOnes], 33, 53, color);
  display.drawBitmap(111, 5, digitBitmaps[minuteTens], 33, 53, color);
  display.drawBitmap(155, 5, digitBitmaps[minuteOnes], 33, 53, color);
}

void Watchy7SEG::drawMemTime(){
  time_t unixSeconds = makeTime(currentTime);
  long diff = ((long)unixSeconds - LASTMEMTIME) / 60;
  int mem_h = (int)diff / 60;
  int mem_m = (int)diff % 60;
  int mem_x[4] = {115, 136, 161, 182};
  const uint8_t *dd_bitmaps[] = {dd_0, dd_1, dd_2, dd_3, dd_4,
                                 dd_5, dd_6, dd_7, dd_8, dd_9};
  const uint16_t bitmap_w = 16;
  const uint16_t bitmap_h = 25;
  const uint16_t color = DARKMODE ? GxEPD_WHITE : GxEPD_BLACK;
  int memDigits[4] = {mem_h / 10, mem_h % 10, mem_m / 10, mem_m % 10};
  Serial.print("diff: ");
  Serial.println(diff);
  //Serial.print("LASTMEMTIME: ");
  //Serial.println(LASTMEMTIME);
  //Serial.print("unixSeconds: ");
  //Serial.println(unixSeconds);
  if ( LASTMEMTIME > 0) {
    for (int i = 0; i < 4; i++) {
      int digit = memDigits[i];
      if (digit < 0 || digit > 9) digit = 0;
      display.drawBitmap(mem_x[i], 112, dd_bitmaps[digit], bitmap_w, bitmap_h, color);
    }
  }
}

void Watchy7SEG::drawSeconds() {
  const int16_t clearX = 115;
  const int16_t clearY = 66;
  const int16_t clearW = 37;
  const int16_t clearH = 25;
  int sec_x[2] = {115, 136};
  const uint8_t *dd_bitmaps[] = {dd_0, dd_1, dd_2, dd_3, dd_4,
                                 dd_5, dd_6, dd_7, dd_8, dd_9};

  int secDigits[2] = {currentTime.Second / 10, currentTime.Second % 10};

  display.fillRect(clearX, clearY, clearW, clearH, DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
  const uint16_t bitmap_w = 16;
  const uint16_t bitmap_h = 25;
  const uint16_t color = DARKMODE ? GxEPD_WHITE : GxEPD_BLACK;
  for (int i = 0; i < 2; i++) {
    int digit = secDigits[i];
    if (digit >= 0 && digit <= 9) {
      display.drawBitmap(sec_x[i], clearY, dd_bitmaps[digit], bitmap_w, bitmap_h, color);
    }
  }
  if (currentTime.Second == 0) {
    if (DISPLAYTYPE == 1) {
      display.setPartialWindow(11, 5, 200 - 11, 53);
      display.fillRect(11, 5, 200 - 11, 53, DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
    }
    drawTime();
    display.fillRect(clearX, clearY, clearW, clearH, DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
    if (DISPLAYTYPE == 1) {
      display.nextPage();
      display.setPartialWindow(clearX, clearY, clearW, clearH);
    }
  }
  if (DISPLAYTYPE == 1) {
    display.nextPage();
  } else {
    display.display(false);
  }
}

void Watchy7SEG::drawDate() {
  display.setFont(&Seven_Segment10pt7b);

  int16_t x1, y1;
  uint16_t w, h;

  String dayOfWeek = ddayNames_P[currentTime.Wday];
  int l = dayOfWeek.length();
  if (l > WEEKDAY_MAXNoOFCHARS)
    l = WEEKDAY_MAXNoOFCHARS;
  dayOfWeek = dayOfWeek.substring(0, l);
  display.getTextBounds(dayOfWeek, 5, 85, &x1, &y1, &w, &h);
  if (currentTime.Wday == 4) {
  }
  display.setCursor(76 - w, 86);
  display.println(dayOfWeek);

  String month = dmonthNames_P[currentTime.Month];
  display.getTextBounds(month, 60, 110, &x1, &y1, &w, &h);
  display.setCursor(79 - w, 110);
  display.println(month);

  const uint8_t *dd_bitmaps[] = {dd_0, dd_1, dd_2, dd_3, dd_4,
                                 dd_5, dd_6, dd_7, dd_8, dd_9};

  int dayDigits[2] = {currentTime.Day / 10, currentTime.Day % 10};

  int fullYear = currentTime.Year + 1970;
  int yearDigits[4];
  yearDigits[0] = fullYear / 1000;
  yearDigits[1] = (fullYear / 100) % 10;
  yearDigits[2] = (fullYear / 10) % 10;
  yearDigits[3] = fullYear % 10;

  int day_x[2] = {8, 29};
  int year_x[4] = {8, 29, 50, 71};
  int y_day = 95;
  int y_year = 129;
  const uint16_t bitmap_w = 16;
  const uint16_t bitmap_h = 25;
  const uint16_t color = DARKMODE ? GxEPD_WHITE : GxEPD_BLACK;

  for (int i = 0; i < 2; i++) {
    int digit = dayDigits[i];
    if (digit >= 0 && digit <= 9) {
      display.drawBitmap(day_x[i], y_day, dd_bitmaps[digit], bitmap_w, bitmap_h,
                         color);
    }
  }

  for (int i = 0; i < 4; i++) {
    int digit = yearDigits[i];
    if (digit >= 0 && digit <= 9) {
      display.drawBitmap(year_x[i], y_year, dd_bitmaps[digit], bitmap_w,
                         bitmap_h, color);
    }
  }
}

void Watchy7SEG::drawSteps() {
  const unsigned char *digitBitmaps[] = {dd_0, dd_1, dd_2, dd_3, dd_4,
                                         dd_5, dd_6, dd_7, dd_8, dd_9};

  if (currentTime.Hour == 0 && currentTime.Minute == 0) {
    sensor.resetStepCounter();
  }
  uint32_t stepCount = sensor.getCounter();
  uint32_t l5 = 61 * stepCount / 10000;
  if (l5 > 61) {
    l5 = 61;
  }
  display.fillRect(131, 148, l5, 9, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);

  int digits[5];
  uint32_t tempSteps = stepCount;
  for (int i = 4; i >= 0; --i) {
    digits[i] = tempSteps % 10;
    tempSteps /= 10;
  }
  const int x_coords[] = {8, 29, 50, 71, 92};
  const int y_coord = 165;
  for (int i = 0; i < 5; ++i) {
    int digit_value = digits[i];
    display.drawBitmap(x_coords[i], y_coord,
                       digitBitmaps[digit_value],
                       16, 25, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
  }
}

void Watchy7SEG::drawBattery() {
  int8_t batteryLevel = 0;
  float VBAT = getBatteryVoltage();

  for (size_t i = 0; i < NUM_LEVELS; ++i) {
    if (VBAT > BatteryLevels[i].voltage) {
      batteryLevel = BatteryLevels[i].level;
      break;
    }
  }
  display.fillRect(155, 169, batteryLevel, 15, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
}

void Watchy7SEG::drawBgrd() {
  int color = DARKMODE ? GxEPD_WHITE : GxEPD_BLACK;
  display.drawBitmap(0, 0, bmbrgd, 200, 200, color);
  switch (DISPMODE) {
  case DISMODE_SECS:
    display.drawBitmap(109, 62, bgrdadd_seconds, 40, 46, color);
    break;
  case DISMODE_MOON:
    display.drawBitmap(109, 62, bgrdadd_moon, 91, 80, color);
    break;
  case DISMODE_SUN:
    display.drawBitmap(109, 62, bgrdadd_sun, 91, 80, color);
    break;
  case DISMODE_WEATHER:
    display.drawBitmap(109, 62, METRIC ? bgrdadd_weather_c : bgrdadd_weather_f, 83, 65, color);
    break;
  }
}

void Watchy7SEG::drawMoon() {
  const unsigned char *waxingBitmaps[] = {luna1, luna12, luna11, luna10,
                                          luna9, luna8, luna7};
  const unsigned char *waningBitmaps[] = {luna1, luna2, luna3, luna4,
                                          luna5, luna6, luna7};

  const float lightThresholds[] = {0.1, 0.25, 0.4, 0.6, 0.75, 0.9, 1.0};

  moonData_t moon;
  double decimalHour = currentTime.Hour + (currentTime.Minute / 60.0);
  moon = moonP.getPhase(currentTime.Year + 1970, currentTime.Month, currentTime.Day, decimalHour);
  int bitmapIndex = -1;
  for (int i = 0; i < 7; ++i) {
    if (moon.percentLit <= lightThresholds[i]) {
      bitmapIndex = i;
      break;
    }
  }

  if (bitmapIndex != -1) {
    const unsigned char *selectedBitmap;
    if (moon.angle <= 180) {
      selectedBitmap = waxingBitmaps[bitmapIndex];
    } else {
      selectedBitmap = waningBitmaps[bitmapIndex];
    }

    display.drawBitmap(131, 74, selectedBitmap, 61, 61, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
  }
}


void Watchy7SEG::drawMoonTimes() {
    int color = DARKMODE ? GxEPD_WHITE : GxEPD_BLACK;
    const double MOON_LATITUDE = GET_LATITUDE((LOC)); 
    const double MOON_LONGITUDE = GET_LONGITUDE((LOC));
    MoonRise moonRise;

    int moon_rise_h, moon_rise_m;
    int moon_set_h, moon_set_m;
    
    time_t now_local = now(); 
    long offset_sec = GMT_OFFSET_SEC + (Watchy::isDST(now_local) ? 3600L : 0L);
    time_t now_utc = now_local - offset_sec;
    
    int year = currentTime.Year + 1970;
    int32_t month = currentTime.Month;
    int32_t day = currentTime.Day;
    float lat = GET_LATITUDE((LOC)); 
    float seasonValue = WatchyDusk2Dawn::getCurrentAstronomicalSeasonValue(year, month, day, lat);
    int yy = 71.0f + (seasonValue / 360.0f) * (132.0f - 71.0f);
    display.drawBitmap(116, yy, arr, 3, 5, color);

    moonRise.queryTime = now_utc; 

    moonRise.calculate(
        MOON_LATITUDE,    
        MOON_LONGITUDE,
        now_utc           
    );

    if (!moonRise.hasRise && !moonRise.hasSet) {
        if (moonRise.isVisible) {
            display.drawBitmap(116, 137, alwayvisible, 51, 5, color);
            display.drawBitmap(116, 67, alwayvisible, 51, 5, color);
        } else {
            display.drawBitmap(116, 137, notvisible, 37, 5, color);
            display.drawBitmap(116, 67, notvisible, 37, 5, color);
        }
    } else {
        long offset_sec = GMT_OFFSET_SEC + (Watchy::isDST(now_local) ? 3600L : 0L); 
        time_t mr_local_time = moonRise.riseTime + offset_sec; // UTC + Offset
        time_t ms_local_time = moonRise.setTime + offset_sec;  // UTC + Offset
        long rise_seconds_since_midnight = mr_local_time % 86400L;
        long set_seconds_since_midnight = ms_local_time % 86400L;
        if (set_seconds_since_midnight < 0) {
            set_seconds_since_midnight += 86400L;
        }
        if (rise_seconds_since_midnight < 0) {
            rise_seconds_since_midnight += 86400L;
        }
        if (moonRise.hasRise) {
            moon_rise_h = rise_seconds_since_midnight / 3600L;
            moon_rise_m = (rise_seconds_since_midnight % 3600L) / 60L;
            drawTimeDigits(moon_rise_h, moon_rise_m, 116, 67);
            display.drawBitmap(139, 67, moonrise, 36, 5, color);
        } else {
            display.drawBitmap(116, 67, norise, 24, 5, color);
        }
        
        if (moonRise.hasSet) {
            moon_set_h = set_seconds_since_midnight / 3600L;
            moon_set_m = (set_seconds_since_midnight % 3600L) / 60L;
            drawTimeDigits(moon_set_h, moon_set_m, 116, 137);
            display.drawBitmap(139, 137, moonset, 34, 5, color);
        } else {
            display.drawBitmap(116, 137, noset, 22, 5, color);
        }
    }

}

void Watchy7SEG::drawSun() {
  time_t ct = now();
  bool isDST = Watchy::isDST(ct);
  int year = currentTime.Year + 1970;
  int32_t month = currentTime.Month;
  int32_t day = currentTime.Day;
  float lat = GET_LATITUDE((LOC)); 
  float lon = GET_LONGITUDE((LOC));
  float tz = LOC_TZ;
  int sr = WatchyDusk2Dawn::sunrise (year, month, day, lat, lon, tz, isDST);
  int ss = WatchyDusk2Dawn::sunset (year, month, day, lat, lon, tz,  isDST);
  int now_minutes = currentTime.Hour * 60 + currentTime.Minute;
  bool isPolarSummer = false;
  int highest_min = 0;
  int lowest_min = 0;
  
  
  if (sr == -1 && ss == -1) {
    float solar_declination = WatchyDusk2Dawn::getSolarDeclination(year, month, day);
    if (lat * solar_declination >= 0) {
      //if (WatchyDusk2Dawn::isPolarWinter(year, month, day, lat, lon, tz, isDST)) {
      //return; 
      isPolarSummer = true;
      highest_min = WatchyDusk2Dawn::getSolarNoonTime (year, month, day, lat, lon, tz, isDST);
      lowest_min = WatchyDusk2Dawn::getSolarMidnightTime (year, month, day, lat, lon, tz, isDST);

      if (lowest_min < highest_min) {
        sr = lowest_min;
        ss = highest_min;
      } else {
        sr = lowest_min;
        ss = highest_min;
      }
    }
  }


  if (isPolarSummer) {
    double total_minutes_in_day = 1440.0;
    double now_adjusted = (now_minutes - sr + total_minutes_in_day);
    now_adjusted = fmod(now_adjusted, total_minutes_in_day);
    double tx = 95.0 / total_minutes_in_day * now_adjusted;
    int t = static_cast<int>(std::round(tx)) * 2;
    if (t < 190) { 
      bool isNorth = IS_NORTH;
      int x = 125 + (isNorth ? SunCurve[t] : (61 - SunCurve[t]));
      int y = 124 - SunCurve[t + 1];

      display.drawBitmap(x - 9, y - 9, sun, 18, 18, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
      display.drawBitmap(x - 9, y - 9, sundisk, 18, 18, DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
    }
  } else {
    if (now_minutes >= sr || now_minutes <= ss) {
      bool isNorth = IS_NORTH;
      double tx = 95.0 / (ss - sr) * (now_minutes - sr);
      int t = static_cast<int>(std::round(tx)) * 2;

      if (t < 190) {
        int x = 125 + (isNorth ? SunCurve[t] : (61 - SunCurve[t]));
        int y = 124 - SunCurve[t + 1];

        display.drawBitmap(x - 9, y - 9, sun, 18, 18, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        display.drawBitmap(x - 9, y - 9, sundisk, 18, 18, DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
      }
    }
  }
}

 
void Watchy7SEG::drawSunTimes() {
  int year = currentTime.Year + 1970;
  int32_t month = currentTime.Month;
  int32_t day = currentTime.Day;
  float lat = GET_LATITUDE((LOC)); 
  float lon = GET_LONGITUDE((LOC));
  float tz = LOC_TZ;
  time_t ct = now();
  bool isDST = Watchy::isDST(ct);
  int sr = WatchyDusk2Dawn::sunrise(year, month, day, lat, lon, tz, isDST);
  int ss = WatchyDusk2Dawn::sunset(year, month, day, lat, lon, tz, isDST);
  int now_minutes = currentTime.Hour * 60 + currentTime.Minute;
  const uint16_t color = DARKMODE ? GxEPD_WHITE : GxEPD_BLACK;
  bool isPolarSummer = false;
  int highest_min = 0;
  int lowest_min = 0;
  /*
  if (sr == -1 || ss == -1) {
    float solar_declination = 0;
    float solar_declination = WatchyDusk2Dawn::getSolarDeclination(year, month, day);
    if (lat * solar_declination < 0) {
      //if (WatchyDusk2Dawn::isPolarWinter(year, month, day, lat, lon, tz, isDST)) {
      display.drawBitmap(139, 67, polarwinter, 44, 5, color);
      display.drawBitmap(139, 137, polarwinter, 44, 5, color);
      return;
    } else {
      isPolarSummer = true;
      highest_min = WatchyDusk2Dawn::getSolarNoonTime(year, month, day, lat, lon, tz, isDST);
      lowest_min = WatchyDusk2Dawn::getSolarMidnightTime(year, month, day, lat, lon, tz, isDST);
      if (lowest_min < highest_min) {
        sr = lowest_min;
        ss = highest_min;
      } else {
        sr = lowest_min;
        ss = highest_min;
      }
    }
  }
  */
  int travel_range = isPolarSummer ? 1440 : ss - sr;
  long current_minutes = now_minutes;
  int tk;

  if (isPolarSummer) {
    long minutes_since_sr = (current_minutes - sr + 1440) % 1440;
    tk = (int)((minutes_since_sr) * 60L / travel_range);
  } else {
    if (current_minutes >= ss) {
      tk = 60;
    } else if (current_minutes <= sr) {
      tk = 0;
    } else {
      tk = (int)((current_minutes - sr) * 60L / travel_range);
    }
  }

  float seasonValue = WatchyDusk2Dawn::getCurrentAstronomicalSeasonValue(year, month, day, lat);
  int yy = 71.0f + (seasonValue / 360.0f) * (132.0f - 71.0f);
  display.drawBitmap(116, yy, arr, 3, 5, color);

  int sunrise_h = sr / 60;
  int sunrise_m = sr % 60;
  int sunset_h = ss / 60;
  int sunset_m = ss % 60;

  if (HOUR24 == false) {
    if (sunrise_h > 12) sunrise_h -= 12;
    if (sunset_h > 12) sunset_h -= 12;
  }

  drawTimeDigits(sunset_h, sunset_m, 116, 137);
  if (isPolarSummer) {
    display.drawBitmap(139, 137, polarsummer, 51, 5, color);
  } else {
    display.drawBitmap(139, 137, sunset, 28, 5, color);
  }
  drawTimeDigits(sunrise_h, sunrise_m, 116, 67);
  if (isPolarSummer) {
    display.drawBitmap(139, 67, polarsummer, 51, 5, color);
  } else {
    display.drawBitmap(139, 67, sunrise, 30, 5, color);
  }

  const uint8_t *md_bitmaps[] = {md_0, md_1, md_2, md_3, md_4,
                                 md_5, md_6, md_7, md_8, md_9};

  int h = 0;
  int m = 0;
  int s = 0;

  if (isPolarSummer) {
    int next_min = 0;
    long time_to_highest = (highest_min - current_minutes + 1440) % 1440;
    long time_to_lowest = (lowest_min - current_minutes + 1440) % 1440;

    if (time_to_highest <= time_to_lowest) {
      next_min = time_to_highest;
      s = -2;
    } else {
      next_min = time_to_lowest;
      s = -3;
    }

    h = next_min / 60;
    m = next_min % 60;

  } else {
    if (current_minutes > sr && current_minutes < ss) {
      int nxtset = ss - current_minutes;
      h = nxtset / 60;
      m = nxtset % 60;
      s = -2;
    } else if (current_minutes > ss || current_minutes < sr) {
      int nxtrise = 0;
      if (current_minutes < sr) {
        nxtrise = sr - current_minutes;
      } else if (current_minutes > ss) {
        nxtrise = (24 * 60 - current_minutes) + sr;
      }
      h = nxtrise / 60;
      m = nxtrise % 60;
      s = -3;
    }
  }

  const uint16_t bitmap_w = 10;
  const uint16_t bitmap_h = 16;
  int mdigits[2] = {m / 10, m % 10};
  int digits[2] = {h / 10, h % 10};
  
  if (digits[0] == 0) {
      display.drawBitmap(152, 111, md_bitmaps[digits[1]], bitmap_w, bitmap_h, color);
      display.drawBitmap(163, 122, NUM_BITMAPS[mdigits[0]], 3, 5, color);
      display.drawBitmap(167, 122, NUM_BITMAPS[mdigits[1]], 3, 5, color);
  } else {
      display.drawBitmap(147, 111, md_bitmaps[digits[0]], bitmap_w, bitmap_h, color);
      display.drawBitmap(158, 111, md_bitmaps[digits[1]], bitmap_w, bitmap_h, color);
      display.drawBitmap(169, 122, NUM_BITMAPS[mdigits[0]], 3, 5, color);
      display.drawBitmap(173, 122, NUM_BITMAPS[mdigits[1]], 3, 5, color);
  }
}


void Watchy7SEG::drawWeather() {
  weatherData currentWeather = getWeatherData((currentTime.Minute == 0 || firstWeatherNotDone) ? OPENWEATHERMAP_URL : "");
  firstWeatherNotDone = false;

  int8_t temperature = currentWeather.temperature;
  bool sunvisible = currentWeather.sunvisible;
  int16_t weatherConditionCode = currentWeather.weatherConditionCode;
  bool tempneg = temperature < 0;
  if (tempneg)
    temperature = -1 * temperature;

  int tmp_x[4] = {109, 130, 151};
  const uint8_t *dd_bitmaps[] = {dd_0, dd_1, dd_2, dd_3, dd_4,
                                 dd_5, dd_6, dd_7, dd_8, dd_9};

  int tmpDigits[3] = {0, temperature / 10, temperature % 10};

  const uint16_t bitmap_w = 16;
  const uint16_t bitmap_h = 25;
  const uint16_t color = DARKMODE ? GxEPD_WHITE : GxEPD_BLACK;
  for (int i = 0; i < 3; i++) {
    int digit = tmpDigits[i];
    if (i > 0 && digit >= 0 && digit <= 9) {
      display.drawBitmap(tmp_x[i], 113, dd_bitmaps[digit], bitmap_w, bitmap_h,
                         color);
    } else if (i == 0 && tempneg) {
      display.drawBitmap(tmp_x[i], 113, dd_minus, bitmap_w, bitmap_h, color);
    }
  }

  const unsigned char *weatherIcon;
  weatherIcon = chip;
  //Serial.print("weatherConditionCode:");
  //Serial.print(weatherConditionCode);
  if (weatherConditionCode > 801) {
    weatherIcon = cloudy;
  } else if (weatherConditionCode == 801) {
    weatherIcon = sunvisible ? cloudsun : cloudsun_night;
  } else if (weatherConditionCode == 800) {
    weatherIcon = sunvisible ? sunny : sunny_night;
  } else if (weatherConditionCode >= 700) {
    weatherIcon = atmosphere;
  } else if (weatherConditionCode >= 600) {
    weatherIcon = snow;
  } else if (weatherConditionCode >= 500) {
    weatherIcon = rain;
  } else if (weatherConditionCode >= 300) {
    weatherIcon = drizzle;
  } else if (weatherConditionCode >= 200) {
    weatherIcon = thunderstorm;
  }
  display.drawBitmap(130, 74, weatherIcon, WEATHER_ICON_WIDTH, WEATHER_ICON_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
}