// Headerguard
#ifndef lcdMenuExec_H
#define lcdMenuExec_H

// Hier werden alle Logik-Einheiten die Ausgeführt werden sollen, wenn ein Knopf gedrückt wird ausgelagert
// Zuerst kommen einige Helper-Methoden für die updateMenu() Funktion, um diese etwas zu entzerren

// Darstellungs Datenbank includen
#include "lcdMenuPresenter.h"

/*
static void overscroll()
passt die menuStates an, falls es einen Overflow / Overscroll durch UP DOWN drücken gegeben hat
wird vor von updateMenu() vorm verarbeiten aufgerufen
*/
static void overscroll(){
  // MENU Overflow-Behandlung bei nach oben / nach unten
  if(menuState.menu > 5){
    menuState.menu = 0; // Overflow nach unten -> zurück auf erste Position (Hauptmenu)
  } else
  if(menuState.menu < 0){
    menuState.menu = 5; // Overlfow nach oben -> zurück auf letzte Position (Daten)
  }

  // UNTERMENU Overflow-Behandlung bei nach oben / nach unten
  // Berücksichtigt die unterschiedlichen Längen der Untermenus
  switch (menuState.menu){
    case m_light:
      // Licht menü hat 7 Seiten
      if(menuState.submenu == 0){ // nicht < 1, da -1 Indikatorzustand ist!
        menuState.submenu = 7; // Overflow nach oben -> zurück auf letzte Position (Dachboden)
      }
      if(menuState.submenu > 7){
        menuState.submenu = 1; // Overflow nach unten -> zurück auf erste Position (Alle Lichter)
      }
      break;
    case m_klappen:
      // Klappen menü hat 2 Seiten
      if(menuState.submenu == 0){ // nicht < 1, da -1 Indikatorzustand ist!
        menuState.submenu = 2; // Overflow nach oben -> zurück auf letzte Position (Klappe 2)
      }
      if(menuState.submenu > 2){
        menuState.submenu = 1; // Overflow nach unten -> zurück auf erste Position (Klappe 1)
      }
      break;
    case m_power:
      // Outlets menü hat 4 Seiten
      if(menuState.submenu == 0){ // nicht < 1, da -1 Indikatorzustand ist!
        menuState.submenu = 4; // Overflow nach oben -> zurück auf letzte Position (Teichsteckdosen)
      }
      if(menuState.submenu > 4){
        menuState.submenu = 1; // Overflow nach unten -> zurück auf erste Position (Alle Steckdosen)
      }
      break;
    case m_misc:
      // Misc menü hat 4 Seiten
      if(menuState.submenu == 0){ // nicht < 1, da -1 Indikatorzustand ist!
        menuState.submenu = 4; // Overflow nach oben -> zurück auf letzte Position (Hochdruckreiniger)
      }
      if(menuState.submenu > 4){
        menuState.submenu = 1; // Overflow nach unten -> zurück auf erste Position (Weidezaun)
      }
      break;
    case m_data: 
    // Das Datenmenu hat 3 Seiten
      if(menuState.submenu == 0){ // nicht < 1, da -1 Indikatorzustand ist!
        menuState.submenu = 3; // Overflow nach oben -> zurück auf letzte Position
      }
      if(menuState.submenu > 3){
        menuState.submenu = 1; // Overflow nach unten -> zurück auf erste Position 
      }
      break;
    // einen default braucht es nicht, da ich in dieser Mehtode zu Beginn sicherstelle, dass es nichts anderes geben kann
  }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*
void controlDevices()
aufgerufen nach Drücken von OK im Untermenu. Switch über beide Menükategorien & dann das jeweilige Gerät steuern
Eigentlich die Funktionalität zu den Menüseiten von updateMenu() -> daher auch gleiche Struktur
*/
void controlDevices(){ Serial.println("STEUERN");
  // switch über das große Menü
  switch(menuState.menu){
    case m_light:
      switch(menuState.submenu){
        // -1 als Indikatorzustand für "im Hauptmenu"
        case 1: // Alle Lichter
          switch(getAllLampStates()){
            case TBD:
            case OFF:
              // (manche) Lampen sind aus -> alle einschalten
              dO_L_Coop1.setIt(on);
              dO_L_Ceiling.setIt(on);
              dO_L_Coop2.setIt(on);
              dO_L_storage.setIt(on);
              dO_L_Entrance.setIt(on);
              dO_L_Floodlight.setIt(on);
              break;
            case ON:
              dO_L_Coop1.setIt(off);
              dO_L_Ceiling.setIt(off);
              dO_L_Coop2.setIt(off);
              dO_L_storage.setIt(off);
              dO_L_Entrance.setIt(off);
              dO_L_Floodlight.setIt(off);
              break;
          }
          break;
        case 2: // Stall 1
          dO_L_Coop1.switchIt();
          break;
        case 3: // Stall 2
          dO_L_Coop2.switchIt();
          break;
        case 4: // Flutlicht
          dO_L_Floodlight.switchIt();
          break;
        case 5: // Eingang
          dO_L_Entrance.switchIt();
          break;
        case 6: // Kammer
          dO_L_storage.switchIt();
          break;
        case 7: // Dachboden
          dO_L_Ceiling.switchIt();
          break;
        // kein Default, da garantiert einer dieser Werte von updateMenu()
      } // Ende Licht-Untermenü
      break;
    case m_klappen:
      switch(menuState.submenu){
        // -1 als Indikatorzustand für "im Hauptmenu"
        // es geht gleich bei 1 los, da "Alle klappen schalten" nicht existiert
        case 1: // Klappe von Stall 1
          if(klappe1.getIt() == err){
            // gerade im Error-Zustand -> kalibrieren!
            schreiben("Klappe Stall 1", "Status:      CAL"); // Muss hier sein, da updateMenu() erst nach Schaltung
            klappe1.calibrate();
            return; // Dann sofort zurück -> nicht nochmal schalten
          }
          // Wenn gerade eh offen oder zu, dann schalten
          schreiben("Klappe Stall 1", "Status:      ..."); // Muss hier sein, da updateMenu() erst nach Schaltung
          klappe1.switchIt();
          break;
        case 2: // Klappe von Stall 2
          if(klappe2.getIt() == err){
            // gerade im Error-Zustand -> kalibrieren!
            schreiben("Klappe Stall 2", "Status:      CAL"); // Muss hier sein, da updateMenu() erst nach Schaltung
            klappe2.calibrate();
            return; // Dann sofort zurück -> nicht nochmal schalten
          }
          // Wenn gerade eh offen oder zu, dann schalten
          schreiben("Klappe Stall 2", "Status:      ..."); // Muss hier sein, da updateMenu() erst nach Schaltung
          klappe2.switchIt();
          break;
        // kein Default, da garantiert einer dieser Werte von updateMenu()
      } // Ende Klappen-Untermenu
      break;
    case m_power:
      switch(menuState.submenu){
        // -1 als Indikatorzustand für "im Hauptmenu"
        case 1: // Alle Steckdosen Schalten
          switch (getAllOutletStates()) {
          case TBD:
          case OFF:
            // (manche) Outlets sind aus -> alle einschalten
            dO_OLT_outdoor.setIt(on);
            dO_OLT_indoor.setIt(on);
            dO_OLT_puddle.setIt(on);
            break;
          case ON:
            // Alle Outlets sind an -> alle ausschalten
            dO_OLT_outdoor.setIt(off);
            dO_OLT_indoor.setIt(off);
            dO_OLT_puddle.setIt(off);
          }
          break;
        case 2: // Außensteckdosen Schalten
          dO_OLT_outdoor.switchIt();
          break;
        case 3: // Innensteckdosen Schalten
          dO_OLT_indoor.switchIt();
          break;
        case 4: // Teichsteckdosen Schalten
          dO_OLT_puddle.switchIt();
          break;
        // kein Default, da garantiert einer dieser Werte von updateMenu()
      } // Ende Outlet-Untermenu
      break;
    case m_misc:
      switch(menuState.submenu){
        // -1 als Indikatorzustand für "im Hauptmenu"
        // es geht gleich bei 1 los, da "Alle Misc Geräte schalten" nicht existiert
        case 1: // Weidezaun Schalten
          dO_fence.switchIt();
          break;
        case 2: // Ventilator
          dO_fan.switchIt();
          break;
        case 3: // Teichfilter Schalten
          dO_pudlePump.switchIt();
          break;
        case 4: // Hochdruckreiniger Schalten
          dO_kaercher.switchIt();
          break;
        // kein Default, da garantiert einer dieser Werte von updateMenu()
      } // Ende Misc-Untermenu
      break;
    case m_data:
      switch(menuState.submenu){
        case 1: // Neuverbindung WiFi
          if(status != WL_CONNECTED){
            // Nur, wenn gerade nicht verbunden
            setupWifi();
          }
          break;
        case 2: // Neuverbindung RFID Arduino 
          if(!readerOnline){
            setupUNO();
          }
          break;
      }
    default:
      // Nichts zu tun - hier kann man nicht klicken | Gleichzeitig für default-notbremse
      // TODO - evtl. WLAN Verbindung neu machen, manuellen Kalibrieren der Klappen, etc.
      return;
  }
}

#endif