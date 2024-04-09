////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Hauptdatei des Betriebssystems Hier läuft der Loop und der Setup ab

// Alle Benötigten Bibliotheken werdne hier zentral inkludiert, damit in allen anderen Dateien darauf zugegriffen werden kann.
#include <Wire.h> // LCD
#include <LiquidCrystal_PCF8574.h> //LCD
#include <WiFi.h> // WiFi Modul

// Alle Unterpgrogammseiten in der Richtigen Reihenfolge mitnehmen

void updateMenu(); // Forward-Deklaration, überall nach Schreiben Verfügbar
bool schreiben();
void resetMenu();

#include "pins.h" // STELLSCHRAUBE: Hier werden alle Pins festgelegt
#include "param.h" // STELLSCHRAUBE: Wichtige Systemkonstanten
#include "sysDef.h" // Beschreibt die Zustände des Systems

#include "instances.h" // Hier werdne die Pins auf die geräte Initialisiert | classes.h brauch ich nicht, wird darin inkludiert

#include "LCDdriver.h" // Hier befindet sich die Logik für das LCD
#include "service.h" // Piep-Funktionalität für den Piezzo Buzzer | benötigt LCD & RFID
#include "RFIDdriver.h" // Hier befindet sich die Logik für den Kartenleser

#include "myWifi.h" // Meine WLAN Funktionalität
#include "homebridge.h" // Serverfunktionalität | eingehnde Requests im Loop & update wenn was geschalten wurde

#include "lcdMenu.h" // Menüstruktur für das Display | inkludiert lcdMenuExec.h und lcdMenuPresenter.h

////////////////////////////////////////////////////////
// GENERAL SETUP FUNKTION
////////////////////////////////////////////////////////
void setup() {

  // Eröffnen des Seriellen Monitors
  Serial.begin(11520); // Zentral, da für alles wichtig
  Serial1.begin(9600);

  pinMode(38, OUTPUT);

  delay(1000);

  // Accessorys hochfahren / einrichten
  setupForLCD(); // LCD bereitstellen
  bootingUp(); // Hochfahranimation

  setupWifi(); // WiFi Verbindung herstellen
  setupUNO(); // Kommunikation mit Arduino Uno Kollegen einrichten

  dO_pudlePump.setIt(on);

  initUnlock(); // Darf nur final hochfahren, wenn es entsperrt wurde

  //Tageslichtsensor Initialisieren: Tag vermuten, wird im Loop bestätigt / widerlegt
  dayNightData.dayState = toDay;
  dayNightData.takeMeasurement = false;
  dayNightData.timeTodayState = millis();
}

////////////////////////////////////////////////////////
// GENERAL LOOP FUNKTION
////////////////////////////////////////////////////////
void loop() {

  dayNightCycle(); // Als erstes: Tag Nacht Loop festlegen, damit dieser für alles andere stets Verfügbar ist
  serverLoop(); // WiFi-Server Loop, wenn Anfragen eingehen. | Steuerung der Digitalzustände & Geräte im Empfangs-Fall
  buttonLoop(); // Button Loop, wenn knöpfe gedrückt wurden | Steuerung des Menüs / der Geräte im Druck-Fall
  standbyLoop(); // Testet wann und ob das Backlight aus sein soll (im service.h)
  tryToUnlock(); // Versuchen aufzusperren, wenn System gesperrt ist
  loopWiFi(); // Kümnmert sich bei Verbindungsabbruch darum, dass das System versucht, sich wieder neu zu verbinden

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
Allgemeines ToDo:
- eigene Einstellungen (es geht doch nicht ohne)
  - Klappen bei Tageslicht (wie bisher, falls online nicht verfügbar)
  - Weidezaun: PERMA, LOCK
  - Steckdosen: LOCK, egal
- Teichfilter gleich nach Launch aktivieren
- Alle Lampen ausschalten, wenn gesperrt wird
- Weide
*/