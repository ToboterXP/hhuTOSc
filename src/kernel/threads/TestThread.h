/*****************************************************************************
 *                                                                           *
 *                             I D L E T H R E A D                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Wird nur aktiviert, wenn kein Thread arbeiten moechte.   *
 *                                                                           *
 * Autor:           Michael, Schoettner, HHU, 13.8.2016                      *
 *****************************************************************************/

#ifndef __TestThread_include__
#define __TestThread_include__


#include "kernel/threads/Thread.h"
#include "kernel/Globals.h"


class TestThread : public Thread {

private:
    TestThread(const Thread &copy); // Verhindere Kopieren

    int i = 0;

public:
    TestThread () : Thread () {  }

    void run () {
        while (1) {
            kout << i++ << " " << tid << endl;
            pcspk.delay(100);
            if (i >= (int)(tid+1) * 3) scheduler.exit();

        }
    }

 };

#endif
