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


#ifndef __Scheduler_include__
#define __Scheduler_include__

#include "lib/Types.h"
#include "kernel/threads/Thread.h"
#include "lib/List.h"
#include "lib/SpinLock.h"

class Scheduler{

private:
    Scheduler (const Scheduler &copy); // Verhindere Kopieren

private:
    List<Thread*> readyQueue = List<Thread*>(NULL);   // auf die CPU wartende Threads

    List<Thread*> killedQueue  = List<Thread*>(NULL); //Threads to be removed;

    Thread* active = NULL;     // aktiver Thread

    void start (Thread* first);

    void dispatch (Thread* next);

    void disposeKilledThreads();

    SpinLock lock;

    // CPU freiwillig abgeben und Auswahl des naechsten Threads
    void insecure_yield ();

public:
    Scheduler () {}

    /*
    // Scheduler initialisiert?
    // Zeitgeber-Unterbrechung kommt evt. bevor der Scheduler fertig
    // intiialisiert wurde!
    bool isInitialized() { return initialized; }

    // ruft nur der Idle-Thread (erster Thread der vom Scheduler gestartet wird)
    void setInitialized() { initialized = true; }*/

    // Scheduler starten
    void schedule ();

    // Thread in ReadyQueue eintragen
    void ready (Thread* that);

    // Thread terminiert sich selbst
    void exit ();

    // Thread mit 'Gewalt' terminieren
    void kill (Thread* that);

    // CPU freiwillig abgeben und Auswahl des naechsten Threads
    void yield ();

    // Umschalten auf naechsten Thread, aber den
    // Aufrufer nicht in die Ready-Queue einfuegen
    void block ();

    // 'that' wieder in die Ready-Queue einfuegen
    void unblock (Thread *that);


    void preempt();

    Thread* get_active () { return active; }
};

#endif
