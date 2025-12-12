#ifndef WatchyDusk2Dawn_h
#define WatchyDusk2Dawn_h

#include "Arduino.h"
#include <math.h>

// Das Enum bleibt als C++-Feature erhalten
enum Season {
    SPRING, 
    SUMMER, 
    AUTUMN, 
    WINTER 
};

// Die Klasse wird zum reinen Container für statische Methoden
class WatchyDusk2Dawn {
    public:
    
    static int sunrise(int y, int m, int d, float latitude, float longitude, float timezone, bool isDST);
    static int sunset(int y, int m, int d, float latitude, float longitude, float timezone, bool isDST);
    
    static int getSolarNoonTime(int y, int m, int d, float latitude, float longitude, float timezone, bool isDST);
    static int getSolarMidnightTime(int y, int m, int d, float latitude, float longitude, float timezone, bool isDST);
    
    //static bool isPolarWinter(int y, int m, int d, float latitude, float longitude, float timezone, bool isDST);
    
    static float getCurrentAstronomicalSeasonValue(int y, int m, int d, float latitude);
    static Season getCurrentSeason(int y, int m, int d, float latitude); 
    
    
    static float getSolarDeclination(int y, int m, int d);
    static bool min2str(char*, int);
    
    static float jDay(int, int, int);
    static float fractionOfCentury(float);
    static float sunTrueLong(float);

    private:
    
    static int sunriseSet(bool isRise, int y, int m, int d, float latitude, float longitude, float timezone, bool isDST);
    static float sunriseSetUTC(bool isRise, float jday, float latitude, float longitude);
    static float equationOfTime(float);
    static float meanObliquityOfEcliptic(float);
    static float eccentricityEarthOrbit(float);
    static float sunDeclination(float);
    static float sunApparentLong(float);
    static float sunEqOfCenter(float);
    static float hourAngleSunrise(float lat, float solarDec);
    static float obliquityCorrection(float);
    static float geomMeanLongSun(float);
    static float geomMeanAnomalySun(float);
    static float radToDeg(float);
    static float degToRad(float);
    static bool zeroPadTime(char*, byte);
};

#endif