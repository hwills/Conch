//
//  Logger.cpp
//  Source code for logging and retrieving past commands
//
//  Authors: Morgan Redding, Hunter Wills, Brian Lam, 
//  and Alex Tran on 10/10/2015.
//

#include "Logger.h"

/* 
 * logCommand 
 * @returns void
 *
 * @params
 *  std::string command
 *  Command line that was just submitted - does not validate
 */
void logCommand(std::string command) {
    if(command != "repeat" && command != "exit") {
        std::ofstream history_file;

        history_file.open("history", std::ios_base::app);
        history_file << command << std::endl; 
    }
}

/* 
 * getLastLine
 *  Used by repeatCommand
 * @returns std::string
 *  The last line of a file
 *
 * @params
 *  std::ifstream& in
 *  Takes in ifstream to a file
 */
std::string getLastLine(std::ifstream& in) {
    std::string line;
    while(in >> std::ws && std::getline(in, line));
    return line;
}

/* 
 * repeatCommand 
 * @returns std::string
 *  The most recently entered command
 *
 * @params
 *  none
 */
std::string repeatCommand() {
    std::ifstream file("history");

    if(file) {
        std::string line = getLastLine(file);
        return line;
    }
    else {
        return "";
    }
}

/* 
 * repeatCommand 
 * @returns std::string
 *  The nth most recent command
 *
 * @params
 *  std::string command
 *  Command line that was just submitted - does not validate
 */
std::string repeatCommand(int n) {
    std::ifstream file("history");

    if(file) {
        std::vector<std::string> lines;
        std::string line;
        while(file >> std::ws && std::getline(file, line)) {
            lines.push_back(line);
        };

        if(n >= lines.size()) {
            n = lines.size() - 1;
        }

        if(n < 0) {
            n = 1;
        }

        return lines[n];
    }

    return "";
}

/* 
 * commandHistory
 * Print out the n most recent commands
 * @returns void
 *
 * @params
 *  int n
 *  The number of recent commands to repeat
 */
void commandHistory(int n) {
    std::ifstream file("history");

    if(file) {
        std::vector<std::string> lines;
        std::string line;
        while(file >> std::ws && std::getline(file, line)) {
            lines.push_back(line);
        };

        if(n > lines.size()) {
            n = lines.size();
        }

        if(n < 0) {
            n = 1;
        }

        std::vector<std::string> lastLines(lines.end() - n, lines.end());
        for(auto it = lastLines.begin(); it != lastLines.end(); ++it) {
            std::cout << *it << "\n";
        }
    }
}