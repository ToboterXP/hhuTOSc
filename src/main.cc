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



int main() {
    kout.clear();

    kout << "Welcome to hhuTOSc" << endl;

    kb.init();

    kout << "Keyboard initialized" << endl;

    allocator.init();

    kout << "Heap initialized" << endl;

    auto sm = applications::sm::SessionManager();
    sm.main();

    while (1);
    return 0;
 }
