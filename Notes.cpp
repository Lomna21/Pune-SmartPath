// Sure, let's break down the roles of istringstream, stname >> temp, and isdigit(c) in detail.

// 1. istringstream stname(key)
// Role: The istringstream class is used to create an input string stream object from a given string. In this case, stname is created from the key string, which represents a station name.

// Purpose: It allows the program to treat the string key as an input stream, enabling operations like reading words from the string using the >> operator, similar to how you would read from standard input (cin).

// Detailed Explanation:

// istringstream is a part of the C++ Standard Library, specifically from the <sstream> header.
// It provides the functionality of an input stream but operates on a string rather than an external input source like the keyboard or a file.

// Example:

// std::string str = "Metro Station Name";
// std::istringstream iss(str);
// std::string word;
// while (iss >> word) {
//     std::cout << word << std::endl;
// }
// This code will output each word from the string str on a new line.

//-------------------------------------------------------------------------------------
// 2. stname >> temp

// Role: The >> operator reads from the stname input string stream into the temp string.
// Purpose: It extracts words (separated by whitespace) from the string stream stname and stores each word in the temp string.

// Detailed Explanation:

// When you use >> with an istringstream object, it reads the next word from the stream and stores it in the specified variable (temp in this case).
// The reading process skips any leading whitespace and continues until it encounters another whitespace character or the end of the string.

// Example:
// Given std::istringstream iss("Metro Station Name");, the following loop:


// std::string word;
// while (iss >> word) {
//     std::cout << word << std::endl;
// }

// will output:
// Metro
// Station
// Name

//----------------------------------------------------------------------------------------
// 3. isdigit(c)

// Role: The isdigit function checks if the character c is a decimal digit (0-9).

// Purpose: It is used to determine whether a character in the string is a digit. This helps in extracting numerical parts from the station name to form the station code.

// Detailed Explanation:

// isdigit is a function defined in the <cctype> header.
// It returns a non-zero value (true) if the character passed to it is a digit (0-9), and zero (false) otherwise.
// It helps in distinguishing between numerical characters and alphabetic characters when processing the station name.

// Example:
// char c = '5';
// if (isdigit(c)) {
//     std::cout << c << " is a digit." << std::endl;
// } else {
//     std::cout << c << " is not a digit." << std::endl;
// }

// This will output:
// 5 is a digit.