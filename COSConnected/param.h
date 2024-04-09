// Headerguard
#ifndef param_H
#define param_H

// Die wichtigsten System-Stellschrauben habe ich hier her gebau

#define timeToStandby 5000 // Zeit in ms, bis standby ausgelöst werden soll
#define standardPiep 1000 // Wie lange soll ein parameterloses Piepen dauern?
#define timeBetweenDayNightTest 25000 // Zeit in ms, nachdem der Zweite Lichtsensor Check erfolgt & den Day / Night Zustand bestätigt

#define timeToError 15000 // Zeit, ab wann die Klappen in den ERR Zustand übergehen

#define timeToReconnect 20000 // Zeit, wo neue WLAN Verbindung angefragt werden soll, wenn die alte abgebrochen wurde

#define wifiTimeOut 5000 // 5 Sekunden Zeit, wie lange er versuchen soll, sich mit dem Router zu Verbinden
#define serialTimeout 5000 // Zeit, ab wann UNO Verbindung fehlschlägt

#define buttonLoopback 500


#endif