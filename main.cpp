//
//  main.cpp
//  ShellFish
//
//  Created by Morgan Redding on 9/24/15.
//  Copyright © 2015 Morgan Redding. All rights reserved.
//

#include <iostream>
#include <string>
#include "Variable.h"
#include "InterpretCommand.h"

std::string find_file(const std::string& file_name) {
    // TODO
    return "";
}

std::string execute_file(const std::string& file_name) {
    // TODO
    return "\"[insert output from file " + file_name  + " here]\"";
}

std::string execute(const std::string& command, int debug_level, std::unordered_map<std::string, Variable>& local_variables, std::unordered_map<std::string, Variable>& global_variables) {
    
    std::vector<std::string> command_parts = split_but_preserve_literal_strings(command, ' ');

    if(command_parts[0] == "-f") {
        return execute_file(command.substr(3, command.size()));
    }
    if(command_parts[0] == "set") {
        local_variables[command_parts[1]] = Variable(command_parts[2]);
        return "";
    }
    if(command_parts[0] == "unset") {
        local_variables.erase(command_parts[1]);
        return "";
    }
    if(command_parts[0] == "export") {
        global_variables[command_parts[1]] = Variable(command_parts[2]);
        return "";
    }
    if(command_parts[0] == "unexport") {
        global_variables.erase(command_parts[1]);
        return "";
    }
    if(command_parts[0] == "echo") {
        return command_parts[1];
    }
    return "UNKNOWN COMMAND: " + command_parts[0];
}

int main(int argc, const char * argv[]) {
    
    std::unordered_map<std::string, Variable> local_variables;
    std::unordered_map<std::string, Variable> global_variables;
    
    local_variables["local1"] = Variable("a");
    global_variables["global1"] = Variable("b");
    local_variables["var"] = Variable("a");
    global_variables["var"] = Variable("b");
    
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
            output = execute(individual_commands[i] + " " + output, debug_level, local_variables, global_variables);
        }
        std::cout << output << "\n";
    }

    return 0;
}
