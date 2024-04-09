#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

/////////////////////////////////////////////////////////////////
// DATA
/////////////////////////////////////////////////////////////////
byte admin[] = {0x65, 0xFA, 0x8E, 0x22};
byte isolde[] = {0x2A, 0xA9, 0x46, 0xBF};
byte tom[] = {0x7A, 0x30, 0x52, 0xBD};
byte flo[] = {0xD9, 0xCA, 0xA0, 0xB2};
byte margit[] = {0x7A, 0x25, 0xA2, 0x98};
byte oma[] = {0x66, 0xFA, 0x8E, 0x22};
byte magda[] = {0x4A, 0x27, 0x55, 0xBD};
byte gast[] = {0xDA, 0x08, 0x5F, 0xBD};
byte tester[] = {0x09, 0xB6, 0xA6, 0xE2};
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
char listener = 'x';

// Instanziieren
MFRC522 reader(SS_PIN, RST_PIN);

void setup() {

  Serial.begin(9600); // Serial Begin

  SPI.begin(); // Init SPI bus
  reader.PCD_Init(); // Init MFRC522 card
}

void loop() {
  // Look for new cards
  if (reader.PICC_IsNewCardPresent() && reader.PICC_ReadCardSerial()) {

    bool unknown = false;
    for(int j=0;j<4;j++){
      // Vergleiche die Karte mit allen in der Datenbank
      if(reader.uid.uidByte[j]!= admin[j] 
      && reader.uid.uidByte[j]!= isolde[j]
      && reader.uid.uidByte[j]!= tom[j]
      && reader.uid.uidByte[j]!= flo[j]
      && reader.uid.uidByte[j]!= margit[j]
      && reader.uid.uidByte[j]!= oma[j]
      && reader.uid.uidByte[j]!= magda[j]
      && reader.uid.uidByte[j]!= gast[j]
      && reader.uid.uidByte[j]!= tester[j]
      ){
        // Es war gerade ein Byte dabei, dass in keines der bekannten Daten gepasst hat!
        unknown = true;
      }
    }

    // Testen, was das lesen ergeben hat
    if(unknown){
      // UUID der Karte wurde nicht erkannt
      Serial.println("BAD");
    } else {
      // UUID der Karte konnte gematcht werden
      Serial.println("OK");
    }
    delay(1500);
  }

  if(Serial.available() > 0){
    String message = Serial.readString(); // Read the incoming data as a string
    Serial.println("back");
  }
}