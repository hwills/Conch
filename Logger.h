//
//  Logger.h
//  ShellFish
//
//  Authors: Morgan Redding, Hunter Wills, Brian Lam, 
//	Lizzie Halper, and Alex Tran on 10/10/2015.
//

#ifndef LOGGER_H
#define LOGGER_H
#include <string.h>
#include <iostream>
#include <vector>
#include <fstream>

void logCommand(std::string command);
std::string repeatCommand();
std::string repeatCommand(int n);
void commandHistory(int n);
#endif
