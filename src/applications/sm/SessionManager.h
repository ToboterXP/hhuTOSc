#ifndef __include_SessionManager
#define __include_SessionManager

#include "kernel/Globals.h"

namespace applications {
namespace sm {

	class SessionManager;

	class Command {
	public:
		char * command;
		char * description;
		void (*callback)(SessionManager*);
		Command() : command(""), description(""), callback(NULL) {}
		Command(char * cmd, char * desc, void (*callback)(SessionManager*)) : command(cmd), description(desc), callback(callback) {}
	};


	class SessionManager : public Thread {
	private:
		char line_buffer[40];

		int current_length = 0;
		const int buffer_length = 40;



	public:
		List<Command> commands = List<Command>(Command());

		SessionManager();
		void registerCommand(Command command);
		void main();
		void execute(char * command);
		void runProgram(Thread * thread);
	};

}
}


#endif
