// Hier werden alle Geräteklassen definiert, die exisitieren.
// Inkludiert von instances.h

// Header-Guards, damit mehrfaches implementieren zulässig ist
#ifndef classes_H
#define classes_H

// Pseudo-Enum, um ein und ausscahlten einfacher zu machen
#define on 1
#define off 0

// Interface für Geräte
class device{
  protected: 
  // Variablen die Ein Gerät benötigt
    int port; // Anschluss
    uint8_t mode; // Input / Outut Gerät
    bool zustand; // Gibt den aktuellen Digital-Zwilling Zustand des Geräts an
  
  public:
    virtual void setMode();

  // Konstruktor
    device (int p){
      // Mode wird automatisch in der respektiven Klasse festgelegt
      port = p;
      zustand = 0;
    }
};

/*
------------------------------------------------
EINGABE GERÄTE / Sensor

Erwarten zum initialisieren einfach einen Pin. Verwendet den normalen INPUT Modus
Funktionalität:
- int readData() aktualisiert den Zustand mit der Hardware & gibt diesen im Anschluss Zurück
*/

class sensor : public device {
  public: 

  // Konstruktor (da Attriburte bereits existieren)
  sensor(int p) : device(p){
    mode = INPUT_PULLUP;
    setMode();
  }

  // Funktionen eines Eingabegeräts
  int readData(){
    zustand = digitalRead(port);
    return zustand;
  }

  private:
    // Methdoe für die Abstrakte Klasse
    void setMode(){
      pinMode(port, mode);
    }
};

/*
------------------------------------------------
EINGABE GERÄT / Button

Erwarten zum initialisieren einfach einen Pin. Speziell für Knöpfe: Verwendet den INPUT_PULLUP Modus
Funktionalität:
- int testPressed() aktualisiert den Zustand mit der Hardware & gibt diesen im Anschluss Zurück. 1 Wenn Knopf gerade gedrückt
1 / HIGH = Nicht gedrückt
*/

class button : public device {
  public: 

  // Konstruktor (da Attriburte bereits existieren)
  button(int p) : device(p){
    mode = INPUT_PULLUP;
    setMode();
  }

  // Funktionen eines Eingabegeräts
  int testPressed(){
    zustand = digitalRead(port);
    return zustand;
  }

  private:
    // Methdoe für die Abstrakte Klasse
    void setMode(){
      pinMode(port, mode);
    }
};


/*
------------------------------------------------
AUSGABGE GERÄTE

Erwarten zum initialisieren einfach einen Pin.
Funktionalität:
- void setIt(int zielzustand) - setzt den Zustand des Gerätes in Hard- & Software
- bool getIt() - gibt Zustand des Geräts zurück
- int switchIt() - schaltet das gerät in den gegenteiligen Zustand in Hard & Software. Gibt den neuen Zustand zurück.
*/

class OUTdevice : public device {
  public:
    // Konstruktor (da Attriburte bereits existieren)
    OUTdevice(int p) : device(p){
      mode = OUTPUT;
      setMode();
      digitalWrite(port, LOW); // Sicherhietshalber erst mal ausschalten bei Initialisierung
      zustand = 0;
    }

    // Funktionen eines Ausgabegerätes
    void setIt(int zielzustand){
      zustand = zielzustand;
      digitalWrite(port, zustand);
    }

    // gibt den aktuellen Zustand des Geräts aus
    bool getIt(){
      return zustand;
    }

    int switchIt(){
      if(!zustand){
        // Gerät war aus -> einschalten
        zustand = HIGH;
      } 
      else {
        // Gerät war an -> ausschalten
        zustand = LOW;
      }
      // Neuen Zustand in Hardware ändern und Ergebnis rückgeben
      digitalWrite(port, zustand);
      return zustand;
    }

  private:
    // Methdoe für die Abstrakte Klasse
    void setMode(){
      pinMode(port, mode);
    }

};

// Zustands-Enum für die Klappen
enum k_zustand {
  opened, 
  closed, 
  err
};

/*
--------------–––--------------------–––--------------------–––--------------------–––------
KLAPPEN

Brauchen eine eigene Klasse, da sie zu komplex sind um sie als einfaches device zu definieren.
Verwendet zwei eigene Enums
Basiert auf den davor gewählten Geräten

Erwartet beim erstellen 2 Sensoren für die Reed-Kontakte und zwei OUTdevice für den Motorstart sowie Richtungsanzeiger

Funktionalität:
- k_zustand switchIt() - fährt die Klappe sicher in den entgegengesetzten Zustand. Wenn bereits im err-Zustand, passiert nichts
- k_zustand openIt() - öffnet Klappe. Wenn schon offen, passiert nicht. Wenn beim Öffnen ein Fehler passiert, wird kalibriert.
- k_zustand closeIt() - schließt Klappe. Wenn schon zu, passiert nichts. Wenn beim Öffnen ein Fehler passiert, wird kalibriert.
*/

class klappe {
  protected:
    sensor* reed_up; // Sensorpin für oberen Reed-Kontakt
    sensor* reed_down; // Sensorpin für unteren Reed-Kontakt
    OUTdevice* drive; // Schaltpin für Motor-Spannung
    OUTdevice* motor_director;
    k_zustand zustand;
  
