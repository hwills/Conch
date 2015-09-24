#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

const int MAX_NUMBER_OF_PIPES = 4;


void shellPrompt() {
        std::cout << "sish >> " << std::endl;
}

std::vector<std::string> parseInput() {
        int maxInputChars = 256;
        char line[maxInputChars];
        std::cin.getline(line, maxInputChars);
}

int main (int argc, char* argv[]) {
    while(true) {
        shellPrompt();
        std::vector<std::string> args = parseInput();
    }
    return 0;
}
