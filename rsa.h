#include <list>
#include "mod.h"

using namespace std;

list<pair<int, int>> public_key_catalogue = {};
list<tuple<int, int, int>> private_key = {};

pair<int, int> get_public_key(int sender_id)
{
    if (sender_id < 0 || sender_id >= public_key_catalogue.size())
    {
        throw runtime_error("Invalid key owner ID");
    }
    return *next(public_key_catalogue.begin(), sender_id);
}

tuple<int, int, int> get_private_key(int sender_id)
{
    if (sender_id < 0 || sender_id >= private_key.size())
    {
        throw runtime_error("Invalid key owner ID");
    }
    return *next(private_key.begin(), sender_id);
}

void RSA_key_generation()
{
    cout << "Choose two different prime numbers p and q by index from 0 to " << primes.size() - 1 << " (inclusive):" << endl;

    int p = primes[stoi(input([&](const string &s)
                              { return is_integer(s, 1, primes.size() - 1); }))];
    cout << "You chose p = " << p << endl;

    int q = primes[stoi(input([&](const string &s)
                              { return (is_integer(s, 1, primes.size() - 1) && stoi(s) != p); }))];
    cout << "You chose q = " << q << endl;

    int n = p * q;
    cout << "n = p * q = " << n << endl;

    int T = (p - 1) * (q - 1);
    cout << "T = (p - 1) * (q - 1) = " << T << endl;

    cout << "Choose an integer e such that 1 < e < T = " << T << " and gcd(e, T) = 1:" << endl;
    int e = stoi(input([&](const string &s)
                       { return is_integer(s, 1, T - 1) && gcd(stoi(s), T) == 1; }));
    cout << "You chose e = " << e << endl;

    int d = mod_inverse(e, T);
    cout << "d = mod_inverse(e, T) = " << d << ", so e*d ≡ " << e << "*" << d << " ≡ 1 mod T (" << T << ")" << endl;

    cout << "Public key: (" << e << ", " << n << ")" << endl;
    cout << "Private key: (" << d << ", " << n << ")" << endl;
    cout << "Do you want to save the keys? (y/n)" << endl;

    if (yes_or_no_input())
    {
        public_key_catalogue.push_back(make_pair(e, n));
        private_key.push_back(make_tuple(d, p, q));

        if (public_key_catalogue.size() != private_key.size())
        {
            throw runtime_error("Public and private key catalogues are out of sync");
        }

        cout << "Keys saved. The key owner ID is " << public_key_catalogue.size() - 1 << endl;
    }
}

void auto_RSA_key_generation()
{
    int p = primes[rand() % primes.size()];
    int q = primes[rand() % primes.size()];

    while (p == q)
    {
        q = primes[rand() % primes.size()];
    }

    int n = p * q;
    int T = (p - 1) * (q - 1);

    int e = 3;
    while (gcd(e, T) != 1)
    {
        e += 2; // Increment by 2 to ensure e remains odd
    }

    int d = mod_inverse(e, T);

    public_key_catalogue.push_back(make_pair(e, n));
    private_key.push_back(make_tuple(d, p, q));

    if (public_key_catalogue.size() != private_key.size())
    {
        throw runtime_error("Public and private key catalogues are out of sync");
    }

    cout << "Auto-generated keys saved. The key owner ID is " << public_key_catalogue.size() - 1 << endl;
}

