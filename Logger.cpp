#include "Logger.h"

void logCommand(std::string command) {
    if (command != "repeat" && command != "exit") {
        std::ofstream history_file;

        history_file.open("history", std::ios_base::app);
        history_file << command << std::endl; 
    }
}

std::string getLastLine(std::ifstream& in)
{

    std::string line;
    while (in >> std::ws && std::getline(in, line));
    return line;
}

std::string repeatCommand() {
    std::ifstream file("history");

    if (file) {
        std::string line = getLastLine(file);
        return line;
    } else {
    	return "";
    }
}

void commandHistory(int n) {
    std::ifstream file("history");

    if (file) {
        std::vector<std::string> lines;
        std::string line;
        while (file >> std::ws && std::getline(file, line)) {
            lines.push_back(line);
        };

        if (n > lines.size()) {
            n = lines.size();
        }

        if (n < 0) {
            n = 1;
        }

        std::vector<std::string> lastLines(lines.end() - n, lines.end());
        for (std::vector<std::string>::iterator it = lastLines.begin(); it != lastLines.end(); ++it) {
            std::cout << *it << "\n";
        }
    }
}