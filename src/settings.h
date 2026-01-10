#ifndef SETTINGS_H
#define SETTINGS_H

// =================================================================================
#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_SEC 3600L

#define DISPLAYTYPE 1
// 1 == correct working display
// 0 == not working display

#define HOUR24 true
#define METRIC true
// ... you have also set your weather url to unit=imperial
#define LOC 49.35, 8.65, 1
#define LOC_TZ 1.0
#define OPENWEATHERMAP_URL "https://api.openweathermap.org/data/3.0/onecall?lat=49.35&lon=8.6&exclude=minutely,hourly,daily,alerts&appid=yourapikey&units=metric&lang=de" // open weather api
#define WLAN_SSID "myWLAN"
#define WLAN_PWD "myWLANPWD"
// power saving WLAN configuration if wanted:
#define WLAN_IP ""	// enter a static ip address outside of the routers dynamic range, set to "" for default WLAN connection	
#define WLAN_GATEWAY "192.168.0.1" // only needed if WLAN_IP is not an empty string
#define WLAN_SUBNET "255.255.255.0" // only needed if WLAN_IP is not an empty string
#define WLAN_DNS "8.8.8.8" // only needed if WLAN_IP is not an empty string, perhaps use GATEWAY IP to speed up

#define WEEKDAY_SUN "Sunday"
#define WEEKDAY_MON "Monday"
#define WEEKDAY_TUS "Tuesday"
#define WEEKDAY_WEN "Wednesday"
#define WEEKDAY_THU "Thursday"
#define WEEKDAY_FRI "Friday"
#define WEEKDAY_SAT "Saturday"
#define WEEKDAY_MAXNoOFCHARS 3
#define MONTH_JAN "Jan"
#define MONTH_FEB "Feb"
#define MONTH_MAR "Mar"
#define MONTH_APR "Apr"
#define MONTH_MAY "May"
#define MONTH_JUN "Jun"
#define MONTH_JUL "Jul"
#define MONTH_AUG "Aug"
#define MONTH_SEP "Sep"
#define MONTH_OCT "Oct"
#define MONTH_NOV "Nov"
#define MONTH_DEC "Dec"

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
