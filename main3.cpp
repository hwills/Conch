//
//  main.cpp
//  ShellFish
//
//  Created by Morgan Redding, Hunter Wills, Bryan Lamb, Lizzie Halper, and Alex Tran on 10/10/2015.
//

#include <iostream>
#include <string>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <vector>

// copied shamelessly from http://stackoverflow.com/questions/7281894/how-do-i-chain-stdout-in-one-child-process-to-stdin-in-another-child-in-c/7282296#7282296
void set_read(int* lpipe) {
	dup2(lpipe[0], STDIN_FILENO);
	close(lpipe[0]); // we have a copy already, so close it
	close(lpipe[1]); // not using this end
}
void set_write(int* rpipe) {
	dup2(rpipe[1], STDOUT_FILENO);
	close(rpipe[0]); // not using this end
	close(rpipe[1]); // we have a copy already, so close it
}

void execute_command(const std::vector<std::string>& files) {

	int all_my_pipes[files.size()][2];
	for(int i = 0; i < files.size(); i++) {
		pipe(all_my_pipes[i]);
	}
	int child_ids[files.size()];

	int* last_pipe = nullptr;

	for(int i = 0; i < files.size(); i++) {
		child_ids[i] = fork();
		if(child_ids[i] == 0) {
			if(i != 0) {
				set_write(last_pipe);
			}
			if(i != files.size() - 1) {
				set_read(all_my_pipes[i]);
			}
			execv(&files[i][0], nullptr);
		}
		last_pipe = all_my_pipes[i];
	}

	for(int i = 0; i < files.size(); i++) {
		waitpid(child_ids[i], NULL, 0);
	}
	std::cout << "done\n";
}

int main(int argc, const char * argv[]) {

	std::vector< std::string > vec;
	vec.push_back("foo");
	vec.push_back("foo");

	execute_command(vec);

	return 0;
}
