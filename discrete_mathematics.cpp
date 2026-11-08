#include <string>
#include <bits/stdc++.h>
#include "usr_comms.h"

using namespace std;

// Careful when adding more, rn the biggest prime 1223 squared is within the range of int, but if you add more primes, it will go out of range and cause an overflow.
array<int, 200> primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997, 1009, 1013, 1019, 1021, 1031, 1033, 1039, 1049, 1051, 1061, 1063, 1069, 1087, 1091, 1093, 1097, 1103, 1109, 1117, 1123, 1129, 1151, 1153, 1163, 1171, 1181, 1187, 1193, 1201, 1213, 1217, 1223};

// @brief Greatest common divisor (GCD) using the Euclidean algorithm
// @param xy The coefficients of Bézout's identity: a*x + b*y = gcd(a, b)
int extended_euclidean_algorithm(int a, int b, pair<int,int>& xy)
{
    if (a == 0)
    {
        xy.first = 0;
        xy.second = 1;
        return b;
    }

    pair<int, int> xy1;
    int gcd = extended_euclidean_algorithm(b % a, a, xy1);

    xy.first = xy1.second - (b / a) * xy1.first;
    xy.second = xy1.first;

    return gcd;
}

int gcd(int a, int b)
{
    if (b == 0) return a;
    return gcd(b, a % b);
}

int mod_inverse(int a, int m)
{
    pair<int, int> xy;
    int g = extended_euclidean_algorithm(a, m, xy);
    if (g != 1)
    {
        throw runtime_error("Modular inverse does not exist");
    }
    else
    {
        return xy.first;
    }
}

int mod_pow(int base, int exp, int mod)
{
    if (mod == 1) return 0;
    int result = 1;
    base = base % mod;
    while (exp > 0)
    {
        if (exp % 2 == 1) // If exp is odd
        {
            result = (result * base) % mod;
        }
        exp = exp >> 1; // Divide exp by 2
        base = (base * base) % mod;
    }
    return result;
}

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
    cout << "Choose two prime numbers p and q by index from 0 to " << primes.size() - 1 << " (inclusive):" << endl;

    int p = primes[stoi(input([&](const string &s)
                              { return is_integer(s, 0, primes.size() - 1); }))];
    cout << "You chose p = " << p << endl;

    int q = primes[stoi(input([&](const string &s)
                              { return is_integer(s, 0, primes.size() - 1); }))];
    cout << "You chose q = " << q << endl;

    int n = p * q;
    cout << "n = p * q = " << n << endl;

    int T = (p - 1) * (q - 1);
    cout << "T = (p - 1) * (q - 1) = " << T << endl;

    cout << "Choose an integer e such that 1 < e < T and gcd(e, T) = 1:" << endl;
    int e = stoi(input([&](const string &s)
                       { return is_integer(s, 1, T - 1) && gcd(stoi(s), T) == 1; }));
    cout << "You chose e = " << e << endl;

    int d = mod_inverse(e, T);
    cout << "d = mod_inverse(e, T) = " << d << endl;

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

    int xp = mod_pow(ciphered_message, dp, p);
    int xq = mod_pow(ciphered_message, dq, q);

    cout << "The receiver deciphers the message using the Chinese Remainder Theorem (CRT):" << endl;
    cout << "From Fermat's little theorem we know that mssg^d mod p = mssg^(d mod (p - 1)) mod p  =  xp and mssg^d mod q = mssg^(d mod (q - 1)) mod q  =  xq. Let's call them: " << endl;
    cout << "dp = d mod (p - 1) = " << dp << endl;
    cout << "dq = d mod (q - 1) = " << dq << endl;
    cout << "xp = mssg^dp mod p = " << xp << endl;
    cout << "xq = mssg^dq mod q = " << xq << endl << endl;

    cout << "According to CRT the true message is (xq * p * invp_modq + xp * q * invq_modp) mod (p * q)\nNote that for this deciphering type we need to store p and q, and not just n=p*q"<< endl;
    cout << "invq_modp = q^-1 mod p = " << invq_modp << endl;
    cout << "invp_modq = p^-1 mod q = " << invp_modq << endl;

    return (xq * p * invp_modq + xp * q * invq_modp) % (p * q);
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
    int receiver = stoi(input([&](const string &s) { return is_integer(s, 0, catalogue_length - 1); }));
    
    auto [e, n] = get_public_key(receiver);
    cout << "You found the receiver's public key in the public catalogue, which is: (" << e << ", " << n << "). Now you can send hime your message!" << endl;

    cout << "Enter the message to send (as an integer):" << endl;
    int message = stoi(input([&](const string &s) { return is_integer(s, 0); }));
    cout << "You entered the message: "<< message << endl;

    int ciphered_message = mod_pow(message, e, n);
    cout << "The message is ciphered to mssg^e mod n = " << ciphered_message << endl << "Now you can send the ciphered message to the receiver!" << endl;

    auto [d, p, q] = get_private_key(receiver);
    cout << "The receiver gets the ciphered message and takes his private key (e^-1 mod n, p, q [p*q = n]) (" << d << ", " << p << ", "<< q << ", which you do not know) and deciphers the message" << endl;

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



int main()
{
    while (true)
    {
        header("Discrete Mathematics");
        cout << "Choose an option:" << endl;
        cout << "1. RSA_key_generation" << endl;
        cout << "2. RSA" << endl;
        cout << "0. Exit" << endl;
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
