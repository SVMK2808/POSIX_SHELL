#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <stack>
#include <iostream>
using namespace std;

// Global stack to track previous directories
stack<string> recordpaths;

// Function to handle the 'cd' command
void cd(int argc, vector<string> &args, char *cwd)
{
    string dest = (args.size() > 1) ? args[1] : "";

    // If 'cd' is called without arguments, go to the home directory
    if (dest.empty() || dest == "~")
    {
        const char *home = getenv("HOME");
        if (chdir(home) != 0)
        {
            perror("Could not change to home directory");
            return;
        }
    }
    // If 'cd -' is called, go to the previous directory
    else if (dest == "-")
    {
        if (!recordpaths.empty())
        {
            string prevDir = recordpaths.top();
            recordpaths.pop();
            if (chdir(prevDir.c_str()) != 0)
            {
                perror("Could not change to previous directory");
                return;
            }
            cout << prevDir << "\n"; // Print the previous directory
        }
        else
        {
            cerr << "No previous directory in history\n";
            return;
        }
    }
    // If 'cd ..' is called, go to the parent directory
    else if (dest == "..")
    {
        if (chdir("..") != 0)
        {
            perror("Could not change to parent directory");
            return;
        }
    }
    // Handle case where a specific directory is provided
    else
    {
        if (chdir(dest.c_str()) != 0)
        {
            perror("No such file or directory");
            return;
        }
    }

    // After successfully changing the directory, update the current working directory
    recordpaths.push(string(cwd)); // Record the current directory before changing
    if (getcwd(cwd, 1024) != nullptr)
    {
        cout << cwd << "\n"; // Print the absolute path of the new working directory
    }
    else
    {
        perror("Unable to get current working directory");
    }
}
