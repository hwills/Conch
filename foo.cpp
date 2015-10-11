#include <iostream>
#include <unistd.h>

int main(int argc, const char * argv[]) {

	std::cout << "a" << std::endl;
	while(true) {
		sleep(1);
		std::cout << "e" << std::endl;
	}
	std::cout << "b" << std::endl;

    return 12;
}
