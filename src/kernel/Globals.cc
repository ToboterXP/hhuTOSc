/*****************************************************************************
 *                                                                           *
 *                               G L O B A L S                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Globale Variablen des Systems.                           *
 *                                                                           *
 * Autor:           Michael Schoettner, 30.7.16                              *
 *****************************************************************************/

#include "kernel/Globals.h"


CPU                   cpu;        // CPU-spezifische Funktionen
PCSPK                 pcspk;      // PC-Lautsprecher
CGA_Stream            kout;       // Ausgabe-Strom fuer Kernel
Keyboard              kb;         // Tastatur
IntDispatcher         intdis;     // Unterbrechungsverteilung
PIC                   pic;        // Interrupt-Controller
EventQueue			  eventQueue;
uint64_t         	total_mem;  // RAM total
//BumpAllocator         allocator;
LinkedListAllocator   allocator;
Scheduler             scheduler;  // Scheduler

extern "C" void dbgPrint();
extern "C" void dbgPrintValue();

void dbgPrint() {
	kout << "Hi" << endl;
	return;
}

void dbgPrintValue(uint32_t a) {
	kout << hex << a << endl;
	return;
}
