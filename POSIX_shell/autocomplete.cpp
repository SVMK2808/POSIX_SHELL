#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <termios.h>
#include <sys/stat.h>
using namespace std;

vector<string> getFilesInDirectory(const string &dir)
{
    vector<string> files;
    DIR *dp = opendir(dir.c_str());
    if (dp)
    {
        struct dirent *entry;
        while ((entry = readdir(dp)) != nullptr)
        {
            // Skip current and parent directory entries
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            {
                files.push_back(entry->d_name);
            }
        }
        closedir(dp);
    }
    return files;
}

bool directoryExists(const string &dir)
{
    struct stat info;
    return (stat(dir.c_str(), &info) == 0 && (info.st_mode & S_IFDIR));
}

vector<string> getMatchingCommands(const string &input)
{
    vector<string> matches;
    vector<string> systemDirs = {"/bin", "/usr/bin", "/sbin", "/usr/sbin", "/usr/local/bin", "System/Applications/Utilities", "/System/Library/CoreServices", "/opt/homebrew/bin"};

    for (const auto &dir : systemDirs)
    {
        vector<string> files = getFilesInDirectory(dir);
        for (const auto &file : files)
        {
            if (file.find(input) == 0)
            {
                matches.push_back(file);
            }
        }
    }
    return matches;
}

vector<string> autoComplete(const string &input)
{
    if (input.find('/') != string::npos)
    {
        string dir = ".";
        string prefix = input;

        size_t pos = input.find_last_of('/');
        if (pos != string::npos)
        {
            dir = input.substr(0, pos);
            prefix = input.substr(pos + 1);
        }

        if (!directoryExists(dir))
        {
            cout << "Directory does not exist: " << dir << endl;
            return {};
        }

        vector<string> files = getFilesInDirectory(dir);
        vector<string> matches;
        for (const auto &file : files)
        {
            if (file.find(prefix) == 0)
            {
                matches.push_back(file);
            }
        }
        return matches;
    }
    else
    {
        return getMatchingCommands(input);
    }
}
void enableRawMode(struct termios &original)
{
    struct termios raw = original;
    raw.c_lflag &= ~(ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disableRawMode(const struct termios &original)
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original);
}

// int main()
// {

//     string input;
//     cout << "Enter commmand or filename (Press Tab for autocomplete) :";
//     cout.flush();

//     struct termios original;
//     tcgetattr(STDIN_FILENO, &original);
//     enableRawMode(original);

//     while (true)
//     {
//         char ch;
//         if (read(STDIN_FILENO, &ch, 1) == -1)
//             break;

//         if (ch == '\t')
//         { // Tab key pressed
//             vector<string> matches = autoComplete(input);

//             if (matches.empty())
//             {
//                 cout << "\nNo matches found!\n";
//             }
//             else if (matches.size() == 1)
//             {
//                 // Autocomplete input if only one match
//                 input = matches[0];
//                 cout << "\r" << input << " "; // Overwrite line with updated input
//             }
//             else
//             {
//                 // Show suggestions
//                 cout << "\nSuggestions: ";
//                 for (const auto &match : matches)
//                 {
//                     cout << match << " ";
//                 }
//                 cout << "\n"
//                      << input; // Redisplay current input
//             }
//         }
//         else if (ch == '\n')
//         { // Enter key pressed
//             cout << "\nExecuting: " << input << endl;
//             input.clear();
//             cout << "Enter command or filename (Press Tab for autocomplete): ";
//             cout.flush();
//         }
//         else if (ch == 127)
//         { // Backspace key pressed
//             if (!input.empty())
//             {
//                 input.pop_back();
//                 cout << "\r" << input << " \b"; // Remove last character visually
//             }
//         }
//         else if (ch == 27)
//         { // Escape key pressed (exit program)
//             break;
//         }
//         else
//         {
//             // Append character to input
//             input += ch;
//             cout << ch;
//         }
//     }
//     disableRawMode(original);
//     return 0;
// }