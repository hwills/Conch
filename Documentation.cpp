//
//  Documentation.cpp
//  Source code for help functions
//
//  Authors: Morgan Redding, Hunter Wills, Brian Lam, 
//  and Alex Tran on 10/10/2015.
//

#include "Documentation.h"

std::unordered_map<std::string, std::string> functions_help;

/*
 * populateDocumentation
 * Populate functions_help with basic details of functions.
 * @returns void
 *
 * @params none
 */
void populateDocumentation() {
    functions_help["show"] = "show [w1] [w2]: display the word(s) followed by a newline";
    functions_help["set"] = "set [w1] [w2]: set the value of the local variable w1 to the value w2";
    functions_help["unset"] = "unset [w]: unset a previously set local variable w";
    functions_help["export"] = "export [w1] [w2]: the global variable w1 with the value w2 to the environment";
    functions_help["unexport"] = "unexport [w]: unexport the global variable w from the environment";
    functions_help["environ"] = "environment: display a listing of all environmental strings that are currently defined";
    functions_help["chdir"] = "chdir [w]: change the current working directory";
    functions_help["exit"] = "exit [i]: exit the shell and return status i. If no value for i is given, exit with a status of 0. Kill off any currently running processes.";
    functions_help["wait"] = "wait [i]: Wait for process with pid i to complete. If i is -1, wait for any children to complete.";
    functions_help["clr"] = "clr: Clear the screen and display a new command line prompty at the top";
    functions_help["dir"] = "dir: List the contents of the current directory";
    functions_help["echo"] = "echo <comment>: Display <comment> on the display followed by a new line";
    functions_help["help"] = "help: Display the user manual using the more filter";
    functions_help["pause"] = "pause: Pause the operation of the shell until the Enter key is pressed";
    functions_help["history"] = "history [n]: Display the last n executed commands.";
    functions_help["repeat"] = "repeat [n]: Print to the screen and execute the command in the history list that corresponds to the nth line. If n is not specified, then repeat should just execute the previous command.";
    functions_help["kill"] = "kill [-n] pid: This command should send a signal to the specified process.";      
}

/*
 * populateDocumentationDetailed
 * Populate functions_help with man-page style details of functions.
 * @returns void
 *
 * @params none
 */
void populateDocumentationDetailed() {
    functions_help["show"] = "SYNPOSIS\n\t show [w1] [w2]: display the word(s) followed by a newline\n\nDESCRIPTION\n\t[w1] [w2] ... \t are the words to be displayed on the screen";
    functions_help["set"] = "SYNPOSIS\n\t set [w1] [w2]: set the value of the local variable w1 to the value w2\n\nDESCRIPTION\n\t[w1] is the name of the variable to be set. \n\t[w2] is the value of [w1]";
    functions_help["unset"] = "SYNPOSIS\n\t unset [w]: unset a previously set local variable w\n\nDESCRIPTION\n\t[w] is the name of the variable that should be unset";
    functions_help["export"] = "SYNPOSIS\n\t export [w1] [w2]: the global variable w1 with the value w2 to the environment\n\nDESCRIPTION\n\t[w1] is the name of the variable to be set. \n\t[w2] is the value of [w1]";
    functions_help["unexport"] = "SYNPOSIS\n\t unexport [w]: unexport the global variable w from the environment\n\nDESCRIPTION\n\t[w] is the name of the variable that should be unexported";
    functions_help["environ"] = "SYNPOSIS\n\t environment: display a listing of all environmental strings that are currently defined\n\nDESCRIPTION\n\tThis does not take in any arguments.";
    functions_help["chdir"] = "SYNPOSIS\n\t chdir [w]: change the current working directory\n\nDESCRIPTION\n\t[w] is the path to the new directory";
    functions_help["exit"] = "SYNPOSIS\n\t exit [i]: exit the shell and return status i. If no value for i is given, exit with a status of 0. Kill off any currently running processes.\n\nDESCRIPTION\n\t[i] is an optional argument, and is an integer that will be used as the return status";
    functions_help["wait"] = "SYNPOSIS\n\t wait [i]: Wait for process with pid i to complete. If i is -1, wait for any children to complete.\n\nDESCRIPTION\n\t[i] is the PID of the process that the shell should wait on.";
    functions_help["clr"] = "SYNPOSIS\n\t clr: Clear the screen and display a new command line prompty at the top\n\nDESCRIPTION\n\tThis does not take in any arguments.";
    functions_help["dir"] = "SYNPOSIS\n\t dir: List the contents of the current directory\n\nDESCRIPTION\n\tThis does not take in any arguments.";
    functions_help["echo"] = "SYNPOSIS\n\t echo <comment>: Display <comment> on the display followed by a new line\n\nDESCRIPTION\n\t<comment> is the comment to be echoed. Spaces and special characters should be escaped.";
    functions_help["help"] = "SYNPOSIS\n\t help [command]: Display the user manual using the more filter\n\nDESCRIPTION\n\t[command] is the name of a command to display help for.";
    functions_help["pause"] = "SYNPOSIS\n\t pause: Pause the operation of the shell until the Enter key is pressed\n\nDESCRIPTION\n\tThis does not take in any arguments.";
    functions_help["history"] = "SYNPOSIS\n\t history [n]: Display the last n executed commands.\n\nDESCRIPTION\n\t[n] is the number of recent commands to list.";
    functions_help["repeat"] = "SYNPOSIS\n\t repeat [n]: Print to the screen and execute the command in the history list that corresponds to the nth line. If n is not specified, then repeat should just execute the previous command.\n\nDESCRIPTION\n\t[n] is an optional argument.";
    functions_help["kill"] = "SYNPOSIS\n\t kill [-n] pid: This command should send a signal to the specified process.\n\nDESCRIPTION\n\t[-n] represents the signal to send. For example, -9.";
}

/*
 * printHelp
 * Print description of shell and basic descriptions of authors
 * @returns std::string
 *      returns the help string
 *
 * @params none
 */
std::string printHelp(){
    populateDocumentation();
    std::string help_string = "************************\n";
    help_string += "Shellfish \n";
    help_string += "************************\n";
    help_string += "Written for the Operating System Course CSE 422 at Washington University in St. Louis \n";
    help_string += "Authors: Hunter Wills, Morgan Redding, Brian Lam, Alex Tran, Elizabeth Halper \n\n";
    help_string += "(Authors are not written in any particular order. Although if they were written in the order of who ";
    help_string += "is the best looking looking, we're pretty sure Brian would go first. Just saying)\n\n";
    help_string += "The following shell commands are defined internally. Type 'help' to see this list. ";
    help_string += "Type 'help name' to find more about the function 'name'. Help documentation text borrowed from the assignment text. \n";
    std::cout << help_string;

    std::cout << std::endl << "Support commands" << std::endl;
    for(auto it = functions_help.begin(); it!=functions_help.end(); ++it) {
        std::cout << "  * " << it->second << std::endl;
    }   
    return "";
}

/*
 * functionHelp
 * Provides help about a specific function
 * @returns std::string
 *  Detailed string description of a function
 *
 * @params std::string _function
 *   Name of the function to print help about
 */
std::string functionHelp(std::string _function){
    populateDocumentationDetailed();
    std::unordered_map<std::string, std::string>::const_iterator help_result = functions_help.find(_function);
    if (help_result == functions_help.end()) {
        std::cout << _function << " is not a recognized internal command." << std::endl;
    } else {
        std::cout << help_result->second << std::endl;
    }
    return "";
}
