/*  WatchyDusk2Dawn.cpp
 *  Get time of sunrise and sunset.
 *  Based on <https://github.com/dmkishi/WatchyDusk2Dawn>
 */

#include "Arduino.h"
#include "WatchyDusk2Dawn.h"
#include <math.h>

/******************************************************************************/
/*                                   PUBLIC STATIC                             */
/******************************************************************************/
// 🛑 KONSTRUKTOR WURDE ENTFERNT

int WatchyDusk2Dawn::sunrise(int y, int m, int d, float latitude, float longitude, float timezone, bool isDST) {
    return sunriseSet(true, y, m, d, latitude, longitude, timezone, isDST);
}

int WatchyDusk2Dawn::sunset(int y, int m, int d, float latitude, float longitude, float timezone, bool isDST) {
    return sunriseSet(false, y, m, d, latitude, longitude, timezone, isDST);
}

bool WatchyDusk2Dawn::min2str(char *str, int minutes) {
    bool isError = false;

    if (minutes < 0 || minutes >= 1440) {
        isError = true;
    }

    float floatHour = minutes / 60.0;
    float floatMinute = 60.0 * (floatHour - floor(floatHour));
    byte byteHour = (byte)floatHour;
    byte byteMinute = (byte)floatMinute;

    if (byteMinute > 59) {
        byteHour += 1;
        byteMinute = 0;
    }

    char strHour[] = "00";
    char strMinute[] = "00";

    isError = isError ? isError : !zeroPadTime(strHour, byteHour);
    isError = isError ? isError : !zeroPadTime(strMinute, byteMinute);

    if (!isError) {
        str[0] = strHour[0];
        str[1] = strHour[1];
        str[2] = ':';
        str[3] = strMinute[0];
        str[4] = strMinute[1];
        str[5] = '\0';
    } else {
        str[0] = 'E';
        str[1] = 'R';
        str[2] = 'R';
        str[3] = 'O';
        str[4] = 'R';
        str[5] = '\0';
    }

    return !isError;
}

float WatchyDusk2Dawn::getSolarDeclination(int y, int m, int d) {
    float jday = jDay(y, m, d);
    float t = fractionOfCentury(jday);
    return sunDeclination(t);
}

/*
bool WatchyDusk2Dawn::isPolarWinter(int y, int m, int d, float latitude, float longitude, float timezone, bool isDST) {
    // Ruft die neue statische Methode mit allen Parametern auf
    int sunrise_time = sunrise(y, m, d, latitude, longitude, timezone, isDST);
    int sunset_time = sunset(y, m, d, latitude, longitude, timezone, isDST);

    if (sunrise_time != -1 || sunset_time != -1) {
        return false;
    }

    float solar_declination = getSolarDeclination(y, m, d);

    // Nutzt den übergebenen latitude-Parameter
    if (latitude * solar_declination < 0) {
        return true;
    } else {
        return false;
    }
}
*/

int WatchyDusk2Dawn::getSolarNoonTime(int y, int m, int d, float latitude, float longitude, float timezone, bool isDST) {
    float jday = jDay(y, m, d);
    float t = fractionOfCentury(jday);
    float E = equationOfTime(t);
    
    // Nutzt die übergebenen Parameter
    float noon_utc_minutes = 720.0 - (4.0 * longitude) - E;
    float total_offset_minutes = timezone * 60.0;
    if (isDST) {
        total_offset_minutes += 60.0;
    }
    float noon_local_minutes = noon_utc_minutes + total_offset_minutes;
    while (noon_local_minutes < 0) {
        noon_local_minutes += 1440.0;
    }
    while (noon_local_minutes >= 1440.0) {
        noon_local_minutes -= 1440.0;
    }
    return round(noon_local_minutes);
}

int WatchyDusk2Dawn::getSolarMidnightTime(int y, int m, int d, float latitude, float longitude, float timezone, bool isDST) {
    // Nutzt die übergebenen Parameter beim Aufruf
    int noon_time = getSolarNoonTime(y, m, d, latitude, longitude, timezone, isDST);
    int midnight_time = noon_time + 720;
    if (midnight_time >= 1440) {
        midnight_time -= 1440;
    }
    return midnight_time;
}


