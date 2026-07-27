English

# Extension of the Repaired Starfield Watchy Firmware incl. Seconds Display via Partial Update

An extended Sstarfield watchface for Watchy

**Version 1.1.0** (the running firmware version is shown in the About screen)

![picture](/assets/watchy_seconds.gif)

Features
* three new display modes added to moon display: moon, sun, waether and seconds (showing seconds for 30 seconds using partial update)
* moon display (up button): moon phase, rise and set time
* sun display(up button): sun position and rise and set time
* weather (up button): weather icon and temperature, new nightly weather icons
* seconds (down button): seconds for 30 seconds with partial display update
* black on white <=> white on black (back button)
* menu (select button)
* back button: Dark Mode / Light Mode
* ntp automaticly daylight time setup
* ntp sync nightly every day at 3am
* weather update hourly or by selecting weather mode
* moon set and rise times
* sun set and rise times
* corrected battery status display (3.2V to 4.2V)
* 24h hours display is fixed configurated
* units are fixed configurated to "metric"
* create you own open weather key/url by defining url
* configration of month and day names, default german
* pressing up-button while showing seconds: syncNTP will be called
* pressing down-button while showing seconds: return to last display mode
* Memory Timestamp: Press Back-button while seconds display mode to set memory timestamp => differnce in hours and seconds will be shown after this in seconds-mode,
  press up-button while seconds display mode to clear memory timestamp, press menu-button while seconds display mode to switch off seconds display mode without any
  memory timestamp changes. (Nice option to remember taking medication.)
* the sun indicator does not display any data when the geocoordinates determine a polar winter, with the polar winter being displayed.
* during the polar summer, the time until the next noon or midnight is displayed.
* 24 Hour Display and imperial units are now configurable in settings.h.
* Season indicator: Display of the local season as vertical text, display of the astronomical season as an arrow on the scale (0° to 360°)
* WLAN with optional energy-saving mode and faster connection time (static IP, 90% reduction in normal connection time, connection in less than a second instead of the previous 3 to 7 seconds)

Changes in 1.1.0
* The hardware RTC now always runs in UTC. The NTP sync writes plain UTC, the GMT offset and the DST hour are added only at display time.
* DST is derived from the UTC time and the configured GMT offset and follows the EU rule (last Sunday in March and October, switching at 02:00 local standard time).
* Sunrise, sunset, moonrise and moonset are calculated from UTC with one shared offset, so they stay correct across the DST switch.
* Setting the time manually converts the entered local time back to UTC before storing it.
* The seconds display no longer leaves artefacts when the seconds roll over to 00.
* The About screen shows the firmware version, the raw UTC time read from the RTC and the active offset including a DST marker.

Note after updating to 1.1.0: flashing does not reset the RTC, it keeps running on its battery. Run Menu -> Sync NTP once, otherwise the old local time is interpreted as UTC
and the watch runs ahead by the offset.

All basic configurations can be done in settings.h.

I own two Watchys. Strangely enough, the displays behave differently; only one Watchy performs the partial update correctly. Set DISPLAYTYPE in settings.h
to 1 if you have a well running display, try to set it to 0, if not.

A magnetic watch strap for watchy: https://makerworld.com/en/models/2048997-magnetic-watch-strap-20mm-and-22mm?from=search#profileId-2211213

