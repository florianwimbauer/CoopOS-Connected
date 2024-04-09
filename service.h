// Erstellt die Logik für den Piezzo Buzzer zum Piepen des Systems so wie eineige weitere Service-Methoden

#ifndef service_H
#define service_H

/*
piepen(int duration)
Piept so lange, wie millis gegeben, wenn nichts gegeben, eine Sekunde
*/
void piepen(int dur = standardPiep){
  dO_piezzo.setIt(1); // Buzzer einschalten
  delay(dur); // gegebene Zeit warten
  dO_piezzo.setIt(0); // Buzzer ausschalten
}

/*
void dayNightCycle()
Loop Export, die Prüft, ob es gerade Nacht / Tag geworden ist. Passendes Enum aus sysDef.h
Es passiert jeweils nichts, wenn
*/
void dayNightCycle(){

  // Timer Teil, der die Zweite Messung freigibt
  if(!dayNightData.takeMeasurement && millis() - dayNightData.timeTodayState > timeBetweenDayNightTest){
    // Wenn gerade keine Messung gemacht werden darf, der Timer Aber schon abgelaufen ist
    // Messung im Anschluss wieder erlauben
    dayNightData.takeMeasurement = true;
  }

  // Print & Logikteil - Tritt nur ein, wenn gerade eine Messung gemacht werden darf
  if(s_light.readData() == off && dayNightData.takeMeasurement){
    // Sensor gibt negativen Wert zurück // STELLSCHRAUBE - wann ist Tag / wann ist Nacht?
    // Wir gehen davon aus OFF == FINSTER
    switch(dayNightData.dayState){
      // night case wird ausgelassen, da damit nichts gemacht werden soll
      case toDay:
        // Es war Tag vermutet, jetzt ist aber immer noch Nacht -> nichts machen, da Fehlalarm 
        schreiben("Fehlalarm", "Status:    NACHT");
        dayNightData.dayState = night;
        dayNightData.timeTodayState = millis(); // Zeitstempel für die nächste Messung setzen
        dayNightData.takeMeasurement = false; // jetzt sind erst mal keine Messungen mehr erlaubt
        delay(1000);
        updateMenu(); // Menu wieder herstellen
        break;
      case toNight:
        // Es war Nacht vermutet & ist immer noch Nacht -> Nacht festlegen
        schreiben("Nacht fix!", "Status:    NACHT");
        dayNightData.dayState = night;
        dayNightData.timeTodayState = millis(); // Zeitstempel für die nächste Messung setzen
        dayNightData.takeMeasurement = false; // jetzt sind erst mal keine Messungen mehr erlaubt
        delay(1000);
        updateMenu(); // Menu wieder herstellen
        break;
      case day:
        // Es war Tag und wir haben Nacht gespottet -> toNight einleiten
        schreiben("Nacht vermutet!", "Status:   NACHT?");
        dayNightData.dayState = toNight;
        dayNightData.timeTodayState = millis(); // Zeitstempel für die nächste Messung setzen
        dayNightData.takeMeasurement = false; // jetzt sind erst mal keine Messungen mehr erlaubt
        delay(1000);
        updateMenu(); // Menu wieder herstellen
        break;
      case night:
        // nischts, da Standard
        return;
    }
  } else 
  if(s_light.readData() == on && dayNightData.takeMeasurement){
    // Sensor gibt positiven Wert zurück // STELLSCHRAUBE - wann ist Tag / wann ist Nacht
    // Wir gehen davon aus ON = HELL
    switch(dayNightData.dayState){
      case toDay:
        // Es war Tag vermutet & es ist Tag -> Tag Festlegen
        schreiben("Tag fix!", "Status:      TAG");
        dayNightData.dayState = day;
        dayNightData.timeTodayState = millis(); // Zeitstempel für die nächste Messung setzen
        dayNightData.takeMeasurement = false; // jetzt sind erst mal keine Messungen mehr erlaubt
        delay(1000);
        updateMenu();
        break;
      case toNight:
        // Es wurde Nacht gemessen, jetzt ist es aber wieder Hell -> Fehlalarm
        schreiben("Fehlalarm", "Status:      TAG");
        dayNightData.dayState = day;
        dayNightData.timeTodayState = millis(); // Zeitstempel für die nächste Messung setzen
        dayNightData.takeMeasurement = false; // jetzt sind erst mal keine Messungen mehr erlaubt
        delay(1000);
        updateMenu();
        break;
      case night:
        // Es war Nacht & wir haben Tag gemessen -> toDay einleiten
        schreiben("Tag vermutet!", "Status:     TAG?");
        dayNightData.dayState = toDay;
        dayNightData.timeTodayState = millis(); // Zeitstempel für die nächste Messung setzen
        dayNightData.takeMeasurement = false; // jetzt sind erst mal keine Messungen mehr erlaubt
        delay(1000);
        updateMenu(); // Menu wieder herstellen
        break;
      case day:
        // nischts, da Standard
        return;
    }
  }
}

