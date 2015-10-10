//
//  main.cpp
//  ShellFish
//
//  Created by Morgan Redding, Hunter Wills, Bryan Lamb, Lizzie Halper, and Alex Tran on 10/10/2015.
//

#include <iostream>
#include <string>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <vector>

// copied shamelessly from http://stackoverflow.com/questions/7281894/how-do-i-chain-stdout-in-one-child-process-to-stdin-in-another-child-in-c/7282296#7282296
void set_read(int* lpipe) {
	dup2(lpipe[0], STDIN_FILENO);
	close(lpipe[0]); // we have a copy already, so close it
	close(lpipe[1]); // not using this end
}
void set_write(int* rpipe) {
	dup2(rpipe[1], STDOUT_FILENO);
	close(rpipe[0]); // not using this end
	close(rpipe[1]); // we have a copy already, so close it
}

char** args_conversion(std::vector< std::string > args) {
	if(args.size() == 1) {
		return nullptr;
	}
	char ** formatted_args = new char*[args.size()];
	for(size_t i = 1; i < args.size(); i++) {
		// need to copy the strings (so that they're not const)
		formatted_args[i - 1] = new char[args[i].size() + 1];
		for(size_t j = 0; j < args[i].size(); j++) {
			formatted_args[i - 1][j] = args[i][j];
		}
		formatted_args[i - 1][args[i].size()] = '\0';
	}
	formatted_args[args.size() - 1] = nullptr;
	return formatted_args;
}

bool is_internal_command(std::string command) {
	if(command == "clr") {
		return true;
	}
	else if(command == "set" || command == "unset") {
		return true;
	}
	else if(command == "export" || command == "unexport") {
		return true;
	}
	else if(command == "echo" || command == "show") {
		return true;
	}
	else if(command == "help") {
		return true;
	}
	else if(command == "dir" || command == "chdir") {
		return true;
	}
	else if(command == "history" || command == "repeat") {
		return true;
	}
	else if(command == "environ") {
		return true;
	}
	else if(command == "wait" || command == "pause") {
		return true;
	}
	else if(command == "kill") {
		return true;
	}
	return false;
}

void execute_command(const std::vector< std::vector<std::string> >& commands) {

	int all_my_pipes[commands.size()][2];
	for(int i = 0; i < commands.size(); i++) {
		pipe(all_my_pipes[i]);
	}
	int child_ids[commands.size()];

	int* last_pipe = nullptr;

	for(int i = 0; i < commands.size(); i++) {
		child_ids[i] = fork();
		if(child_ids[i] == 0) {
			if(i != 0) {
				set_write(last_pipe);
			}
			if(i != commands.size() - 1) {
				set_read(all_my_pipes[i]);
			}
			if(is_internal_command(commands[i][0])) {
				// run internal command
				exit(0);
			}
			else {
				execv(&commands[i][0][0], args_conversion(commands[i]));
			}
		}
		last_pipe = all_my_pipes[i];
	}

	for(int i = 0; i < commands.size(); i++) {
		waitpid(child_ids[i], NULL, 0);
	}
	std::cout << "done\n";
}

int main(int argc, const char * argv[]) {

	std::vector< std::vector< std::string > > vec(2);
	vec[0].push_back("foo");
	vec[1].push_back("foo");

	execute_command(vec);

	return 0;
}
