//
//  Documentation.h
//  ShellFish
//
//  Authors: Morgan Redding, Hunter Wills, Brian Lam, 
//	Lizzie Halper, and Alex Tran on 10/10/2015.
//

#include <iostream>
#include <string.h>
#include <unordered_map>

std::unordered_map<std::string, std::string> functions_documentation;
functions_documentation["show"] = "show w1 w2: display the word(s) followed by a newline";
functions_documentation["set"] = "set w1 w2: set the value of the local variable w1 to the value w2";
functions_documentation["unset"] = "unset w: unset a previously set local variable w";
functions_documentation["export"] = "export the global variable w1 with the value w2 to the environment";


std::string printHelp(){
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

	return "";
}