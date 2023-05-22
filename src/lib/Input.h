/*****************************************************************************
 *                                                                           *
<<<<<<<< HEAD:src/lib/SpinLock.h
 *                              S P I N L O C K                              *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Implementierung eines Spinlocks mithilfe der cmpxchg     *
 *					Instruktion. 											 *
 *                                                                           *
 * Autor:           Michael Schoettner, 2.2.2018                             *
========
 *                                I N P U T                                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Hilfsfunktion zum Warten bis auf der Tastatur die Ein-   *
 *                  -gabetaste gedrückt wird.                                *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 2.05.2022                       *
>>>>>>>> aufgabe-5:src/lib/Input.h
 *****************************************************************************/
#ifndef __Input_include__
#define __Input_include__


char getch();
void waitForReturn();

#endif
