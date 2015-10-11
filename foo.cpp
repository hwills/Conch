#include <iostream>
#include <unistd.h>

int main(int argc, const char * argv[]) {

	std::cout << "a" << std::endl;
	sleep(1);
	std::cout << "b" << std::endl;

    return 12;
}
