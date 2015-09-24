#include "InterpretCommands.h"
#include "conch.h"
const int MAX_NUMBER_OF_PIPES = 4;

void shellPrompt() {
        std::cout << "sish >> ";
}

std::string readInput() {
    int maxInputChars = 256;
    char line[maxInputChars];
    std::cin.getline(line, maxInputChars);
    return (std::string) line;
}

int main (int argc, char* argv[]) {
    std::unordered_map<std::string, Variable> localVariables;
    std::unordered_map<std::string, Variable> globalVariables;
    while(true) {
        shellPrompt();
        std::string command = readInput();
        std::vector<std::string> args = splitByPipes(command);
        for (auto it = args.begin(); it != args.end(); ++it){
            std::cout << *it << std::endl;
        }
    }
}


