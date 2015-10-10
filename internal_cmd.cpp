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
change_dir(string current_dir, string raw_path, string &result)
{
  /* Process the given path
   * This can be empty, sub directory, or full path 
   */

  string path;
  if (raw_path.size()==0) 
  {
    //empty
    path= current_dir;
  }
  else if (raw_path[0] != '/') 
  {
    // sub directory
    path= current_dir+"/"+raw_path;
  }
  else 
  {
    //full path
    path= raw_path;
  }

  DIR *dir;
  struct dirent *ent;
  const char * final_path= path.c_str(); 
  dir = opendir(final_path);
  if (dir) 
  {
    closedir(dir);
    result= path;
    return 0;
  }

  return 1;
}

int
list_files(string current_dir, string raw_path, vector<string> &result)
{
  /* Process the given path
   * This can be empty, sub directory, or full path 
   */

  string path;
  if (raw_path.size()==0) 
  {
    //empty
    path= current_dir;
  }
  else if (raw_path[0] != '/') 
  {
    // sub directory
    path= current_dir+"/"+raw_path;
  }
  else 
  {
    //full path
    path= raw_path;
  }

  DIR *dir;
  struct dirent *ent;
  const char * final_path= path.c_str(); 
  dir = opendir(final_path);
  if (dir) 
  {
    while ((ent = readdir(dir)) != NULL) 
    {
      result.push_back(ent->d_name);
      /* cout << ent->d_name << endl; */ 
    }
    closedir(dir);
    return 0;
  }
  
  return 1;
}

int 
help(string help_directory, string command, string &result)
{
  command= help_directory+"/"+command+".txt";
  ifstream file(command.c_str());
  if (file.is_open())
  {
    stringstream sstr;
    while (file >> sstr.rdbuf());
    result= sstr.str();
    return 0;
  }
  return 1;
}

int 
add_to_history(string command, deque<string> &history)
{
  int max_command=3;
  int current_size= history.size();
  while (current_size >=max_command) 
  {
    history.pop_front();
    current_size--;
  }
  history.push_back(command);
  return 0;
}

int 
find_file(string current_dir, string target, string &result) 
{
  //Check if the target file is in current directory
  vector<string> files_in_current_dir;
  list_files(current_dir, "", files_in_current_dir);
  for (int i=0; i< files_in_current_dir.size(); i++)
  {
    if (target.compare(files_in_current_dir[i])==0)
    {
      result= current_dir+"/"+target;
      return 0;
    }
  }

  //Check if the target file is in the evn PATH
  string path(getenv("PATH"));
  istringstream iss(path);
  string single_path;
  while (getline(iss, single_path, ':'))
  {
    vector<string> files_in_single_path;
    list_files(single_path, "", files_in_single_path);
    for (int i=0; i< files_in_single_path.size(); i++)
    {
      if (target.compare(files_in_single_path[i])==0)
      {
        result= single_path+"/"+target;
        return 0;
      }
    }
  }

  return 1;
}

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