/// @brief Rivest-Shamir-Adleman algorythm (RSA) using the Chinese Remainder Theorem (CRT) to decipher a message.
int decipher_RSA_using_CRT(int ciphered_message, int d, int p, int q)
{
    header("Decipher RSA using the Chinese Remainder Theorem (CRT)");

    int dp = d % (p - 1);
    int dq = d % (q - 1);

    int invq_modp = mod_inverse(q, p);
    int invp_modq = mod_inverse(p, q);

    int xp = ciphered_message % p == 0 ? 0 : mod_pow(ciphered_message, dp, p);
    int xq = ciphered_message % q == 0 ? 0 : mod_pow(ciphered_message, dq, q);

    cout << "The receiver deciphers the message using the Chinese Remainder Theorem (CRT):" << endl;
    cout << "From Fermat's little theorem we know that mssg^d mod p = mssg^(d mod (p - 1)) mod p  =  xp and mssg^d mod q = mssg^(d mod (q - 1)) mod q  =  xq. Let's call them: " << endl;
    cout << "dp = d mod (p - 1) = " << dp << endl;
    cout << "xp = mssg^dp mod p = " << xp << endl;
    cout << "dq = d mod (q - 1) = " << dq << endl;
    cout << "xq = mssg^dq mod q = " << xq << endl << endl;

    cout << "According to CRT the true message is (xq * p * invp_modq + xp * q * invq_modp) mod (p * q)\nNote that for this deciphering type we need to store p and q, and not just n = p*q" << endl;
    cout << "invq_modp = q^-1 mod p = " << invq_modp << endl;
    cout << "invp_modq = p^-1 mod q = " << invp_modq << endl;

    long long message = static_cast<long long>(xq) * p * invp_modq +
                        static_cast<long long>(xp) * q * invq_modp;
    return static_cast<int>(message % (static_cast<long long>(p) * q));
}

/// @brief Rivest-Shamir-Adleman algorythm (RSA)
void RSA()
{
    header("Rivest-Shamir-Adleman algorythm (RSA)");

    while (public_key_catalogue.size() < 1)
    {
        cout << "There are too few key pairs.\nDo you want to generate a key pair? (y/n)" << endl;
        if (yes_or_no_input())
        {
            RSA_key_generation();
        }
        else
        {
            {
                cout << "Not enough keys available. Generating a key pair..." << endl;
                auto_RSA_key_generation();
            }
        }
    }

    int catalogue_length = public_key_catalogue.size();

    cout << "Enter the receiver's ID (0 to " << catalogue_length - 1 << "):" << endl;
    int receiver = stoi(input([&](const string &s)
                              { return is_integer(s, 0, catalogue_length - 1); }));

    auto [e, n] = get_public_key(receiver);
    cout << "You found the receiver's public key in the public catalogue, which is: (" << e << ", " << n << "). Now you can send him a message away from prying eyes!" << endl;

    cout << "Enter the message to send (as an integer 0 < mssg < n-1 = " << n-1 << "):" << endl;
    int message = stoi(input([&](const string &s)
                             { return is_integer(s, 0, n - 1); }));

    int ciphered_message = mod_pow(message, e, n);
    cout << "The message is ciphered to mssg^e mod n = " << ciphered_message << endl
         << "Now you can send the ciphered message to the receiver!" << endl;

    auto [d, p, q] = get_private_key(receiver);
    cout << "The receiver gets the ciphered message and takes his private key (d = e^-1 mod T, p, q [p*q = n]) (" << d << ", " << p << ", " << q << ", which you do not know) and deciphers the message" << endl;

    int deciphered_message;

    cout << "Choose an option:" << endl;
    cout << "1. using the standard method" << endl;
    cout << "2. using Chinese Reaminder Theorem" << endl;
    cout << "0. Exit" << endl;
    switch (stoi(input([&](const string &s)
                       { return is_integer(s, 0, 2); })))
    {
    case 0:
        cout << "Exiting..." << endl;
        return;

    case 1:
        cout << "The receiver deciphers the message using the standard method: mssg^d mod n" << endl;
        deciphered_message = mod_pow(ciphered_message, d, p * q);
        break;

    case 2:
        deciphered_message = decipher_RSA_using_CRT(ciphered_message, d, p, q);
        break;

    default:
        cout << "Invalid choice." << endl;
        break;
    }

    cout << "The receiver now has the message: " << deciphered_message << endl;
}