#include <iostream>
#include <pwd.h>
#include <limits.h>
#include <unistd.h>
#include <string>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/utsname.h>
#include "execfunct.h"
#include <signal.h>
using namespace std;
string path;
string working_path = "";
void getpath()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    path = string(cwd);
    working_path = path;
}
string prompt()
{
    struct passwd *pw = getpwuid(getuid());
    const char *username = pw ? pw->pw_name : "?";
    struct utsname sysinfo;
    uname(&sysinfo);
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    string display_path = (strcmp(cwd, path.c_str()) == 0) ? "~" : cwd;
    string prom = string(username) + "@" + string(sysinfo.sysname) + ":" + display_path + ">";

    return prom;
}

void handle_sigint(int sig)
{
    if (kill(getpgrp(), SIGINT) == -1)
    {
        perror("kill(SIGINT)");
    }
}

void handle_sigtstp(int sig)
{
    if (kill(getpgrp(), SIGTSTP) == -1)
    {
        perror("kill(SIGTSTP)");
    }
}

void handle_sigquit(int sig)
{
    exit(0); // Log out of the shell
}

void handle_command(string command)
{
    if (command.find('|') != string::npos)
    {
        pipeline(command);
    }

    // else if (command.find('|') != string::npos && (command.find('>') != string::npos || command.find('<') != string::npos))
    // {
    //     pipelineIORedirect(command);
    // }

    else if (command.find('>') != string::npos || command.find('<') != string::npos)
    {
        IORedirect(command);
    }

    else if (command.back() == '\t')
    {
        vector<string> matches = autoComplete(command);
        if (matches.empty())
        {
            cout << "\nNo Matches Found!\n";
        }
        else if (matches.size() == 1)
        {
            command = matches[0];
            cout << "\r" << command << " ";
        }
        else
        {
            cout << "\nSuggestions: ";
            for (const auto &match : matches)
            {
                cout << match << " ";
            }
            cout << "\n"
                 << command;
        }
        cout.flush();
        return;
    }

    else if (command.empty())
    {
        return;
    }

    char *cmd = new char[command.length() + 1];
    strcpy(cmd, command.c_str());

    char *arg = strtok(cmd, " ");
    vector<string> args;
    while (arg != NULL)
    {
        args.push_back(string(arg));
        arg = strtok(NULL, " ");
    }

    if (args[0] == "pwd")
    {
        pwd();
    }
    else if (args[0] == "cd")
    {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        cd(args.size(), args, cwd);
    }
    else if (args[0] == "ls")
    {
        ls(args.size(), args);
    }
    else if (args[0] == "echo")
    {
        echo(args);
    }
    else if (args[0] == "pinfo")
    {
        pid_t pid = stol(args[1]);
        pinfo(pid);
    }
    else if (args[0] == "history")
    {
        history_disp(args[0]);
    }
    else if (args[0] == "search")
    {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        string curr(cwd);
        if (search(curr, args[1]))
        {
            cout << "True\n";
        }
        else
        {
            cout << "False\n";
        }
    }
    else
    {
        if (command.back() == '&')
        {
            command.pop_back();
            background(command);
        }
        else
        {
            foreground(command);
        }
    }
}
int main()
{
    signal(SIGINT, handle_sigint);
    signal(SIGTSTP, handle_sigtstp);
    signal(SIGQUIT, handle_sigquit);

    getpath();
    while (true)
    {
        char *line = readline(prompt().c_str());

        // if (!line)
        // {
        //     break;
        // }
        string s(line);

        add_history(line);
        handle_command(s);

        if (strcmp(s.c_str(), "exit") == 0 || cin.eof())
        {
            break;
        }
        free(line);
    }
    return 0;
}