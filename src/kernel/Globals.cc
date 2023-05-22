/*****************************************************************************
 *                                                                           *
 *                               G L O B A L S                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Globale Variablen des Systems.                           *
 *                                                                           *
 * Autor:           Michael Schoettner, 8.7.2022                             *
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
volatile uint64_t              systime=0;  // Millisekunden seit Systemstart
PIT                   pit; // Zeitgeber

char * dbgString = "";



extern "C" void dbgPrint();
extern "C" void dbgPrint2();
extern "C" void dbgPrintValue(uint32_t a);

void dbgPrint() {
	kout << "Hi" << endl;
	//dbgString = "dbg1";
	return;
}

void dbgPrint2() {
	kout << "Hi2" << endl;
	//dbgString = "dbg1";
	return;
}


void dbgPrintValue(uint32_t a) {
	kout << hex << a << endl;
	return;
}
