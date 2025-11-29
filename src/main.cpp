#include "Watchy_7_SEG.h"
#include "settings.h"

// Hier wird die 'settings' Variable tatsächlich DEFINIERT und initialisiert.
// Sie kann in anderen Dateien verwendet werden, aber nur hier definiert werden.
watchySettings settings = {
  .cityID = "",
  .weatherAPIKey = "",
  .weatherURL = OPENWEATHERMAP_URL,
  .weatherUnit = "metric",
  .weatherLang = "",
  .weatherUpdateInterval = 0,
  .ntpServer = NTP_SERVER,
  .gmtOffset = GMT_OFFSET_SEC,
  .vibrateOClock = true
};

Watchy7SEG watchy(settings);

void setup(){
    watchy.init();
}

void loop(){

}
