/*****************************************************************************
 *                                                                           *
 *                             S E M A P H O R E                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Implementierung von Sempahor-Objekten.                   *
 *                                                                           *
 * Autor:           Michael Schoettner, 2.9.2016                             *
 *****************************************************************************/

#ifndef __Semaphore_include__
#define __Semaphore_include__


#include "lib/List.h"
#include "lib/SpinLock.h"
#include "kernel/threads/Thread.h"


class Semaphore {

private:
    Semaphore (const Semaphore &copy); // Verhindere Kopieren

    // Queue fuer wartende Threads.
    List<Thread*> waitQueue =  List<Thread*>(NULL);

    //Liste aller aktuell nutzenden Threads
    List<Thread*> acquiredList =  List<Thread*>(NULL);
    SpinLock lock;

    SpinLock access_lock; //prevents simultaneous access

    int counter;

public:
    // Konstruktor: Initialisieren des Semaphorzaehlers
    Semaphore (int c) : counter(c) {}

    // 'Passieren': Warten auf das Freiwerden eines kritischen Abschnitts.
    void acquire ();

    // 'Vreigeben': Freigeben des kritischen Abschnitts.
    void release ();

    //returns true if the given thread has acquired the semaphore
    bool has_acquired(Thread * thread);

    bool is_locked();
 };

#endif
