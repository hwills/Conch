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

void repeatCommand() {
    std::ifstream file("history");

    if (file)
    {

        std::string line = getLastLine(file);
        std::cout << line << '\n';
    }
    else
    std::cout << "Unable to retrieve history of commands.\n";
}