// Headerguard
#ifndef lcdMenuPresenter_H
#define lcdMenuPresenter_H

/*
void updateMenu()
wird nach jedem Tastendruck aufgerufen um den neuen Zustand auf das Display zu drucken.
Die eigentliche GUI des Menüs: LOOKUP Table für alle Seiten
Hardware-Wandler für digitalen Menu-Zustand
*/
void updateMenu(){
  antiStandby(); // Gerät aufwecken, wenn noch nicht geschehen, da ein Knopf gedrückt wurde / sich am Gerät definitiv was geändert hat

  if(sysState.sysLock == locked){
    // Wen System gesperrt ist, einfach zurückgeben, was man kennt
    schreiben(" CoopOS Connect ", "    Gesperrt    ");
    return;
  }
  
  // switch über das große Menü
  switch(menuState.menu){
    case m_home:
      schreiben(" CoopOS Connect ", "      Menu      ");
      break;
    case m_light:
      switch(menuState.submenu){
        // -1 als Indikatorzustand für "im Hauptmenu"
        case -1: 
          schreiben("> Beleuchtung", "  Klappen");
          break;
        case 1: // Alle Lichter
          switch(getAllLampStates()){
            case TBD:
              // Man weiß es nicht, wie genau die Situation ist
              schreiben("Alle Lichter", "Status:      ???");
              break;
            case ON:
              // Alles an
              schreiben("Alle Lichter", "Status:       ON");
              break;
            case OFF:
              schreiben("Alle Lichter", "Status:      OFF");
              break;
          }
          break;
        case 2: // Stall 1
          if(dO_L_Coop1.getIt()){
            // Licht ist an
            schreiben("Licht Stall 1", "Status:       ON");
            break;
          }
          // Licht ist aus
          schreiben("Licht Stall 1", "Status:      OFF");
          break;
        case 3: // Stall 2
          if(dO_L_Coop2.getIt()){
            // Licht ist an
            schreiben("Licht Stall 2", "Status:       ON");
            break;
          }
          // Licht ist aus
          schreiben("Licht Stall 2", "Status:      OFF");
          break;
        case 4: // Flutlicht
          if(dO_L_Floodlight.getIt()){
            // Licht ist an
            schreiben("Strahler", "Status:       ON");
            break;
          }
          // Licht ist aus
          schreiben("Strahler", "Status:      OFF");
          break;
        case 5: // Eingang
          if(dO_L_Entrance.getIt()){
            // Licht ist an
            schreiben("Licht Eingang", "Status:       ON");
            break;
          }
          // Licht ist aus
          schreiben("Licht Eingang", "Status:      OFF");
          break;
        case 6: // Kammer
          if(dO_L_storage.getIt()){
            // Licht ist an
            schreiben("Licht Kammer", "Status:       ON");
            break;
          }
          // Licht ist aus
          schreiben("Licht Kammer", "Status:      OFF");
          break;
        case 7: // Dachboden
          if(dO_L_Entrance.getIt()){
            // Licht ist an
            schreiben("Licht Dachboden", "Status:       ON");
            break;
          }
          // Licht ist aus
          schreiben("Licht Dachboden", "Status:      OFF");
          break;
        // kein Default, da Overflow-Probleme gerade behandelt wurden
      } // Ende Licht-Untermenü
      break;
    case m_klappen:
      switch(menuState.submenu){
        // -1 als Indikatorzustand für "im Hauptmenu"
        case -1:
          schreiben("  Beleuchtung", "> Klappen");
          break;
        // es geht gleich bei 1 los, da "Alle klappen schalten" nicht existiert
        case 1: // Klappe von Stall 1
          switch(klappe1.getIt()){
            case err:
              schreiben("Klappe Stall 1", "Status:      ERR");
              break;
            case opened:
              schreiben("Klappe Stall 1", "Status:     OPEN");
              break;
            case closed:
              schreiben("Klappe Stall 1", "Status:   CLOSED");
              break;
          }
          break;
        case 2: // Klappe von Stall 2
          switch(klappe2.getIt()){
            case err:
              schreiben("Klappe Stall 2", "Status:      ERR");
              break;
            case opened:
              schreiben("Klappe Stall 2", "Status:     OPEN");
              break;
            case closed:
              schreiben("Klappe Stall 2", "Status:   CLOSED");
              break;
          }
          break;
        // kein Default, da Overflow-Probleme gerade behandelt wurden
      } // Ende Klappen-Untermenu
      break;
    case m_power:
      switch(menuState.submenu){
        // -1 als Indikatorzustand für "im Hauptmenu"
        case -1:
          schreiben("> Steckdosen", "  Geraete");
          break;
        case 1: // Alle Steckdosen
          switch(getAllOutletStates()){
            case TBD:
              schreiben("Alle Steckdosen", "Status:      ???");
              break;
            case ON:
              schreiben("Alle Steckdosen", "Status:       ON");
              break;
            case OFF:
              schreiben("Alle Steckdosen", "Status:      OFF");
              break;
          }
          break;
        case 2: // Außensteckdosen 
          switch(dO_OLT_outdoor.getIt()){
            case on:
              schreiben("Aussensteckdose", "Status:       ON");
              break;
            case off:
              schreiben("Aussensteckdose", "Status:      OFF");
              break;
          }
          break;
        case 3: // Innensteckdosen 
          switch(dO_OLT_indoor.getIt()){
            case on:
              schreiben("Innensteckdose", "Status:       ON");
              break;
            case off:
              schreiben("Innensteckdose", "Status:      OFF");
              break;
          }
          break;
        case 4: // Teichsteckdosen 
          switch(dO_OLT_puddle.getIt()){
            case on:
              schreiben("Teichsteckdose", "Status:       ON");
              break;
            case off:
              schreiben("Teichsteckdose", "Status:      OFF");
              break;
          }
          break;
        // kein Default, da Overflow-Probleme gerade behandelt wurden
      } // Ende Outlet-Untermenu
      break;
    case m_misc:
      switch(menuState.submenu){
        // -1 als Indikatorzustand für "im Hauptmenu"
        case -1:
          schreiben("  Steckdosen", "> Geraete");
          break;
        // es geht gleich bei 1 los, da "Alle Misc Geräte schalten" nicht existiert
        case 1: // Weidezaun 
          switch(dO_fence.getIt()){
            case on:
              schreiben("Weidezaun", "Status:   SCHARF");
              break;
            case off:
              schreiben("Weidezaun", "Status:      OFF");
              break;
          }
          break;
        case 2: // Ventilator
          switch(dO_fan.getIt()){
            case on:
              schreiben("Luefter", "Status:       ON");
              break;
            case off:
              schreiben("Luefter", "Status:      OFF");
              break;
          }
          break;
        case 3: // Teichfilter 
          switch(dO_pudlePump.getIt()){
            case on:
              schreiben("Teichfilter", "Status:       ON");
              break;
            case off:
              schreiben("Teichfilter", "Status:      OFF");
              break;
          }
          break;
        case 4: // Hochdruckreiniger 
          switch(dO_kaercher.getIt()){
            case on:
              schreiben("HochD-Reiniger", "Status:   BEREIT");
              break;
            case off:
              schreiben("HochD-Reiniger", "Status:      OFF");
              break;
          }
          break;
        // kein Default, da Overflow-Probleme gerade behandelt wurden
      } // Ende Misc-Untermenu
      break;
    case m_data:
      switch(menuState.submenu){
        // -1 als Indikatorzustand für "im Hauptmenu"
        case -1:
          schreiben("> Daten", "     <---->     ");
          break;
        // es geht gleich bei 1 los, da 0 der Overflow Zustand ist
        case 1: // WiFi Status
          switch(status){
            case WL_CONNECTED:
              // Verbindung ist oke
              schreiben("WiFi Verbindung", "Status:       OK");
              break;
            default:
              // Irgendwas is los 
              schreiben("Wifi Verbindung", "Status:      ERR");
          }
          break;
        case 2: // RFID connection
          if(readerOnline){
            schreiben("RFID Controller", "Status:   ONLINE");
          } else {
            schreiben("RFID Controller", "Status:  OFFLINE");
          }
          break;
        case 3: // Tages Zustand (Licht)
          switch(dayNightData.dayState){
            case day:
              schreiben("Lichtsensor", "Status:      Tag");
              break;
            case night:
              schreiben("Lichtsensor", "Status:    Nacht");
              break;
            case toNight:
              schreiben("Lichtsensor", "Status:   Nacht?");
              break;
            case toDay:
              schreiben("Lichtsensor", "Status:     Tag?");
              break;
          }
          break;
      } // Ende Untermenuswitch
      break;
    default:
      return;
  } // Ende Menuswitch
}