// Loop-Export, je nachdem wie der Systemzustand ist, wird das Backlight ausgeschalten / eingeschalten
void standbyLoop(){
  if(millis() - timeStamp > timeToStandby){
    // Wenn mehr Zeit als die Grenze seit letzter Bedienung vergangen ist -> goStandby
    sysState.sysStandOn = standby; // Digitalzustand setzen
    display.setBacklight(0); // Hardware Zustand Setzen (Backlight aus)
  }
}

/*
void antiStandby()
resettet den Standby-Countdown, wenn system noch aktiv
weckt System aus dem Standby-Countdown, wenn im standby
sollte eigentlich nur von updateMenu() aufgerufen werden
*/
void antiStandby(){
  // aktualisieren des Zeitstempels
  timeStamp = millis();
  // System ggf. aufwecken
  if(sysState.sysStandOn == standby){
    // System war im Standby -> aufwecken
    sysState.sysStandOn = active; // Digitalzustand setzen -> aufwecken
    display.setBacklight(255); // Hardware Zustand Setzen (Backlight an)
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

// Hier stehen die ganzen switch-cases, was faktisch auf dem Display angezeigt werden kann

/*
Für die Geräte, wo die ganze Kategorie geschalten werden kann, wird hier die Logik geprüft
*/
enum printState{
  ON, // Alle Geräte sind an
  OFF, // Alle Geräte sind aus
  TBD // Manche Geräte sind an & manche geräte sind aus
};

/* 
printState getAllLampStates()
gibt den Zustand aller Lampen zurück nach dem obigen Enum. 
Wird nur hier von updateMenu() und von der controllDevices() Funktion aufgerufen
*/
printState getAllLampStates(){
  int i = 
    dO_L_Coop1.getIt() +
    dO_L_Ceiling.getIt() +
    dO_L_Coop2.getIt() +
    dO_L_storage.getIt() +
    dO_L_Entrance.getIt() +
    dO_L_Floodlight.getIt();

  if(i == 6){
    // Alle Geräte sind an
    return ON;
  } else 
  if(i == 0){
    // Alle Geräte sind aus
    return OFF;
  } else {
    // Manche Geräte sind aus, Manche Geräte sind an
    return TBD;
  }
}

/*
printState getAllOutletStates()
analog zu getAllLampStates()
*/

printState getAllOutletStates(){
  int i = 
    dO_OLT_outdoor.getIt() +
    dO_OLT_indoor.getIt() +
    dO_OLT_puddle.getIt();

  if(i == 3){
    // Alle Geräte sind an
    return ON;
  } else 
  if(i == 0){
    // Alle Geräte sind aus
    return OFF;
  } else {
    // Manche Geräte sind aus, Manche Geräte sind an
    return TBD;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Sperr Funktionalität
////////////////////////////////////////////////////////////////////////////////////////////////

/*
void lockdown()
sperrt das System. Wird aufgerufen vom roten LOCK knopf bzw. von einer HTTP-Anfrage
*/
void unlockSystem(){
  sysState.sysLock = unlocked;

  schreiben("System wird", "entsperrt!");
  piepen();

  dO_locks.setIt(on); // Schaltkreis scharf schalten, damit Türen funktionieren | invertierte Logik
  dO_fence.setIt(off); // Weidezaun ausschalten -> hardgecodete STELLSCHRAUBE

  schreiben("Weidezaun:   AUS","Türen:      FREI");
  delay(1000);

  resetMenu(); // menü auf Basiswerte setzen
  updateMenu(); // In Menü übergehen

}

/*
void unlock()
entsperrt das System. Wird aufgerufen vom RFID-Loop bzw. von einer HTTP-Anfrage
*/
void lockSystem(){
  sysState.sysLock = locked;

  schreiben("System wird", "gesperrt!");
  piepen();

  dO_locks.setIt(off);
  dO_fence.setIt(on);

  schreiben("Weidezaun:   EIN", "Türen:      LOCK");
  delay(1500);

  resetMenu();
  updateMenu(); // Zeigt dann den lock Bildschirm an
}

#endif