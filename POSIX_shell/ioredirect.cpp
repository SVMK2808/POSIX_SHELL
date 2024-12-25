#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string>
#include <vector>
#include <cstring>

using namespace std;

vector<string> split(const string &str)
{
    vector<string> tokens;
    string token;
    for (char c : str)
    {
        if (isspace(c))
        {
            if (!token.empty())
            {
                tokens.push_back(token);
                token.clear();
            }
        }
        else
        {
            token += c;
        }
    }
    if (!token.empty())
    {
        tokens.push_back(token);
    }
    return tokens;
}

void IORedirect(string &command)
{
    vector<string> tokens = split(command);

    // Handle redirection
    string outputFile;
    string inputFile;
    string errorFile;
    bool redirectOutput = false;
    bool redirectInput = false;
    bool redirectError = false;
    bool redirectErrorToStdout = false;

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        if (tokens[i] == ">")
        {
            redirectOutput = true;
            outputFile = tokens[i + 1];
            tokens.erase(tokens.begin() + i, tokens.begin() + i + 2);
        }
        else if (tokens[i] == "<")
        {
            redirectInput = true;
            inputFile = tokens[i + 1];
            tokens.erase(tokens.begin() + i, tokens.begin() + i + 2);
        }
        else if (tokens[i] == "2>")
        {
            redirectError = true;
            errorFile = tokens[i + 1];
            tokens.erase(tokens.begin() + i, tokens.begin() + i + 2);
        }
        else if (tokens[i] == "2>&1")
        {
            redirectErrorToStdout = true;
            tokens.erase(tokens.begin() + i);
        }
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        cerr << "Forking failed." << endl;
        return;
    }
    else if (pid == 0)
    { // Child process
        if (redirectOutput)
        {
            int fd = open(outputFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1)
            {
                cerr << "Could not open output file." << endl;
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        if (redirectInput)
        {
            int fd = open(inputFile.c_str(), O_RDONLY);
            if (fd == -1)
            {
                cerr << "Could not open input file." << endl;
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        if (redirectError)
        {
            int fd = open(errorFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1)
            {
                cerr << "Could not open error file." << endl;
                exit(1);
            }
            dup2(fd, STDERR_FILENO);
            close(fd);
        }

        if (redirectErrorToStdout)
        {
            dup2(STDOUT_FILENO, STDERR_FILENO);
        }

        vector<char *> args;
        for (const string &token : tokens)
        {
            args.push_back(const_cast<char *>(token.c_str()));
        }
        args.push_back(nullptr);

        if (execvp(args[0], &args[0]) == -1)
        {
            perror("execvp");
            exit(1);
        }
    }
    else
    { // Parent process
        wait(nullptr);
    }
}