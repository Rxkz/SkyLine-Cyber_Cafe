//display.cpp
#include "display.h"        // Custom display and UI functions
#include <iostream>         // Basic input/output operations
#include <conio.h>         // Console input/output, password hiding
#include <iostream>         // Redundant include - can be removed
#include"logo.h"           // Logo and animation functionality
#include <thread>          // Thread management and sleep functions
#include <chrono>          // Time and duration utilities
#include <string>          // String handling and manipulation
#include <windows.h>       // Windows-specific functions and APIs

#ifdef _WIN32
#include <windows.h>       // Redundant Windows include - can be removed
#endif

using namespace std;


// Function to clear the console screen and reset cursor position
void clearConsole() {
#ifdef _WIN32
    // For Windows systems only

    // Execute system command to clear screen
    system("cls");

    // Get handle (access) to console window
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Create coordinates (0,0) for cursor position
    COORD coordScreen = { 0, 0 };

    // Move cursor to top-left corner
    SetConsoleCursorPosition(hConsole, coordScreen);
#else
    // For non-Windows systems (Linux/Mac)

    // Execute clear command for Unix-based systems
    system("clear");
#endif
}

// Function to display text centered in the console window
void centerText(const std::string& text) {
    // Define standard console width (80 characters)
    const int CONSOLE_WIDTH = 80;

    // Calculate space needed before text for centering
    // Subtracts text length from width and divides by 2
    int padding = (CONSOLE_WIDTH - text.length()) / 2;

    // If padding is positive, add spaces before text
    if (padding > 0) {
        // Create string of spaces using padding amount
        std::cout << std::string(padding, ' ');
    }

    // Output the text and move to next line
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

// Function to display a formatted, centered menu with items in a grid layout
void displayCenteredMenu(const std::vector<std::string>& menuItems, const std::string& title) {
    // Clear screen and add initial spacing
    clearConsole();
    std::cout << "\n\n\n\n\n\n\n";
    cout << "\n";

    // Define padding for consistent alignment
    const string EXTRA_PADDING = "\t\t\t";

    // Display title if provided
    if (!title.empty()) {
        std::cout << "\n";
        // Display title in yellow with padding
        centerText(YELLOW + EXTRA_PADDING + title + RESET);
        std::cout << "\n";
    }

    // Define menu layout constants
    const int ITEMS_PER_ROW = 2;     // Number of items per row
    const int ITEM_WIDTH = 35;        // Width of each menu item
    const int TOTAL_WIDTH = ITEM_WIDTH * ITEMS_PER_ROW + 10;  // Total menu width

    // Create top border of menu
    std::string topBorder = EXTRA_PADDING + "+" + std::string(TOTAL_WIDTH - 2, '-') + "+";
    centerText(topBorder);

    // Create separator line between rows
    std::string separator = EXTRA_PADDING + "|" + std::string(TOTAL_WIDTH - 2, '-') + "|";

    // Display menu items in grid format
    for (size_t i = 0; i < menuItems.size(); i += ITEMS_PER_ROW) {
        // Start building line with border
        std::string line = EXTRA_PADDING + "|";

        // Add first item of the row
        std::string item1 = (i < menuItems.size()) ? menuItems[i] : "";
        line += " " + item1 + std::string(ITEM_WIDTH - item1.length() - 1, ' ');

        // Add second item if it exists
        if (i + 1 < menuItems.size()) {
            // Add second item with padding
            std::string item2 = menuItems[i + 1];
            line += "|" + std::string(1, ' ') + item2;
            line += std::string(ITEM_WIDTH - item2.length() - 1, ' ') + "|";
        }
        else {
            // Fill with spaces if no second item
            line += "|" + std::string(ITEM_WIDTH - 1, ' ') + "|";
        }

        // Display the completed line
        centerText(line);

        // Add separator between rows (except last)
        if (i + ITEMS_PER_ROW < menuItems.size()) {
            centerText(separator);
        }
    }

    // Create and display bottom border
    std::string bottomBorder = EXTRA_PADDING + "+" + std::string(TOTAL_WIDTH - 2, '-') + "+";
    centerText(bottomBorder);

    // Add spacing after menu
    std::cout << "\n\n";
}

// Function to add spaces after a string to reach desired width
std::string padString(const std::string& str, int width) {
    // Check if string is already longer than or equal to desired width
    if (str.length() >= width) {
        return str;  // Return original string without padding
    }

    // Add spaces after string to reach desired width
    // Creates new string with (width - current length) spaces
    return str + std::string(width - str.length(), ' ');
}


// Function to securely get password input while showing asterisks
std::string getHiddenPassword() {
    // Store password characters
    std::string password;

    // Variable for character input
    char ch;

    // Calculate center of console for alignment
    const int CONSOLE_WIDTH = 80;     // Standard console width
    const int PROMPT_PADDING = (CONSOLE_WIDTH - 20) / 2;  // Left padding for centering

    // Display initial cursor position with proper centering
    std::cout << std::string(PROMPT_PADDING, ' ') << "> ";

    // Continue getting input until Enter is pressed
    while (true) {
        // Get character without displaying it
        ch = _getch();

        // Check for Enter key (ASCII 13)
        if (ch == 13) {
            std::cout << std::endl;
            break;  // Exit input loop
        }

        // Handle backspace (ASCII 8) when password isn't empty
        else if (ch == 8 && !password.empty()) {
            // Remove last character from password
            password.pop_back();

            // Clear current line (\r moves to line start)
            std::cout << "\r" << std::string(CONSOLE_WIDTH, ' ');

            // Redisplay prompt and asterisks
            std::cout << "\r" << std::string(PROMPT_PADDING, ' ') << "> "
                << std::string(password.length(), '*');
        }

        // Handle regular character input (ignore backspace if password empty)
        else if (ch != 8) {
            // Add character to password
            password += ch;

            // Display asterisk for the character
            std::cout << '*';
        }
    }

    // Return completed password
    return password;
}


// Function to check if a string contains a valid integer number
bool isInteger(const std::string& str) {
    // Check if string is empty
    if (str.empty()) {
        return false;  // Empty string is not an integer
    }

    // Determine starting position for digit check
    // If first character is minus sign, start from position 1
    size_t start = (str[0] == '-') ? 1 : 0;

    // Check if string only contains minus sign
    if (start == str.length()) {
        return false;  // Just a minus sign is not valid
    }

    // Use all_of to verify remaining characters
    // Checks if every character from start to end is a digit
    return std::all_of(str.begin() + start, str.end(), ::isdigit);
}

// Function to get user input with formatted prompt and centered display
void getUserInput(const std::string& prompt, std::string& input) {
    // Display the prompt text centered on screen
    centerText(prompt);

    // Add empty line for visual spacing
    centerText("");

    // Create centered input cursor
    // Calculate padding: (console width - cursor width) / 2
    std::cout << std::string((80 - 20) / 2, ' ') << "> ";

    // Get user input with spaces using getline
    std::getline(std::cin, input);
}

// Function to get and validate integer input within a specified range
void getIntegerInput(const std::string& prompt, int& input, int min, int max) {
    // Flag to track input validity
    bool validInput = false;

    // Keep asking until valid input received
    do {
        // String to store raw input
        std::string inputStr;

        // Display prompt centered
        centerText(prompt);

        // Add spacing line
        centerText("");

        // Display centered input cursor
        std::cout << std::string((80 - 20) / 2, ' ') << "> ";

        // Get user input as string
        std::getline(std::cin, inputStr);

        // Check if input string is numeric
        if (isInteger(inputStr)) {
            try {
                // Convert string to integer
                input = std::stoi(inputStr);

                // Validate number is within allowed range
                if (input >= min && input <= max) {
                    validInput = true;  // Input is valid
                }
                else {
                    // Show range error in red
                    centerText(RED + "Please enter a number between " + std::to_string(min) +
                        " and " + std::to_string(max) + RESET);

                    // Pause for error message visibility
                    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                }
            }
            catch (...) {
                // Handle number conversion errors
                centerText(RED + "Invalid number format." + RESET);
                std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            }
        }
        else {
            // Show error for non-numeric input
            centerText(RED + "Please enter a valid number." + RESET);
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        }

        // Clear screen if input was invalid
        if (!validInput) {
            clearConsole();
        }
    } while (!validInput);  // Continue until valid input received
}

void getUserInput(const std::string& prompt, int& input) {
    getIntegerInput(prompt, input, INT_MIN, INT_MAX);
}