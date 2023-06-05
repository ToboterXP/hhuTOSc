#include "./SessionManager.h"

#include "applications/music.h"
#include "applications/thread_test/ThreadTest.h"

#include <string.h>

namespace applications {
namespace sm {

bool a = false;

void SessionManager::main() {
	kout.AcquireGraphicsLock();

	kout << "Starting Session Manager..." << endl;

	for(int i=0; i<buffer_length; i++) line_buffer[i] = '\0';
	kout << "> " << end;

	//kout.t_color = RGB_24(128, 128, 128);

	int cursorX, cursorY;

	kout.getCursorPos(cursorX, cursorY);
	//kout.getCharacter(cursorX, cursorY)->attribute = kout.attribute(CGA_Color::LIGHT_GREY, CGA_Color::BLACK, true);
	kout.show(cursorX, cursorY, '_', ~CGA::STD_ATTR);

	while (1) {
		dbgString = "1";
		Event event = eventQueue.PopEvent();

		if (event.type == EventType::KEY_DOWN && event.keyEvent.has_key) {
			dbgString = "2";
			kout.getCursorPos(cursorX, cursorY);
			//kout.getCharacter(cursorX, cursorY)->attribute = CGA::STD_ATTR
			//cpu.disable_int();
			kout.clearLine(cursorY);
			//cpu.enable_int();
			kout.setCursorPos(0, cursorY);
			cursorX -= 2; // detract prompt

			/*char cursorChar = ' ';
			if (cursorX < current_length) {
				cursorChar = line_buffer[cursorX];
			}
			kout.show(cursorX+2, cursorY, cursorChar, CGA::STD_ATTR);*/


			Key key = event.keyEvent.key;
			dbgString = "3";
			if (key.ascii() == '\n') { //execute
				kout << "> " << line_buffer << endl;
				execute(line_buffer);
				current_length = 0;
				for(int i=0; i<buffer_length; i++) line_buffer[i] = '\0';
				kout.getCursorPos(cursorX, cursorY);
			}

			else if (key.ascii() == '\b') { //#
				dbgString = "4";
				if (cursorX > 0 && current_length > 0) {
					for (int i = cursorX-1; i < buffer_length-1; i++) {
						line_buffer[i] = line_buffer[i+1];
					}

					line_buffer[buffer_length-1] = '\0';
					current_length--;
					cursorX--;
				}
			}

			else if (key.scancode() == Key::scan::left && cursorX > 0) {
				cursorX--;
			}

			else if (key.scancode() == Key::scan::right && cursorX < current_length) {
				cursorX++;
			}

			else if (key.ascii() && current_length < buffer_length-1) {
				dbgString = "5";
				for (int i = current_length; i > cursorX; i--) {
					line_buffer[i] = line_buffer[i-1];
				}
				line_buffer[cursorX] = key.ascii();
				current_length++;
				cursorX++;
			}

			dbgString = "6";
			kout << "> " << line_buffer << end;
			kout.setCursorPos(cursorX+2, cursorY);
			kout.getCursorPos(cursorX, cursorY);

			kout.show(cursorX, cursorY, '_', CGA::STD_ATTR);
			//kout.getCharacter(cursorX, cursorY)->attribute = ~CGA::STD_ATTR;
			dbgString = "7";
		}

		eventQueue.Wait();
	}
}

void c_hello(SessionManager * sm) {
	kout << "Hello World " << "012345678901234567890123456789" << endl;
}

void c_reboot(SessionManager * sm) {
	kout << "Rebooting..." << endl;
	kb.reboot();
}

void c_clear(SessionManager * sm) {
	kout.clear();
}

void c_help(SessionManager * sm) {
	auto current = sm->commands.get_first();
	while (current) {
		kout << current->data.command <<": "<<current->data.description<<endl;
		current = current->GetNext();
	}
}

/*void c_panic(SessionManager * sm) {
	((void (*)()) 0x1000000000)();
}*/

void c_heap(SessionManager * sm) {
	allocator.dump_free_memory();
}

void c_tetris(SessionManager * sm) {
	sm->runProgram(new Music(0));
}

void c_aerodynamic(SessionManager * sm) {
	sm->runProgram(new Music(1));
}

void c_thread_test(SessionManager * sm) {
	kout.ReleaseGraphicsLock();

	for (int i=0; i<3; i++) {
		ThreadTest* n = new ThreadTest(i);
		scheduler.ready(n);
	}
	pcspk.delay(100);
	kout.AcquireGraphicsLock();
}

void c_systime(SessionManager * sm) {
	int seconds = (systime /1000) % 60;
	int minutes = (systime /60000) % 60;
	int hours = (systime / 3600000);

	kout << dec << "Time since system start ("<<systime<<"ms): "<<hours<<" Hours, "<<minutes << " Minutes, "<<seconds<<" Seconds"<<endl;
}

SessionManager::SessionManager() {
	registerCommand(Command("hello", "Prints a debug message", &c_hello));
	registerCommand(Command("help", "Prints all commands", &c_help));
	registerCommand(Command("reboot", "Reboots the PC", &c_reboot));
	registerCommand(Command("clear", "Clears the screen", &c_clear));
	registerCommand(Command("heap", "Displays the heap's state", &c_heap));
	registerCommand(Command("systime", "Displays time since system start", &c_systime));
	registerCommand(Command("thread_test", "Start three threads that acquire the graphics lock, and print some text", &c_thread_test));
	//registerCommand(Command("panic", "Cause a GPF", &c_panic));
	registerCommand(Command("tetris", "Plays the tetris theme (Terminate via Delete)", &c_tetris));
	registerCommand(Command("aerodynamic", "Plays the aerodynamic theme (Terminate via Delete)", &c_aerodynamic));
}

void SessionManager::registerCommand(Command command) {
	commands.append(command);
}

void SessionManager::execute(char * command) {
	auto current = commands.get_first();
	while (current) {
		if (strcmp(current->data.command, command) == 0) {
			current->data.callback(this);
			return;
		}
		current = current->GetNext();
	}

	kout << "Invalid command (Enter help for a list of commands)" << endl;
}

void SessionManager::runProgram(Thread* thread) {
	dbgString = "A";
	kout.ReleaseGraphicsLock();

	scheduler.ready(thread);
	dbgString = "B";

	scheduler.yield();

	//while(1);

	while (!thread->hasTerminated()) {
		Key key = kb.get_last_key();
		if (key.scancode() == Key::scan::del) {
			scheduler.kill(thread);
			break;
		}
	}

	pcspk.off();
	eventQueue.Flush();

	cpu.force_enable_int();

	kout.AcquireGraphicsLock();
}

}
}
