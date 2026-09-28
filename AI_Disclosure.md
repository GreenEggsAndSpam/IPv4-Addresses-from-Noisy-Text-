AI use
The model used was ChatGPT's GPT-6 Sol light model 
Date: 9/27/26

I used to generate the first C++ solution. My initial prompt was this:

Write a C or C++ program that reads a line of text and extracts a single valid IPv4 address, optionally followed by a port number, embedded anywhere in that text.

Token Rules
Only digits, periods (.), and colons (:) can be part of a valid token.
Every other character is garbage and is skipped.
A candidate token must match the address grammar in full. No partial matches, and no truncating a longer run to find a valid piece inside it.
Address Grammar

Address: four octets separated by periods (octet.octet.octet.octet).

Each octet is 1–3 digits.
Each octet has a value from 0 to 255.
No leading zero unless the value is exactly 0.

Port (optional): :port may follow the fourth octet.

1–5 digits.
Value from 0 to 65535.
Same leading-zero rule as octets.
If a colon is present, the port must be fully valid, or the entire match (address included) is rejected.
Reject the Token If
The octet count is wrong.
An octet is empty.
An octet or the port is out of range.
A leading zero is not allowed.
There is a second colon.
A colon does not come immediately after the fourth octet.
A stray period or colon is directly adjacent to an otherwise-valid address.

Restrictions

Not allowed:

Any string-to-number conversion function: atoi, atol, atoll, strtol, strtoul, strtod, stoi, stol, stoul, sscanf, or scanf with numeric conversions.
Any address-parsing library function: inet_aton, inet_pton, inet_addr, or equivalents.
Any regular-expression facility (std::regex, POSIX regex.h, or similar). The parsing and validation logic must be your own character-by-character code.

Input loop (main)

Continuously prompt the user for input.
Stop when the user enters END (case-sensitive).
Then print Program terminated. and exit.


I also asked ChatGPT about this compiler warning:
I am getting this warning how can I fix it? 
main.cpp:50:40: warning: implicit conversion changes signedness: 'int' to 'unsigned long'
          [-Wsign-conversion]
       50 |         value = value * 10 + (token[i] - '0');
          |                            ~  ~~~~~~~~~^~~~~

The original ChatGPT response supplied the token scan, digit parser, candidate validation, and main input loop. ChatGPT explained that the digit expression is an int and suggested casting it after checking that it is a digit. 


Review and tests
The important rule in the code is that an entire consecutive run of digits, periods, and colons is one candidate. It cannot accept part of a malformed run, such as 999.192.168.1.1 or 1.2.3.4:65536. The number parser checks length, leading zeros, and range while accumulating digits. The four octets are shifted into a 32-bit address. The output parameters are set to 0 and -1 if no valid candidate is found.
I tried test cases covering valid addresses, numeric limits, leading zeros, malformed ports, and extra punctuation. I have not found any bugs yet. 

Verification statement
I reviewed the submitted code and understand how each part works. I tested valid addresses, malformed addresses, and boundary values. The results matched my expectations. I am not aware of any remaining bugs.


