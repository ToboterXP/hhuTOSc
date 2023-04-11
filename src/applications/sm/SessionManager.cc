#include "./SessionManager.h"

#include <string.h>

namespace applications {
namespace sm {
void SessionManager::main() {
	kout << "Starting Session Manager..." << endl;

	for(int i=0; i<buffer_length; i++) line_buffer[i] = '\0';
	kout << "> " << end;

	int cursorX, cursorY;

	while (1) {
		kout.getCursorPos(cursorX, cursorY);
		kout.getCharacter(cursorX, cursorY)->attribute = ~CGA::STD_ATTR;

		Key key = kb.key_hit();

		kout.getCursorPos(cursorX, cursorY);
		kout.getCharacter(cursorX, cursorY)->attribute = CGA::STD_ATTR;

		kout.clearLine(cursorY);
		kout.setCursorPos(0, cursorY);
		cursorX -= 2; // detract prompt

		if (key.ascii() == '\n') { //execute
			kout << "> " << line_buffer << endl;
			execute(line_buffer);
			for(int i=0; i<buffer_length; i++) line_buffer[i] = '\0';
			kout.getCursorPos(cursorX, cursorY);
		}

		else if (key.ascii() == '\b') { //
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
			for (int i = current_length; i > cursorX; i--) {
				line_buffer[i] = line_buffer[i-1];
			}
			line_buffer[cursorX] = key.ascii();
			current_length++;
			cursorX++;
		}

		kout << "> " << line_buffer << end;
		kout.setCursorPos(cursorX+2, cursorY);
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
		kout << "heap_test: Displays the heap test program" << endl;
		kout << "tetris: Plays the tetris theme" << endl;
	}

	else if (strcmp(command, "heap_test") == 0) {
		allocator.dump_free_memory();
		kout<<endl;

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
		kout<<endl;
	}

	else if (strcmp(command, "tetris") == 0) {
		pcspk.tetris();
	}

	else {
		kout << "Invalid Command" << endl;
	}
}

}
}
