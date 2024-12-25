#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string>
#include <iostream>
using namespace std;

bool search(string &directory, string &filename)
{
    DIR *dir = opendir(directory.c_str());
    if (dir == nullptr)
    {
        return false;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }
        string full_path = directory + "/" + entry->d_name;

        struct stat file_stat;
        if (stat(full_path.c_str(), &file_stat) == 0)
        {
            if (S_ISDIR(file_stat.st_mode))
            {
                if (search(full_path, filename)) // Pass the full path of the subdirectory
                {
                    return true;
                }
            }
            else if (strcmp(entry->d_name, filename.c_str()) == 0)
            {
                return true;
            }
        }
    }
    closedir(dir);
    return false;
}
