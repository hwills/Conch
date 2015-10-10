#include <stdio.h>
#include <dirent.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <queue>

using namespace std;


int 
main ()
{
  /* test dir */
  /* vector<string> result; */
  /* list_files(".", "", result); */
  /* for (int i=0; i< result.size(); i++) */
  /* { */
  /*   cout << result[i] << endl; */
  /* } */

  /* test help */
  /* string result=""; */
  /* help("./help", "echo", result); */
  /* cout << result << endl; */

  /* test history */
  /* std::deque<string> history; */
  /* add_to_history("echo", history); */
  /* add_to_history("set", history); */
  /* add_to_history("hello", history); */
  /* add_to_history("hello", history); */
  /* for (int i=0; i< history.size(); i++) */ 
  /* { */
  /*   cout << history[i] << endl; */
  /* } */

  /* test clr */
  /* cout << "\033[2J\033[1;1H"; */
  /* cout << "hello:"; */
  /* return 0; */

  /* test find_file */
  string result="";
  find_file(".", "chmod", result);
  cout << result << endl;
 

  /* dir */
  /* change_dir */
  /* history */
  /* repeat */
  /* find_file */
  /* clr */
  /* help */
}
