// Headerguard
#ifndef instances_H
#define instances_H
// Hier werden alle benötigten Geräte Instanziiert

// Klassen Definitionen inkludierne, damit diese nun instanziert werden können
#include "classes.h"

// Instanzen der Schalter
button db_buttonUP(p_buttonUP);
button db_buttonDOWN(p_buttonDOWN);
button db_buttonBACK(p_buttonBACK);
button db_buttonOK(p_buttonOK);
button db_buttonLOCK(p_buttonLOCK);

// Instanzen der Lampen
OUTdevice dO_L_storage(p_lampStorage);
OUTdevice dO_L_Coop1(p_lampCoop1);
OUTdevice dO_L_Coop2(p_lampCoop2);
OUTdevice dO_L_Entrance(p_lampEntrance);
OUTdevice dO_L_Ceiling(p_lampCeiling);
OUTdevice dO_L_Floodlight(p_lampFlood);

// Instanz der Steckdosen
OUTdevice dO_OLT_outdoor(p_outletOutdoors);
OUTdevice dO_OLT_indoor(p_outletIndoors);
OUTdevice dO_OLT_puddle(p_outletPudle);

// Instanz der Misc-Geräte
OUTdevice dO_fence(p_fence);
OUTdevice dO_fan(p_fan);
OUTdevice dO_pudlePump(p_pudlePump);
OUTdevice dO_locks(p_locks);
OUTdevice dO_kaercher(p_kaercher);

OUTdevice dO_piezzo(p_piezzo);

// Instanz der Sensoren (inkl. der Klappen-Sensorik)
sensor s_light (p_lightsensor);

sensor sfk_k1_reedUP(p_klappe1_reedUp);
sensor sfk_k1_reedDOWN(p_klappe1_reedDown);

sensor sfk_k2_reedUP(p_klappe2_reedUp);
sensor sfk_k2_reedDOWN(p_klappe2_reedDown);

// Klappen Schaltausgänge
OUTdevice k1_direc(p_klappe1_direc);
OUTdevice k1_driver(p_klappe1_driver);

OUTdevice k2_direc(p_klappe2_direc);
OUTdevice k2_driver(p_klappe2_driver);

// Instanzierung der eigentlichen Klappen mit den hier gebauten Sensoren & Devices
klappe klappe1(&sfk_k1_reedUP, &sfk_k1_reedDOWN, &k1_driver, &k1_direc);
klappe klappe2(&sfk_k2_reedUP, &sfk_k2_reedDOWN, &k2_driver, &k2_direc);

// Instanzierung des Displays
LiquidCrystal_PCF8574 display(I2C_ADDRESS);

// Instanzierung des Kartenlesers
//MFRC522 cardreader(SS_PIN, RST_PIN);
// Outgesourced zu einem Arduino Uno, da es mit dieser Architketur nicht wirklich geht

// Instanzierung des WLAN-Signal Lichts
OUTdevice dO_sig_WiFiState(p_indicator);

#endif