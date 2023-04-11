#ifndef __include_SessionManager
#define __include_SessionManager

#include "kernel/Globals.h"

namespace applications {
namespace sm {


	class SessionManager {
	private:
		char line_buffer[40];

		int current_length = 0;
		const int buffer_length = 40;

	public:
		void main();
		void execute(char * command);
	};

}
}


#endif
