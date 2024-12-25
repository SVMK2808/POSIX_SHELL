#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

vector<string> split(const char *str, const char *delim)
{
    vector<string> tokens;
    char *token = strtok(const_cast<char *>(str), delim);
    while (token != nullptr)
    {
        tokens.push_back(token);
        token = strtok(nullptr, delim);
    }
    return tokens;
}

void pipeline(string &command)
{
    vector<string> commands = split(command.c_str(), "|");

    int prevPipe[2] = {-1, -1};

    for (size_t i = 0; i < commands.size(); ++i)
    {
        int pipefd[2];
        pid_t pid;

        if (i < commands.size() - 1)
        {
            if (pipe(pipefd) == -1)
            {
                cerr << "Pipe creation failed." << endl;
                return;
            }
        }

        pid = fork();

        if (pid < 0)
        {
            cerr << "Forking failed." << endl;
            return;
        }
        else if (pid == 0)
        { // Child process
            if (i > 0)
            {
                dup2(prevPipe[0], 0); // Redirect stdin to the read end of the previous pipe
                close(prevPipe[0]);   // Close the read end of the previous pipe
            }

            if (i < commands.size() - 1)
            {
                dup2(pipefd[1], 1); // Redirect stdout to the write end of the current pipe
                close(pipefd[1]);   // Close the write end of the current pipe
            }

            vector<char *> args;
            vector<string> commandArgs = split(commands[i].c_str(), " ");
            for (const auto &arg : commandArgs)
            {
                args.push_back(const_cast<char *>(arg.c_str())); // Add each argument
            }
            args.push_back(nullptr);

            execvp(args[0], &args[0]);
            cerr << "Execvp failed." << endl;
            exit(1);
        }
        else
        { // Parent process
            if (i > 0)
            {
                close(prevPipe[0]); // Close the read end of the previous pipe
            }
            if (i < commands.size() - 1)
            {
                close(pipefd[1]); // Close the write end of the current pipe
            }
            prevPipe[0] = pipefd[0]; // Save the read end of the current pipe for the next command
        }
    }

    for (size_t i = 0; i < commands.size(); ++i)
    {
        wait(NULL); // Wait for all child processes to finish
    }

    return;
}