float WatchyDusk2Dawn::getCurrentAstronomicalSeasonValue(int y, int m, int d, float latitude) {
    int year = y;
    int32_t month = m;
    int32_t day = d;
    
    float JD = jDay(year, month, day); 
    float T = fractionOfCentury(JD); 
    float sun_long = sunTrueLong(T); 
    
    sun_long = fmod(sun_long, 360.0);
    if (sun_long < 0) {
        sun_long += 360.0;
    }

    return sun_long;
}


Season WatchyDusk2Dawn::getCurrentSeason(int y, int m, int d, float latitude) {
    float sun_long = getCurrentAstronomicalSeasonValue(y, m, d, latitude);

    if (latitude < 0) {
        sun_long = fmod(sun_long + 180.0f, 360.0);
    }
    
    Season season_NH;
    
    if (sun_long >= 0.0 && sun_long < 90.0) {
        season_NH = SPRING;
    } else if (sun_long >= 90.0 && sun_long < 180.0) {
        season_NH = SUMMER;
    } else if (sun_long >= 180.0 && sun_long < 270.0) {
        season_NH = AUTUMN;
    } else { 
        season_NH = WINTER;
    }

    if (latitude < 0) {
        switch(season_NH) {
            case SPRING: return AUTUMN;
            case SUMMER: return WINTER;
            case AUTUMN: return SPRING;
            case WINTER: return SUMMER;
        }
    }
    
    return season_NH;
}


    
/******************************************************************************/
/*                                  PRIVATE STATIC                            */
/******************************************************************************/
int WatchyDusk2Dawn::sunriseSet(bool isRise, int y, int m, int d, float latitude, float longitude, float timezone, bool isDST) {
    float jday, newJday, timeUTC, newTimeUTC;
    int timeLocal;

    jday = jDay(y, m, d);
    // 🛑 Nutzt die übergebenen Parameter
    timeUTC = sunriseSetUTC(isRise, jday, latitude, longitude);

    newJday = jday + timeUTC / (60 * 24);
    // 🛑 Nutzt die übergebenen Parameter
    newTimeUTC = sunriseSetUTC(isRise, newJday, latitude, longitude);

    if (!isnan(newTimeUTC)) {
        // 🛑 Nutzt den übergebenen timezone-Parameter
        timeLocal = (int)round(newTimeUTC + (timezone * 60));
        timeLocal += (isDST) ? 60 : 0;
    } else {
        timeLocal = -1;
    }

    return timeLocal;
}

float WatchyDusk2Dawn::sunriseSetUTC(bool isRise, float jday, float latitude, float longitude) {
    float t = fractionOfCentury(jday);
    float eqTime = equationOfTime(t);
    float solarDec = sunDeclination(t);
    // 🛑 Nutzt den übergebenen latitude-Parameter
    float hourAngle = hourAngleSunrise(latitude, solarDec);

    hourAngle = isRise ? hourAngle : -hourAngle;
    float delta = longitude + radToDeg(hourAngle);
    float timeUTC = 720 - (4 * delta) - eqTime; // in minutes
    return timeUTC;
}

float WatchyDusk2Dawn::equationOfTime(float t) {
    float epsilon = obliquityCorrection(t);
    float l0 = geomMeanLongSun(t);
    float e = eccentricityEarthOrbit(t);
    float m = geomMeanAnomalySun(t);

    float y = tan(degToRad(epsilon) / 2.0f);
    y *= y;

    float sin2l0 = sin(2.0f * degToRad(l0));
    float sinm = sin(degToRad(m));
    float cos2l0 = cos(2.0f * degToRad(l0));
    float sin4l0 = sin(4.0f * degToRad(l0));
    float sin2m = sin(2.0f * degToRad(m));

    float Etime = y * sin2l0 - 2.0f * e * sinm + 4.0f * e * y * sinm * cos2l0 - 0.5f * y * y * sin4l0 - 1.25f * e * e * sin2m;
    return radToDeg(Etime) * 4.0f; // in minutes of time
}

