//
//  main.cpp
//  ShellFish
//
//  Created by Morgan Redding, Hunter Wills, Bryan Lamb, Lizzie Halper, and Alex Tran on 10/10/2015.
//

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <vector>
#include <unordered_map>
#include "Documentation.h"
#include "Logger.h"

void execute_command(const std::vector< std::vector<std::string> >& commands);
std::vector<std::string> split_but_preserve_literal_strings(const std::string& txt, const char symbol_to_split_by);

std::string current_dir;
extern char ** environ;

std::unordered_map<std::string, std::string> local_variables;


bool is_legal_variable_character(char c) {
	if(c < 47) {
		return false;
	}
	if(c < 58) {
		return true;
	}
	if(c < 65) {
		return false;
	}
	if(c < 91) {
		return true;
	}
	if(c == 95) {
		return true;
	}
	if(c < 97) {
		return false;
	}
	if(c < 123) {
		return true;
	}
	return false;
}


std::string substitute_variable_values(const std::string& txt) {
	std::string rtn = "";
	for(int i = 0; i < txt.size(); i++) {
		if(txt[i] == '#') {
			break;
		}
		if(txt[i] == '$') {
			i++;
			int start = i;
			while(i < txt.size() && !(txt[i] == ' ')) {//&& is_legal_variable_character(txt[i])) { TODO: DICUSS THIS LINE WITH MORGAN
				i++;
			}
			auto it = local_variables.find(txt.substr(start, i - start));
			if(it == local_variables.end()) {
				char * var_val = getenv(&txt.substr(start, i - start)[0]);
				if(var_val == NULL) {
					i = start;
					rtn += '$';
				}
				else {
					rtn += var_val;
				}
			}
			else {
				rtn += it->second;
			}
			i--;
		}
		else {
			rtn += txt[i];
		}
	}
	return rtn;
}


// copied shamelessly from http://stackoverflow.com/questions/7281894/how-do-i-chain-stdout-in-one-child-process-to-stdin-in-another-child-in-c/7282296#7282296
void set_read(int* lpipe) {
	dup2(lpipe[0], STDIN_FILENO);
	close(lpipe[0]); // we have a copy already, so close it
	close(lpipe[1]); // not using this end
}

int list_files(std::string current_dir, std::string raw_path, std::vector<std::string> &result) {
  /* Process the given path
   * This can be empty, sub directory, or full path 
   */

	std::string path;
	if(raw_path.size()==0)  {
		//empty
		path= current_dir;
	}
	else if(raw_path[0] != '/')  {
		// sub directory
		path= current_dir+"/"+raw_path;
	}
	else 
	{
		//full path
		path= raw_path;
	}

	DIR *dir;
	struct dirent *ent;
	const char * final_path = path.c_str(); 
	dir = opendir(final_path);
	if(dir)  {
		while ((ent = readdir(dir)) != NULL)  {
			result.push_back(ent->d_name);
			// cout << ent->d_name << endl;
		}
		closedir(dir);
		return 0;
	}

	return 1;
}


void set_write(int* rpipe) {
	dup2(rpipe[1], STDOUT_FILENO);
	close(rpipe[0]); // not using this end
	close(rpipe[1]); // we have a copy already, so close it
}

