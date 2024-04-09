// Headerguard
#ifndef homebridge_H
#define homebridge_H
// Hier werden alle Anfragen, die der Arduino über HTTP bekommt bearbeitet

/*
void anwerPoll()
sendet bei einem Polling-Request den entsprechenden Zustand des Systems / Geräts als HTTP Response zurück
wird aufgerufen von serverLoop()
Wir wissen bereits, dass es sich um einen Poll handelt. Entsprechenden Zustand zurückschreiben
*/

void answerPoll(String request, WiFiClient client){

  // HTTP Response Header senden
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/plain");
  client.println("Connection: close");
  client.println();

  // Entsprechenden Status mitschicken

  if (request.indexOf("/l_coop1") != -1) { // Licht Stall 1
    client.println(dO_L_Coop1.getIt());
  } else
  if (request.indexOf("/l_coop2") != -1) { // Licht Stall 2
    client.println(dO_L_Coop2.getIt());
  } else 
  if (request.indexOf("/l_storage") != -1) { // Licht Kammer
    client.println(dO_L_storage.getIt());
  } else 
  if (request.indexOf("/l_entrance") != -1) { // Licht Eingang
    client.println(dO_L_Entrance.getIt());
  } else 
  if (request.indexOf("/l_ceiling") != -1) { // Licht Dachboden
    client.println(dO_L_Ceiling.getIt());
  } else 
  if (request.indexOf("/l_flood") != -1) { // Licht Flutlicht
    client.println(dO_L_Floodlight.getIt());
  } else 
  if (request.indexOf("/olt_outdoor") != -1) { // Steckdosen Outdoor
    client.println(dO_OLT_outdoor.getIt());
  } else 
  if (request.indexOf("/olt_indoor") != -1) { // Steckdosen Indoor
    client.println(dO_OLT_indoor.getIt());
  } else 
  if (request.indexOf("/olt_puddle") != -1) { // Steckdosen Teich
    client.println(dO_OLT_puddle.getIt());
  } else 
  if (request.indexOf("/fence") != -1) { // Weidezaun
    client.println(dO_fence.getIt());
  } else 
  if (request.indexOf("/fan") != -1) { // Lüfter
    client.println(dO_fan.getIt());
  } else 
  if (request.indexOf("/pump") != -1) { // Teichfilterpumpe
    client.println(dO_pudlePump.getIt());
  } else 
  if (request.indexOf("/kaercher") != -1) { // Kärcher
    client.println(dO_kaercher.getIt());
  } else
  if (request.indexOf("/sysLock") != -1) { // Lock Zustand (nicht nur Geräte, ganzer Zustand)
    if(sysState.sysLock == locked){
      client.println(1); // Gesperrt ist einfach 1, da das Schloss dan ein ist?
    }
    if(sysState.sysLock == unlocked){
      client.println(0); // Gesperrt ist einfach 0, da das Schloss dann aus ist?
    }
    // Es kann keinen anderen Fall geben
  } else 
  if (request.indexOf("/klappe1") != -1) { // Lock Zustand (nicht nur Geräte, ganzer Zustand)
    if(klappe1.getIt() == opened){
      client.println("0"); // Geöffnet ist einfach 0, laut Dokumentation des Plugins
    }
    if(klappe1.getIt() == closed){
      client.println("1"); // Geschlossen ist einfach 1, laut Dokumentation des Plugins
    }
    if(klappe1.getIt() == err){
      client.println("4"); // Err / Unbekannt ist einfach 4, laut Dokumentation des Plugins
    }
    // Es kann keinen anderen Fall geben
  } else
  if (request.indexOf("/klappe2") != -1) { // Lock Zustand (nicht nur Geräte, ganzer Zustand)
    if(klappe2.getIt() == opened){
      client.println("0"); // Geöffnet ist einfach 0, laut Dokumentation des Plugins
    }
    if(klappe2.getIt() == closed){
      client.println("1"); // Geschlossen ist einfach 1, laut Dokumentation des Plugins
    }
    if(klappe2.getIt() == err){
      client.println("4"); // Err / Unbekannt ist einfach 4, laut Dokumentation des Plugins
    }
    // Es kann keinen anderen Fall geben
  }
  
  else {
    // Grundsätzlich Unbekannte Anfrage
    Serial.println("Unbekannter Poll!");
    client.println("err");
  }

  client.println("");
  // client.stop ist nicht notwendig, da das Aufrufende Funktion übernimmt
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/*
void serverLoop() - LOOP Export
Export des Loops
hier werden eigehende Anfragen vom HomeKit verarbeitet & im Anschluss das menu Geupdated
sollte es sich um eine Polling-Anfrage handeln, wird abgebrochen & an answerPoll übergeben
*/
void serverLoop(){

  if(WiFi.status() != WL_CONNECTED){
    // Wenn keien WLAN Verindung besteht -> nutzlos
    return;
  }
  
  WiFiClient client = server.available();

  if(!client){
    // Wenn gerade kein Client da sein will, sofort raus aus der Funktion
    return;
  }

  // Wenn ein Client da ist
  while(!client.available()){
    // Warten, bis der verbundene Client Verfügbar wird
    delay(10);
  }
  // Wenn der verbundene Client verfügbar ist
  // Lesen der Anfrage
  String request = client.readStringUntil('\r');
  client.flush();
  Serial.println(request);

  // Bearbeiten der Anfrage

  if (request.indexOf("/poll") != -1) {
    // Es handelt sich um einen Polling-Request -> bearbeiten & dann sofort returnen. Kein Menu-Update!
    answerPoll(request, client);
    client.stop();
    return;
  } 
  
  // Alle anderen Anfragen -> Schaltanfragen
  if (request.indexOf("/l_coop1") != -1) { // Licht Stall 1
    if(request.indexOf("/setH") != -1){
      // Einschalten
      dO_L_Coop1.setIt(on);
    } else 
    if(request.indexOf("/setL") != -1){
      // Ausschalten
      dO_L_Coop1.setIt(off);
    } else {
      // Fehler
      Serial.println("Fehlerhafte Anfrag");
    }
  } else
  if (request.indexOf("/l_coop2") != -1) { // Licht Stall 2
    if(request.indexOf("/setH") != -1){
      // Einschalten
      dO_L_Coop2.setIt(on);
    } else 
    if(request.indexOf("/setL") != -1){
      // Ausscahlten
      dO_L_Coop2.setIt(off);
    } else {
      // Fehler
      Serial.println("Fehlerhafte Anfrag");
    }
  } else 
  if (request.indexOf("GET /l_storage") != -1) { // Licht Kammer
    if(request.indexOf("/setH") != -1){
      // Einschalten
      dO_L_storage.setIt(on);
    } else 
    if(request.indexOf("/setL") != -1){
      // Ausscahlten
      dO_L_storage.setIt(off);
    } else {
      // Fehler
      Serial.println("Fehlerhafte Anfrag");
    }
  } else 
  if (request.indexOf("/l_entrance") != -1) { // Licht Eingang
    if(request.indexOf("/setH") != -1){
      // Einschalten
      dO_L_Entrance.setIt(on);
    } else 
    if(request.indexOf("/setL") != -1){
      // Ausscahlten
      dO_L_Entrance.setIt(off);
    } else {
      // Fehler
      Serial.println("Fehlerhafte Anfrag");
    }
  } else 
  if (request.indexOf("GET /l_ceiling") != -1) { // Licht Dachboden
    if(request.indexOf("/setH") != -1){
      // Einschalten
      dO_L_Ceiling.setIt(on);
    } else 
    if(request.indexOf("/setL") != -1){
      // Ausscahlten
      dO_L_Ceiling.setIt(off);
    } else {
      // Fehler
      Serial.println("Fehlerhafte Anfrag");
    }
  } else 
  if (request.indexOf("GET /l_flood") != -1) { // Licht Flutlicht
    if(request.indexOf("/setH") != -1){
      // Einschalten
      dO_L_Floodlight.setIt(on);
    } else 
    if(request.indexOf("/setL") != -1){
      // Ausscahlten
      dO_L_Floodlight.setIt(off);
    } else {
      // Fehler
      Serial.println("Fehlerhafte Anfrag");
    }
  } else 
  if (request.indexOf("GET /olt_outdoor") != -1) { // Steckdosen Outdoor
    if(request.indexOf("/setH") != -1){
      // Einschalten
      dO_OLT_outdoor.setIt(on);
    } else 
    if(request.indexOf("/setL") != -1){
      // Ausscahlten
      dO_OLT_outdoor.setIt(off);
    } else {
      // Fehler
      Serial.println("Fehlerhafte Anfrag");
    }
  } else 
  if (request.indexOf("GET /olt_indoor") != -1) { // Steckdosen Indoor
    if(request.indexOf("/setH") != -1){
      // Einschalten
      dO_OLT_indoor.setIt(on);
    } else 
    if(request.indexOf("/setL") != -1){
      // Ausscahlten
      dO_OLT_indoor.setIt(off);
    } else {
      // Fehler
      Serial.println("Fehlerhafte Anfrag");
    }
  } else 
  if (request.indexOf("GET /olt_puddle") != -1) { // Steckdosen Teich
    if(request.indexOf("/setH") != -1){
      // Einschalten
      dO_OLT_puddle.setIt(on);
    } else 
    if(request.indexOf("/setL") != -1){
      // Ausscahlten
      dO_OLT_puddle.setIt(off);
    } else {
      // Fehler
      Serial.println("Fehlerhafte Anfrag");
    }
  } else 
  if (request.indexOf("GET /fence") != -1) { // Weidezaun
    if(request.indexOf("/setH") != -1){
      // Einschalten
      dO_fence.setIt(on);
    } else 
    if(request.indexOf("/setL") != -1){
      // Ausscahlten
      dO_fence.setIt(off);
    } else {
      // Fehler
      Serial.println("Fehlerhafte Anfrag");
    }
  } else 
  if (request.indexOf("GET /fan") != -1) { // Lüfter
    if(request.indexOf("/setH") != -1){
      // Einschalten
      dO_fan.setIt(on);
    } else 
    if(request.indexOf("/setL") != -1){
      // Ausscahlten
      dO_fan.setIt(off);
    } else {
      // Fehler
      Serial.println("Fehlerhafte Anfrag");
    }
  } else 
  if (request.indexOf("GET /pump") != -1) { // Teichfilterpumpe
    if(request.indexOf("/setH") != -1){
      // Einschalten
      dO_pudlePump.setIt(on);
    } else 
    if(request.indexOf("/setL") != -1){
      // Ausscahlten
      dO_pudlePump.setIt(off);
    } else {
      // Fehler
      Serial.println("Fehlerhafte Anfrag");
    }
  } else 
  if (request.indexOf("GET /kaercher") != -1) { // Kärcher
    if(request.indexOf("/setH") != -1){
      // Einschalten
      dO_kaercher.setIt(on);
    } else 
    if(request.indexOf("/setL") != -1){
      // Ausscahlten
      dO_kaercher.setIt(off);
    } else {
      // Fehler
      Serial.println("Fehlerhafte Anfrag");
    }
  } 
  if (request.indexOf("GET /sysLock") != -1) { // Lock Zustand (nicht nur Geräte, ganzer Zustand)
    if(request.indexOf("/unlock") != -1){
      // Aufschließen
      unlockSystem();
    } else 
    if(request.indexOf("/lockdown") != -1){
      // Sperren
      lockSystem();
    } else {
      // Fehler
      Serial.println("Fehlerhafte Anfrag");
    }
  } else 
  if (request.indexOf("GET /klappe1") != -1) { // Lock Zustand (nicht nur Geräte, ganzer Zustand)
    // Schreiben, dass Klappe 1 nun unterwegs ist
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println("");

    if(request.indexOf("/open") != -1){
      // Klappe 1 öffnen
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/plain");
      client.println("");
      client.println("0");
      client.stop();

      klappe1.openIt();
      
      updateMenu();
      return;
    } else 
    if(request.indexOf("/close") != -1){
      // Klappe 1 schließen
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/plain");
      client.println("");
      client.println("1");
      client.stop();
      klappe1.closeIt();
      updateMenu();
      return;
    } else {
      // Fehler
      Serial.println("Fehlerhafte Anfrag");
    }
    client.println("");
    client.stop();
    updateMenu();
    return; // Damit nicht nochmal einen Antwort geschrieben wird
  } else 
  if (request.indexOf("GET /klappe2") != -1) { // Lock Zustand (nicht nur Geräte, ganzer Zustand)
    if(request.indexOf("/open") != -1){
      // Klappe 1 öffnen
      klappe2.openIt();
    } else 
    if(request.indexOf("/close") != -1){
      // Klappe 1 schließen
      klappe2.closeIt();
    } else {
      // Fehler
      Serial.println("Fehlerhafte Anfrag");
    }
  }
  
  else {
    // Grundsätzlich Unbekannte Anfrage
    Serial.println("Grundsätzlich falsche Anfrage");
  }

  // Alle Normalen Schaltanfragen
  

  // HTTP Response schreiben
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/plain");
  client.println("");
  client.println("Aktion Ausgeführt");

  client.stop(); // Verbindung zu Client lösen
  updateMenu(); // Veränderten Zustand an das Menü senden

}

#endif