  public:
    // Konstruktor
    klappe(sensor* reed_up, sensor* reed_down, OUTdevice* drive, OUTdevice* motor_director) : 
      reed_up(reed_up), 
      reed_down(reed_down), 
      drive(drive), 
      motor_director(motor_director)
    {
      // Zu Beginn wissen wir den Zustand nicht.
      zustand = err;
      //calibrate(); // calibrate setzt dann die Zustandsvariable schon richtig
      // Sicherheitshalber
    }

    k_zustand calibrate(){
      // Testen ob Klappe offen ist
      if(!reed_up->readData()){
        if(!reed_down->readData()){
          // Beide Sensoren sind ausgelöst -> Fehlerzustand
          zustand = err;
          return zustand;
        }
        // Nur oberer Sensor ist ausgelößt, Klappe ist offen
        zustand = opened;
        return zustand;
      }
      // Testen ob Klappe geschlossen ist
      if(!reed_down->readData()){
        // ERR Tester brauch ich nicht, weil dieser oben schon ausgelöst hätte

        // Nur unterer Sensor ist ausgelößt, Klappe ist offen
        zustand = closed;
        return zustand;
      }
      // Wenn Klappe weder offen noch geschlossen ist -> Einfach via. öffnen neuen Zustand festlegen.
      zustand = forceAndChoose(goOpen);
      return zustand;
    }

    k_zustand switchIt(){
      switch (zustand) {
        case opened:
          closeIt();
          break;
        case closed:
          openIt();
          break;
        case err:
          zustand = err;
          break;
      }
      return zustand;
    }

    k_zustand openIt(){
      if(zustand == opened){
        // nichts zu tun, da schon geöffent
        // Auch am Systemzustand ändert sich nichts
        return opened;
      }
      // Ansonten aufmachen.
      zustand = forceAndChoose(goOpen);
      // Wen beim öffnen ein Fehler passiert, versuchen zu kalibrieren,
      if(zustand == err){
        return calibrate();
      }
      // Calibrate ändert ggf. den Systemzustand. hier nicht notwendig
      return zustand;
    }

    k_zustand closeIt(){
      if(zustand == closed){
        // nichts zu tun, da schon geöffent
        // Auch am Systemzustand ändert sich nichts
        return closed;
      }
      // Ansonten aufmachen.
      zustand = forceAndChoose(goClose);
      // Wen beim öffnen ein Fehler passiert, versuchen zu kalibrieren,
      if(zustand == err){
        return calibrate();
      }
      // Calibrate ändert ggf. den Systemzustand. hier nicht notwendig
      return zustand;
    }

    k_zustand getIt(){
      return zustand;
    }

  private: // private Submethoden um das ganzte etwas zu entzerren

    enum k_direction{
      goOpen,
      goClose
    };

    // Zwanghaftes Fahren in gegebene Richtung bis Sensor sich meldet. Eigentliche Fahrlogik
    k_zustand forceAndChoose(k_direction direc){

      unsigned long errTimer = millis(); // Zeitstempel für Err-Timer Nehmen.

      if(direc == goOpen){
        // Force Open
        motor_director->setIt(HIGH); // Richtung anwählen STELLSCHRAUBE: Je nachdem wie der Motor verkabelt ist
        drive->setIt(HIGH); // Motor an
        // Warten auf Kontakt
        while(reed_up->readData()){ //"solang ich kein Signal von oben habe" HIGH = kein Kontakt, LOW = Kontakt
          // Wenn schon zu viel Zeit vorbei, dann mit err zurückgeben
          if(millis() - errTimer > timeToError){
            drive->setIt(LOW); // Motor aus
            motor_director->setIt(LOW); // Energiespaaren, Richtungsrelais in Neutralzustand
            return err;
          }
        }
        // Klappe ist angekommen
        drive->setIt(LOW); // Motor aus
        motor_director->setIt(LOW); // Energiespaaren, Richtungsrelais in Neutralzustand
        return opened; // Erfolg

      }

      if(direc == goClose){
        // Force Close
        motor_director->setIt(LOW); // Richtung anwählen STELLSCHRAUBE: Je nachdem wie der Motor verkabelt ist // Sicherheitshalber. Normal immer LOW
        drive->setIt(HIGH); // Motor an
        // Warten auf Kontakt
        while(reed_down->readData()){ //"solang ich kein Signal von oben habe" HIGH = kein Kontakt, LOW = Kontakt
          // Wenn schon zu viel Zeit vorbei, dann mit err zurückgeben
          if(millis() - errTimer > timeToError){
            drive->setIt(LOW); // Motor aus
            return err;
          }
        }
        // Klappe ist angekommen
        drive->setIt(LOW); // Motor aus
        return closed; // Erfolg

      }
      // Wenn schon mit err-Zustand gestartet, gleich zurück an den Handler für evtl. Kalibrierung
      return err;  // tritt nie ein -> niemand Sollt ERR übergeben 
    }
};

#endif // Ende des classes_H Define-Bereich (für den Header-Guard)