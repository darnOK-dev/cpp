#include <functional>
#include <iostream>
#include <string>
#include <limits>

using namespace std;

void header(string s)
{
    cout << "\n\n " + string(s.size() + 2, '=') + "\n| " + s + " |\n " + string(s.size() + 2, '=') + "\n";
}

string input(
    const function<bool(const string&)>& is_valid)
{
    string value;
    
    while (cin >> value)
    {
        if (value == "q")
        {
            throw runtime_error("User quit the program");
        }

        if (is_valid(value))
        {
            return value;
        }

        cout << "Invalid input. Try again, or enter q to quit:\n";
    }

    throw runtime_error("Input stream closed unexpectedly");
}

bool is_integer(const string& s, int min = numeric_limits<int>::min(), int max = numeric_limits<int>::max())
{
    if (s.empty() || (!isdigit(s[0]) && s[0] != '-' && s[0] != '+'))
    {
        return false;
    }
    
    char* p;
    long value = strtol(s.c_str(), &p, 10);
    
    return *p == 0 && value >= min && value <= max;
}

bool is_yes_or_no(const string& s)
{
    char c = s[0];
    return (c == 'y' || c == 'Y' || c == 'n' || c == 'N') && s.size() <= 3;
}

/// @return true if yes, false if no
bool yes_or_no_input()
{
    char c = input([&](const string& s) { return is_yes_or_no(s); })[0];
    if (c == 'y' || c == 'Y')
    {
        return true;
    }
    else if (c == 'n' || c == 'N')
    {
        return false;
    }
    else
    {
        throw runtime_error("Invalid input");
    }
}
