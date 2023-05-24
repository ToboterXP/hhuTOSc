/*****************************************************************************
 *                                                                           *
 *                               G L O B A L S                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Globale Variablen des Systems.                           *
 *                                                                           *
 * Autor:           Michael Schoettner, 30.7.16                              *
 *****************************************************************************/
#ifndef __Globals_include__
#define __Globals_include__

#include "devices/PIT.h"
#include "kernel/CPU.h"
#include "kernel/interrupts/PIC.h"
#include "kernel/allocator/BumpAllocator.h"
#include "kernel/allocator/LinkedListAllocator.h"
#include "devices/PCSPK.h"
#include "devices/VGA_Stream.h"
#include "devices/Keyboard.h"
#include "kernel/interrupts/IntDispatcher.h"
#include "kernel/threads/Scheduler.h"

#include "kernel/events/EventQueue.h"

extern CPU                   cpu;        // CPU-spezifische Funktionen
extern PCSPK                 pcspk;      // PC-Lautsprecher
extern VGA_Stream            kout;       // Ausgabe-Strom fuer Kernel
extern Keyboard              kb;         // Tastatur
extern IntDispatcher         intdis;     // Unterbrechungsverteilung
extern PIC                   pic;        // Interrupt-Controller
extern EventQueue            eventQueue; //EventQueue
extern uint64_t          total_mem;  // RAM total
//extern BumpAllocator         allocator;
extern LinkedListAllocator   allocator;
extern Scheduler        scheduler;  // Scheduler

extern volatile uint64_t              systime;    // wird all 10ms hochgezaehlt
extern PIT                   pit;        // Zeitgeber

extern char * dbgString;

#endif