/*
resetMenu()
setzt das Menü auf die Standardwerte zurück
*/
void resetMenu(){
  menuState.menu = m_home; // nach Hause
  menuState.submenu = -1; // ungenutzt, daher zurück in -1 Indikator zustand | 0 wäre Overscroll
  // updateMenu(); // Zustand in Hardware ausgeben | will ich hier aber eigentlich nicht
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
// Weitere Anzeige-Exekutions Methoden

/*
void systemAlreadyLocked() - Statische Methode nur für diesen Header
Gibt eien Meldung auf dem lcdAus, wenn nach sperren nochmal gesperrt werden soll
*/
void systemAlreadyLocked(){
  // Ich habe hier auf den Zustandswechsel von Standby verzichtet - es bleibt ja in der Funktion
  schreiben("System bereits", "gespertt!");
  piepen();
  delay(500);
  updateMenu();
}

////////////////////////////////////////////////////////////////////////////////
// hat auch irgendwie was mit dem Menu zu tun

/*
bootingUp()
Kümmert sich um die Start-Prozedur des Systems (display)
*/
void bootingUp(){
  schreiben("ChickenOSConnect", "by. F. Wimbauer");
  piepen(); // Hehe :)
  delay(1500);
  resetMenu(); // Menü auf standard stellen & anzeigen (Funktion ruft am Ende updateMenu() auf!)
  updateMenu();
}

#endif