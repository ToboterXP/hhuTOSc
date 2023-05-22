#ifndef __tetris__
#define __tetris__

#include "kernel/threads/Thread.h"
#include "kernel/Globals.h"


class Music : public Thread {
public:
	int music;
	Music(int type) : music(type) {}
	void main() {
		dbgString = "C";
		switch (music) {
		case 0:
			pcspk.tetris();
			break;
		case 1:
			pcspk.aerodynamic();
			break;
		}
	}
};

#endif
