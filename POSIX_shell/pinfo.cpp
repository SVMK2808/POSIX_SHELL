#include <unistd.h>
#include <string>
#include <sys/stat.h>
#include <cstdlib>
#include <bits/stdc++.h>
#include <fcntl.h>
#include <iostream>

using namespace std;

string readfile(const string &filePath)
{
    int fd = open(filePath.c_str(), O_RDONLY);
    if (fd == -1)
    {
        return "N/A";
    }
    char buffer[4096];
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);
    if (bytesRead == -1)
    {
        close(fd);
        return "N/A";
    }
    buffer[bytesRead] = '\0';
    close(fd);
    return string(buffer);
}

string getMemoryUsage(const string &pid)
{
    string filePath = "/proc/" + pid + "/status";
    int fd = open(filePath.c_str(), O_RDONLY);
    if (fd == -1)
    {
        return "Memory usage info not found";
    }

    char buffer[4096];
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);
    if (bytesRead == -1)
    {
        close(fd);
        return "Memory usage info not found";
    }
    buffer[bytesRead] = '\0';
    close(fd);

    string content(buffer);
    size_t pos = content.find("VmRSS:");
    if (pos != string::npos)
    {
        size_t endPos = content.find('\n', pos);
        return content.substr(pos, endPos - pos);
    }
    return "Memory usage info not found";
}

string Process_stat(const string &pid)
{
    string filePath = "/proc/" + pid + "/status";
    int fd = open(filePath.c_str(), O_RDONLY);
    if (fd == -1)
    {
        return "Process info not found";
    }
    char buffer[4096];
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer));
    if (bytesRead == -1)
    {
        close(fd);
        return "Process info not found";
    }
    buffer[bytesRead] = '\0';
    close(fd);

    string content(buffer);
    size_t pos = content.find("State:");
    if (pos != string::npos)
    {
        size_t endPos = content.find('\n', pos);
        return content.substr(pos, endPos - pos);
    }
    return "Process status info not found";
}

string get_exec_path(string pid)
{
    char execRealPath[4096];
    ssize_t len = readlink(("/proc/" + pid + "/exe").c_str(), execRealPath, sizeof(execRealPath) - 1);

    if (len != -1)
    {
        execRealPath[len] = '\0';
        return string(execRealPath);
    }
    return "Executable path not found";
}

void pinfo(pid_t processid)
{

    string Pid = to_string(processid);
    string Proc_stat = Process_stat(Pid);
    string Proc_Mem_usage = getMemoryUsage(Pid);
    string exec_path = get_exec_path(Pid);

    cout << "PID :" << Pid << "\n";
    cout << "Process Status:" << Proc_stat << "\n";
    cout << "Memory" << Proc_Mem_usage << "\n";
    cout << "Executable Path --" << exec_path << "\n";
}