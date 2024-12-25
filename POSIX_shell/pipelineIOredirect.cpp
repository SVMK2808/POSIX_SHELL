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

void apply_redirection(const string &outputFile, const string &inputFile, const string &errorFile)
{
    if (!outputFile.empty())
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

    if (!inputFile.empty())
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

    if (!errorFile.empty())
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
}

int main()
{
    string command;
    while (true)
    {
        cout << "$ ";
        getline(cin, command);

        if (command == "exit")
        {
            break;
        }

        vector<string> commands = split(command);

        // Find the pipe positions
        vector<size_t> pipePositions;
        for (size_t i = 0; i < commands.size(); ++i)
        {
            if (commands[i] == "|")
            {
                pipePositions.push_back(i);
            }
        }

        // Create child processes for each command in the pipeline
        vector<pid_t> pids;
        for (size_t i = 0; i <= pipePositions.size(); ++i)
        {
            int pipefd[2];

            if (i < pipePositions.size())
            {
                if (pipe(pipefd) == -1)
                {
                    cerr << "Pipe creation failed." << endl;
                    return 1;
                }
            }

            pid_t pid = fork();

            if (pid < 0)
            {
                cerr << "Forking failed." << endl;
                return 1;
            }
            else if (pid == 0)
            { // Child process
                string outputFile, inputFile, errorFile;

                // Handle I/O redirection
                for (size_t j = (i == 0 ? 0 : pipePositions[i - 1] + 1);
                     j < (i == pipePositions.size() ? commands.size() : pipePositions[i]);
                     ++j)
                {
                    if (commands[j] == ">")
                    {
                        if (j + 1 < commands.size()) // Check if the next index is valid
                        {
                            outputFile = commands[j + 1];
                            commands.erase(commands.begin() + j, commands.begin() + j + 2);
                            j--; // Adjust index after erasing
                        }
                        else
                        {
                            cerr << "Output redirection requires a filename." << endl;
                            exit(1);
                        }
                    }
                    else if (commands[j] == "<")
                    {
                        if (j + 1 < commands.size()) // Check if the next index is valid
                        {
                            inputFile = commands[j + 1];
                            commands.erase(commands.begin() + j, commands.begin() + j + 2);
                            j--; // Adjust index after erasing
                        }
                        else
                        {
                            cerr << "Input redirection requires a filename." << endl;
                            exit(1);
                        }
                    }
                    else if (commands[j] == "2>")
                    {
                        if (j + 1 < commands.size()) // Check if the next index is valid
                        {
                            errorFile = commands[j + 1];
                            commands.erase(commands.begin() + j, commands.begin() + j + 2);
                            j--; // Adjust index after erasing
                        }
                        else
                        {
                            cerr << "Error redirection requires a filename." << endl;
                            exit(1);
                        }
                    }
                }

                // Handle pipe connections
                if (i > 0)
                {
                    close(pipefd[1]);              // Close write end of previous pipe
                    dup2(pipefd[0], STDIN_FILENO); // Redirect stdin from previous pipe
                }

                if (i < pipePositions.size())
                {
                    close(pipefd[0]);               // Close read end of current pipe
                    dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to next pipe
                }

                // Apply I/O redirection
                apply_redirection(outputFile, inputFile, errorFile);

                // Execute the command
                vector<char *> args;
                for (size_t j = (i == 0 ? 0 : pipePositions[i - 1] + 1);
                     j < (i == pipePositions.size() ? commands.size() : pipePositions[i]);
                     ++j)
                {
                    if (commands[j] != "|")
                    {
                        args.push_back(const_cast<char *>(commands[j].c_str()));
                    }
                }
                args.push_back(nullptr);

                if (args.size() > 0 && execvp(args[0], &args[0]) == -1)
                {
                    perror("execvp");
                    exit(1);
                }
            }
            else
            { // Parent process
                pids.push_back(pid);
                if (i > 0)
                {
                    close(pipefd[0]); // Close read end of previous pipe
                }
                if (i < pipePositions.size())
                {
                    close(pipefd[1]); // Close write end of current pipe
                }
            }
        }

        // Wait for all child processes to finish
        for (pid_t pid : pids)
        {
            waitpid(pid, nullptr, 0);
        }
    }

    return 0;
}