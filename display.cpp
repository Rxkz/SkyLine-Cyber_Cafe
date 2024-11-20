//display.cpp
#include "display.h"
#include <iostream>
#include <conio.h>
#include <iostream>
#include"logo.h"
#include <thread>
#include <chrono>
#include <string>
#include <windows.h> 
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;


void clearConsole() {
#ifdef _WIN32
    // Clear screen
    system("cls");
    // Reset cursor to top
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordScreen = { 0, 0 };
    SetConsoleCursorPosition(hConsole, coordScreen);
#else
    system("clear");
#endif
}

void centerText(const std::string& text) {
    const int CONSOLE_WIDTH = 80; // Default console width
    int padding = (CONSOLE_WIDTH - text.length()) / 2;
    if (padding > 0) {
        std::cout << std::string(padding, ' ');
    }
    std::cout << text << std::endl;
}

void displayLogo(int artChoice) {
    // Clear console and reset cursor position
    clearConsole();

    // Configure console
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Set console to appropriate size and clear it
    system("mode con: cols=200 lines=40");

    string art1[] = {
       "\t\t\t\t                                            /\\",
       "\t\t\t\t                                            **",
       "\t\t\t\t                                           [||]",
       "\t\t\t\t                                          (====)",
       "\t\t\t\t                                           [||]",
       "\t\t\t\tLLL         OOOOOOO    GGGGGGG             [||]     NNN    NN",
       "\t\t\t\tLLL         OOO OOO    GGG  GG             [||]     NNNN   NN",
       "\t\t\t\tLLL         OOO OOO    GGG                 [||]     NNN NN NN",
       "\t\t\t\tLLL         OOO OOO    GGG GGGG            [||]     NNN  NNNN",
       "\t\t\t\tLLL         OOO OOO    GGG  GGG          ========   NNN   NNN",
       "\t\t\t\tLLLLLLLL    OOOOOOO     GGGGGG          %%%%%%%%%   NNN    NN",
       "\t\t\t\t                                                               "
    };

    string art2[] = {
        "\t\t\t\t              /\\",
        "\t\t\t\t              **",
        "\t\t\t\t             [||]",
        "\t\t\t\t            (====)",
        "\t\t\t\t             [||]",
        "\t\t\t\tSSSSSSS      [||]    GGGGGGG    NNN    NN        UUUU    UUUU    PPPPPPP ",
        "\t\t\t\tSSSS         [||]    GGGG  GG   NNNNN  NN        UUUU    UUUU    PPPP  PP",
        "\t\t\t\tSSSSSSS      [||]    GGGG       NNN NNNNN        UUUU    UUUU    PPPPPPP ",
        "\t\t\t\t    SSSS     [||]    GGGG GGGG  NNN  NNNN        UUUU    UUUU    PPPP    ",
        "\t\t\t\t    SSSS   ========  GGGG  GGG  NNN   NNN        UUUU    UUUU    PPPP    ",
        "\t\t\t\tSSSSSSS   %%%%%%%%%%  GGGGGG    NNN    NN         UUUUUUUUUU     PPPP    ",
        "\t\t\t\t                                                               "
    };

    string art3[] = {
        "\t\t\t\t                                        /\\",
        "\t\t\t\t                                        **",
        "\t\t\t\t                                       [||]",
        "\t\t\t\t                                      (====)",
        "\t\t\t\t                                       [||]",
        "\t\t\t\tAAAAAAAA    DDDDDDDD    MMM    MMM     [||]     NNN    NN",
        "\t\t\t\tAAA  AAA    DDD   DD    MMMM  MMMM     [||]     NNNN   NN",
        "\t\t\t\tAAAAAAAA    DDD   DD    MMM MM MMM     [||]     NNN NN NN",
        "\t\t\t\tAAA  AAA    DDD   DD    MMM    MMM     [||]     NNN  NNNN",
        "\t\t\t\tAAA  AAA    DDD   DD    MMM    MMM   ========   NNN   NNN",
        "\t\t\t\tAAA  AAA    DDDDDDDD    MMM    MMM  %%%%%%%%%   NNN    NN",
        "\t\t\t\t                                                               "
    };

    // Display selected logo with animation
    const string* selectedArt = nullptr;
    int artSize = 12;  // All arts have 12 lines

    switch (artChoice) {
    case 1: selectedArt = art1; break;
    case 2: selectedArt = art2; break;
    case 3: selectedArt = art3; break;
    default: return;  // Invalid choice, return without showing anything
    }

    // Display the selected art with animation
    for (int i = 0; i < artSize; i++) {
        cout << selectedArt[i] << endl;
        Sleep(50);  // Animation delay
        cout.flush();
    }

    // Add some spacing after the logo
    cout << "\n\n";
}

