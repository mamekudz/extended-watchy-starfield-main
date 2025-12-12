English

# Extension of the Repaired Starfield Watchy Firmware incl. Seconds Display via Partial Update

An extended Sstarfield watchface for Watchy


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
* sun display based on north or south hemisphere (left to right or right to left)
* sun display shows no data while polar winter or polar sun position in polar summer if geocoordinates indicates a polar position.
* 24 Hour Display and imperial units are now configurable in settings.h.
* Season indicator: Display of the local season as vertical text, display of the astronomical season as an arrow on the scale (0° to 360°)

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
* Die Sonnenanzeige zeigt keine Daten an, wenn die Geokoordinaten eine polare Position anzeigen, d. h. während des Polarwinters oder bei polarer Sonnenposition im Polarsommer.
* 24-Stunden-Anzeige und imperiale Einheiten sind jetzt in settings.h konfigurierbar.
* Saisonanzeige: Anzeige der lokalen Jahreszeit als vertikaler Text, Anzeige der astronomischen Jahreszeit als Pfeil auf der Skala (0° bis 360°)

Alle Grundkonfigurationen können in settings.h vorgenommen werden.

Ich besitze zwei Watchys. Seltsamerweise verhalten sich die Anzeigen unterschiedlich; nur eine Watchy führt die Teilaktualisierung korrekt durch. Setzen Sie DISPLAYTYPE in settings.h
auf 1, wenn Sie ein gut funktionierendes Display haben, versuchen Sie es sonst mit 0.

Ein magnetisches Armband für die Watchy: https://makerworld.com/en/models/2048997-magnetic-watch-strap-20mm-and-22mm?from=search#profileId-2211213

Dieses Repository enthält eine Erweiterung der reparierten Version der Starfield Watchy-Firmware (https://github.com/Szybet/watchy-starfield).
Vielen Dank an https://github.com/Szybet

