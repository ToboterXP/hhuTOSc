/*****************************************************************************
 *                                                                           *
 *                                   P I T                                   *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Programmable Interval Timer.                             *
 *                                                                           *
 * Autor:           Michael Schoettner, 23.8.2016                            *
 *****************************************************************************/

#ifndef __PIT_include__
#define __PIT_include__

#include "kernel/interrupts/ISR.h"
#include "kernel/IOPort.h"
#include "lib/List.h"

typedef void (*CallbackHandler)();

typedef struct _PITTimer {
    uint64_t divider;
    uint64_t counter;
    CallbackHandler callback;
} PITTimer;


class PIT : public ISR {

private:
    PIT(const PIT &copy); // Verhindere Kopieren

    IOport control;     // Steuerregister (write only)
    IOport data0;       // Zaehler-0 Datenregister (read/write)
    IOport data2;       // Zaehler-2 Datenregister
    IOport ppi;         // Status-Register des PPI

    List<PITTimer> timers;
    PITTimer empty;
private:
    enum { time_base = 838 };  /* ns */
    const uint64_t interval = 10; //microseconds
    uint16_t timer;
public:

    PIT() : control(0x43), data0(0x40), data2(0x42), ppi(0x61), timers(empty) {

    }

    // Aktivierung der Unterbrechungen fuer den Zeitgeber
    void init ();

    void AddTimer(uint32_t microseconds, CallbackHandler callback);

    void RemoveTimer(CallbackHandler callback);

    // Unterbrechnungsroutine des Zeitgebers.
    void trigger ();
};

#endif
