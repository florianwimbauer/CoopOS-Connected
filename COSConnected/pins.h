// Hier werden alle Werte definiert, die existieren. Änderungen an der Verkabelung müssen hier eingetragen werden
// Headerguard
#ifndef pins_H
#define pins_H

// -------------------------------------------

/*
Die folgenden Pins sind fix vergeben, weil dies entweder spezielle Ports sind
oder von der Bibliothek so vorgegebe wurde
- TX: 0
- RX: 1
- LCD SDA: 20
- LCD SCL: 21
Alles andere kann frei Belegt werden
*/

// I2C Adresse für das Display (es muss Port 20 & 21 verwendet werden)
#define I2C_ADDRESS 0x27

// Pins für die Knöpfe
#define p_buttonUP 2
#define p_buttonDOWN 3
#define p_buttonOK 4
#define p_buttonBACK 5
#define p_buttonLOCK 6

// Service Zeug
#define p_piezzo 7 // Pieper
#define p_indicator 8 // Signallampe für WLAN

// Pins für den Tageslichtsensor
#define p_lightsensor 9

// Pins für die Lampen
#define p_lampCeiling 10
#define p_lampCoop1 11
#define p_lampCoop2 12
#define p_lampStorage 13
#define p_lampEntrance 14
#define p_lampFlood 15

// Pins für die Steckdosen
#define p_outletIndoors 16
#define p_outletOutdoors 17
#define p_outletPudle 18

// Port 19 ist nicht belegt
// Port 20 und 21 ist Belegt mit Display SCA/SCL

// Pins für die Misc-Geräte
#define p_pudlePump 22 // Teichumwelzpumpe
#define p_fence 23 // Weidezaun
#define p_locks 24// Türschlösser
#define p_fan 25 // Lüfter
#define p_kaercher 26 // Hochdruckreiniger Muss noch gebaut werden

// Pins für die Klappen
#define p_klappe1_reedUp 27
#define p_klappe1_reedDown 28
#define p_klappe1_direc 31
#define p_klappe1_driver 30

#define p_klappe2_reedUp 42
#define p_klappe2_reedDown 32
#define p_klappe2_direc 33
#define p_klappe2_driver 34

// insgesamt werden dafür 30 Pins benötigt
#endif