float WatchyDusk2Dawn::meanObliquityOfEcliptic(float t) {
    float seconds = 21.448 - t * (46.8150 + t * (0.00059 - t * 0.001813));
    float e0 = 23 + (26 + (seconds / 60)) / 60;
    return e0; // in degrees
}

float WatchyDusk2Dawn::eccentricityEarthOrbit(float t) {
    float e = 0.016708634 - t * (0.000042037 + 0.0000001267 * t);
    return e; // unitless
}

float WatchyDusk2Dawn::sunDeclination(float t) {
    float e = obliquityCorrection(t);
    float lambda = sunApparentLong(t);

    float sint = sinf(degToRad(e)) * sinf(degToRad(lambda));
    float theta = radToDeg(asinf(sint));
    return theta; // in degrees
}

float WatchyDusk2Dawn::sunApparentLong(float t) {
    float o = sunTrueLong(t); 
    float omega = 125.04f - 1934.136f * t; 
    // NEU: sin -> sinf
    float lambda = o - 0.00569f - 0.00478f * sinf(degToRad(omega)); 
    return lambda; 
}

float WatchyDusk2Dawn::sunTrueLong(float t) {
    float l0 = geomMeanLongSun(t);
    float c = sunEqOfCenter(t);
    float O = l0 + c;
    return O; // in degrees
}

float WatchyDusk2Dawn::sunEqOfCenter(float t) {
    float m = geomMeanAnomalySun(t);
    float mrad = degToRad(m);
    float sinm = sin(mrad);
    float sin2m = sin(mrad * 2);
    float sin3m = sin(mrad * 3);
    float C = sinm * (1.914602 - t * (0.004817 + 0.000014 * t)) + sin2m * (0.019993 - 0.000101 * t) + sin3m * 0.000289;
    return C; // in degrees
}

float WatchyDusk2Dawn::hourAngleSunrise(float lat, float solarDec) {
    float latRad = degToRad(lat);
    float sdRad = degToRad(solarDec);
    float HAarg = (cos(degToRad(90.833)) / (cos(latRad) * cos(sdRad)) - tan(latRad) * tan(sdRad));
    float HA = acos(HAarg);
    return HA; // in radians (for sunset, use -HA)
}

float WatchyDusk2Dawn::obliquityCorrection(float t) {
    float e0 = meanObliquityOfEcliptic(t);
    float omega = 125.04f - 1934.136f * t;
    float e = e0 + 0.00256f * cos(degToRad(omega));
    return e; // in degrees
}

float WatchyDusk2Dawn::geomMeanLongSun(float t) {
    float L0 = 280.46646f + t * (36000.76983f + t * 0.0003032f);
    while (L0 > 360) {
        L0 -= 360;
    }
    while (L0 < 0) {
        L0 += 360;
    }
    return L0; // in degrees
}

float WatchyDusk2Dawn::geomMeanAnomalySun(float t) {
    float M = 357.52911f + t * (35999.05029f - 0.0001537f * t);
    return M; // in degrees
}

float WatchyDusk2Dawn::jDay(int year, int month, int day) {
    if (month <= 2) {
        year -= 1;
        month += 12;
    }

    int A = floor(year / 100);
    int B = 2 - A + floor(A / 4);
    return floor(365.25 * (year + 4716)) + floor(30.6001 * (month + 1)) +
           day + B - 1524.5;
}

float WatchyDusk2Dawn::fractionOfCentury(float jd) {
    return (jd - 2451545) / 36525;
}

float WatchyDusk2Dawn::radToDeg(float rad) {
    return 180 * rad / PI;
}

float WatchyDusk2Dawn::degToRad(float deg) {
    return PI * deg / 180;
}

bool WatchyDusk2Dawn::zeroPadTime(char *str, byte timeComponent) {
    if (timeComponent >= 100) {
        return false;
    }

    str[0] = (floor(timeComponent / 10)) + '0';
    str[1] = (timeComponent % 10) + '0';
    return true;
}