void run_internal(const std::vector<std::string> &args) {
	// TODO: SPIT OUT ERRORS
	// TODO: make it so they can either accept an additinal argument OR input
	// 		e.g. so I can do either "set foo bar" or "set foo\nbar"
	if(args[0] == "set") {
		if(args.size() > 2) {
			if(args[1] != "$" && args[1] != "?" && args[1] != "!") {
				local_variables[args[1]] = args[2];
			}
		}
		else if(args.size() == 1) {
			std::string value;
			std::getline(std::cin, value);
			local_variables[args[1]] = value;

		}
	}
	else if(args[0] == "unset") {
		if(args.size() > 1) {
			if(args[1] != "$" && args[1] != "?" && args[1] != "!") {
			local_variables.erase(args[1]);
			}
		}
	}
	else if(args[0] == "export") {
		if(args.size() > 2) {
			setenv(&args[1][0], &args[2][0], getenv(&args[1][0]) == NULL ? 0 : 1);
		}
	}
	else if(args[0] == "unexport") {
		if(args[1] != "$" && args[1] != "?" && args[1] != "!") {
			unsetenv(&args[1][0]);
		}
	}
	else if(args[0] == "echo") {
		if(args.size() > 1) {
			std::cout << args[1] << std::endl;
		}
	}
	else if(args[0] == "show") {
		if(args.size() > 1) {
			std::cout << args[1] << std::endl;
		}
	}
	else if(args[0] == "help") {
		if(args.size() == 1) {
			printHelp();
		}
		else if(args.size() > 1) {
			functionHelp(args[1]);
		}
		//TODO: USE MORE FILTER and MAKE MANS
	}
	else if(args[0] == "dir") {
		std::vector<std::string> result;
		list_files(current_dir, "", result);
		for(unsigned int i=0; i< result.size(); i++) {
			if(result[i][0] != '.') {
				std::cout << result[i] + " ";
			}
		}
		std::cout << std::endl;
	}
	else if(args[0] == "history") {
		int n = std::stoi(args[1]);
		commandHistory(n);
	}
	else if(args[0] == "clr") {
		std::cout << "\033[2J\033[1;1H";
	}
	else if(args[0] == "environ") {
		char** env;
		for(env = environ; *env != 0; env++) {
			char* thisEnv = *env;
			std::cout << thisEnv << std::endl;	
		}
	}
	else if(args[0] == "chdir") {
		if(args.size() == 2) {
			if(!access((current_dir + "/" + args[1]).c_str(), F_OK)) {
				std::vector<std::string> components = split_but_preserve_literal_strings(args[1], '/');
				for(int i = 0; i < components.size(); i++) {
					if(components[i] == "..") {
						int found_index = current_dir.find_last_of('/');
						current_dir = current_dir.substr(0, found_index);
					}
					else if(components[i] != ".") {
						current_dir += "/" + components[i];
					}
				}
				std::cout << current_dir << std::endl;
			}
			else {
				std::cout << "This is not a dir you have access to." << std::endl;
			}
		}
	}
	else if(args[0] == "wait") {
		std::cout << "WAIT NOT IMPLEMENTED YET" << std::endl;
			//TODO: FINISH THIS
	}
	else if(args[0] == "pause") {
		std::cin.ignore();
	}
	else if(args[0] == "repeat") {
		std::string lastCommand = repeatCommand();
		std::vector<std::string> _args;
		_args.push_back(lastCommand);
		// execute_command(_args);
	}
	else if(args[0] == "kill") {
		std::cout << "KILL IS NOT IMPLEMENTED YET" << std:: endl;
		//TODO: FINISH THIS
	}

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
	else if(command == "exit") {
		return true;
	}
	return false;
}

void execute_command(const std::vector< std::vector<std::string> >& commands) {

	if(commands.size() == 1 && is_internal_command(commands[0][0])) {
		if(commands[0][0] == "exit") {
			if(commands[0].size() == 1) {
					exit(0);
			}
			else if(commands[0].size() > 1) {
				try {
					exit(stoi(commands[0][1]));
				}
				catch (...) {//if they attempted to exit with a non integer value
					exit(-1);
				}
			}
		}
		run_internal(commands[0]);// run internal command
		return;
	}
				
	//make sure we have access to run all of the commands
	for(int i = 0; i < commands.size(); ++i) {
		if(is_internal_command(commands[i][0])) {
			continue;
		}
		else if(!access((current_dir + "/" + commands[i][0]).c_str(), X_OK)) {
			continue;
		}
		else {
			std::string path_var = "PATH";
			std::string path = getenv(&path_var[0]);
			std::vector<std::string> path_components = split_but_preserve_literal_strings(path, ':');
			bool success = false;
			for(int j = 0; j < path_components.size(); ++j) {
				if(!access((path_components[j] + "/" + commands[i][0]).c_str(), X_OK)) {
					success = true;
					break;
				}
			}
			if(!success) {
				std::cout << "COMMAND NOT FOUND: " << commands[i][0] << std::endl;
				return;
			}
		}
	}


	int all_my_pipes[commands.size()][2];
	for(unsigned int i = 0; i < commands.size(); i++) {
		pipe(all_my_pipes[i]);
	}
	int child_ids[commands.size()];

	int* last_pipe = nullptr;

	for(unsigned int i = 0; i < commands.size(); i++) {
		child_ids[i] = fork();
		if(child_ids[i] == 0) {
				std::string shell = "parent";
				std::string shellpath = current_dir + "/sish";
				setenv(&shell[0], &shellpath[0], getenv(&shell[0]) == NULL ? 0 : 1);
			if(i != 0) {
				set_write(last_pipe);
			}
			if(i != commands.size() - 1) {
				set_read(all_my_pipes[i]);
			}
			if(is_internal_command(commands[i][0])) {
				run_internal(commands[i]);// run internal command
				exit(0);
			}
			else {
				std::string command = commands[i][0];
				if(!access((current_dir + "/" + commands[i][0]).c_str(), X_OK)) {
					command = current_dir + "/" + commands[i][0];
				}
				else {
					std::string path_var = "PATH";
					std::string path = getenv(&path_var[0]);
					std::vector<std::string> path_components = split_but_preserve_literal_strings(path, ':');
					for(int j = 0; j < path_components.size(); ++j) {
						if(!access((path_components[j] + "/" + commands[i][0]).c_str(), X_OK)) {
							command = path_components[j] + "/" + commands[i][0];
							break;
						}
					}
				}
				execv(&command[0], args_conversion(commands[i]));
			}
		}
		last_pipe = all_my_pipes[i];
	}

	for(unsigned int i = 0; i < commands.size(); i++) {
		waitpid(child_ids[i], NULL, 0);
	}
}