void displayCenteredMenu(const std::vector<std::string>& menuItems, const std::string& title) {
    clearConsole();
    std::cout << "\n\n\n\n\n\n\n";
    cout << "\n";

    const string EXTRA_PADDING = "\t\t\t";

    if (!title.empty()) {
        std::cout << "\n";
        centerText(YELLOW + EXTRA_PADDING + title + RESET);
        std::cout << "\n";
    }

    const int ITEMS_PER_ROW = 2;
    const int ITEM_WIDTH = 35;
    const int TOTAL_WIDTH = ITEM_WIDTH * ITEMS_PER_ROW + 10;

    // Create top border with padding
    std::string topBorder = EXTRA_PADDING + "+" + std::string(TOTAL_WIDTH - 2, '-') + "+";
    centerText(topBorder);

    // Create the vertical separator
    std::string separator = EXTRA_PADDING + "|" + std::string(TOTAL_WIDTH - 2, '-') + "|";

    // Display items in grid
    for (size_t i = 0; i < menuItems.size(); i += ITEMS_PER_ROW) {
        // Create base line with consistent spacing
        std::string line = EXTRA_PADDING + "|";

        // Add first menu item
        std::string item1 = (i < menuItems.size()) ? menuItems[i] : "";
        line += " " + item1 + std::string(ITEM_WIDTH - item1.length() - 1, ' ');

        // Add separator and second menu item if it exists
        if (i + 1 < menuItems.size()) {
            std::string item2 = menuItems[i + 1];
            line += "|" + std::string(1, ' ') + item2;
            line += std::string(ITEM_WIDTH - item2.length() - 1, ' ') + "|";
        }
        else {
            // If no second item, fill with spaces
            line += "|" + std::string(ITEM_WIDTH - 1, ' ') + "|";
        }

        centerText(line);

        // Add separator line between items (except for last row)
        if (i + ITEMS_PER_ROW < menuItems.size()) {
            centerText(separator);
        }
    }

    std::string bottomBorder = EXTRA_PADDING + "+" + std::string(TOTAL_WIDTH - 2, '-') + "+";
    centerText(bottomBorder);
    std::cout << "\n\n";
}

std::string padString(const std::string& str, int width) {
    if (str.length() >= width) return str;
    return str + std::string(width - str.length(), ' ');
}


std::string getHiddenPassword() {
    std::string password;
    char ch;
    // Calculate center position
    const int CONSOLE_WIDTH = 80;
    const int PROMPT_PADDING = (CONSOLE_WIDTH - 20) / 2;

    // Print the initial cursor position
    std::cout << std::string(PROMPT_PADDING, ' ') << "> ";

    while (true) {
        ch = _getch();
        if (ch == 13) { // Enter key
            std::cout << std::endl;
            break;
        }
        else if (ch == 8 && !password.empty()) { // Backspace
            password.pop_back();
            // Clear the current line and reprint
            std::cout << "\r" << std::string(CONSOLE_WIDTH, ' '); // Clear entire line
            std::cout << "\r" << std::string(PROMPT_PADDING, ' ') << "> "
                << std::string(password.length(), '*');
        }
        else if (ch != 8) {
            password += ch;
            // Print the asterisk at the current position
            std::cout << '*';
        }
    }
    return password;
}

// input display bug 

// In display.cpp, remove the duplicate isInteger function and keep just one version:

bool isInteger(const std::string& str) {
    if (str.empty()) return false;

    // Check if first character is a minus sign
    size_t start = (str[0] == '-') ? 1 : 0;

    // String should contain at least one digit after the minus sign
    if (start == str.length()) return false;

    // Check if all remaining characters are digits
    return std::all_of(str.begin() + start, str.end(), ::isdigit);
}

void getUserInput(const std::string& prompt, std::string& input) {
    centerText(prompt);
    centerText(""); // Empty line for spacing
    std::cout << std::string((80 - 20) / 2, ' ') << "> ";
    std::getline(std::cin, input);
}

void getIntegerInput(const std::string& prompt, int& input, int min, int max) {
    bool validInput = false;
    do {
        std::string inputStr;
        centerText(prompt);
        centerText(""); // Empty line for spacing
        std::cout << std::string((80 - 20) / 2, ' ') << "> ";
        std::getline(std::cin, inputStr);

        // Check if the input is a valid integer
        if (isInteger(inputStr)) {
            try {
                input = std::stoi(inputStr);
                if (input >= min && input <= max) {
                    validInput = true;
                }
                else {
                    centerText(RED + "Please enter a number between " + std::to_string(min) +
                        " and " + std::to_string(max) + RESET);
                    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                }
            }
            catch (...) {
                centerText(RED + "Invalid number format." + RESET);
                std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            }
        }
        else {
            centerText(RED + "Please enter a valid number." + RESET);
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        }

        if (!validInput) {
            clearConsole();
        }
    } while (!validInput);
}

void getUserInput(const std::string& prompt, int& input) {
    getIntegerInput(prompt, input, INT_MIN, INT_MAX);
}