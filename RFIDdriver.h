// Hier werden die Custom-Methoden zusammgebaut, die für das RFID-Pad zuständig sind
// Header-Guards, damit mehrfaches implementieren zulässig ist
#ifndef rfid_H
#define rfid_H

String message;

// Auslagerung des general Setup Teils für den RFID Sensor
void setupUNO(){
  schreiben("Initialisiere", "Kartenleser ...");

  Serial1.println("Handshake"); // Handshake zum Uno schicken
  unsigned long int serialStamp = millis(); // Zeitstempel nehmen

  while(millis() - serialStamp < serialTimeout){
    if(Serial1.available() > 0){
      message = Serial1.readString();
    }
  } // Warten bis Verbindung kommt oder Timeout

  if(message.indexOf("back") != -1) { 
    // Passende Antwort wurde gegeben - alles oke, Rückgabe
    schreiben("RFID Sensor", "Verbunden!");
    readerOnline = true;
    
  } else {
    // Es wurde keine Antwort / falsche Antwort erhalten
    schreiben("RFID Sensor", "Antwortet nicht!");
    readerOnline = false;
  }
  delay(1500);
}

/* 
void tryToUnlock(){}
Veruscht das System zu entsperren, wenn es gesperrt ist
Loop Export
*/
void tryToUnlock(){
  if(sysState.sysLock != locked){
    return;
  }

  if(Serial1.available() > 0){
    // Uno hat neue Karte entdeckt
    String info = Serial1.readString();
    Serial.println(info);

    if(info.indexOf("OK") != -1){
      // UUID der Karte konnte gematcht werden -> entsperren des Systems
      unlockSystem();
    } else {
      schreiben("Karte unbekannt!", "Kein Zugang!");
      delay(1000);
      piepen();
    }
  }
}

/*
void initUnlock() - Setup Export
hält die Ausführung so lange auf, bis ein normales unlock passiert.
Wird vom setup() Hauptteil als letztes Aufgerufen
*/

void initUnlock(){
  sysState.sysLock = locked; // System wird gesperrt
  schreiben("Nach neustart", "System freigeben");

  while(sysState.sysLock == locked){
    // Solange wie das System weiterhin gesperrt ist, warten wir auf die Freigabe
    tryToUnlock();
  }
}

#endif