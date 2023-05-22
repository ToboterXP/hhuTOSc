/*****************************************************************************
 *                                                                           *
 *                                M A I N                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Startroutine, wird direkt vom Bootlader angesprungen,    *
 *                  nachdem dieser in den Protected Mode geschaltet hat und  *
 *                  die GDT und IDT initalisiert hat.                        *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 15.8.2016                       *
 *****************************************************************************/

#include "kernel/Globals.h"
#include "applications/sm/SessionManager.h"

#include "applications/thread_test/ThreadTest.h"

extern "C" void _init_interrupts();
//VM Passwort: root

class T : public Thread {
public:
    virtual void main() {
        while (1) {
            //kout << systime << endl;
            pcspk.delay(100);
        }
    }
};

int main() {
    kout.clear();

    kout << "Welcome to hhuTOSc" << endl;

    _init_interrupts();

    kout << "Interrupts initialized" << endl;

    kb.init();

    kout << "Keyboard initialized" << endl;

    allocator.init();

    kout << "Heap initialized" << endl;


    pit.init();
    //InitTimer();
    kout << "Timer initialized" << endl;

    cpu.force_enable_int();

    /*while (1) {
        while (!eventQueue.IsEmpty()) {
            Event e = eventQueue.PopEvent();
            if (e.type == EventType::KEY_DOWN && e.keyEvent.has_key) kout << e.keyEvent.key.ascii() << endl;
        }
        eventQueue.Wait();
    }*/

    //allocator.dump_free_memory();

    /*auto sm = new applications::sm::SessionManager();
    sm->main();*/

    /*for (int i=0; i<3; i++) {
        ThreadTest* n = new ThreadTest(i);
        scheduler.ready(n);
    }*/

    //scheduler.ready(new T());

    scheduler.ready(new applications::sm::SessionManager());

    /*scheduler.ready(new T());
    scheduler.ready(new T());*/

    scheduler.schedule();

    /*while (1) {
        kout << systime << endl;
        pcspk.delay(1000);
    }*/


    //pcspk.aerodynamic();

    while (1);
    return 0;
 }
