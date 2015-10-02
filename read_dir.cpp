#include <stdio.h>
#include <dirent.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int
list_files(std::string current_dir, std::string raw_path, std::string &result)
{
  /* Process the given path
   * This can be empty, sub directory, or full path 
   */

  std::string path;
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
list_files(std::string current_dir, std::string raw_path, vector<string> &result)
{
  /* Process the given path
   * This can be empty, sub directory, or full path 
   */

  std::string path;
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
      cout << ent->d_name << endl; 
    }
    closedir(dir);
    return 0;
  }
  
  return 1;
}

int 
main ()
{
  vector<string> result;
  list_files(".", "", result);
}
