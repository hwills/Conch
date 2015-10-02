#include <stdio.h>
#include <dirent.h>
#include <iostream>

using namespace std;



int
list_files(std::string current_dir, std::string raw_path)
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
      cout << ent->d_name << endl; 
    }
    closedir(dir);
  }
  return 0;
}

int 
main ()
{
  list_files(".", "hello");
}
