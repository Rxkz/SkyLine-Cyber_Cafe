#include "display.h"
#include <iostream>
#include <conio.h>
#ifdef _WIN32
#include <windows.h>
#endif

void clearConsole() {
#ifdef _WIN32
    system("cls");
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

void displayLogo() {
    clearConsole();
    std::vector<std::string> logoLines = {
        MAGENTA + " _____  _  __ __  _    " + YELLOW + "▲" + MAGENTA + "  _   _  ______ " + RESET,
        MAGENTA + "/  ___|| |/ / \\ \\| |   " + YELLOW + "/!\\" + MAGENTA + " | \\ | ||  ___|" + RESET,
        MAGENTA + "\\ `--  |   /   \\ \\ |  " + YELLOW + "/|!|\\" + MAGENTA + "|  \\| || |    " + RESET,
        MAGENTA + " `--. \\|  /     \\ \\| " + YELLOW + "//|!|\\\\" + MAGENTA + "| . ` || |___ " + RESET,
        MAGENTA + "/\\__/ /| . \\    _) ) " + YELLOW + " |!| " + MAGENTA + "| |\\  ||  ___| " + RESET,
        MAGENTA + "\\____/ |_|\\_\\  |___/ " + YELLOW + " |!| " + MAGENTA + "|_| \\_||_____|" + RESET,
        YELLOW + "                  =====   " + RESET,
        "",
        GREEN + "Cyber Cafe" + RESET
    };

    std::cout << "\n\n";
    for (const auto& line : logoLines) {
        centerText(line);
    }
    std::cout << "\n";
    centerText(GREEN + "Welcome to Skyline Cyber Cafe!" + RESET);
    std::cout << "\n";
}

void displayCenteredMenu(const std::vector<std::string>& menuItems, const std::string& title) {
    clearConsole();
    displayLogo();

    if (!title.empty()) {
        std::cout << "\n";
        centerText(YELLOW + title + RESET);
        std::cout << "\n";
    }

    // Calculate dimensions
    const int ITEMS_PER_ROW = 2;  // Number of items per row
    const int ITEM_WIDTH = 35;     // Width of each menu box
    const int TOTAL_WIDTH = ITEM_WIDTH * ITEMS_PER_ROW + 10;  // Total width including borders and spacing

    // Create top border
    std::string topBorder = "+" + std::string(TOTAL_WIDTH - 2, '-') + "+";
    centerText(topBorder);

    // Display items in grid
    for (size_t i = 0; i < menuItems.size(); i += ITEMS_PER_ROW) {
        std::string line = "| ";

        // Add items in current row
        for (size_t j = 0; j < ITEMS_PER_ROW && (i + j) < menuItems.size(); j++) {
            // Create a padded item string
            std::string item = menuItems[i + j];
            std::string paddedItem = item + std::string(ITEM_WIDTH - 2 - item.length(), ' ');
            line += paddedItem;
            if (j < ITEMS_PER_ROW - 1 && (i + j + 1) < menuItems.size()) {
                line += "| ";
            }
        }

        // Pad the rest of the line if we don't have enough items
        if ((i + ITEMS_PER_ROW) > menuItems.size()) {
            line += std::string(ITEM_WIDTH * (ITEMS_PER_ROW - (menuItems.size() - i)) - 2, ' ');
        }
        line += " |";

        centerText(line);

        // Add separator line between rows if not the last row
        if (i + ITEMS_PER_ROW < menuItems.size()) {
            std::string separator = "|" + std::string(TOTAL_WIDTH - 2, '-') + "|";
            centerText(separator);
        }
    }

    // Create bottom border
    std::string bottomBorder = "+" + std::string(TOTAL_WIDTH - 2, '-') + "+";
    centerText(bottomBorder);
    std::cout << "\n";
}

std::string padString(const std::string& str, int width) {
    if (str.length() >= width) return str;
    return str + std::string(width - str.length(), ' ');
}

void getUserInput(const std::string& prompt, std::string& input) {
    centerText(prompt);
    centerText(""); // Empty line for spacing
    std::cout << std::string((80 - 20) / 2, ' ') << "> ";
    std::getline(std::cin, input);
}

void getUserInput(const std::string& prompt, int& input) {
    centerText(prompt);
    centerText(""); // Empty line for spacing
    std::cout << std::string((80 - 20) / 2, ' ') << "> ";
    std::cin >> input;
    std::cin.ignore();
}

std::string getHiddenPassword() {
    std::string password;
    char ch;
    while (true) {
        ch = _getch();
        if (ch == 13) { // Enter key
            std::cout << std::endl;
            break;
        }
        else if (ch == 8 && !password.empty()) { // Backspace
            password.pop_back();
            std::cout << "\b \b";
        }
        else if (ch != 8) {
            password += ch;
            std::cout << '*';
        }
    }
    return password;
}