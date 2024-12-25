#include <unistd.h>
#include <iostream>
#include <string>
#include <limits.h>
#include <fcntl.h>
using namespace std;
void pwd()
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("Unable to get the present directory");
        exit(EXIT_FAILURE);
    }
    write(STDOUT_FILENO, cwd, strlen(cwd));
    write(STDOUT_FILENO, "\n", 1);
}