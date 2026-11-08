#include <string>
#include <bits/stdc++.h>
#include "usr_comms.h"
#include "rsa.h"

using namespace std;

int main()
{
    while (true)
    {
        header("Discrete Mathematics");
        cout << "Choose an option:\n1. RSA_key_generation\n2. RSA\n0. Exit" << endl;
        switch (stoi(input([&](const string& s) { return is_integer(s, 0, 2); })))
        {
            case 0:
                cout << "Exiting..." << endl;
                return 0;

            case 1:
                RSA_key_generation();
                break;
            
            case 2:
                RSA();
                break;
            
            default:
                cout << "Invalid choice." << endl;
                break;
        }
    }
}
