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

#include "kernel/Globals.h"
#include "kernel/threads/Thread.h"

// Funktionen, die auf der Assembler-Ebene implementiert werden, muessen als
// extern "C" deklariert werden, da sie nicht dem Name-Mangeling von C++
// entsprechen.
extern "C"
{
    void Thread_start  (uint64_t* context);
    void Thread_switch (uint64_t** context_now, uint64_t** context_then);
}

uint32_t Thread::next_tid = 0;


/*****************************************************************************
 * Prozedur:        Thread_init                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Bereitet den Kontext der Koroutine fuer den ersten       *
 *                  Aufruf vor.                                              *
 *****************************************************************************/
void Thread_init (uint64_t **stackptr, uint64_t *stack,
                     void (*kickoff)(Thread*), void* object) {

    uint64_t *sp = stack;

    // Stack initialisieren. Es soll so aussehen, als waere soeben die
    // eine Funktion aufgerufen worden, die als Parameter den Zeiger
    // "object" erhalten hat.
    // Da der Funktionsaufruf simuliert wird, kann fuer die Ruecksprung-
    // adresse nur ein unsinniger Wert eingetragen werden. Die aufgerufene
    // Funktion muss daher dafuer sorgen, dass diese Adresse nie benoetigt
    // wird, sie darf also nicht terminieren, sonst kracht's.
    *(--sp) = 0x0; // Ruecksprungadresse

    // Nun legen wir noch die Adresse der Funktion "kickoff" ganz oben auf
    // den Stack. Wenn dann bei der ersten Aktivierung dieser Koroutine der
    // Stackpointer so initialisiert wird, dass er auf diesen Eintrag
    // verweist, genuegt ein ret, um die Funktion kickoff zu starten.
    // Genauso sollen auch alle spaeteren Threadn-Wechsel ablaufen.

    *(--sp) = (uint64_t)kickoff;  // Adresse

    // Nun sichern wir noch alle relevanten Register auf dem Stack
    *(--sp) = 0;    	// r8
    *(--sp) = 0;   		// r9
    *(--sp) = 0;   		// r10
    *(--sp) = 0;   		// r11
    *(--sp) = 0;   		// r12
    *(--sp) = 0;   		// r13
    *(--sp) = 0;   		// r14
    *(--sp) = 0;   		// r15
    *(--sp) = 0;   		// rax
    *(--sp) = 0;   		// rbx
    *(--sp) = 0;   		// rcx
    *(--sp) = 0;   		// rdx

    *(--sp) = 0;   		// rsi
    *(--sp) = (uint64_t)object; 	// rdi -> 1. Param fuer 'kickoff'
    *(--sp) = 0;   		// rbp
    *(--sp) = cpu.getflags(); // flags

    // Zum Schluss speichern wir den Zeiger auf den zuletzt belegten
    // Eintrag auf dem Stack in 'stackptr'. Daruber gelangen wir in
    // Thread_start an die noetigen Register
    *stackptr = sp;		// aktuellen Stack-Zeiger speichern
}


/*****************************************************************************
 * Funktion:        kickoff                                                  *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Funktion zum Starten einer Korutine. Da diese Funktion   *
 *                  nicht wirklich aufgerufen, sondern nur durch eine        *
 *                  geschickte Initialisierung des Stacks der Koroutine      *
 *                  angesprungen wird, darf er nie terminieren. Anderenfalls *
 *                  wuerde ein sinnloser Wert als Ruecksprungadresse         *
 *                  interpretiert werden und der Rechner abstuerzen.         *
 *****************************************************************************/
void kickoff (Thread* object) {
    object->run();

    // object->run() kehrt hoffentlich nie hierher zurueck
    for (;;) {}
}


/*****************************************************************************
 * Methode:         Thread::Thread                                     *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Initialer Kontext einer Koroutine einrichten.            *
 *                                                                           *
 * Parameter:                                                                *
 *      stack       Stack für die neue Koroutine                             *
 *****************************************************************************/
Thread::Thread () {
    tid = Thread::next_tid++;

    this->stack = new uint64_t[1024];
    Thread_init (&context, stack + sizeof(uint64_t[1024]), kickoff, this);
 }

 void Thread::free() {
     delete stack;
 }


/*****************************************************************************
 * Methode:         Thread::switchToNext                          	     *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Auf die nächste Koroutine umschalten.                    *
 *****************************************************************************/
void Thread::switchTo (Thread* next) {
    Thread_switch(&context, &next->context);
}


/*****************************************************************************
 * Methode:         Thread::start                                         *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Aktivierung der Koroutine.                               *
*****************************************************************************/
void Thread::start () {
    Thread_start(context);
}
