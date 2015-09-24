#include <iostream>
#include <unordered_map>
#include <vector>

bool isLegalVariableCharacter(char c) {
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
    if(c < 97) {
        return false;
    }
    if(c < 123) {
        return true;
    }
    return false;
}

std::vector<std::string> splitByPipes(const std::string& txt) {

    std::vector<std::string> rtn;

    int start = 0;
    for(int i = 0; i < txt.size(); i++) {
        if(txt[i] == '|') {
            while(i < txt.size() && txt[i] != '|') {
                i++;
            }
            rtn.push_back(txt.substr(start, i - start));
            start = i + 1;
        }
    }
    rtn.push_back(txt.substr(start, txt.size() - start));

    return rtn;
}

struct Variable {
    Variable() {};
    Variable(std::string value) : value(value) {};
    std::string value;
};

std::unordered_map<std::string, Variable> localVariables;
std::unordered_map<std::string, Variable> globalVariables;

std::string substituteVariableValues(const std::string& txt) {
    std::string rtn = "";
    for(int i = 0; i < txt.size(); i++) {
        if(txt[i] == '$') {
            i++;
            int start = i;
            while(i < txt.size() && isLegalVariableCharacter(txt[i])) {
                i++;
            }
            std::unordered_map<std::string, Variable>::iterator it = localVariables.find(txt.substr(start, i - start));
            if(it == localVariables.end()) {
                it = globalVariables.find(txt.substr(start, i - start));
                if(it == globalVariables.end()) {
                    i = start;
                    rtn += '$';
                }
                else {
                    rtn += it->second.value;
                }
            }
            else {
                rtn += it->second.value;
            }
            i--;
        }
        else {
            rtn += txt[i];
        }
    }
    return rtn;
}