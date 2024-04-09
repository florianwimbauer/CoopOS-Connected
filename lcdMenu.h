// Headerguard
#ifndef lcdMenu_H
#define lcdMenu_H

// Hier läuft das Menü des LCD Bildschirms des Arduinos - basierend auf Legacycode
// Auch der Loop-Export für die Knöpfe läuft hier

// Auführungen nach Logikaufdröselung
#include "lcdMenuExec.h"

// Loop Export für die Bedienlogik
void buttonLoop(){
  // Testen ob einer der Knöpfe gedrückt wurde
  if(!db_buttonUP.testPressed()){
    // UP wurde gedrückt
    /////////////////////////////////////////////////////////////////
    // Wenn gesperrt, abbruch mit gesperrt-Meldung
    if(sysState.sysLock == locked){
      systemAlreadyLocked();
      return;
    }
    // Eigentliche Logik
    if(menuState.submenu == sm_empty){
      // Wenn in keinem Untermenu -> Steuerung des Hauptmenus
      menuState.menu--;
    } else {
      // Wir befinden uns gerade im Untermenu -> Steuerung des Untermenus
      menuState.submenu--;
    }
    // Anzeige aktualisieren & evtl. Menu- / Submenu-Overflow korrigieren
    overscroll();
    updateMenu();
    delay(buttonLoopback); // rückkopplungs Delay
    return;
    /////////////////////////////////////////////////////////////////
  } else
  if(!db_buttonDOWN.testPressed()){
    // DOWN wurde gedrückt | Gleiche Logik wie UP nur mit ++ anstelle von --
    /////////////////////////////////////////////////////////////////
    // Wenn gesperrt, abbruch mit gesperrt-Meldung
    if(sysState.sysLock == locked){
      systemAlreadyLocked();
      return;
    }
    // Eigentliche Logik
    if(menuState.submenu == sm_empty){ 
      // Wenn in keinem Untermenu -> Steuerung des Hauptmenus
      menuState.menu++;
    } else {
      // Wir befinden uns gerade im Untermenu -> Steuerung des Untermenus
      menuState.submenu++;
    }
    // Anzeige aktualisieren & evtl. Menu- / Submenu-Overflow korrigieren
    overscroll();
    updateMenu();
    delay(buttonLoopback); // rückkopplungs Delay
    return; 
    /////////////////////////////////////////////////////////////////
  } else 
  if(!db_buttonBACK.testPressed()){
    // BACK wurde gedrückt
    /////////////////////////////////////////////////////////////////
    // Wenn gesperrt, abbruch mit gesperrt-Meldung
    if(sysState.sysLock == locked){
      systemAlreadyLocked();
      return;
    }
    // Eigentliche Logik
    if(menuState.submenu == sm_empty){
      // Wenn bereits in keinem Untermenu, rücksprung auf den Homebildschirm
      menuState.menu = m_home; 
    } else {
      // Rücksprung aus dem Submenu | Menu bleibt ja unangetastet
      menuState.submenu = sm_empty;
    }
    // Anzeige aktualisieren & evtl. Menu- / Submenu-Overflow korrigieren
    updateMenu();
    delay(buttonLoopback); // rückkopplungs Delay
    return;
    /////////////////////////////////////////////////////////////////
  } else 
  if(!db_buttonLOCK.testPressed()){ 
    ////////////////////////////////////////////////////////////////
    /*
    LOCK wurde gedrückt
    - wenn System bereits gesperrt -> Anzeige / nichts
    - sonst: menu zurücksetzen & gerät sperren
    */
    if(sysState.sysLock == locked){
      systemAlreadyLocked(); // kümmert sich angemessen um Standby
      updateMenu(); // Menüanzeige wieder herstellen
      return;
    }
    // System sperren, wenn noch nicht passiert
    lockSystem();
    updateMenu(); // Menüdarstellung wieder Herstellen
    delay(buttonLoopback); // rückkopplungs Delay 
    return; 
    /////////////////////////////////////////////////////////////////
  } else 
  if(!db_buttonOK.testPressed()){
    // OK wurde gedrückt | Kompliziertester Knopf
    /////////////////////////////////////////////////////////////////
    // Wenn gesperrt, abbruch mit gesperrt-Meldung
    if(sysState.sysLock == locked){
      systemAlreadyLocked();
      delay(buttonLoopback);
      return;
    } 
    // Eigentliche Logik 
    if(menuState.submenu == sm_empty){
      // Aktuell in keinem Untermenu
      menuState.submenu = 1; // Springen in das erste Untermenü mit dem aktuellen menu-Wert
      updateMenu();
    } else {
      // Aktuell bereits in einem Untermenu
      // HIER WERDEN ALLE AKTIONEN AUSGEFÜHRT
      controlDevices(); // das ausgewählte gerät Schalten
      updateMenu(); // den neuen Zustand auch auf dem Bildschirm ausgeben
    }
    delay(buttonLoopback); // rückkopplungs Delay
    return;
    /////////////////////////////////////////////////////////////////
  }
  // Wurde nichts gedrückt, dann geben wir wieder zurück an den Main-Loop
}
///////////////////////////////////////////////////////////////////////////////////////////
#endif