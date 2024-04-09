// Headerguard
#ifndef sysDef_H
#define sysDef_H

// Alle Zustände des Systems

enum SysLock {
  locked,
  unlocked
};

enum SysStandOn{
  active,
  standby
};

/* 
Struktur zur Vollständigen Beschreibung des Systems
- gesperrt oder nicht
- aktiv oder im standby
*/
struct SysState{
  SysLock sysLock;
  SysStandOn sysStandOn;
};

// Initialisierung des Structs ein mal für das ganze System
SysState sysState;

////////////////////////////////////////////////////////////////////////////////

// Zeitstempel für Standby-Logik
unsigned long timeStamp;

bool readerOnline = false;

////////////////////////////////////////////////////////////////////////////////

// Enum für lokalen Tag / Nacht Rythmus
enum eDayNight{
  day, // Gesicher Tag
  night, // Gesichter Nacht
  toNight, // Nacht vermutet -> Testen
  toDay // Tag vermutet -> Testen
};

// Struct, welches das ganze Tag / Nacht Zeug in eine Datenstruktur zusammen packt
struct DayNightData{
  eDayNight dayState;
  unsigned long timeTodayState; // Variable um millis() nach Tag / Nacht Vermutung zu lagern
  bool takeMeasurement; // Tester, ob eine Messung gerade erlaubt ist
};

// Initialisierung des Structs
DayNightData dayNightData;

////////////////////////////////////////////////////////////////////////////////

/* 
Beschreibt die aktuelle Situation in der sich das Menü befindet
erste Stufe: Hauptmenü-Switch
zweite Stude: Untermneü Switch

wird verwendet lcdMenu.h
*/
struct MenuState {
  int menu;
  int submenu;
};

MenuState menuState;

// Makros für die erste Stufe definieren, damit es etwas übersichtlicher wird
// kann kein Enum sein, da man sonst keine aritmetischen Funktionen mehr ausführen kann beim navigieren
#define m_home 0
#define m_light 1
#define m_klappen 2
#define m_power 3
#define m_misc 4 // Teich, Zaun & Lüfter
#define m_data 5

#define sm_empty -1 // Indikator, das man gerade nicht im Untermenu ist
// STELLSCHRAUBE: Könnte evtl. zu Bugs führen. Im Zweifel eine große (negative) zahl verwenden

#endif