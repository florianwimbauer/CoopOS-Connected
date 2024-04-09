#include "mstd_type_traits"
// Hier werden die Custom-Methoden zusammgebaut, die für das LCD Disply zuständig sind
// Header-Guards, damit mehrfaches implementieren zulässig ist
#ifndef lcd_H
#define lcd_H

void antiStandby(); // Forward-Deklaration, um in Schreiben verwendet werden zu können

// Auslagerung des general Setup Teils für das LCD Display
void setupForLCD(){
  display.begin(16,2); 
  display.setBacklight(255);
}

/*
Schreibfunktion für das Display
Erwartet:
- String für die erste Zeile (max. 16 Zeichen!)
- optional: string für die Zweite Zeile (max. 16 Zeichen!)
Gibt true / false zurück, je nachdem ob druck erfolgreich war (bzgl. Länge)
*/
bool schreiben(String firstLine, String secondLine = ""){
  // Logik für Rückgabe - wenn Eingabe zu Lang
  if(firstLine.length() > 16 || secondLine.length() > 16){
    return false;
  }
  // Anzeigelogik
  display.clear(); // Anzeige leer machen
  antiStandby(); // Macht Displa Hell & setzt Standby Timer zurück
  display.setCursor(0,0); // Cursor auf oben links setzen
  display.print(firstLine); // erste Zeile drucken
  // Testen, ob Zweite Zeiel vorhanden ist
  if(secondLine != ""){
    display.setCursor(0,1); // Cursor auf unten links setzen
    display.print(secondLine); // Zweite Zeile Drucken
  }
  return true;
}


#endif