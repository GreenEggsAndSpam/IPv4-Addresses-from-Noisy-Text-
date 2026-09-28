#include <iostream>
#include <string>
#include <cctype>

using namespace std;


// Checks whether the character can be part of an IPv4 candidate token.
bool isTokenChar(char c)
{
    return isdigit(static_cast<unsigned char>(c)) ||
           c == '.' ||
           c == ':';
}


// Parses digits from token[start, end).
// Returns false if:
//   - there are no digits
//   - there is a leading zero
//   - there are too many digits
//   - the value is greater than maxValue
bool parseNumber(const string& token,
                 size_t start,
                 size_t end,
                 int maxDigits,
                 unsigned long maxValue,
                 unsigned long& value)
{
    if (start >= end)
        return false;

    size_t length = end - start;

    if (length > static_cast<size_t>(maxDigits))
        return false;

    // Leading zeros are not allowed unless the number is exactly 0.
    if (length > 1 && token[start] == '0')
        return false;

    value = 0;

    for (size_t i = start; i < end; i++)
    {
        if (!isdigit(static_cast<unsigned char>(token[i])))
            return false;

        // Manual digit accumulation
        value = value * 10 + (token[i] - '0');

        if (value > maxValue)
            return false;
    }

    return true;
}


// Attempts to parse one complete candidate token.
bool parseCandidate(const string& token,
                    unsigned long& address,
                    int& port)
{
    address = 0;
    port = -1;

    size_t pos = 0;

    // Parse exactly four octets.
    for (int octetNumber = 0; octetNumber < 4; octetNumber++)
    {
        size_t start = pos;

        // Move through the digits of this octet.
        while (pos < token.size() &&
               isdigit(static_cast<unsigned char>(token[pos])))
        {
            pos++;
        }

        unsigned long octet;

        if (!parseNumber(token, start, pos, 3, 255, octet))
            return false;

        // Build the 32-bit address.
        address = (address << 8) | octet;

        // First three octets must be followed by periods.
        if (octetNumber < 3)
        {
            if (pos >= token.size() || token[pos] != '.')
                return false;

            pos++;
        }
    }

    // If we reached the end, there is no port.
    if (pos == token.size())
    {
        port = -1;
        return true;
    }

    // Anything after the fourth octet must begin with a colon.
    if (token[pos] != ':')
        return false;

    pos++;

    size_t portStart = pos;

    while (pos < token.size() &&
           isdigit(static_cast<unsigned char>(token[pos])))
    {
        pos++;
    }

    // Nothing may remain after the port.
    if (pos != token.size())
        return false;

    unsigned long portValue;

    if (!parseNumber(token, portStart, pos, 5, 65535, portValue))
        return false;

    port = static_cast<int>(portValue);

    return true;
}


// Returns true if a valid address was found, false otherwise.
// On success: outAddress holds the 32-bit value,
// and outPort holds the port number, or -1 if no port was present.
// On failure: outAddress is set to 0 and outPort is set to -1.
bool extractIPv4(const string& str,
                 unsigned long& outAddress,
                 int& outPort)
{
    outAddress = 0;
    outPort = -1;

    size_t i = 0;

    while (i < str.size())
    {
        // Skip garbage characters.
        if (!isTokenChar(str[i]))
        {
            i++;
            continue;
        }

        // Find the entire run of digits, periods, and colons.
        size_t start = i;

        while (i < str.size() && isTokenChar(str[i]))
        {
            i++;
        }

        // The entire run must be valid.
        string candidate = str.substr(start, i - start);

        unsigned long address;
        int port;

        if (parseCandidate(candidate, address, port))
        {
            outAddress = address;
            outPort = port;
            return true;
        }
    }

    return false;
}


int main()
{
    string input;

    while (true)
    {
        cout << "Enter text: ";
        getline(cin, input);

        if (input == "END")
        {
            cout << "Program terminated." << endl;
            break;
        }

        unsigned long address;
        int port;

        if (extractIPv4(input, address, port))
        {
            // Recover the four octets from the 32-bit value.
            unsigned long a = (address >> 24) & 255;
            unsigned long b = (address >> 16) & 255;
            unsigned long c = (address >> 8) & 255;
            unsigned long d = address & 255;

            cout << "Extracted IPv4 address: "
                 << a << "."
                 << b << "."
                 << c << "."
                 << d
                 << " (decimal value: "
                 << address
                 << ", port: ";

            if (port == -1)
                cout << "none";
            else
                cout << port;

            cout << ")" << endl;
        }
        else
        {
            cout << "No valid IPv4 address found." << endl;
        }
    }

    return 0;
}