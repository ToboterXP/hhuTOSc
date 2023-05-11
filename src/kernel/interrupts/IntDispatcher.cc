/*****************************************************************************
 *                                                                           *
 *                         I N T D I S P A T C H E R                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Zentrale Unterbrechungsbehandlungsroutine des Systems.   *
 *                  Der Parameter gibt die Nummer des aufgetretenen          *
 *                  Interrupts an. Wenn eine Interrupt Service Routine       *
 *                  registriert ist, wird diese aufgerufen.                  *
 *                                                                           *
 * Autor:           Michael Schoettner, 31.8.2016                            *
 *****************************************************************************/
#include "kernel/CPU.h"
#include "kernel/Globals.h"
#include "kernel/interrupts/IntDispatcher.h"


extern "C" void int_disp (unsigned int vector);


/*****************************************************************************
 * Prozedur:        int_disp                                                 *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Low-Level Interrupt-Behandlung.                          *
 *                  Diese Funktion ist in der IDT fuer alle Eintraege einge- *
 *                  tragen. Dies geschieht bereits im Bootloader.            *
 *                  Sie wird also fuer alle Interrupts aufgerufen. Von hier  *
 *                  aus sollen die passenden ISR-Routinen der Treiber-Objekte*
 *                  mithilfe von 'IntDispatcher::report' aufgerufen werden.  *
 * Parameter:                                                                *
 *      vector:     Vektor-Nummer der Unterbrechung                          *
 *****************************************************************************/
void int_disp (unsigned int vector) {

    cpu.disable_int();
    //kout << "Int "<< dec << vector <<endl;

    if (intdis.report(vector) != 0) {
        if (vector < 32) kout << "Error "<< dec << vector <<endl;
        cpu.die();
    }

    cpu.enable_int();
}


/*****************************************************************************
 * Konstruktor:     IntDispatcher::IntDispatcher                             *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Initialisierung der ISR map mit einer Default-ISR.       *
 *****************************************************************************/
IntDispatcher::IntDispatcher () {
    for (unsigned int slot=0; slot<size; slot++)
        map[slot] = 0;
}


/*****************************************************************************
 * Methode:         IntDispatcher::assign                                    *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Registrierung einer ISR.                                 *
 *                                                                           *
 * Parameter:                                                                *
 *      vector:     Vektor-Nummer der Unterbrechung                          *
 *      isr:        ISR die registriert werden soll                          *
 *                                                                           *
 * Rueckgabewert:   0 = Erfolg, -1 = Fehler                                  *
 *****************************************************************************/
int IntDispatcher::assign (unsigned int vector, ISR& isr) {
    if (vector > 255) return -1;
    map[vector] = &isr;
    return 0;

}


/*****************************************************************************
 * Methode:         IntDispatcher::report                                    *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Eingetragene ISR ausfuehren.                             *
 *                                                                           *
 * Parameter:                                                                *
 *      vector:     Gesuchtes ISR-Objekt fuer diese Vektor-Nummer.           *
 *                                                                           *
 * Rueckgabewert:   0 = ISR wurde aufgerufen, -1 = unbekannte Vektor-Nummer  *
 *****************************************************************************/
int IntDispatcher::report (unsigned int vector) {
    if (map[vector] != NULL) {
        map[vector]->trigger();
        return 0;
    } else {
        return -1;
    }
}
