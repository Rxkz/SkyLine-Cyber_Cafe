//userdata.cpp
#define _CRT_SECURE_NO_WARNINGS

#include "userdata.h"
#include <iostream>
#include <fstream>
#include <conio.h>// hide passwords 
#include <iomanip>
#include <string>
#include <algorithm>
#include <ctime>
#include <map>
#include <thread>
#include <conio.h>// For std::this_thread::sleep_for

#ifdef _WIN32
#include <windows.h>
#endif

// ============= Global Dec =============

int userIDCounter = 1;

// ============= Display  =============
void centerText(const std::string& text) {
    const int CONSOLE_WIDTH = 80; // Default console width
    int padding = (CONSOLE_WIDTH - text.length()) / 2;
    if (padding > 0) {
        std::cout << std::string(padding, ' ');
    }
    std::cout << text << std::endl;
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

// Helper function to pad strings for grid display
std::string padString(const std::string& str, int width) {
    if (str.length() >= width) return str;
    return str + std::string(width - str.length(), ' ');
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

void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


// ============= User Sessions  =============

void User::startSession() {
    Session newSession;
    newSession.startTime = time(0);
    sessions.push_back(newSession);
    isLoggedIn = true;
    centerText(GREEN + "Session started successfully!" + RESET);
}

void User::endSession(double sessionBill) {
    if (!sessions.empty() && isLoggedIn) {
        sessions.back().endTime = time(0);
        sessions.back().cost = sessionBill;
        totalBill += sessionBill;
        isLoggedIn = false;
    }
    centerText(BLUE + "Session ended successfully!" + RESET);
    centerText("Session Bill: NZD " + std::to_string(sessionBill));
}

int User::getTotalTime() const {
    int totalMinutes = 0;
    for (const auto& session : sessions) {
        if (session.endTime > 0) {
            totalMinutes += static_cast<int>(difftime(session.endTime, session.startTime) / 60);
        }
        else if (isLoggedIn) {
            totalMinutes += static_cast<int>(difftime(time(0), session.startTime) / 60);
        }
    }
    return totalMinutes;
}

// ============= Validation  =============

//function to check a validusername Santosh MTD
bool isValidUserName(const std::string& username) {
    if (username.length() < 6 || username.length() > 20) {
        return false;
    }
    if (!isalpha(username[0])) {
        return false;
    }
    for (char ch : username) {
        if (!(isalnum(ch) || ch == '@' || ch == '.' || ch == '-' || ch == '_')) {
            return false;
        }
    }
    return true;
}

// this was old email verification so istaed of change all the function name in all other pages we just replaced this fuctions implemention to call our accual email validation 
bool isValidEmail(const std::string& email) {

    return isValidDetailedEmail(email);

}

//For Email Validation Santosh MTD
bool isValidDetailedEmail(const std::string& email) {
    // List of valid domains
    std::vector<std::string> validDomains = {
        "gmail.com", "outlook.com", "example.com", "test.com", "hotmail.com",
        "live.com", "yahoo.com", "icloud.com", "me.com", "mac.com",
        "protonmail.com", "protonmail.ch", "zoho.com", "aol.com", "gmx.com",
        "gmx.de", "gmx.net", "mail.com", "usa.com", "email.com", "consultant.com",
        "musician.org", "photographer.com", "yandex.com", "yandex.ru",
        "fastmail.com", "fastmail.fm", "tutanota.com", "tutanota.de",
        "posteo.de", "runbox.com", "kolabnow.com", "hushmail.com",
        "mail.ru", "list.ru"
    };

    // List of special characters that cannot start or end the email
    std::vector<char> specialChars = {
        '.', '-', '_', '+', '%', '&', '\'', '"', '!', '#', '$', '^', '*',
        '(', ')', ',', ':', ';', '=', '~', '`', '/', '?', '<', '>',
        '[', ']', '{', '}'
    };

    // Basic structure checks
    size_t atPos = email.find('@');
    size_t dotPos = email.find('.');

    // Invalid conditions:
    // 1. No @ symbol
    // 2. No dot
    // 3. Contains spaces
    // 4. Multiple @ symbols
    // 5. Starts or ends with @ or .
    // 6. Email too long (max 254 characters)
    if (atPos == std::string::npos ||
        dotPos == std::string::npos ||
        email.find(' ') != std::string::npos ||
        email.find('@', atPos + 1) != std::string::npos ||
        email.front() == '@' || email.front() == '.' ||
        email.back() == '@' || email.back() == '.' ||
        email.length() > 254) {
        return false;
    }

    // Check for special characters at start or end
    for (char c : specialChars) {
        if (email.front() == c || email.back() == c) {
            return false;
        }
    }

    // Extract domain and check against valid domains
    std::string domain = email.substr(atPos + 1);
    return std::find(validDomains.begin(), validDomains.end(), domain) != validDomains.end();
}

bool isDuplicateEmail(const std::string& email) {
    const std::string filename = "User_Registration.json";
    std::ifstream file(filename);

    if (!file.good()) {
        return false;  // File doesn't exist, so no duplicates
    }

    try {
        nlohmann::json userData = nlohmann::json::parse(file);

        for (const auto& user : userData["users"]) {
            if (user["email"] == email) {
                return true;  // Found duplicate email
            }
        }
    }
    catch (const std::exception& e) {
        centerText(RED + "Error checking email: " + e.what() + RESET);
        return false;
    }

    file.close();
    return false;
}

// this was old Password verification so istaed of change all the function name in all other pages we just replaced this fuctions implemention to call our accual email validation
bool isValidPassword(const std::string& password) {
    return isValidDetailedPassword(password);
}
//FOR PASSWORD Validtion Santosh MTD
bool isValidDetailedPassword(const std::string& password) {
    if (password.length() < 8 || password.length() > 20) {
        return false;
    }
    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;

    for (char c : password) {
        if (c >= 'A' && c <= 'Z') hasUpper = true;
        else if (c >= 'a' && c <= 'z') hasLower = true;
        else if (c >= '0' && c <= '9') hasDigit = true;
        else if (!(c == ' ' || (c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))) {
            hasSpecial = true;
        }
    }
    return hasUpper && hasLower && hasDigit && hasSpecial;
}

//For Hiding Passwords Santosh MTD
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

//phoneneo validtion Santosh MTD
bool isValidPhoneNumber(const std::string& phone) {
    std::string cleanNumber;
    for (char c : phone) {
        if (isdigit(c)) {
            cleanNumber += c;
        }
    }

    // Check for invalid characters
    for (char c : phone) {
        if (!isdigit(c) && c != ' ' && c != '-' && c != '+' && c != '(' && c != ')') {
            return false;
        }
    }

    // Check length of actual digits
    if (cleanNumber.length() < 10 || cleanNumber.length() > 13) {
        return false;
    }

    // Verify all cleaned characters are digits
    for (char c : cleanNumber) {
        if (!isdigit(c)) {
            return false;
        }
    }

    return true;
}
//duplpicate number
bool isDuplicatePhone(const std::string& phone) {
    const std::string filename = "User_Registration.json";
    std::ifstream file(filename);

    if (!file.good()) {
        return false;  // File doesn't exist, so no duplicates
    }

    try {
        nlohmann::json userData = nlohmann::json::parse(file);

        for (const auto& user : userData["users"]) {
            if (user["phoneno"] == phone) {
                return true;  // Found duplicate phone
            }
        }
    }
    catch (const std::exception& e) {
        centerText(RED + "Error checking phone: " + e.what() + RESET);
        return false;
    }

    file.close();
    return false;
}


// ============= JSON  =============

void saveUserToJson(const UserRegistration& user) {
    nlohmann::json userData;
    const std::string filename = "User_Registration.json";

    std::ifstream inFile(filename);
    if (inFile.good()) {
        userData = nlohmann::json::parse(inFile);
    }
    else {
        userData["users"] = nlohmann::json::array();
    }
    inFile.close();

    nlohmann::json newUser;
    newUser["username"] = user.username;
    newUser["password"] = user.password;
    newUser["fullname"] = user.fullname;
    newUser["email"] = user.email;
    newUser["phoneno"] = user.phoneno;  // Now storing as string
    newUser["joinDate"] = user.joinDate;

    userData["users"].push_back(newUser);

    std::ofstream outFile(filename);
    outFile << userData.dump(2);
    outFile.close();

    centerText(GREEN + "Registration successful! Data saved to " + filename + RESET);
}

void handleRegistration(std::vector<User>& users, int& userIDCounter) {
    clearConsole();
    UserRegistration newUser;
    newUser.joinDate = time(0);

    // Username with validation
    do {
        getUserInput("Enter Username (min 6 chars, start with letter, special chars: @.-_ only):", newUser.username);
        if (!isValidUserName(newUser.username)) {
            centerText(RED + "Invalid username format. Please try again." + RESET);
        }
    } while (!isValidUserName(newUser.username));

    // Password with validation and hiding
    std::string confirmPassword;
    do {
        centerText("Enter Password (min 8 chars, must include uppercase, lowercase, number, special char):");
        newUser.password = getHiddenPassword();

        if (!isValidDetailedPassword(newUser.password)) {
            centerText(RED + "Invalid password format. Please try again." + RESET);
            continue;
        }

        centerText("Confirm Password:");
        confirmPassword = getHiddenPassword();

        if (newUser.password != confirmPassword) {
            centerText(RED + "Passwords do not match. Please try again." + RESET);
        }
    } while (!isValidDetailedPassword(newUser.password) || newUser.password != confirmPassword);

    getUserInput("Enter Full Name:", newUser.fullname);

    // Email with validation and duplicate check
    std::string confirmEmail;
    bool validEmail = false;
    do {
        getUserInput("Enter Email:", newUser.email);
        if (!isValidDetailedEmail(newUser.email)) {
            centerText(RED + "Invalid email format. Please try again." + RESET);
            continue;
        }

        if (isDuplicateEmail(newUser.email)) {
            centerText(RED + "Email already registered. Please login or use a different email." + RESET);
            continue;
        }

        getUserInput("Confirm Email:", confirmEmail);
        if (newUser.email != confirmEmail) {
            centerText(RED + "Emails do not match. Please try again." + RESET);
            continue;
        }
        validEmail = true;
    } while (!validEmail);

    // Phone number validation with duplicate check
    std::string phoneStr;
    bool validPhone = false;
    do {
        getUserInput("Enter Phone Number (10-13 digits, can include spaces, -, +, (, )):", phoneStr);
        if (!isValidPhoneNumber(phoneStr)) {
            centerText(RED + "Invalid phone number format. Please enter a valid number." + RESET);
            continue;
        }

        // Clean the phone number before checking for duplicates
        std::string cleanNumber;
        for (char c : phoneStr) {
            if (isdigit(c)) {
                cleanNumber += c;
            }
        }

        if (isDuplicatePhone(cleanNumber)) {
            centerText(RED + "Phone number already registered. Please use a different number." + RESET);
            continue;
        }

        validPhone = true;
        newUser.phoneno = cleanNumber;
    } while (!validPhone);

    saveUserToJson(newUser);
    users.emplace_back(newUser.fullname, newUser.email, newUser.password, userIDCounter++);
}


void updateUserInJson(const User& user, const std::string& originalEmail) {
    const std::string filename = "User_Registration.json";

    try {
        // Read existing JSON file
        std::ifstream inFile(filename);
        if (!inFile.good()) {
            centerText(RED + "Error: Could not open user file" + RESET);
            return;
        }

        // Parse JSON with exception handling
        nlohmann::json userData;
        try {
            userData = nlohmann::json::parse(inFile);
        }
        catch (const nlohmann::json::exception& e) {
            centerText(RED + "Error parsing JSON: " + e.what() + RESET);
            inFile.close();
            return;
        }
        inFile.close();

        // Validate user data before updating
        if (!userData.contains("users") || !userData["users"].is_array()) {
            centerText(RED + "Error: Invalid JSON structure" + RESET);
            return;
        }

        // Find and update the user
        bool userFound = false;
        for (auto& jsonUser : userData["users"]) {
            try {
                if (jsonUser["email"].get<std::string>() == originalEmail) {
                    // Update fields with size validation
                    std::string newName = user.name.substr(0, 100); // Limit name to 100 chars
                    std::string newEmail = user.email.substr(0, 100); // Limit email to 100 chars
                    std::string newPassword = user.password.substr(0, 50); // Limit password to 50 chars
                    std::string newPhone = user.phoneno.substr(0, 15); // Limit phone to 15 chars

                    jsonUser["fullname"] = newName;
                    jsonUser["email"] = newEmail;
                    jsonUser["password"] = newPassword;
                    jsonUser["phoneno"] = newPhone;
                    userFound = true;
                    break;
                }
            }
            catch (const std::exception& e) {
                centerText(RED + "Error processing user data: " + e.what() + RESET);
                continue;
            }
        }

        if (!userFound) {
            centerText(YELLOW + "Warning: User not found in JSON file" + RESET);
            return;
        }

        // Write updated JSON back to file
        std::ofstream outFile(filename);
        if (!outFile.good()) {
            centerText(RED + "Error: Could not open file for writing" + RESET);
            return;
        }

        try {
            outFile << userData.dump(2);
            outFile.close();
            centerText(GREEN + "User data updated successfully!" + RESET);
        }
        catch (const std::exception& e) {
            centerText(RED + "Error writing to file: " + e.what() + RESET);
            outFile.close();
        }

    }
    catch (const std::exception& e) {
        centerText(RED + "An unexpected error occurred: " + e.what() + RESET);
    }
}


void loadAdminsFromJson(std::vector<Admin>& admins) {
    const std::string filename = "Admin_List.json";
    std::ifstream file(filename);

    if (!file.good()) {
        return;
    }

    try {
        nlohmann::json adminData = nlohmann::json::parse(file);

        for (const auto& admin : adminData["admins"]) {
            std::string email = admin["email"];
            std::string password = admin["password"];
            bool exists = false;
            for (const auto& existingAdmin : admins) {
                if (existingAdmin.email == email) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                admins.emplace_back(email, password);
            }
        }
    }
    catch (const std::exception& e) {
        centerText(RED + "Error loading admins: " + e.what() + RESET);
    }
    file.close();
}

void saveAdminToJson(const std::string& email, const std::string& password) {
    nlohmann::json adminData;
    const std::string filename = "Admin_List.json";

    std::ifstream inFile(filename);
    if (inFile.good()) {
        adminData = nlohmann::json::parse(inFile);
    }
    else {
        adminData["admins"] = nlohmann::json::array();
    }
    inFile.close();

    nlohmann::json newAdmin;
    newAdmin["email"] = email;
    newAdmin["password"] = password;

    adminData["admins"].push_back(newAdmin);

    std::ofstream outFile(filename);
    outFile << adminData.dump(2);
    outFile.close();

    centerText(GREEN + "Admin added successfully!" + RESET);
}

void loadUsersFromJson(std::vector<User>& users, int& userIDCounter) {
    const std::string filename = "User_Registration.json";
    std::ifstream file(filename);

    if (!file.good()) {
        return;
    }

    try {
        nlohmann::json userData = nlohmann::json::parse(file);
        users.clear();
        userIDCounter = 1;

        for (const auto& user : userData["users"]) {
            std::string fullname = user["fullname"];
            std::string email = user["email"];
            std::string password = user["password"];
            users.emplace_back(fullname, email, password, userIDCounter++);
        }
    }
    catch (const std::exception& e) {
        centerText(RED + "Error loading users: " + e.what() + RESET);
    }
    file.close();
}

void handleNewAdminRegistration(std::vector<Admin>& admins) {
    clearConsole();
    std::string email, password;

    getUserInput("Enter new admin email:", email);
    while (!isValidEmail(email)) {
        centerText(RED + "Invalid email format." + RESET);
        getUserInput("Enter new admin email:", email);
    }

    getUserInput("Enter password (min 8 characters):", password);
    while (!isValidPassword(password)) {
        centerText(RED + "Password must be at least 8 characters." + RESET);
        getUserInput("Enter password (min 8 characters):", password);
    }

    admins.emplace_back(email, password);
    saveAdminToJson(email, password);
}


// ============= Admin  =============


Admin::Admin(std::string e, std::string p) : email(e), password(p) {}

void handleAdminMenu(Admin& admin, std::vector<User>& users, std::vector<Admin>& admins) {
    int choice;
    do {
        std::vector<std::string> menuItems = {
            "1. View All Users",
            "2. View Online Users",
            "3. View Paid Bills",
            "4. View Unpaid Bills",
            "5. Search/Edit User",
            "6. Delete User",
            "7. System Stats",
            "8. Add New Admin",
            "9. Return to Main Menu"
        };

        displayCenteredMenu(menuItems, "Admin Menu");
        getUserInput("Enter choice:", choice);

        switch (choice) {
        case 1: admin.viewAllUsers(users); break;
        case 2: admin.viewOnlineUsers(users); break;
        case 3: admin.viewUsersByPaymentStatus(users, true); break;
        case 4: admin.viewUsersByPaymentStatus(users, false); break;
        case 5: admin.searchAndEditUser(users); break;
        case 6: admin.searchAndDeleteUser(users); break;
        case 7: admin.viewTotalStats(users); break;
        case 8: handleNewAdminRegistration(admins); break;
        case 9: centerText(CYAN + "Logging out...\n" + RESET); break;
        default: centerText(RED + "Invalid choice\n" + RESET);
        }

        if (choice != 9) {
            centerText("\nPress Enter to continue...");
            std::cin.get();
        }
    } while (choice != 9);
}

void Admin::viewAllUsers(const std::vector<User>& users) {
    clearConsole();
    centerText(BLUE + "\n---- Complete User List ----" + RESET);
    int count = 1;
    for (const auto& user : users) {
        std::string joinDateStr = std::ctime(&user.joinDate);
        centerText(YELLOW + "User #" + std::to_string(count++) + RESET);
        centerText("User ID: " + std::to_string(user.userID));
        centerText("Name: " + user.name);
        centerText("Email: " + user.email);
        centerText("Join Date: " + joinDateStr);
        centerText("Total Bill: NZD " + std::to_string(user.totalBill));
        centerText("Status: " + std::string(user.isLoggedIn ? "Online" : "Offline"));
        centerText("Total Time: " + std::to_string(user.getTotalTime()) + " minutes");
        centerText("-------------------");
    }
}

void Admin::viewOnlineUsers(const std::vector<User>& users) {
    clearConsole();
    centerText(GREEN + "\n---- Currently Online Users ----" + RESET);
    bool foundOnline = false;
    for (const auto& user : users) {
        if (user.isLoggedIn) {
            centerText("User ID: " + std::to_string(user.userID));
            centerText("Name: " + user.name);
            centerText("Current Session Time: " + std::to_string(user.getTotalTime()) + " minutes");
            centerText("Current Bill: NZD " + std::to_string(user.totalBill));
            centerText("-------------------");
            foundOnline = true;
        }
    }
    if (!foundOnline) {
        centerText("No users currently online.");
    }
}


// admin to search and edit a user
void Admin::searchAndEditUser(std::vector<User>& users) {
    clearConsole();
    std::string searchTerm;
    const size_t MAX_INPUT_LENGTH = 100;

    // Get search term with length validation
    while (true) {
        getUserInput("Enter user ID, name, or email to search:", searchTerm);
        if (searchTerm.empty()) {
            centerText(RED + "Search term cannot be empty. Please try again." + RESET);
            continue;
        }
        if (searchTerm.length() > MAX_INPUT_LENGTH) {
            centerText(RED + "Search term is too long. Maximum length is " +
                std::to_string(MAX_INPUT_LENGTH) + " characters." + RESET);
            continue;
        }
        break;
    }

    try {
        bool userFound = false;
        for (auto& user : users) {
            if (std::to_string(user.userID) == searchTerm ||
                user.name == searchTerm ||
                user.email == searchTerm) {
                userFound = true;

                // Create the menu items with current user data
                std::vector<std::string> menuItems = {
                    "1. Name: " + user.name,
                    "2. Email: " + user.email,
                    "3. Password",
                    "4. Phone Number",
                    "5. Cancel"
                };

                displayCenteredMenu(menuItems, "Edit User");

                std::string originalEmail = user.email; // Store for updating JSON
                int choice;
                getUserInput("What would you like to edit? (1-5):", choice);

                switch (choice) {
                case 1: { // Edit Name
                    std::string newName;
                    while (true) {
                        getUserInput("Enter new name:", newName);
                        if (newName.empty()) {
                            centerText(RED + "Name cannot be empty." + RESET);
                            continue;
                        }
                        if (newName.length() > MAX_INPUT_LENGTH) {
                            centerText(RED + "Name is too long. Maximum length is " +
                                std::to_string(MAX_INPUT_LENGTH) + " characters." + RESET);
                            continue;
                        }
                        user.name = newName;
                        updateUserInJson(user, originalEmail);
                        centerText(GREEN + "Name updated successfully!" + RESET);
                        break;
                    }
                    break;
                }
                case 2: { // Edit Email
                    std::string newEmail;
                    while (true) {
                        getUserInput("Enter new email:", newEmail);
                        if (newEmail.length() > MAX_INPUT_LENGTH) {
                            centerText(RED + "Email is too long. Maximum length is " +
                                std::to_string(MAX_INPUT_LENGTH) + " characters." + RESET);
                            continue;
                        }
                        if (!isValidEmail(newEmail)) {
                            centerText(RED + "Invalid email format." + RESET);
                            continue;
                        }
                        if (isDuplicateEmail(newEmail) && newEmail != originalEmail) {
                            centerText(RED + "Email already exists!" + RESET);
                            continue;
                        }
                        user.email = newEmail;
                        updateUserInJson(user, originalEmail);
                        centerText(GREEN + "Email updated successfully!" + RESET);
                        break;
                    }
                    break;
                }
                case 3: { // Edit Password
                    std::string newPassword;
                    while (true) {
                        centerText("Enter new password:");
                        newPassword = getHiddenPassword();
                        if (newPassword.length() > MAX_INPUT_LENGTH) {
                            centerText(RED + "Password is too long. Maximum length is " +
                                std::to_string(MAX_INPUT_LENGTH) + " characters." + RESET);
                            continue;
                        }
                        if (!isValidPassword(newPassword)) {
                            centerText(RED + "Invalid password format." + RESET);
                            continue;
                        }
                        user.password = newPassword;
                        updateUserInJson(user, originalEmail);
                        centerText(GREEN + "Password updated successfully!" + RESET);
                        break;
                    }
                    break;
                }
                case 4: { // Edit Phone Number
                    std::string newPhone;
                    while (true) {
                        getUserInput("Enter new phone number:", newPhone);
                        if (newPhone.length() > MAX_INPUT_LENGTH) {
                            centerText(RED + "Phone number is too long. Maximum length is " +
                                std::to_string(MAX_INPUT_LENGTH) + " characters." + RESET);
                            continue;
                        }
                        if (!isValidPhoneNumber(newPhone)) {
                            centerText(RED + "Invalid phone number format." + RESET);
                            continue;
                        }
                        if (isDuplicatePhone(newPhone) && newPhone != user.phoneno) {
                            centerText(RED + "Phone number already exists!" + RESET);
                            continue;
                        }
                        user.phoneno = newPhone;
                        updateUserInJson(user, originalEmail);
                        centerText(GREEN + "Phone number updated successfully!" + RESET);
                        break;
                    }
                    break;
                }
                case 5:
                    centerText(YELLOW + "Edit cancelled." + RESET);
                    break;
                default:
                    centerText(RED + "Invalid choice." + RESET);
                }
                break;
            }
        }

        if (!userFound) {
            centerText(RED + "User not found." + RESET);
        }
    }
    catch (const std::exception& e) {
        centerText(RED + "An error occurred: " + e.what() + RESET);
        centerText(YELLOW + "Please try again." + RESET);
    }
}

//search and delete user
void Admin::searchAndDeleteUser(std::vector<User>& users) {
    clearConsole();
    std::string searchTerm;
    getUserInput("Enter user ID, name, or email to search:", searchTerm);

    auto it = std::find_if(users.begin(), users.end(),
        [&searchTerm](const User& user) {
            return std::to_string(user.userID) == searchTerm ||
            user.name == searchTerm ||
        user.email == searchTerm;
        });

    if (it != users.end()) {
        centerText("User found: " + it->name + " (ID: " + std::to_string(it->userID) + ")");

        std::string confirmation;
        getUserInput("Are you sure you want to delete? (y/n):", confirmation);

        if (!confirmation.empty() && (confirmation[0] == 'y' || confirmation[0] == 'Y')) {
            // Store the email for JSON deletion
            std::string emailToDelete = it->email;

            // Remove from vector
            users.erase(it);

            // Update JSON file
            const std::string filename = "User_Registration.json";
            try {
                // Read current JSON data
                std::ifstream inFile(filename);
                if (!inFile.good()) {
                    centerText(RED + "Error: Could not open user file" + RESET);
                    return;
                }
                nlohmann::json userData = nlohmann::json::parse(inFile);
                inFile.close();

                // Find and remove the user from JSON
                auto& jsonUsers = userData["users"];
                auto jsonIt = std::find_if(jsonUsers.begin(), jsonUsers.end(),
                    [&emailToDelete](const nlohmann::json& user) {
                        return user["email"] == emailToDelete;
                    });

                if (jsonIt != jsonUsers.end()) {
                    jsonUsers.erase(jsonIt);

                    // Write updated JSON back to file
                    std::ofstream outFile(filename);
                    outFile << userData.dump(2);
                    outFile.close();

                    centerText(GREEN + "User deleted successfully from both memory and file!" + RESET);
                }
                else {
                    centerText(YELLOW + "Warning: User not found in JSON file" + RESET);
                }
            }
            catch (const std::exception& e) {
                centerText(RED + "Error updating JSON file: " + e.what() + RESET);
                // Revert the vector change if JSON update failed
                loadUsersFromJson(users, userIDCounter);
            }
        }
        else {
            centerText(YELLOW + "Deletion cancelled." + RESET);
        }
    }
    else {
        centerText(RED + "User not found." + RESET);
    }
}

// admin to get bill data 
std::vector<Receipt> loadReceiptsFromJson() {
    std::vector<Receipt> receipts;
    const std::string filename = "Receipts.json";
    std::ifstream file(filename);

    if (!file.good()) {
        return receipts;
    }

    try {
        nlohmann::json receiptData = nlohmann::json::parse(file);
        for (const auto& receipt : receiptData["receipts"]) {
            Receipt r;
            r.userID = receipt["userID"];
            r.userName = receipt["userName"];
            r.userEmail = receipt["userEmail"];
            r.totalAmount = receipt["totalAmount"];
            r.receiptDate = receipt["receiptDate"];
            r.paymentMethod = receipt["paymentMethod"];
            r.isPaid = receipt["isPaid"];

            // Load services
            for (const auto& service : receipt["services"]) {
                ServiceUsage s;
                s.serviceType = service["type"];
                s.duration = service["duration"];
                s.quantity = service["quantity"];
                s.rate = service["rate"];
                s.cost = service["cost"];
                s.timestamp = service["timestamp"];
                r.services.push_back(s);
            }
            receipts.push_back(r);
        }
    }
    catch (const std::exception& e) {
        centerText(RED + "Error loading receipts: " + e.what() + RESET);
    }
    file.close();
    return receipts;
}

void Admin::viewUsersByPaymentStatus(const std::vector<User>& users, bool paid) {
    clearConsole();
    std::vector<Receipt> receipts = loadReceiptsFromJson();

    centerText(YELLOW + "\n---- Users with " + (paid ? "Paid" : "Unpaid") + " Bills ----" + RESET);
    bool found = false;

    for (const auto& receipt : receipts) {
        if (receipt.isPaid == paid) {
            centerText("User ID: " + std::to_string(receipt.userID));
            centerText("Name: " + receipt.userName);
            centerText("Email: " + receipt.userEmail);
            centerText("Total Amount: NZD " + std::to_string(receipt.totalAmount));
            centerText("Date: " + std::string(ctime(&receipt.receiptDate)));
            if (paid) {
                centerText("Payment Method: " + receipt.paymentMethod);
            }
            centerText("Services Used:");
            for (const auto& service : receipt.services) {
                std::string serviceDetails = " - " + service.serviceType;
                if (service.duration > 0) {
                    serviceDetails += " (" + std::to_string(service.duration) + " minutes)";
                }
                else if (service.quantity > 0) {
                    serviceDetails += " (" + std::to_string(service.quantity) + " pages)";
                }
                serviceDetails += ": NZD " + std::to_string(service.cost);
                centerText(serviceDetails);
            }
            centerText("-------------------");
            found = true;
        }
    }

    if (!found) {
        centerText("No " + std::string(paid ? "paid" : "unpaid") + " bills found.");
    }
}

void Admin::viewTotalStats(const std::vector<User>& users) {
    clearConsole();
    std::vector<Receipt> receipts = loadReceiptsFromJson();

    // Initialize counters and totals for each service
    struct ServiceStats {
        int userCount = 0;
        int totalDuration = 0;
        int totalQuantity = 0;
        double totalRevenue = 0.0;
    };

    std::map<std::string, ServiceStats> serviceStats;
    int totalUsers = users.size();
    int onlineUsers = 0;
    double totalRevenue = 0.0;
    int totalTransactions = 0;

    // Count online users
    for (const auto& user : users) {
        if (user.isLoggedIn) onlineUsers++;
    }

    // Process all receipts
    for (const auto& receipt : receipts) {
        if (receipt.isPaid) {
            totalRevenue += receipt.totalAmount;
            totalTransactions++;

            for (const auto& service : receipt.services) {
                ServiceStats& stats = serviceStats[service.serviceType];
                stats.userCount++;
                stats.totalRevenue += service.cost;

                if (service.duration > 0) {
                    stats.totalDuration += service.duration;
                }
                if (service.quantity > 0) {
                    stats.totalQuantity += service.quantity;
                }
            }
        }
    }

    // Display Statistics
    centerText(CYAN + "\n===== SKYLINE CYBER CAFE SYSTEM STATISTICS =====" + RESET);
    centerText("----------------------------------------");

    // General Statistics
    centerText(YELLOW + "General Statistics:" + RESET);
    centerText("Total Registered Users: " + std::to_string(totalUsers));
    centerText("Currently Online Users: " + std::to_string(onlineUsers));
    centerText("Total Completed Transactions: " + std::to_string(totalTransactions));
    centerText("Total Revenue: $" + std::to_string(totalRevenue));
    centerText("----------------------------------------");

    // Service-specific Statistics
    centerText(YELLOW + "Service-wise Statistics:" + RESET);

    // Internet Browsing Stats
    if (serviceStats.count("Internet Browsing")) {
        const auto& stats = serviceStats["Internet Browsing"];
        centerText(GREEN + "\nInternet Browsing:" + RESET);
        centerText("Total Users Logged-In: " + std::to_string(stats.userCount));
        centerText("Total Minutes Used: " + std::to_string(stats.totalDuration));
        centerText("Average Session Length: " +
            std::to_string(stats.userCount > 0 ? stats.totalDuration / stats.userCount : 0) + " minutes");
        centerText("Total Revenue: $" + std::to_string(stats.totalRevenue));
    }

    // Gaming Stats
    if (serviceStats.count("Gaming")) {
        const auto& stats = serviceStats["Gaming"];
        centerText(GREEN + "\nGaming:" + RESET);
        centerText("Total Users: " + std::to_string(stats.userCount));
        centerText("Total Gaming Minutes: " + std::to_string(stats.totalDuration));
        centerText("Average Gaming Session: " +
            std::to_string(stats.userCount > 0 ? stats.totalDuration / stats.userCount : 0) + " minutes");
        centerText("Total Revenue: $" + std::to_string(stats.totalRevenue));
    }

    // Printing Stats
    if (serviceStats.count("Printing")) {
        const auto& stats = serviceStats["Printing"];
        centerText(GREEN + "\nPrinting:" + RESET);
        centerText("Total Users: " + std::to_string(stats.userCount));
        centerText("Total Pages Printed: " + std::to_string(stats.totalQuantity));
        centerText("Average Pages per User: " +
            std::to_string(stats.userCount > 0 ? stats.totalQuantity / stats.userCount : 0));
        centerText("Total Revenue: $" + std::to_string(stats.totalRevenue));
    }

    // Scanning Stats
    if (serviceStats.count("Scanning")) {
        const auto& stats = serviceStats["Scanning"];
        centerText(GREEN + "\nScanning:" + RESET);
        centerText("Total Users: " + std::to_string(stats.userCount));
        centerText("Total Pages Scanned: " + std::to_string(stats.totalQuantity));
        centerText("Average Pages per User: " +
            std::to_string(stats.userCount > 0 ? stats.totalQuantity / stats.userCount : 0));
        centerText("Total Revenue: $" + std::to_string(stats.totalRevenue));
    }

    centerText("\n----------------------------------------");
}


// ============= Payments   =============

bool User::hasPaidBill() const {
    return std::all_of(sessions.begin(), sessions.end(),
        [](const Session& s) { return s.isPaid; });
}

// New payment handling function
void handlePayment(User& user) {
    centerText(GREEN + "Your total bill is: NZD " + std::to_string(user.totalBill) + RESET);

    std::vector<std::string> paymentMenuItems = {
        "1. EFTPOS",
        "2. Card Payment",
        "3. Cash",
        "4. Account Transfer",
        "5. Cancel Payment"
    };

    displayCenteredMenu(paymentMenuItems, "Payment Methods");

    int paymentChoice;
    getUserInput("Choose payment method:", paymentChoice);

    if (paymentChoice >= 1 && paymentChoice <= 4) {
        char confirm;
        centerText("Are you sure you want to proceed with this payment method? (y/n):");
        std::cin >> confirm;
        std::cin.ignore();

        if (confirm == 'y' || confirm == 'Y') {
            switch (paymentChoice) {
            case 1: { // EFTPOS
                centerText("Processing payment...");
                // Simulate processing time
                std::this_thread::sleep_for(std::chrono::seconds(3));
                centerText(GREEN + "Your payment via EFTPOS has been completed successfully." + RESET);
                centerText(GREEN + "Thank you for visiting Skyline Cyber Café!" + RESET);
                user.totalBill = 0.0;
                break;
            }
            case 2: { // Card Payment
                if (handleCardPayment()) {
                    centerText(GREEN + "Payment successful! Thank you for choosing Skyline Cyber Cafe!" + RESET);
                    user.totalBill = 0.0;
                }
                break;
            }
            case 3: // Cash
                centerText("Please proceed to the counter to pay your bill.");
                break;
            case 4: { // Account Transfer
                centerText(BLUE + "Processing account transfer..." + RESET);
                // Simulate processing time
                std::this_thread::sleep_for(std::chrono::seconds(5));
                centerText(GREEN + "Payment received via account transfer." + RESET);
                centerText(GREEN + "Thank you for choosing Skyline Cyber Cafe!" + RESET);
                user.totalBill = 0.0;
                break;
            }
            }
        }
        else {
            centerText(RED + "Payment cancelled." + RESET);
        }
    }
    else if (paymentChoice != 5) {
        centerText(RED + "Invalid payment method." + RESET);
    }
}

// Helper function for card payment processing
bool handleCardPayment() {
    std::string cardNumber, cardHolder, expiryDate, cvv;
    bool validInput;

    // Card Number validation
    do {
        getUserInput("Enter Card Number (16 digits):", cardNumber);
        if (cardNumber.length() == 16 && std::all_of(cardNumber.begin(), cardNumber.end(), ::isdigit)) {
            std::string formattedCard = cardNumber.substr(0, 2) + "-" +
                cardNumber.substr(2, 4) + "-" +
                cardNumber.substr(6, 7) + "-" +
                cardNumber.substr(13, 3);
            centerText(YELLOW + "Confirm card number: " + formattedCard + " (y/n):" + RESET);
            char confirm;
            std::cin >> confirm;
            std::cin.ignore();
            validInput = (confirm == 'y' || confirm == 'Y');
        }
        else {
            centerText(RED + "Invalid card number format." + RESET);
            validInput = false;
        }
    } while (!validInput);

    // Cardholder Name
    do {
        getUserInput("Enter Cardholder Name:", cardHolder);
        centerText(YELLOW + "Confirm name: " + cardHolder + " (y/n):" + RESET);
        char confirm;
        std::cin >> confirm;
        std::cin.ignore();
        validInput = (confirm == 'y' || confirm == 'Y');
    } while (!validInput);

    // Expiry Date
    do {
        getUserInput("Enter Expiry Date (MM/YY):", expiryDate);
        if (expiryDate.length() == 5 && expiryDate[2] == '/' &&
            std::isdigit(expiryDate[0]) && std::isdigit(expiryDate[1]) &&
            std::isdigit(expiryDate[3]) && std::isdigit(expiryDate[4])) {
            int month = std::stoi(expiryDate.substr(0, 2));
            if (month >= 1 && month <= 12) {
                centerText(YELLOW + "Confirm expiry date: " + expiryDate + " (y/n):" + RESET);
                char confirm;
                std::cin >> confirm;
                std::cin.ignore();
                validInput = (confirm == 'y' || confirm == 'Y');
            }
            else {
                centerText(RED + "Invalid month." + RESET);
                validInput = false;
            }
        }
        else {
            centerText(RED + "Invalid date format." + RESET);
            validInput = false;
        }
    } while (!validInput);

    // CVV
    do {
        getUserInput("Enter CVV (3 digits):", cvv);
        if (cvv.length() == 3 && cvv != "000" && std::all_of(cvv.begin(), cvv.end(), ::isdigit)) {
            centerText(YELLOW + "Confirm CVV: " + cvv + " (y/n):" + RESET);
            char confirm;
            std::cin >> confirm;
            std::cin.ignore();
            validInput = (confirm == 'y' || confirm == 'Y');
        }
        else {
            centerText(RED + "Invalid CVV." + RESET);
            validInput = false;
        }
    } while (!validInput);

    centerText("Processing payment...");
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return true;
}


// ============= User input and menu   =============
User::User(std::string n, std::string e, std::string p, int id)
    : name(n), email(e), password(p), userID(id), phoneno(""), totalBill(0.0),
    isLoggedIn(false), joinDate(time(0)) {}

void getUserInput(const std::string& prompt, int& input) {
    centerText(prompt);
    centerText(""); // Empty line for spacing
    std::cout << std::string((80 - 20) / 2, ' ') << "> ";
    std::cin >> input;
    std::cin.ignore();
}

void getUserInput(const std::string& prompt, std::string& input) {
    centerText(prompt);
    centerText(""); // Empty line for spacing
    std::cout << std::string((80 - 20) / 2, ' ') << "> ";
    std::getline(std::cin, input);
}

void handleUserMenu(User& user) {
    bool logoutPermitted = false;
    std::vector<ServiceUsage> currentServices;
    Receipt receipt;
    receipt.userID = user.userID;
    receipt.userName = user.name;
    receipt.userEmail = user.email;
    receipt.receiptDate = time(0);
    receipt.isPaid = false;
    receipt.paymentMethod = "";

    int userChoice;
    const double browsingRate = 0.50;
    const double gamingRate = 1.20;
    const double printCost = 0.70;
    const double scanCost = 0.52;
    bool hasUsedService = false;

    do {
        std::vector<std::string> menuItems;
        menuItems = {
            "1. Internet Browsing ($0.50/min)",
            "2. Gaming ($1.20/min)",
            "3. Printing ($0.70/page)",
            "4. Scanning ($0.52/page)"
        };

        if (hasUsedService || user.totalBill > 0) {
            menuItems.push_back("5. View/Pay Total Bill");
        }
        menuItems.push_back("6. Back to Main Menu");

        displayCenteredMenu(menuItems, "User Menu");
        getUserInput("Choose an option:", userChoice);

        ServiceUsage usage;
        usage.timestamp = time(0);

        switch (userChoice) {
        case 1: { // Internet Browsing
            usage.serviceType = "Internet Browsing";
            usage.rate = browsingRate;
            usage.quantity = 0;

            centerText(GREEN + "The rate is $0.50 per minute." + RESET);
            centerText("Special Offer: 15% off for 120 minutes or more!");
            getUserInput("Enter minutes used:", usage.duration);

            usage.cost = usage.duration * browsingRate;
            if (usage.duration >= 120) {
                usage.cost *= 0.85; // 15% discount
                centerText(GREEN + "Discount applied! 15% off for using 120 minutes or more!" + RESET);
            }

            user.startSession();
            user.endSession(usage.cost);
            currentServices.push_back(usage);
            hasUsedService = true;
            break;
        }
        case 2: { // Gaming
            usage.serviceType = "Gaming";
            usage.rate = gamingRate;
            usage.quantity = 0;

            centerText(GREEN + "The rate is $1.20 per minute." + RESET);
            centerText("Special Offer: 20% off for 120 minutes or more!");
            getUserInput("Enter minutes played:", usage.duration);

            usage.cost = usage.duration * gamingRate;
            if (usage.duration >= 120) {
                usage.cost *= 0.8; // 20% discount
                centerText(GREEN + "Discount applied! 20% off for playing 120 minutes or more!" + RESET);
            }

            user.startSession();
            user.endSession(usage.cost);
            currentServices.push_back(usage);
            hasUsedService = true;
            break;
        }
        case 3: { // Printing
            usage.serviceType = "Printing";
            usage.rate = printCost;
            usage.duration = 0;

            centerText(GREEN + "The cost for printing is $0.70 per page." + RESET);
            getUserInput("Enter the number of pages to print:", usage.quantity);

            usage.cost = usage.quantity * printCost;
            user.startSession();
            user.endSession(usage.cost);
            currentServices.push_back(usage);
            hasUsedService = true;
            break;
        }
        case 4: { // Scanning
            usage.serviceType = "Scanning";
            usage.rate = scanCost;
            usage.duration = 0;

            centerText(GREEN + "The cost for scanning is $0.52 per page." + RESET);
            getUserInput("Enter the number of pages to scan:", usage.quantity);

            usage.cost = usage.quantity * scanCost;
            user.startSession();
            user.endSession(usage.cost);
            currentServices.push_back(usage);
            hasUsedService = true;
            break;
        }
        case 5: { // View/Pay Total Bill
            if (hasUsedService || user.totalBill > 0) {
                // Display the detailed invoice
                clearConsole();
                centerText(YELLOW + "=== SKYLINE CYBER CAFE INVOICE ===" + RESET);
                centerText("Customer: " + user.name);
                centerText("ID: " + std::to_string(user.userID));
                centerText("Email: " + user.email);
                centerText("----------------------------------------");

                centerText("Services Used:");
                double subtotal = 0.0;
                for (const auto& service : currentServices) {
                    std::string serviceDetails = service.serviceType;
                    if (service.duration > 0) {
                        serviceDetails += " (" + std::to_string(service.duration) + " minutes @ $" +
                            std::to_string(service.rate) + "/min)";
                    }
                    else if (service.quantity > 0) {
                        serviceDetails += " (" + std::to_string(service.quantity) + " pages @ $" +
                            std::to_string(service.rate) + "/page)";
                    }

                    subtotal += service.cost;
                    std::string costLine = serviceDetails + " = $" + std::to_string(service.cost);
                    centerText(costLine);
                }

                centerText("----------------------------------------");
                centerText("Subtotal: $" + std::to_string(subtotal));
                centerText("Total Amount Due: $" + std::to_string(user.totalBill));
                centerText("========================================");

                centerText("\nPress Enter to proceed to payment options...");
                std::cin.get();

                std::vector<std::string> paymentMenuItems = {
                    "1. EFTPOS",
                    "2. Card Payment",
                    "3. Cash",
                    "4. Account Transfer",
                    "5. Cancel Payment"
                };

                displayCenteredMenu(paymentMenuItems, "Payment Methods");

                int paymentChoice;
                getUserInput("Choose payment method:", paymentChoice);

                bool paymentSuccessful = false;

                if (paymentChoice >= 1 && paymentChoice <= 4) {
                    char confirm;
                    centerText("Are you sure you want to proceed with this payment method? (y/n):");
                    std::cin >> confirm;
                    std::cin.ignore();

                    if (confirm == 'y' || confirm == 'Y') {
                        switch (paymentChoice) {
                        case 1: { // EFTPOS
                            centerText("Processing EFTPOS payment...");
                            std::this_thread::sleep_for(std::chrono::seconds(3));
                            centerText(GREEN + "Payment via EFTPOS completed successfully." + RESET);
                            receipt.paymentMethod = "EFTPOS";
                            paymentSuccessful = true;
                            break;
                        }
                        case 2: { // Card Payment
                            if (handleCardPayment()) {
                                receipt.paymentMethod = "Card";
                                paymentSuccessful = true;
                            }
                            break;
                        }
                        case 3: { // Cash
                            centerText("Please proceed to the counter to pay your bill.");
                            centerText("Press 'Y' when payment is confirmed by staff:");
                            char staffConfirm;
                            std::cin >> staffConfirm;
                            std::cin.ignore();
                            if (staffConfirm == 'y' || staffConfirm == 'Y') {
                                receipt.paymentMethod = "Cash";
                                paymentSuccessful = true;
                            }
                            break;
                        }
                        case 4: { // Account Transfer
                            centerText(BLUE + "Processing account transfer..." + RESET);
                            std::this_thread::sleep_for(std::chrono::seconds(5));
                            centerText(GREEN + "Account transfer completed successfully." + RESET);
                            receipt.paymentMethod = "Account Transfer";
                            paymentSuccessful = true;
                            break;
                        }
                        }

                        if (paymentSuccessful) {
                            receipt.isPaid = true;
                            receipt.receiptDate = time(0);
                            receipt.services = currentServices;
                            receipt.totalAmount = user.totalBill;
                            saveReceiptToJson(receipt);

                            // Display the receipt
                            displayReceipt(receipt);

                            // Handle email option
                            char emailChoice;
                            centerText("\nWould you like a copy of the receipt emailed to " + user.email + "? (y/n):");
                            std::cin >> emailChoice;
                            std::cin.ignore();

                            if (emailChoice == 'y' || emailChoice == 'Y') {
                                centerText(BLUE + "Sending receipt to " + user.email + "..." + RESET);
                                std::this_thread::sleep_for(std::chrono::seconds(2));
                                centerText(GREEN + "Receipt has been sent to your email!" + RESET);
                            }

                            user.totalBill = 0.0;
                            centerText(GREEN + "\nThank you for choosing Skyline Cyber Cafe!" + RESET);
                            centerText(CYAN + "We hope to see you again soon!" + RESET);
                            std::this_thread::sleep_for(std::chrono::seconds(2));
                            return; // Automatically exit to main menu after payment
                        }
                    }
                    else {
                        centerText(RED + "Payment cancelled." + RESET);
                    }
                }
                else if (paymentChoice != 5) {
                    centerText(RED + "Invalid payment method." + RESET);
                }
            }
            else {
                centerText(RED + "No services used yet!" + RESET);
            }
            break;
        }
        case 6: // Back to Main Menu
            if (!logoutPermitted && (hasUsedService || user.totalBill > 0)) {
                centerText(RED + "Please pay your bill before logging out." + RESET);
                userChoice = 0; // Prevent logout
            }
            else {
                centerText(CYAN + "Returning to main menu..." + RESET);
            }
            break;
        default:
            centerText(RED + "Invalid choice. Please try again." + RESET);
        }

        if (userChoice != 6) {
            centerText("\nPress Enter to continue...");
            std::cin.get();
        }
    } while (userChoice != 6 || !logoutPermitted);
}


// ============= Receipts   =============

void saveReceiptToJson(const Receipt& receipt) {
    nlohmann::json receiptData;
    const std::string filename = "Receipts.json";

    // Load existing receipts
    std::ifstream inFile(filename);
    if (inFile.good()) {
        receiptData = nlohmann::json::parse(inFile);
    }
    else {
        receiptData["receipts"] = nlohmann::json::array();
    }
    inFile.close();

    // Create new receipt JSON object
    nlohmann::json newReceipt;
    newReceipt["userID"] = receipt.userID;
    newReceipt["userName"] = receipt.userName;
    newReceipt["userEmail"] = receipt.userEmail;
    newReceipt["totalAmount"] = receipt.totalAmount;
    newReceipt["receiptDate"] = receipt.receiptDate;
    newReceipt["paymentMethod"] = receipt.paymentMethod;
    newReceipt["isPaid"] = receipt.isPaid;

    // Add services
    newReceipt["services"] = nlohmann::json::array();
    for (const auto& service : receipt.services) {
        nlohmann::json serviceObj;
        serviceObj["type"] = service.serviceType;
        serviceObj["duration"] = service.duration;
        serviceObj["quantity"] = service.quantity;
        serviceObj["rate"] = service.rate;
        serviceObj["cost"] = service.cost;
        serviceObj["timestamp"] = service.timestamp;
        newReceipt["services"].push_back(serviceObj);
    }

    receiptData["receipts"].push_back(newReceipt);

    // Save to file
    std::ofstream outFile(filename);
    outFile << receiptData.dump(2);
    outFile.close();
}

void displayReceipt(const Receipt& receipt) {
    clearConsole();
    centerText(BLUE + "=== SKYLINE CYBER CAFE RECEIPT ===" + RESET);
    centerText("Receipt Date: " + std::string(ctime(&receipt.receiptDate)));
    centerText("Customer: " + receipt.userName);
    centerText("ID: " + std::to_string(receipt.userID));
    centerText("Email: " + receipt.userEmail);
    centerText("----------------------------------------");

    centerText("Services Used:");
    for (const auto& service : receipt.services) {
        std::string serviceDetails = service.serviceType;
        if (service.duration > 0) {
            serviceDetails += " (" + std::to_string(service.duration) + " minutes)";
        }
        else if (service.quantity > 0) {
            serviceDetails += " (" + std::to_string(service.quantity) + " pages)";
        }

        std::string costLine = serviceDetails +
            " @ $" + std::to_string(service.rate) +
            " = $" + std::to_string(service.cost);
        centerText(costLine);
    }

    centerText("----------------------------------------");
    centerText("Total Amount: $" + std::to_string(receipt.totalAmount));
    centerText("Payment Status: " + std::string(receipt.isPaid ? "PAID" : "UNPAID"));
    if (receipt.isPaid) {
        centerText("Payment Method: " + receipt.paymentMethod);
    }
    centerText("========================================");
}

void displayInvoice(const std::vector<ServiceUsage>& services, double totalAmount, const User& user) {
    clearConsole();
    centerText(YELLOW + "=== SKYLINE CYBER CAFE INVOICE ===" + RESET);
    //centerText("Date: " + std::string(ctime(&time(0))));
    centerText("Customer: " + user.name);
    centerText("ID: " + std::to_string(user.userID));
    centerText("Email: " + user.email);
    centerText("----------------------------------------");

    centerText("Services Used:");
    double subtotal = 0.0;
    for (const auto& service : services) {
        std::string serviceDetails = service.serviceType;
        if (service.duration > 0) {
            serviceDetails += " (" + std::to_string(service.duration) + " minutes @ $" +
                std::to_string(service.rate) + "/min)";
        }
        else if (service.quantity > 0) {
            serviceDetails += " (" + std::to_string(service.quantity) + " pages @ $" +
                std::to_string(service.rate) + "/page)";
        }

        subtotal += service.cost;
        std::string costLine = serviceDetails + " = $" + std::to_string(service.cost);
        centerText(costLine);
    }

    centerText("----------------------------------------");
    centerText("Subtotal: $" + std::to_string(subtotal));
    centerText("Total Amount Due: $" + std::to_string(totalAmount));
    centerText("========================================");
}


void handleReceiptEmail(const Receipt& receipt) {
    char emailChoice;
    centerText("\nWould you like a copy of the receipt emailed to " + receipt.userEmail + "? (y/n):");
    std::cin >> emailChoice;
    std::cin.ignore();

    if (emailChoice == 'y' || emailChoice == 'Y') {
        // Simulating email sending
        centerText(BLUE + "Sending receipt to " + receipt.userEmail + "..." + RESET);
        std::this_thread::sleep_for(std::chrono::seconds(2));
        centerText(GREEN + "Receipt has been sent to your email!" + RESET);
    }

    centerText(GREEN + "\nThank you for choosing Skyline Cyber Cafe!" + RESET);
    centerText(CYAN + "We hope to see you again soon!" + RESET);
    std::this_thread::sleep_for(std::chrono::seconds(2));
}
