#include <iostream>
#include <unistd.h>
#include <vector>
#include <string>
#include <stdexcept> // Include for exception handling
using namespace std;
const size_t MAX_HIST_SIZE = 20;

vector<string> history;

void disp_hist(const vector<string> &args)
{
    int num = 0;
    if (args.size() > 1)
    {
        try
        {
            num = min((int)MAX_HIST_SIZE, stoi(args[1]));
        }
        catch (const invalid_argument &e)
        {
            cout << "Invalid number format. Displaying last entries instead.\n";
            num = MAX_HIST_SIZE; // Default to MAX_HIST_SIZE if conversion fails
        }
        catch (const out_of_range &e)
        {
            cout << "Number is out of range. Displaying last entries instead.\n";
            num = MAX_HIST_SIZE; // Default to MAX_HIST_SIZE if out of range
        }
    }

    num = min(num, (int)history.size()); // Ensure we do not exceed available history
    for (int i = max(0, (int)history.size() - num); i < history.size(); i++)
    {
        cout << history[i] << "\n";
    }
}

void history_disp(string &input)
{
    if (history.size() >= MAX_HIST_SIZE)
    {
        history.erase(history.begin());
    }
    history.push_back(input);

    disp_hist(history);
}