// splits a string by a character, ignoring characters between quotes
std::vector<std::string> split_but_preserve_literal_strings(const std::string& txt, const char symbol_to_split_by) {

	std::vector<std::string> rtn;
	std::string t = "";

	for(unsigned int i = 0; i < txt.size(); i++) {
		if(txt[i] == '\\') {
			t += txt[i];
			i++;
			t += txt[i];
			continue;
		}
		if(txt[i] == '\'') {
			i++;
			while(i < txt.size() && txt[i] != '\'') {
				if(txt[i] == '\\') {
					t += txt[i];
					i++;
				}
				t += txt[i];
				i++;
			}
		}
		else if(txt[i] == '"') {
			i++;
			while(i < txt.size() && txt[i] != '"') {
				if(txt[i] == '\\') {
					t += txt[i];
					i++;
				}
				t += txt[i];
				i++;
			}
		}
		else if(txt[i] == symbol_to_split_by) {
			if(t != "") {
				rtn.push_back(t);
				t = "";
			}
		}
		else {
			t += txt[i];
		}
	}
	
	rtn.push_back(t);
	return rtn;
}

void ctrlC_handler(int sig) {
	// TODO
}

int main(int argc, const char * argv[]) {

	// blocks user from quitting program
	sigset_t newsigset;
	sigemptyset(&newsigset);
	sigaddset(&newsigset, SIGTSTP); // block ctrl-Z
	// sigaddset(&newsigset, SIGINT);  // block ctrl-C
	if(sigprocmask(SIG_BLOCK, &newsigset, NULL) < 0) {
		perror("could not block the signal");
	}
	signal(SIGINT, ctrlC_handler); // redirect ctrl-C

	bool x_flag = false;
	int debug_level = 0;

	if(argc > 1) {
		if(argv[1][0] == '-') {
			if(argv[1][1] == 'x') {
				x_flag = true;
			}
			else if(argv[1][1] == 'd') {
				debug_level = 1;
				if(argc > 2) {
					try {
						debug_level = atoi(argv[2]);
					}
					catch(int e) {
						std::cout << "debug level not an integer" << std::endl;
						return 0;
					}
				}
			}
		}
		else {
			std::cout << "argument error" << std::endl;
			return 0;
		}
	}
	if(argc > 2) {
		if(argv[2][0] == '-' && argv[2][1] == 'd') {
			debug_level = 1;
			if(argc > 3) {
				try {
					debug_level = atoi(argv[3]);
				}
				catch(int e) {
					std::cout << "debug level not an integer" << std::endl;
					return 0;
				}
			}
		}
	}

	char buf[256];
	current_dir = getcwd(buf, 256);
	std::string shell = "shell";
	std::string shellpath = current_dir + "/sish";
	setenv(&shell[0], &shellpath[0], getenv(&shell[0]) == NULL ? 0 : 1);
	//TODO: BACKGROUND COMMANDS
	local_variables["$"] = std::to_string(getpid());
	local_variables["?"] = "\"no foreground commands run yet\"";
	local_variables["!"] = "\"no background commands run yet\"";
	//TODO: SET SPECIAL VARIABLES
	//TODO: STDIN STDOUT REDIRECTION
	//TODO: TERMINAL GENERATED SIGNALS
	//TODO: DEBUG MODE, EXECUTE FILE MODE, -X mode
	//TODO: MAN PAGE FOR OUR SHELL

	while(true) {
	
		std::string command;
		
		std::cout << "sish >> ";
		std::getline(std::cin, command);
		
		std::string command_after_var_substitution = substitute_variable_values(command);
		logCommand(command);
		
		std::vector<std::string> individual_commands = split_but_preserve_literal_strings(command_after_var_substitution, '|');
		
		std::vector<std::vector<std::string> > args(individual_commands.size());

		for(unsigned int i = 0; i < individual_commands.size(); i++) {
			args[i] = split_but_preserve_literal_strings(individual_commands[i], ' ');
		}
		execute_command(args);
	}

	return 0;
}
