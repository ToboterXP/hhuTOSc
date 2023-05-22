/*****************************************************************************
 *                                                                           *
 *                          S C H E D U L E R                                *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Implementierung eines einfachen Zeitscheiben-Schedulers  *
 *                  welcher praeemptive Threads in einer 'readQueue' ver-    *
 *                  waltet.                                                  *
 *                                                                           *
 *                  Der Scheduler wird mit 'schedule' gestartet. Neue Threads*
 *                  können mit 'ready' hinzugefügt werden. Ein Thread muss   *
 *                  die CPU freiwillig mit 'yield' abgeben, damit andere auch*
 *                  rechnen koennen. Ein Thread kann sich selbst mit 'exit'  *
 *                  terminieren. Ein Thread kann einen anderen Thread mit    *
 *                  'kill' beenden. Ein erzwungener Threadwechsel erfolgt    *
 *                  mit der Funktion 'preempt', welche von der Timer-ISR     *
 *                  aufgerufen wird.                                         *
 *                                                                           *
 * Autor:           Michael, Schoettner, HHU, 21.12.2018                     *
 *****************************************************************************/


#include "kernel/threads/Scheduler.h"
#include "kernel/Globals.h"

extern "C" void preempt ();

/*****************************************************************************
 * Methode:         Scheduler::schedule                                      *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Scheduler starten. Wird nur einmalig aus main.cc gerufen.*
 *****************************************************************************/
void Scheduler::schedule () {
    start(readyQueue.pop_last());
}


/*****************************************************************************
 * Methode:         Scheduler::preempt                                       *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Diese Funktion wird aus der ISR des PITs aufgerufen und  *
 *                  schaltet auf den naechsten Thread um, sofern einer vor-  *
 *                  handen ist.                                              *
 *****************************************************************************/

void Scheduler::preempt () {
    if (active) {
        if (lock.acquire()) return;
        insecure_yield();
    }
}


void Scheduler::ready (Thread * that) {
    //cpu.disable_int();
    lock.waitForAcquire();
    readyQueue.prepend(that);
    lock.release();
    //cpu.enable_int();
}


/*****************************************************************************
 * Methode:         Scheduler::exit                                          *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Thread ist fertig und terminiert sich selbst. Hier muss  *
 *                  nur auf den naechsten Thread mithilfe des Dispatchers    *
 *                  umgeschaltet werden. Der aktuell laufende Thread ist     *
 *                  nicht in der readyQueue.                                 *
 *****************************************************************************/
void Scheduler::exit () {
    //cpu.disable_int();
    lock.waitForAcquire();
    killedQueue.append(active);

    if (readyQueue.is_empty()) {
        kout << "No Threads - Halting"<< endl;
        cpu.die();
    }

    dispatch(readyQueue.pop_last());
}


/*****************************************************************************
 * Methode:         Scheduler::kill                                          *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Thread mit 'Gewalt' terminieren. Er wird aus der         *
 *                  readyQueue ausgetragen und wird dann nicht mehr aufge-   *
 *                  rufen.                                          *
 *                                                                           *
 * Parameter:                                                                *
 *      that        Zu terminierender Thread                                 *
 *****************************************************************************/
void Scheduler::kill (Thread * that) {
    //cpu.disable_int();
    if (that == active) {
        //cpu.enable_int();
        exit();
        return;
    }

    lock.waitForAcquire();

    ListBlock<Thread*>* current = readyQueue.get_first();

    while (current) {
        if (current->data == that) {
            readyQueue.remove(current);
            killedQueue.append(that);
            lock.release();
            return;
        }
        current = current->GetNext();
    }
    lock.release();
    //cpu.enable_int();
}

void Scheduler::disposeKilledThreads() {
    while (!killedQueue.is_empty()) {
        Thread* dead = killedQueue.pop_last();
        dead->free();
        delete dead;
    }
}


/*****************************************************************************
 * Methode:         Scheduler::yield                                         *
 *---------------------------------------------------------------------------*
 * Beschreibung:    CPU freiwillig abgeben und Auswahl des naechsten Threads.*
 *                  Naechsten Thread aus der readyQueue holen, den aktuellen *
 *                  in die readyQueue wieder eintragen. Das Umschalten soll  *
 *                  mithilfe des Dispatchers erfolgen.                       *
 *                                                                           *
 *                  Achtung: Falls nur der Idle-Thread läuft, so ist die     *
 *                           readyQueue leer.                                *
 *****************************************************************************/

void Scheduler::yield () {
    lock.waitForAcquire();
    cpu.disable_int();
    insecure_yield();
    cpu.enable_int();
}

void Scheduler::insecure_yield () {
    cpu.disable_int();

    //kout << "Yield "<<active->tid<<endl;

    //have to clean up threads here, since they are necessary for context switch
    disposeKilledThreads();

    if (active->getStackSize() > 3000) kout << "Stack Warning "<<active->tid<< " "<< active->getStackSize() << endl;

    readyQueue.prepend(active);

    dispatch(readyQueue.pop_last());
    cpu.enable_int();
}

/*****************************************************************************
 * Methode:         Dispatcher::start                                        *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Thread starten.                                          *
 *                                                                           *
 * Parameter:                                                                *
 *      first       Zu startender Thread.                                    *
 *****************************************************************************/
void Scheduler::start (Thread* first) {
    if (!active) {
        active = first;
        active->start ();
    }
}


/*****************************************************************************
 * Methode:         Dispatcher::dispatch                                     *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Auf einen gegebenen Thread wechseln.                     *
 *                                                                           *
 * Parameter:                                                                *
 *      next        Thread der die CPU erhalten soll.                        *
 *****************************************************************************/
void Scheduler::dispatch (Thread* next) {
    dbgString = "Dispatch 1";
    Thread* current = active;
    active = next;
    //cpu.enable_int();
    lock.release();
    dbgString = "Dispatch 2";

    current->switchTo (next);
}

// Umschalten auf naechsten Thread, aber den
// Aufrufer nicht in die Ready-Queue einfuegen
void Scheduler::block () {
    //cpu.disable_int();
    dispatch(readyQueue.pop_last());
}


// 'that' wieder in die Ready-Queue einfuegen
void Scheduler::unblock (Thread *that) {
    //cpu.disable_int();
    lock.waitForAcquire();
    readyQueue.prepend(that);
    lock.release();
    //cpu.enable_int();
}
