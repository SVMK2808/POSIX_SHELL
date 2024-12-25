#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string>
#include <iostream>
#include <pwd.h>
#include <grp.h>
#include <cstring>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <sys/types.h>

using namespace std;

// Function to print file information for the `-l` flag
void print_file_info(const char *a, const struct stat &file)
{
    cout << (S_ISDIR(file.st_mode) ? "d" : "-");

    cout << ((file.st_mode & S_IRUSR) ? "r" : "-");
    cout << ((file.st_mode & S_IWUSR) ? "w" : "-");
    cout << ((file.st_mode & S_IXUSR) ? "x" : "-");
    cout << ((file.st_mode & S_IRGRP) ? "r" : "-");
    cout << ((file.st_mode & S_IWGRP) ? "w" : "-");
    cout << ((file.st_mode & S_IXGRP) ? "x" : "-");
    cout << ((file.st_mode & S_IROTH) ? "r" : "-");
    cout << ((file.st_mode & S_IWOTH) ? "w" : "-");
    cout << ((file.st_mode & S_IXOTH) ? "x" : "-");

    cout << " " << file.st_nlink;

    struct passwd *p = getpwuid(file.st_uid);
    struct group *g = getgrgid(file.st_gid);

    if (p != NULL)
        cout << " " << p->pw_name;
    else
        cout << " ???";

    if (g != NULL)
        cout << " " << g->gr_name;
    else
        cout << " ???";

    cout << " " << file.st_size;

    char time[80];
    struct tm *t = localtime(&file.st_mtime);
    strftime(time, sizeof(time), "%b %d %H:%M", t);
    cout << " " << time;

    cout << " " << a << "\n";
}

// Function to process `ls` functionality
void ls(int argc, vector<string> &input)
{
    bool showAll = false;
    bool showLong = false;
    vector<string> directories;

    // Parse flags and directories/files
    for (int i = 1; i < argc; i++)
    {
        if (input[i] == "-a")
        {
            showAll = true;
        }
        else if (input[i] == "-l")
        {
            showLong = true;
        }
        else if (input[i] == "-al" || input[i] == "-la")
        {
            showAll = true;
            showLong = true;
        }
        else
        {
            directories.push_back(input[i]); // Consider as directory or file
        }
    }

    if (directories.empty())
    {
        directories.push_back("."); // If no directory is specified, use the current directory
    }

    for (const auto &dir : directories)
    {
        DIR *d;
        struct dirent *entry;
        struct stat fileStat;

        // Handle special case for `~` directory
        if (dir == "~")
        {
            d = opendir(getenv("HOME"));
        }
        else
        {
            d = opendir(dir.c_str());
        }

        if (d == nullptr)
        {
            // If it's not a directory, try to handle it as a file
            if (stat(dir.c_str(), &fileStat) == 0)
            {
                if (showLong)
                {
                    print_file_info(dir.c_str(), fileStat);
                }
                else
                {
                    cout << dir << "\n";
                }
            }
            else
            {
                perror(("ls: cannot access " + dir).c_str());
            }
            continue;
        }

        // Read directory contents
        while ((entry = readdir(d)) != nullptr)
        {
            // Skip hidden files unless `-a` is specified
            if (!showAll && entry->d_name[0] == '.')
            {
                continue;
            }

            if (showLong)
            {
                if (stat((dir + "/" + entry->d_name).c_str(), &fileStat) == -1)
                {
                    perror("stat");
                    continue;
                }
                print_file_info(entry->d_name, fileStat);
            }
            else
            {
                cout << entry->d_name << "\n";
            }
        }

        closedir(d);
    }
}
