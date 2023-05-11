/*****************************************************************************
 *                                                                           *
 *                                   P I T                                   *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Programmable Interval Timer.                             *
 *                                                                           *
 * Autor:           Michael Schoettner, 3.7.2022                             *
 *****************************************************************************/

#include "devices/PIT.h"
#include "kernel/IOport.h"
#include "kernel/Globals.h"


/*****************************************************************************
 * Methode:         PIT::plugin                                              *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Unterbrechungen fuer den Zeitgeber erlauben. Ab sofort   *
 *                  wird bei Ablauf des definierten Zeitintervalls die       *
 *                  Methode 'trigger' aufgerufen.                            *
 *****************************************************************************/
void PIT::init () {
    intdis.assign(intdis.timer, *this);
    pic.allow(0);

    control.outb(0b00110000); //Timer 0, Lowbyte/Hibyte, IRQ on terminal count

    timer = 1193182 * interval / 1000000;
    trigger();//properly set the timers
}


/*****************************************************************************
 * Methode:         PIT::trigger                                             *
 *---------------------------------------------------------------------------*
 * Beschreibung:    ISR fuer den Zeitgeber. Wird aufgerufen, wenn der        *
 *                  Zeitgeber eine Unterbrechung ausloest. Anzeige der Uhr   *
 *                  aktualisieren und Thread wechseln durch Setzen der       *
 *                  Variable 'forceSwitch', wird in 'int_disp' behandelt.    *
 *****************************************************************************/
void PIT::trigger () {
    //restart timer
    data0.outb(timer&0xff);
    data0.outb((timer>>8) & 0xff);

    //process timers
    auto current = timers.get_first();
    while (current) {
        current->data.counter--;
        if (current->data.counter <= 0) {
            current->data.counter = current->data.divider;
            (*current->data.callback)();
        }
        current = current->GetNext();
    }
}

void PIT::AddTimer(uint32_t microseconds, CallbackHandler callback) {
    PITTimer new_timer;
    new_timer.divider = microseconds < interval ? 1 : microseconds/interval;
    new_timer.counter = new_timer.divider;
    new_timer.callback = callback;
    timers.append(new_timer);
}

void PIT::RemoveTimer(CallbackHandler callback) {
    auto current = timers.get_first();
    while (current) {
        if (current->data.callback == callback) {
            timers.remove(current);
            return;
        }
        current = current->GetNext();
    }
}
