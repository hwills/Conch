//
//  main.cpp
//  ShellFish
//
//  Authors: Morgan Redding, Hunter Wills, Brian Lam, 
//	Lizzie Halper, and Alex Tran on 10/10/2015.
//

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sstream>
#include <fstream>
#include <queue>
#include "InterpretCommand.h"
#include "Documentation.h"

#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

std::string current_dir;
bool kill_child = false;
	
int fd[2];

std::string find_file(const std::string& file_name) {
	// TODO
	return "";
}

std::string execute_file(const char* file, char *const *args) {
	return "[dummy value]";
}


int
change_dir(std::string current_dir, std::string raw_path, std::string &result)
{
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
  const char * final_path= path.c_str(); 
  dir = opendir(final_path);
  if (dir) 
  {
    closedir(dir);
    result= path;
    return 0;
  }

  return 1;
}

int
list_files(std::string current_dir, std::string raw_path, std::vector<std::string> &result)
{
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

int 
add_to_history(std::string command, std::deque<std::string> &history)
{
  int max_command=3;
  int current_size= history.size();
  while (current_size >=max_command) 
  {
    history.pop_front();
    current_size--;
  }
  history.push_back(command);
  return 0;
}

int 
find_file(std::string current_dir, std::string target, std::string &result) 
{
  //Check if the target file is in current directory
  std::vector<std::string> files_in_current_dir;
  list_files(current_dir, "", files_in_current_dir);
  for (int i=0; i< files_in_current_dir.size(); i++)
  {
    if (target.compare(files_in_current_dir[i])==0)
    {
      result= current_dir+"/"+target;
      return 0;
    }
  }

  //Check if the target file is in the evn PATH
  std::string path(getenv("PATH"));
  std::istringstream iss(path);
  std::string single_path;
  while (getline(iss, single_path, ':'))
  {
    std::vector<std::string> files_in_single_path;
    list_files(single_path, "", files_in_single_path);
    for (int i=0; i< files_in_single_path.size(); i++)
    {
      if (target.compare(files_in_single_path[i])==0)
      {
        result= single_path+"/"+target;
        return 0;
      }
    }
  }

  return 1;
}


void my_exec(const char* path, char * argv[]) {
	int pid;
	if((pid = fork()) == 0) {
		execv(path, argv);
	}
	else {
		if(waitpid(pid, NULL, 0) < 0) {
			// TODO: handle error
		}
		// TODO: write actual child output to pipe (with a first character that is not 'k')
		write(fd[1], "child output", 13);
		exit(0);
	}
}

std::string execute(const std::string& command, int debug_level, std::unordered_map<std::string, std::string>& local_variables, std::unordered_map<std::string, std::string>& global_variables) {
	
	std::vector<std::string> command_parts = split_but_preserve_literal_strings(command, ' ');
	int command_size = command_parts.size();

	if(command_parts[0] == "-f") {
		std::string file_name = command.substr(3, command.size());
		// TODO: convert 'command_parts' to "char *const *" and replace "nullptr" with it
		return execute_file(&file_name[0], nullptr);
	}
	else if(command_parts[0] == "set") {
		local_variables[command_parts[1]] = command_parts[2];
		return "";
	}
	else if(command_parts[0] == "unset") {
		local_variables.erase(command_parts[1]);
		return "";
	}
	else if(command_parts[0] == "export") {
		global_variables[command_parts[1]] = command_parts[2];
		std::unordered_map<std::string, std::string>::iterator i = global_variables.find(command_parts[1]);
		setenv(&(i->first[0]), &(i->second[0]), i == global_variables.end() ? 0 : 1);
		return "";
	}
	else if(command_parts[0] == "unexport") {
		unsetenv(&command_parts[1][0]);
		global_variables.erase(command_parts[1]);
		return "";
	}
	else if(command_parts[0] == "echo") {
		return command_parts[1];
	}
	else if(command_parts[0] == "show") {
		return command_parts[1];
	}
	else if(command_parts[0] == "help") {
		if (command_size == 1) {
			return printHelp();
		} else {
			return functionHelp(command_parts[1]);
		}
                //TODO: USE MORE FILTER
	}
	else if(command_parts[0] == "exit") {
		throw 0; //TODO: make this return the value passed to exit
	}
	else if(command_parts[0] == "dir") {
            std::vector<std::string> result;
            list_files(".", "", result);
            std::string to_return = "";
            for (int i=0; i< result.size(); i++)
            {
                to_return += result[i] + " ";
            }
	    return to_return;
            //TODO: MAYBE WE DONT WANT TO SHOW THINGS THAT START WITH . like .git
	}
	else if(command_parts[0] == "history") {
		return "HISTORY IS NOT YET IMPLEMENTED";
                //TODO: FIX ALEX's CODE TO USE A FILE
	}
	else if(command_parts[0] == "clr") {
            std::cout << "\033[2J\033[1;1H";
	    return "";
            //TODO: GET RID OF EXTRA LINE
	}
	else if(command_parts[0] == "enviorn") {
		return "ENVIORN NOT IMPLEMENTED YET";
                //TODO: FINISH THIS
	}
	else if(command_parts[0] == "chdir") {
		return "CHDIR IS NOT IMPLEMENTED YET";
                //TODO: FINISH THIS (ALEX's CODE MAY WORK?)
	}
	else if(command_parts[0] == "wait") {
		return "WAIT NOT IMPLEMENTED YET";
                //TODO: FINISH THIS
	}
	else if(command_parts[0] == "pause") {
		return "PAUSE IS NOT IMPLEMENTED YET";
                //TODO: FINISH THIS
	}
	else if(command_parts[0] == "repeat") {
		return "REPEAT IS NOT IMPLEMENTED YET";
                //TODO: FINISH THIS (ALEX's CODE NEEDS TO USE A FILE)
	}
	else if(command_parts[0] == "kill") {
		return "KILL IS NOT IMPLEMENTED YET";
                //TODO: FINISH THIS
	}
	else if(command_parts[0] == "") {
		return "";
	}
	else { //check if external command
            //USE PATH VARIABLE TO MAKE FIND COMMANDS
            /*std::vector<std::string> paths;
            for (int i = 0; i < paths.length(); ++i) {
                if(!access((paths[i]+"/"+command_parts[0]).c_str(), X_OK)) {
                    exec_external(paths[i]+"/"+command_parts[0]);
                    return "OUTPUT OF EXTRNAL COMMANDS";//TODO: get this
                }
            }*/
            if(!access((current_dir+"/"+command_parts[0]).c_str(), X_OK)) {
                //exec_external(current_dir+"/"+command_parts[0]);
                return "OUTPUT OF EXTERNAL COMMAND"; //TODO: get this
            }
	    else {
		return "UNKNOWN COMMAND: " + command_parts[0];
	    }
	}
}

std::string exec_external(std::string path_to_command) {
/*    if (debug_level >= 1) {
            std::cout << "DEBUG: Beginning execution of <" << (current_dir+"/"+command_parts[0]).c_str() << ">" << std::endl;
    }
    //run external command as child process (fork and execv)
    char read_buffer[80];
    pipe(fd);
    int pid = fork();
    if(pid == 0) {
        close(fd[0]); // child closes input side of pipe
        char * argv[command_parts.size() - 1];
        for(int i = 1; i < command_parts.size(); i++) {
                //TODO: get command line args to work
                //argv[i-1] = command_parts[i].c_str();
        }
        my_exec((path_to_command).c_str(), argv);
    }
    else {
        close(fd[1]);
        read(fd[0], read_buffer, sizeof(read_buffer));
        if(read_buffer[0] == 'k') {
            std::cout << "DEBUG: Trying to kill my child" << std::endl;
            kill(pid, SIGINT);
        }
        else {
            std::cout << "BUFFER:" << read_buffer << std::endl;
        }
        if (debug_level >= 1) {
            std::cout << "DEBUG: Waiting for process <" << pid << "> to complete" << std::endl;
        }
        //wait for command completion (pass signals as necessecary)
        while(true) {
            // TODO: watch for a signals to send to child
        }
    }*/
    return "";
}

void ctrlC_handler(int sig) {
	write(fd[1], "kill_me", 7);
	kill_child = true;
}

int main(int argc, const char * argv[]) {

	// blocks user from quitting program (MUHAHAHAHA)
	sigset_t newsigset;
	sigemptyset(&newsigset);
	sigaddset(&newsigset, SIGTSTP); // ctrl-Z
	// sigaddset(&newsigset, SIGINT);  // ctrl-C
	if(sigprocmask(SIG_BLOCK, &newsigset, NULL) < 0) {
		perror("could not block the signal");
	}
	signal(SIGINT, ctrlC_handler);

	std::unordered_map<std::string, std::string> local_variables;
	std::unordered_map<std::string, std::string> global_variables;
	
	// these are dummy variables
	// TODO: remove from final product
	local_variables["local1"] = "a";
	global_variables["global1"] = "b";
	local_variables["var"] = "a";
	global_variables["var"] = "b";
	
	//set the current path
	char buf[256];
	current_dir = getcwd(buf, 256);

        //TODO: READ IN HISTORY FROM HISTORY FILE
        //TODO: SET SHELL ENVIORN VARIABLE
        //TODO: SET PARENT ENVIORN VARIABLE
        //TODO: BACKGROUND COMMANDS
        //TODO: SET SPECIAL VARIABLES
        //TODO: STDIN STDOUT REDIRECTION
        //TODO: FINISH PIPES AND EXTERNAL COMMANDS
        //TODO: TERMINAL GENERATED SIGNALS
        //TODO: DEBUG MODE, EXECUTE FILE MODE, -X mode


	while(true) {
		
		int debug_level;
		std::string command;
		
		std::cout << "sish >> ";
		std::getline(std::cin, command);
		
		command = remove_excess_spaces(command);
		
		std::string command_after_var_substitution = substitute_variable_values(command, local_variables, global_variables);
		
		// take care of '-x' flag
		if(command_after_var_substitution.substr(0, 2) == "-x") {
			command_after_var_substitution = command_after_var_substitution.substr(3, command_after_var_substitution.size());
			std::cout << command_after_var_substitution << std::endl;
		}
		
		// take care of '-d' flag (debug level)
		if(command_after_var_substitution.substr(0, 2) == "-d") {
			int i = 3;
			while(i < command_after_var_substitution.size() && is_numeric(command_after_var_substitution[i])) {
				i++;
			}
			try {
				debug_level = std::stoi(command_after_var_substitution.substr(3, i - 3));
			}
			catch(const std::exception& e) {
				std::cout << "Error in reading value for DEBUG_LEVEL" << std::endl;
				continue;
			}
			command_after_var_substitution = command_after_var_substitution.substr(i + 1, command_after_var_substitution.size());
		}
		else {
			debug_level = 0;
		}

		
		std::vector<std::string> individual_commands = split_string(command_after_var_substitution, '|');
		
		std::string output = "";
		for(int i = 0; i < individual_commands.size(); i++) {
			try {
				output = execute(individual_commands[i] + " " + output, debug_level, local_variables, global_variables);
			} 
			catch(int i) { //TODO: replace this with the appropriate exception handling
				std::cout << "Goodbye." << "\n";
				return i;
			}
		}
		std::cout << output << "\n";
	}

	return 0;
}
