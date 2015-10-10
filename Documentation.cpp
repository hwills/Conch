#include "Documentation.h"

std::unordered_map<std::string, std::string> functions_help;

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

std::string functionHelp(std::string _function){
	populateDocumentation();
	std::unordered_map<std::string, std::string>::const_iterator help_result = functions_help.find(_function);
	if (help_result == functions_help.end()) {
		std::cout << _function << " is not a recognized internal command." << std::endl;
	} else {
		std::cout << help_result->second;
	}
	return "";
}