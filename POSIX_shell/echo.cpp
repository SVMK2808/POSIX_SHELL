#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

// Function to process input for 'echo', maintaining spaces inside double quotes, removing double quotes, and keeping single quotes within double quotes
string input_processor(string &s)
{
    string result;
    bool inside_double_quotes = false;
    bool last_was_space = false;

    if (s.empty())
    {
        return result; // Return empty string if input is empty
    }

    for (ssize_t i = 0; i < s.length(); i++)
    {
        char c = s[i];

        // Handle double quotes (remove them but keep content inside)
        if (c == '"')
        {
            inside_double_quotes = !inside_double_quotes; // Toggle the state
            continue;                                     // Skip adding the double quotes to the result
        }
        // Handle single quotes (preserve them even inside double quotes)
        else if (c == '\'')
        {
            result += c;
        }
        else if (c == '\\' && i + 1 < s.length()) // Check for escape sequences
        {
            char next_char = s[i + 1];
            if (next_char == 'n')
            {
                result += '\n'; // Convert \n to newline
                i++;            // Skip the next character
            }
            else if (next_char == 't')
            {
                result += '\t'; // Convert \t to tab
                i++;            // Skip the next character
            }
            else
            {
                result += c; // If not an escape sequence, add the backslash
            }
        }
        else
        {
            // Maintain spaces outside of quotes
            if (!inside_double_quotes && isspace(c))
            {
                if (!last_was_space)
                {
                    result += ' '; // Only add one space for consecutive spaces outside quotes
                    last_was_space = true;
                }
            }
            else
            {
                result += c; // Append character as is inside quotes or non-space characters outside
                last_was_space = false;
            }
        }
    }

    // Remove trailing spaces if any
    if (!result.empty() && isspace(result.back()))
    {
        result.pop_back();
    }

    return result;
}

// 'echo' function to print the processed input
void echo(vector<string> &input)
{
    // Concatenate all parts of input for echo
    string s;
    for (size_t i = 1; i < input.size(); ++i)
    {
        if (i > 1)
        {
            s += ' '; // Add space between arguments
        }
        s += input[i];
    }

    // Process input to handle quotes and spaces properly
    string processed_input = input_processor(s);

    // Write the processed input to stdout
    write(STDOUT_FILENO, processed_input.c_str(), processed_input.length());
    write(STDOUT_FILENO, "\n", 1);
}

// int main()
// {
//     string input;
//     cout << "Enter command: ";
//     getline(cin, input);

//     // Split the input string into words using space as delimiter
//     vector<string> words;
//     size_t pos = 0, prev = 0;
//     while ((pos = input.find_first_of(' ', prev)) != string::npos)
//     {
//         words.push_back(input.substr(prev, pos - prev));
//         prev = pos + 1;
//     }
//     words.push_back(input.substr(prev));

//     // Check if the first word is "echo"
//     if (words.size() > 0 && words[0] == "echo")
//     {
//         echo(words);
//     }
//     else
//     {
//         cout << "Invalid command" << endl;
//     }

//     return 0;
// }