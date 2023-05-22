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

void SessionManager::execute(char * command) {
	if (strcmp(command, "hello") == 0) {
		kout << "Hello World " << "012345678901234567890123456789" << endl;
	}

	else if (strcmp(command, "reboot") == 0) {
		kout << "Rebooting..." << endl;
		kb.reboot();
	}

	else if (strcmp(command, "clear") == 0) {
		kout.clear();
	}

	else if (strcmp(command, "help") == 0) {
		kout << "hello: Prints a debug message" << endl;
		kout << "clear: Clears the screen" << endl;
		kout << "reboot: Reboots the OS" << endl;
		kout << "heap: Displays the heap" << endl;
		kout << "thread_test: Starts three threads that all attempt to acquire the graphics lock, and print some text" << endl;
		kout << endl<<"Programs (terminate via Delete):" << endl;
		kout << "tetris: Plays the tetris theme" << endl;
		kout << "aerodynamic: Plays the aerodynamic theme" << endl;

	}

	else if (strcmp(command, "heap") == 0) {
		allocator.dump_free_memory();
		/*kout<<endl;

	    void * a = allocator.alloc(256);
	    void * b = allocator.alloc(512);

	    kout <<"Allocated size 0x100: "<<a<<endl;
		kout <<"Allocated size 0x200: "<<b<<endl;

	    allocator.dump_free_memory();
		kout<<endl;

	    allocator.free(a);

	    allocator.dump_free_memory();
		kout<<endl;

	    allocator.free(b);

	    allocator.dump_free_memory();
		kout<<endl;*/
	}

	else if (strcmp(command, "tetris") == 0) {
		runProgram(new Music(0));
	}

	else if (strcmp(command, "aerodynamic") == 0) {
		runProgram(new Music(1));
	}

	else if (strcmp(command, "thread_test") == 0) {
		kout.ReleaseGraphicsLock();

		for (int i=0; i<3; i++) {
	        ThreadTest* n = new ThreadTest(i);
	        scheduler.ready(n);
	    }
		pcspk.delay(100);
		kout.AcquireGraphicsLock();
	}

	else {
		kout << "Invalid Command" << endl;
	}
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
