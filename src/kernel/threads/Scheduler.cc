/*****************************************************************************
 *                                                                           *
 *                          S C H E D U L E R                                *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Implementierung eines einfachen Zeitscheiben-Schedulers  *
 *                  welcher kooperative Threads in einer 'readQueue' ver-    *
 *                  waltet.                                                  *
 *                                                                           *
 *                  Der Scheduler wird mit 'schedule' gestartet. Neue Threads*
 *                  können mit 'ready' hinzugefügt werden. Ein Thread muss   *
 *                  die CPU freiwillig mit 'yield' abgeben, damit andere auch*
 *                  rechnen koennen. Ein Thread kann sich selbst mit 'exit'  *
 *                  terminieren. Ein Thread kann einen anderen Thread mit    *
 *                  'kill' beenden.                                          *
 *                                                                           *
 * Autor:           Michael, Schoettner, HHU, 21.12.2018                     *
 *****************************************************************************/

#include "kernel/threads/Scheduler.h"
#include "kernel/threads/TestThread.h"



/*****************************************************************************
 * Methode:         Scheduler::schedule                                      *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Scheduler starten. Wird nur einmalig aus main.cc gerufen.*
 *****************************************************************************/
void Scheduler::schedule () {
    start(readyQueue.pop_last());
}


/*****************************************************************************
 * Methode:         Scheduler::ready                                         *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Thread in readyQueue eintragen.                          *
 *                                                                           *
 * Parameter:                                                                *
 *      that        Einzutragender Thread                                    *
 *****************************************************************************/
void Scheduler::ready (Thread * that) {
    readyQueue.prepend(that);
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
    killedQueue.append(active);

    if (readyQueue.is_empty()) {
        kout << "No Threads - Halting" << endl;
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
    if (that == active) return exit();

    ListBlock<Thread*>* current = readyQueue.get_first();

    while (current) {
        if (current->data == that) {
            readyQueue.remove(current);
            killedQueue.append(that);
            return;
        }
        current = current->GetNext();
    }
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
    //have to clean up threads here, since they are necessary for context switch
    disposeKilledThreads();

    readyQueue.prepend(active);

    dispatch(readyQueue.pop_last());
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
    Thread* current = active;
    active = next;
    current->switchTo (next);
}
