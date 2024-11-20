#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <vector>

// Color definitions
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN = "\033[36m";
const std::string RESET = "\033[0m";

// Display functions
void clearConsole();
void centerText(const std::string& text);
void displayLogo(int artdis);
void displayCenteredMenu(const std::vector<std::string>& menuItems, const std::string& title);


// Input functions
void getUserInput(const std::string& prompt, std::string& input);
void getUserInput(const std::string& prompt, int& input);
void getIntegerInput(const std::string& prompt, int& input, int min = INT_MIN, int max = INT_MAX);
bool isInteger(const std::string& str);
std::string getHiddenPassword();


#endif // DISPLAY_H