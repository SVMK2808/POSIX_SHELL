#ifndef EXECFUNCT_H
#define EXECFUNCT_H
#include <vector>
using namespace std;
void cd(int argc, vector<string> &args, char *cwd);
void echo(vector<string> &i);
void pwd();
void ls(int argc, vector<string> &input);
void pinfo(pid_t processid);
void IORedirect(string &command);
vector<string> autoComplete(const string &input);
void pipelineIORedirect(string &s);
void pipeline(string &command);
void foreground(const std::string &input);
void background(const std::string &input);
void history_disp(string &input);
bool search(string &directory, string &filename);
#endif