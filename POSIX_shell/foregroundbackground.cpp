#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <cstring>
using namespace std;

void exec_fore_back(const char *command, char *const argv[], bool is_background)
{
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("Fork Failed");
        return;
    }
    else if (pid == 0)
    {
        if (is_background)
            ;
        // Child process
        else
        {
            if (execvp(command, argv) < 0)
            {
                perror("Execvp Failed");
                exit(EXIT_FAILURE);
            }
        }
    }
    else
    {
        // Parent process
        if (is_background)
        {
            std::cout << "Process running in background with PID: " << pid << "\n";
        }
        else
        {
            int stat;
            waitpid(pid, &stat, 0);

            if (WIFEXITED(stat))
            {
                std::cout << "Process " << pid << " exited with status " << WEXITSTATUS(stat) << "\n";
            }
        }
    }
}

void parse_command(const std::string &input, char *argv[], int &argc)
{
    char *token = strtok(const_cast<char *>(input.c_str()), " ");
    argc = 0;
    while (token != NULL && argc < 100)
    {
        argv[argc++] = token;
        token = strtok(nullptr, " ");
    }

    argv[argc] = nullptr;
}

void foreground(const std::string &input)
{
    bool is_background = false;
    char *argv[100];
    int argc = 0;

    parse_command(input, argv, argc);

    if (argc > 0)
    {
        exec_fore_back(argv[0], argv, is_background);
    }
}

void background(const std::string &input)
{
    bool is_background = true;
    char *argv[100];
    int argc = 0;

    parse_command(input, argv, argc);

    if (argc > 0)
    {
        exec_fore_back(argv[0], argv, is_background);
    }
}

// int main()
// {
//     std::string input;
//     while (true)
//     {
//         std::cout << "shell> ";
//         std::getline(std::cin, input);

//         if (input.empty())
//             continue;

//         if (input.back() == '&')
//         {
//             input.pop_back(); // Remove the '&' character
//             background(input);
//         }
//         else
//         {
//             foreground(input);
//         }
//     }

//     return 0;
// }