This repository contains a extendtion of the repaired version of the Starfield Watchy firmware (https://github.com/Szybet/watchy-starfield).
Thanks you to https://github.com/Szybet

-----------------------------------------------------------------------------------
Deutsch

# Erweiterung der reparierten Starfield Watchy-Firmware inkl. Sekundenanzeige durch Teil-Update

Ein erweitertes Sstarfield-Zifferblatt für Watchy

**Version 1.1.0** (die laufende Firmware-Version wird im About-Screen angezeigt)

![picture](/assets/watchy_seconds.gif)


Funktionen

* Drei neue Anzeigemodi für die Mondanzeige hinzugefügt: Mond, Sonne, Wetter und Sekunden (Anzeige der Sekunden für 30 Sekunden mit teilweiser Aktualisierung)
* Mondanzeige (Aufwärts-Taste): Mondphase, Auf- und Untergangszeit
* Sonnenanzeige (Aufwärts-Taste): Sonnenposition und Auf- und Untergangszeit
* Wetter (Aufwärts-Taste): Wettersymbol und Temperatur, neue nächtliche Wettersymbole
* Sekunden (Abwärts-Taste): Sekunden für 30 Sekunden mit teilweiser Aktualisierung der Anzeige
* Schwarz auf Weiß <=> Weiß auf Schwarz (Zurück-Taste)
* Menü (Auswahl-Taste)
* Zurück-Taste: Dunkelmodus / Hellmodus
* Automatische Sommerzeit-Einstellung über NTP
* NTP-Synchronisierung täglich um 3 Uhr morgens
* Wetteraktualisierung stündlich oder durch Auswahl des Wettermodus
* Mondauf- und -untergangszeiten
* Sonnenauf- und -untergangszeiten
* Korrigierte Batteriestatusanzeige (3,2 V bis 4,2 V)
* 24-Stunden-Anzeige ist fest konfiguriert
* Einheiten sind fest auf „metrisch” konfiguriert
* Erstellen Sie Ihren eigenen Open-Weather-Schlüssel/URL, indem Sie die URL definieren
* Konfiguration von Monats- und Tagesnamen, Standard ist Deutsch
* Drücken der Aufwärts-Taste während der Anzeige der Sekunden: syncNTP wird aufgerufen
* Drücken der Abwärts-Taste während der Anzeige der Sekunden: Rückkehr zum letzten Anzeigemodus
* Speicherzeitstempel: Drücken Sie die Zurück-Taste während der Sekundenanzeige, um den Speicherzeitstempel zu setzen => die Differenz in Stunden und Sekunden wird danach im Sekundenmodus angezeigt.
  Drücken Sie die Aufwärts-Taste während der Sekundenanzeige, um den Speicherzeitstempel zu löschen, drücken Sie die Menü-Taste während der Sekundenanzeige, um den Sekundenanzeigemodus ohne Änderungen am Speicherzeitstempel auszuschalten.
  (Praktische Option, um an die Einnahme von Medikamenten zu erinnern.)
* Sonnenanzeige basierend auf der nördlichen oder südlichen Hemisphäre (von links nach rechts oder von rechts nach links)
* Die Sonnenanzeige zeigt keine Daten an, wenn die Geokoordinaten einen Polarwinter bestimmen, mit Anzeige des Polarwinters. 
* Im Polarsommer wird die Dauer bis zum nächsten Mittag oder bis zur nächsten Mitternacht angezeigt.
* 24-Stunden-Anzeige und imperiale Einheiten sind jetzt in settings.h konfigurierbar.
* Saisonanzeige: Anzeige der lokalen Jahreszeit als vertikaler Text, Anzeige der astronomischen Jahreszeit als Pfeil auf der Skala (0° bis 360°)
* WLAN mit optionalem Energiesparmodus und schnellerer Verbindungszeit (statische IP, Reduzierung um 90% der normalen Verbdingunszeit, erbindung innerhalb weniger als einer Sekunde anstatt wie bisher 3 bis 7 Sekunden)


Änderungen in 1.1.0
* Die Hardware-Uhr läuft jetzt grundsätzlich in UTC. Der NTP-Sync schreibt reines UTC, der GMT-Offset und die Sommerzeitstunde werden erst bei der Anzeige addiert.
* Die Sommerzeit wird aus der UTC-Zeit und dem konfigurierten GMT-Offset bestimmt und folgt der EU-Regel (letzter Sonntag im März und Oktober, Umschaltung um 02:00 Normalzeit).
* Sonnen- und Mondauf- bzw. -untergang werden aus UTC mit einem gemeinsamen Offset berechnet und bleiben dadurch über den Zeitumstellungstermin hinweg korrekt.
* Beim manuellen Stellen der Uhr wird die eingegebene Ortszeit vor dem Speichern nach UTC zurückgerechnet.
* Die Sekundenanzeige hinterlässt beim Übergang auf 00 keine Bildreste mehr.
* Der About-Screen zeigt die Firmware-Version, die rohe UTC-Zeit aus der RTC und den aktiven Offset inklusive Sommerzeit-Kennzeichnung.

Hinweis nach dem Update auf 1.1.0: Ein Flash setzt die RTC nicht zurück, sie läuft auf ihrer Batterie weiter. Einmal Menü -> Sync NTP ausführen, sonst wird die alte Ortszeit
als UTC interpretiert und die Uhr geht um den Offset vor.

Alle Grundkonfigurationen können in settings.h vorgenommen werden.

Ich besitze zwei Watchys. Seltsamerweise verhalten sich die Anzeigen unterschiedlich; nur eine Watchy führt die Teilaktualisierung korrekt durch. Setzen Sie DISPLAYTYPE in settings.h
auf 1, wenn Sie ein gut funktionierendes Display haben, versuchen Sie es sonst mit 0.

Ein magnetisches Armband für die Watchy: https://makerworld.com/en/models/2048997-magnetic-watch-strap-20mm-and-22mm?from=search#profileId-2211213

Dieses Repository enthält eine Erweiterung der reparierten Version der Starfield Watchy-Firmware (https://github.com/Szybet/watchy-starfield).
Vielen Dank an https://github.com/Szybet

