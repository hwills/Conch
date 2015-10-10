#include "Logger.h"

void logCommand(std::string command) {
	  std::ofstream history_file;

	  history_file.open("history", std::ios_base::app);
	  history_file << command << std::endl; 
}

std::string getLastLine(std::ifstream& in)
{
    std::string line;
    while (in >> std::ws && std::getline(in, line)) // skip empty lines
        ;

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