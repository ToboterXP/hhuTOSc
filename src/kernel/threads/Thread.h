/*****************************************************************************
 *                                                                           *
 *                  C O R O U T I N E                                        *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Implementierung eines Koroutinen-Konzepts.               *
 *                                                                           *
 *                  Die Koroutinen sind miteinander verkettet, weswegen die  *
 *                  Klasse Thread ein Subtyp von 'Chain' ist.             *
 *                                                                           *
 *                  Im Konstruktor wird der initialie Kontext der Koroutine  *
 *                  eingerichtet. Mit 'start' wird ein Koroutine aktiviert.  *
 *                  Das Umschalten auf die naechste Koroutine erfolgt durch  *
 *                  Aufruf von 'switchToNext'.                               *
 *                                                                           *
 *                  Bei einem Koroutinenwechsel werden die Register auf dem  *
 *                  Stack gesichert. Die Instanzvariable 'context' zeigt auf *
 *                  den letzten hierfuer genutzten Stackeintrag.             *
 *                                                                           *
 * Autor:           Michael, Schoettner, HHU, 13.01.2023                     *
 *****************************************************************************/

#ifndef __Thread_include__
#define __Thread_include__

#include "lib/Types.h"


class Thread {

private:
    Thread(const Thread &copy); // Verhindere Kopieren

private:
    uint64_t* stack;
    uint64_t* context; 	// Stack-Zeiger auf gesicherten Kontext

    static uint32_t next_tid;

public:
    uint32_t tid;

    Thread ();

    // Thread aktivieren
    void start ();

    //removes all ancillary objects from the stack
    void free();

    // Auf die naechste Thread umschalten
    void switchTo (Thread* next);

    // Methode der Thread, muss in Sub-Klasse implementiert werden
    virtual void run () = 0;

 };

#endif
