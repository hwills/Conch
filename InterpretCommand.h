//
//  InterpretCommand.h
//  ShellFish
//
//  Created by Morgan Redding on 9/24/15.
//  Copyright © 2015 Morgan Redding. All rights reserved.
//

#ifndef InterpretCommand_h
#define InterpretCommand_h

#include <iostream>
#include <unordered_map>
#include <vector>

// if there are multiple spaces in a row, kill them all except one of them; spaces on the ends are killed entirely
// the only exception is that if the spaces are between quotes, they should be preserved
std::string remove_excess_spaces(const std::string& txt) {
    std::string rtn = "";
    char inside_quotes = 0;
    for(int i = 0; i < txt.size(); i++) {
        if(txt[i] == '\\') {
            rtn += txt[i];
            i++;
            rtn += txt[i];
        }
        else if(txt[i] == '\'' && inside_quotes != '"') {
            inside_quotes = inside_quotes == '\'' ? 0 : '\'';
            rtn += txt[i];
        }
        else if(txt[i] == '"' && inside_quotes != '\'') {
            inside_quotes = inside_quotes == '"' ? 0 : '"';
            rtn += txt[i];
        }
        else if(inside_quotes == 0 && txt[i] == ' ') {
            if(rtn[rtn.size() - 1] != ' ') {
                rtn += ' ';
            }
        }
        else {
            rtn += txt[i];
        }
    }
    if(rtn[0] == ' ') { // remove a space if it is the first character
        rtn = rtn.substr(1, rtn.size() - 1);
    }
    if(rtn[rtn.size() - 1] == ' ') { // remove a space if it is the last character
        rtn = rtn.substr(0, rtn.size() - 1);
    }

    return rtn;
}

// returns 'true' for numbers (0 - 9)
bool is_numeric(char c) {
    if(c < 48) {
        return false;
    }
    return c < 58;
}

// returns 'true' for alphanumeric characters and underscore
bool is_legal_variable_character(char c) {
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
    if(c == 95) {
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

std::vector<std::string> split_string(const std::string& txt, const char symbol_to_split_by) {
    std::vector<std::string> rtn;
    int start = 0;
    for(int i = 0; i < txt.size(); i++) {
        if(txt[i] == symbol_to_split_by) {
            while(i < txt.size() && txt[i] != symbol_to_split_by) {
                i++;
            }
            rtn.push_back(txt.substr(start, i - start));
            start = i + 1;
        }
    }
    rtn.push_back(txt.substr(start, txt.size() - start));
    return rtn;
}

std::string substitute_variable_values(const std::string& txt, const std::unordered_map<std::string, std::string>& local_variables, const std::unordered_map<std::string, std::string>& global_variables) {
    std::string rtn = "";
    for(int i = 0; i < txt.size(); i++) {
        if(txt[i] == '$') {
            i++;
            int start = i;
            while(i < txt.size() && is_legal_variable_character(txt[i])) {
                i++;
            }
            std::unordered_map<std::string, std::string>::const_iterator it = local_variables.find(txt.substr(start, i - start));
            if(it == local_variables.end()) {
                it = global_variables.find(txt.substr(start, i - start));
                if(it == global_variables.end()) {
                    i = start;
                    rtn += '$';
                }
                else {
                    rtn += it->second;
                }
            }
            else {
                rtn += it->second;
            }
            i--;
        }
        else {
            rtn += txt[i];
        }
    }
    return rtn;
}

#endif /* InterpretCommand_h */
