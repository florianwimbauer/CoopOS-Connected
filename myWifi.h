// Hier künmmern wir uns um das ganze WLAN Zeug

#ifndef myWifi_H
#define myWifi_H

///////////////////////////
// Meine WLAN Zugangsdaten
char ssid[] = "WimLAN";
char pass[] = "6777063957657990";
///////////////////////////

// Initialieren des Servers
bool wifiAgain = false;
unsigned long wifiTimeStamp;
int status = WL_IDLE_STATUS;
WiFiServer server(80);

// Auslagerung des WLAN-Setups
void setupWifi(){
  dO_sig_WiFiState.setIt(on); // WLAN Problemlampe einschatlen, da erst verbunden wird
  // Verbinden mit dem Router
  unsigned long int wifiErrTimer = millis(); // Zeitstempel nehmen

  schreiben("Verbinden mit", ssid);
  while (status != WL_CONNECTED && millis() - wifiErrTimer < wifiTimeOut) {
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
    // wait  seconds for connection:
    delay(1000);
  }

  if(status == WL_CONNECTED){
    // Wen verbunden, dann normal weiter
    server.begin(); // start the web server on port 80
    Serial.println(WiFi.localIP());
    dO_sig_WiFiState.setIt(off); // WLAN Problem ausschalten
    schreiben("WiFi Verbindung", "Erfolgreich!");
  } else {
    // Wenn nicht verbunden nach Timeout, dann Error!
    schreiben("WiFi Verbindung", "Fehlgeschlagen!");
  }

  delay(1500);
}

/*
void loopWiFi() - LOOP EXPORT
aktualisiert die globale "status" Variable nach aktuellem WLAN Zustand & verbindet sich in gewissen Intervallen wieder, wenn die Verbindung verloren gegangen ist.
*/
void loopWiFi(){

  status = WiFi.status(); // Aktualisiert den Stats zustand im Loop
  if(status != WL_CONNECTED){ // Wenn das system gerade nicht verbunden ist
    if(!wifiAgain){
      // Wenn wiederholung noch nicht erlaubt, einschalten
      wifiAgain = true;
      wifiTimeStamp = millis();
      return;
    }
    // Testen, ob ein reconnect gerade erlaubt ist
    if(wifiAgain && millis() - wifiTimeStamp > timeToReconnect){
      // Zeit ist abgelaufen & ich möchte mich immer noch reconnecten
      setupWifi();
      wifiAgain = false;
    }
  }
}


#endif