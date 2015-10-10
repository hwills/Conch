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
#include "Documentation.h"

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
  if (raw_path.size()==0) 
  {
    //empty
    path= current_dir;
  }
  else if (raw_path[0] != '/') 
  {
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
  const char * final_path= path.c_str(); 
  dir = opendir(final_path);
  if (dir) 
  {
    while ((ent = readdir(dir)) != NULL) 
    {
      result.push_back(ent->d_name);
      /* cout << ent->d_name << endl; */ 
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
	if(args[0] == "set") {
            //TODO: WRITE THIS COMMAND
	    //local_variables[command_parts[1]] = command_parts[2];
	}
	else if(args[0] == "unset") {
            //TODO: WRITE THIS COMMAND
	    //local_variables.erase(command_parts[1]);
	}
	else if(args[0] == "export") {
            //TODO: WRITE THIS COMMAND
	    //global_variables[command_parts[1]] = command_parts[2];
	    //std::unordered_map<std::string, std::string>::iterator i = global_variables.find(command_parts[1]);
	    //setenv(&(i->first[0]), &(i->second[0]), i == global_variables.end() ? 0 : 1);
	}
	else if(args[0] == "unexport") {
            //TODO: WRITE THIS COMMAND
	    //unsetenv(&command_parts[1][0]);
	    //global_variables.erase(command_parts[1]);
	}
	else if(args[0] == "echo") {
	    if (args.size() > 1) {
                std::cout << args[1] << std::endl;
            }
	}
	else if(args[0] == "show") {
	    if (args.size() > 1) {
                std::cout << args[1] << std::endl;
            }
	}
	else if(args[0] == "help") {
	    if (args.size() == 1) {
                printHelp();
	    }
            else if (args.size() > 1) {
                functionHelp(args[1]);
	    }
            //TODO: USE MORE FILTER
	}
	else if(args[0] == "exit") {
	    if (args.size() == 1) {
                exit(0);
	    }
            else if (args.size() > 1) {
                exit(stoi(args[1]));//TODO: TRY
	    }
	}
	else if(args[0] == "dir") {
            std::vector<std::string> result;
            list_files(".", "", result);
            for (int i=0; i< result.size(); i++)
            {
                std::cout << result[i] + " ";
            }
            std::cout << std::endl;
            //TODO: MAYBE WE DONT WANT TO SHOW THINGS THAT START WITH . like .git
	}
	else if(args[0] == "history") {
	    std::cout << "HISTORY IS NOT YET IMPLEMENTED" << std::endl;
            //TODO: FIX ALEX's CODE TO USE A FILE
	}
	else if(args[0] == "clr") {
            std::cout << "\033[2J\033[1;1H";
            //TODO: GET RID OF EXTRA LINE
	}
	else if(args[0] == "enviorn") {
	    std::cout << "ENVIORN NOT IMPLEMENTED YET" << std::endl;
            //TODO: FINISH THIS
	}
	else if(args[0] == "chdir") {
	    std::cout << "CHDIR IS NOT IMPLEMENTED YET" << std::endl;
            //TODO: FINISH THIS (ALEX's CODE MAY WORK?)
	}
	else if(args[0] == "wait") {
	    std::cout << "WAIT NOT IMPLEMENTED YET" << std::endl;
            //TODO: FINISH THIS
	}
	else if(args[0] == "pause") {
	    std::cout << "PAUSE IS NOT IMPLEMENTED YET" << std::endl;
            //TODO: FINISH THIS
        }
	else if(args[0] == "repeat") {
	    std::cout << "REPEAT IS NOT IMPLEMENTED YET" << std::endl;
            //TODO: FINISH THIS (ALEX's CODE NEEDS TO USE A FILE)
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
				run_internal(commands[i]);// run internal command
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

// splits a string by a character, ignoring characters between quotes
std::vector<std::string> split_but_preserve_literal_strings(const std::string& txt, const char symbol_to_split_by) {

	std::vector<std::string> rtn;
	std::string t = "";

	for(int i = 0; i < txt.size(); i++) {
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

int main(int argc, const char * argv[]) {

	// int debug_level;
	// // take care of '-x' flag
	// 	if(command_after_var_substitution.substr(0, 2) == "-x") {
	// 		command_after_var_substitution = command_after_var_substitution.substr(3, command_after_var_substitution.size());
	// 		std::cout << command_after_var_substitution << std::endl;
	// 	}
		
	// 	// take care of '-d' flag (debug level)
	// 	if(command_after_var_substitution.substr(0, 2) == "-d") {
	// 		int i = 3;
	// 		while(i < command_after_var_substitution.size() && is_numeric(command_after_var_substitution[i])) {
	// 			i++;
	// 		}
	// 		try {
	// 			debug_level = std::stoi(command_after_var_substitution.substr(3, i - 3));
	// 		}
	// 		catch(const std::exception& e) {
	// 			std::cout << "Error in reading value for DEBUG_LEVEL" << std::endl;
	// 			continue;
	// 		}
	// 		command_after_var_substitution = command_after_var_substitution.substr(i + 1, command_after_var_substitution.size());
	// 	}
	// 	else {
	// 		debug_level = 0;
	// 	}
	while(true) {
		
		std::string command;
		
		std::cout << "sish >> ";
		std::getline(std::cin, command);
		
		std::string command_after_var_substitution = command;//substitute_variable_values(command, local_variables, global_variables);
		
		std::vector<std::string> individual_commands = split_but_preserve_literal_strings(command_after_var_substitution, '|');
		
		std::vector<std::vector<std::string> > args(individual_commands.size());

		for(int i = 0; i < individual_commands.size(); i++) {
			args[i] = split_but_preserve_literal_strings(individual_commands[i], ' ');
		}
		execute_command(args);
	}

	return 0;
}
