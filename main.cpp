//
//  main.cpp
//  ShellFish
//
//  Created by Morgan Redding, Hunter Wills, Bryan Lamb, Lizzie Halper, and Alex Tran on 10/10/2015.
//

#include <iostream>
#include <string>
#include <stdlib.h>
#include "InterpretCommand.h"

#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

std::string current_dir;

std::string find_file(const std::string& file_name) {
    // TODO
    return "";
}

std::string execute_file(const char* file_name, char *const *arguments) {
    
    /*
    int pid;
    if((pid = fork()) == 0) {
        // child should execute file and listen for interupt command
        int err = execvp(file_name, arguments);
        exit(err);
    }
    else {
        int status;
        int pid2 = waitpid(pid, &status, 0); // wait for child process
    }
    */
    
    return "\"[insert output from file " + std::string(file_name)  + " here]\"";
}

std::string execute(const std::string& command, int debug_level, std::unordered_map<std::string, std::string>& local_variables, std::unordered_map<std::string, std::string>& global_variables) {
    
    std::vector<std::string> command_parts = split_but_preserve_literal_strings(command, ' ');

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
        setenv(&(i->first[0]), &(i->second[0]), 1);
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
    else if(command_parts[0] == "exit") {
        throw 0; //TODO: replace this with an appropriate thrown value for good exiting
    }
    else if(!access((current_dir+"/"+command_parts[0]).c_str(), X_OK)) { //check if external command
        if (debug_level >= 1) {
            std::cout << "DEBUG: Beginning execution of <" << (current_dir+"/"+command_parts[0]).c_str() << ">" << std::endl;
        }
        //run external command as child process (fork and execv)
        int pid = fork();
        if(pid == 0) {
            char * argv[command_parts.size() - 1];
            for(int i = 1; i < command_parts.size(); i++) {
                //TODO: get command line args to work
                //argv[i-1] = command_parts[i].c_str();
            }
            execv((current_dir+"/"+command_parts[0]).c_str(), argv);
        }
        else {
            if (debug_level >= 1) {
                std::cout << "DEBUG: Waiting for process <" << pid << "> to complete" << std::endl;
            }
            //wait for command completion (pass signals as necessecary)
            while(0 == kill(pid, 0)) {
                //TODO: watch for a signals to send to child
            }
        }
        return "";
    }
    else {
        return "UNKNOWN COMMAND: " + command_parts[0];
    }
}

void sig_terminate_handle(int sig) {
    std::cout << "foobar\n";
}

int main(int argc, const char * argv[]) {
    
    // blocks user from quitting program (MUHAHAHAHA)
    sigset_t newsigset;
    sigemptyset(&newsigset);
    sigaddset(&newsigset, SIGTSTP); // ctrl-Z
    sigaddset(&newsigset, SIGINT);  // ctrl-C
    if(sigprocmask(SIG_BLOCK, &newsigset, NULL) < 0) {
        perror("could not block the signal");
    }
    
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
            catch(...) { //TODO: replace this with the appropriate exception handling
                std::cout << "Goodbye." << "\n";
                return 0;
            }
        }
        std::cout << output << "\n";
    }

    return 0;
}
