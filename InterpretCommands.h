#ifndef INTERPRET_COMMANDS_H
#define INTERPRET_COMMANDS_H
#include <iostream>
#include <unordered_map>
#include <vector>
#include "Variable.h"
#include "conch.h"

bool isLegalVariableCharacter(char c);

std::vector<std::string> splitByPipes(const std::string& txt);

std::string substituteVariableValues(const std::string& txt, const std::unordered_map<std::string, Variable>& globalVars, const std::unordered_map<std::string, Variable>& localVars);
#endif
