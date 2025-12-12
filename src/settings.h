#ifndef SETTINGS_H
#define SETTINGS_H

// =================================================================================

#define DISPLAYTYPE 1
// 1 == correct working display
// 0 == not working display

#define HOUR24 true
#define METRIC true
// ... you have also set your weather url to unit=imperial
#define LOC 49.35, 8.65, 1
#define LOC_TZ 1.0
#define OPENWEATHERMAP_URL "https://api.openweathermap.org/data/3.0/onecall?lat=49.35&lon=8.6&exclude=minutely,hourly,daily,alerts&appid=yourapikey&units=metric&lang=de" // open weather api
#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_SEC 3600L

#define WEEKDAY_SUN "Sonntag"
#define WEEKDAY_MON "Montag"
#define WEEKDAY_TUS "Dienstag"
#define WEEKDAY_WEN "Mittwoch"
#define WEEKDAY_THU "Donnerstag"
#define WEEKDAY_FRI "Freitag"
#define WEEKDAY_SAT "Samstag"
#define WEEKDAY_MAXNoOFCHARS 4
#define MONTH_JAN "Jan"
#define MONTH_FEB "Feb"
#define MONTH_MAR "Mar"
#define MONTH_APR "Apr"
#define MONTH_MAY "Mai"
#define MONTH_JUN "Jun"
#define MONTH_JUL "Jul"
#define MONTH_AUG "Aug"
#define MONTH_SEP "Sep"
#define MONTH_OCT "Okt"
#define MONTH_NOV "Nov"
#define MONTH_DEC "Dez"

// =================================================================================

#define EXPAND_MACRO(x) x
#define GET_LATITUDE_IMPL(A, B, C) A
#define GET_LONGITUDE_IMPL(A, B, C) B
#define GET_TIMEZONE_IMPL(A, B, C) C
#define PASS_LOC(M) M
#define GET_LATITUDE(M) EXPAND_MACRO(GET_LATITUDE_IMPL M)
#define GET_LONGITUDE(M) EXPAND_MACRO(GET_LONGITUDE_IMPL M)
#define GET_TIMEZONE(M) EXPAND_MACRO(GET_TIMEZONE_IMPL M)

#define IS_NORTH GET_LATITUDE((LOC)) > 0

#endif
