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
* corrected battery status display (3,4V to 4.2V)
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
* sun display shows no data while polar winter or polar summer if geocoordinates indicates a polar position.

All basic configurations can be done in settings.h.

I own two Watchys. Strangely enough, the displays behave differently; only one Watchy performs the partial update correctly. Set DISPLAYTYPE in settings.h
to 1 if you have a well running display, try to set it to 0, if not.

This repository contains a extendtion of the repaired version of the Starfield Watchy firmware (https://github.com/Szybet/watchy-starfield).
Thanks you to https://github.com/Szybet


