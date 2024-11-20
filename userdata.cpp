//userdata.cpp
#define _CRT_SECURE_NO_WARNINGS

#include "userdata.h"
#include "display.h"
#include "validation.h"
#include"logo.h"
#include <iostream>
#include <fstream>
#include <conio.h>// hide passwords 
#include <iomanip>
#include <string>
#include <algorithm>
#include <ctime>
#include <map>
#include <string>
#include <thread>
#include <conio.h>// For std::this_thread::sleep_for

#ifdef _WIN32
#include <windows.h>
#endif



using namespace std;

// ============= Global Dec =============

int userIDCounter = 1;




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
    newUser["phoneno"] = user.phoneno;
    newUser["joinDate"] = user.joinDate;

    userData["users"].push_back(newUser);

    std::ofstream outFile(filename);
    outFile << userData.dump(2);
    outFile.close();




    centerText(GREEN + "Registration successful!  ");
    std::cout << "\n";

}


void handleRegistration(std::vector<User>& users, int& userIDCounter) {
    clearConsole();

    UserRegistration newUser;
    newUser.joinDate = time(0);

    // Username with validation
    do {
        // clearConsole();
        displayLogo(2);

        centerText(YELLOW + "\t\t\tEnter Username:" + RESET);
        cout << string((80 - 20) / 2, ' ') << "> ";

        // Get username with character input control
        string input;
        char ch;
        while ((ch = _getch()) != 13) {  // 13 is Enter key
            if (ch == 8) {  // Backspace
                if (!input.empty()) {
                    input.pop_back();
                    cout << "\b \b";  // Erase character from screen
                }
            }
            else if (ch >= 32 && ch <= 126) {  // Printable characters only
                input += ch;
                cout << ch;
            }
        }
        cout << endl;

        newUser.username = input;

        if (!isValidUserName(newUser.username)) {
            centerText(RED + "Invalid username format. It must be at least 6 characters long, start with a letter," + RESET);
            centerText(RED + "and can only include the special characters: @, ., -, and _. Please try again..." + RESET);
            centerText("");
            Sleep(1500);
        }
    } while (!isValidUserName(newUser.username));

    // Password with validation and hiding
    std::string confirmPassword;
    do {
        //clearConsole();
       // displayLogo(2);
        centerText(YELLOW + "\t\t\tEnter Password:" + RESET);
        cout << string((80 - 20) / 2, ' ') << "> ";

        // Get password securely
        string password;
        char ch;
        while ((ch = _getch()) != 13) {
            if (ch == 8) {  // Backspace
                if (!password.empty()) {
                    password.pop_back();
                    cout << "\b \b";
                }
            }
            else if (ch >= 32 && ch <= 126) {
                password += ch;
                cout << "*";
            }
        }
        cout << endl;
        newUser.password = password;

        if (!isValidDetailedPassword(newUser.password)) {
            centerText(RED + "Invalid password format. Must be 8+ characters, include uppercase, lowercase," + RESET);
            centerText(RED + "number, and special character. Try again.." + RESET);
            Sleep(1500);
            continue;
        }

        centerText(YELLOW + "\t\t\tConfirm Password:" + RESET);
        cout << string((80 - 20) / 2, ' ') << "> ";

        // Get confirmation password
        string confirm;
        while ((ch = _getch()) != 13) {
            if (ch == 8) {
                if (!confirm.empty()) {
                    confirm.pop_back();
                    cout << "\b \b";
                }
            }
            else if (ch >= 32 && ch <= 126) {
                confirm += ch;
                cout << "*";
            }
        }
        cout << endl;
        confirmPassword = confirm;

        if (newUser.password != confirmPassword) {
            centerText(RED + "Passwords do not match. Please try again." + RESET);
            Sleep(1500);
        }
    } while (!isValidDetailedPassword(newUser.password) || newUser.password != confirmPassword);

    // Full Name
    do {
        //  clearConsole();
        //  displayLogo(2);
        centerText(YELLOW + "\t\t\tEnter Full Name:" + RESET);
        cout << string((80 - 20) / 2, ' ') << "> ";

        string input;
        char ch;
        while ((ch = _getch()) != 13) {
            if (ch == 8) {
                if (!input.empty()) {
                    input.pop_back();
                    cout << "\b \b";
                }
            }
            else if ((ch >= 32 && ch <= 126) && input.length() < 50) {  // Limit name length
                input += ch;
                cout << ch;
            }
        }
        cout << endl;
        newUser.fullname = input;

        if (newUser.fullname.empty()) {
            centerText(RED + "Name cannot be empty. Please try again." + RESET);
            Sleep(1500);
        }
    } while (newUser.fullname.empty());

    // Email with validation and duplicate check
    std::string confirmEmail;
    bool validEmail = false;
    do {
        //clearConsole();
       // displayLogo(2);
        centerText(YELLOW + "\t\t\tEnter Email:" + RESET);
        cout << string((80 - 20) / 2, ' ') << "> ";

        string input;
        char ch;
        while ((ch = _getch()) != 13) {
            if (ch == 8) {
                if (!input.empty()) {
                    input.pop_back();
                    cout << "\b \b";
                }
            }
            else if (ch >= 32 && ch <= 126) {
                input += ch;
                cout << ch;
            }
        }
        cout << endl;
        newUser.email = input;

        if (!isValidDetailedEmail(newUser.email)) {
            centerText(RED + "Invalid email format. Please try again." + RESET);
            Sleep(1500);
            continue;
        }

        if (isDuplicateEmail(newUser.email)) {
            centerText(RED + "Email already registered. Please login or use a different email." + RESET);
            Sleep(1500);
            continue;
        }

        centerText(YELLOW + "\t\t\tConfirm Email:" + RESET);
        cout << string((80 - 20) / 2, ' ') << "> ";

        input.clear();
        while ((ch = _getch()) != 13) {
            if (ch == 8) {
                if (!input.empty()) {
                    input.pop_back();
                    cout << "\b \b";
                }
            }
            else if (ch >= 32 && ch <= 126) {
                input += ch;
                cout << ch;
            }
        }
        cout << endl;
        confirmEmail = input;

        if (newUser.email != confirmEmail) {
            centerText(RED + "Emails do not match. Please try again." + RESET);
            Sleep(1500);
            continue;
        }
        validEmail = true;
    } while (!validEmail);

    // Phone number validation with duplicate check
    bool validPhone = false;
    do {
        // clearConsole();
        // displayLogo(2);
        centerText(YELLOW + "\t\t\tEnter Phone Number:" + RESET);
        cout << string((80 - 20) / 2, ' ') << "> ";

        string input;
        char ch;
        while ((ch = _getch()) != 13) {
            if (ch == 8) {
                if (!input.empty()) {
                    input.pop_back();
                    cout << "\b \b";
                }
            }
            else if ((ch >= 32 && ch <= 126) && input.length() < 15) {  // Limit phone length
                input += ch;
                cout << ch;
            }
        }
        cout << endl;

        std::string phoneStr = input;

        if (!isValidPhoneNumber(phoneStr)) {
            centerText(RED + "Invalid phone number format. Must be 10-13 digits and may include spaces, +, -, (, )." + RESET);
            centerText(RED + "Please enter a valid number" + RESET);
            Sleep(1500);
            continue;
        }

        // Clean the phone number
        std::string cleanNumber;
        for (char c : phoneStr) {
            if (isdigit(c)) {
                cleanNumber += c;
            }
        }

        if (isDuplicatePhone(cleanNumber)) {
            centerText(RED + "Phone number already registered. Please use a different number." + RESET);
            Sleep(1500);
            continue;
        }
        validPhone = true;
        newUser.phoneno = cleanNumber;
    } while (!validPhone);

    // Save user data
    saveUserToJson(newUser);
    users.emplace_back(newUser.fullname, newUser.email, newUser.password, userIDCounter++);

    // clearConsole();
   //  displayLogo(2);
    // centerText(GREEN + "Registration successful!" + RESET);
    centerText(YELLOW + "Welcome to Skyline Cyber Cafe, " + newUser.fullname + "!" + RESET);
    Sleep(2000);
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


//// ============= Admin  =============


Admin::Admin(std::string e, std::string p) : email(e), password(p) {}

void handleAdminMenu(Admin& admin, std::vector<User>& users, std::vector<Admin>& admins) {
    int choice = 0;  // Initialize choice variable
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

    do {
        ShowLogoAnimation();
        displayCenteredMenu(menuItems, "\tAdmin Menu");

        std::string inputStr;
        centerText(YELLOW + "\t\t\tEnter choice (1-9):");
        centerText("");
        std::cout << std::string((80 - 20) / 2, ' ') << "\t\t> ";
        std::getline(std::cin, inputStr);

        // Clear screen for error message if needed
        clearConsole();
        StopLogoAnimation();

        if (!isInteger(inputStr) || std::stoi(inputStr) < 1 || std::stoi(inputStr) > 9) {
            centerText(RED + "\t\tInvalid choice. Please enter a number between 1 and 9." + RESET);
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            continue;
        }

        choice = std::stoi(inputStr);

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
        }

        if (choice != 9) {
            centerText("\nPress Enter to continue...");
            std::cin.get();
            clearConsole();
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
            centerText(YELLOW + "User ID: " + std::to_string(receipt.userID) + RESET);
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
    centerText(CYAN + "\t\t===== SKYLINE CYBER CAFE SYSTEM STATISTICS =====" + RESET);
    centerText(YELLOW + "----------------------------------------");

    // General Statistics
    centerText(YELLOW + "General Statistics:" + RESET);
    centerText("Total Registered Users: " + std::to_string(totalUsers));
    centerText("Currently Online Users: " + std::to_string(onlineUsers));
    centerText("Total Completed Transactions: " + std::to_string(totalTransactions));
    centerText("Total Revenue: $" + std::to_string(totalRevenue));
    centerText(YELLOW + "----------------------------------------");

    // Service-specific Statistics
    cout << "\n";
    centerText(CYAN + "  Service-wise Statistics:" + RESET);

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

    centerText(YELLOW + "----------------------------------------");
}


// ============= Payments   =============

bool User::hasPaidBill() const {
    return std::all_of(sessions.begin(), sessions.end(),
        [](const Session& s) { return s.isPaid; });
}

// New payment handling function
void handlePayment(User& user, std::vector<ServiceUsage>& currentServices) {  // Added currentServices parameter
    bool validPayment = false;
    Receipt receipt;  // Create receipt object
    receipt.userID = user.userID;
    receipt.userName = user.name;
    receipt.userEmail = user.email;
    receipt.receiptDate = time(0);
    receipt.isPaid = false;
    receipt.services = currentServices;  // Store current services
    receipt.totalAmount = user.totalBill;

    do {
        clearConsole();
        ShowLogoAnimation();
        centerText(GREEN + "Your total bill is: NZD " + std::to_string(user.totalBill) + RESET);

        std::vector<std::string> paymentMenuItems = {
            "1. EFTPOS",
            "2. Card Payment",
            "3. Cash",
            "4. Account Transfer",
            "5. Cancel Payment"
        };

        displayCenteredMenu(paymentMenuItems, "\tPayment Methods");


        // Get and validate user input
        std::string input;
        centerText(YELLOW + "\t\t\tChoose payment method (1-5):");
        centerText("");  // Empty line for spacing
        std::cout << std::string((80 - 20) / 2, ' ') << "\t\t> ";
        std::getline(std::cin, input);

        StopLogoAnimation();

        // Validate input
        if (!isInteger(input) || std::stoi(input) < 1 || std::stoi(input) > 5) {
            centerText(RED + "Invalid choice. Please enter a number between 1 and 5." + RESET);
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            continue;
        }

        int paymentChoice = std::stoi(input);

        if (paymentChoice >= 1 && paymentChoice <= 4) {
            clearConsole();
            centerText("Are you sure you want to proceed with this payment method? (y/n):");
            std::string confirmInput;
            std::cout << std::string((80 - 20) / 2, ' ') << "> ";
            std::getline(std::cin, confirmInput);

            if (!confirmInput.empty() && (confirmInput[0] == 'y' || confirmInput[0] == 'Y')) {
                switch (paymentChoice) {
                case 1: { // EFTPOS
                    centerText(BLUE + "Processing payment...");
                    std::this_thread::sleep_for(std::chrono::seconds(3));
                    centerText(GREEN + "Your payment via EFTPOS has been completed successfully." + RESET);
                    receipt.paymentMethod = "EFTPOS";
                    receipt.isPaid = true;
                    user.totalBill = 0.0;
                    validPayment = true;
                    break;
                }
                case 2: { // Card Payment
                    if (handleCardPayment()) {
                        std::this_thread::sleep_for(std::chrono::seconds(5));
                        centerText(GREEN + "Payment successful!" + RESET);
                        receipt.paymentMethod = "Card";
                        receipt.isPaid = true;
                        user.totalBill = 0.0;
                        validPayment = true;
                    }
                    break;
                }
                case 3: { // Cash
                    centerText(RED + "Please proceed to the counter to pay your bill.");
                    centerText("Press 'Y' when payment is confirmed by staff:");
                    std::string staffConfirm;
                    std::cout << std::string((80 - 20) / 2, ' ') << "> ";
                    std::getline(std::cin, staffConfirm);
                    if (!staffConfirm.empty() && (staffConfirm[0] == 'y' || staffConfirm[0] == 'Y')) {
                        centerText(GREEN + "Payment confirmed!" + RESET);
                        receipt.paymentMethod = "Cash";
                        receipt.isPaid = true;
                        user.totalBill = 0.0;
                        validPayment = true;
                    }
                    break;
                }
                case 4: { // Account Transfer
                    centerText(BLUE + "Processing account transfer..." + RESET);
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                    centerText(GREEN + "Payment received via account transfer." + RESET);
                    receipt.paymentMethod = "Account Transfer";
                    receipt.isPaid = true;
                    user.totalBill = 0.0;
                    validPayment = true;
                    break;
                }
                }

                if (validPayment) {
                    // Save the receipt to JSON and display it
                    saveReceiptToJson(receipt);
                    displayReceipt(receipt);
                    handleReceiptEmail(receipt);
                }
            }
            else {
                centerText(YELLOW + "Payment cancelled. Returning to payment menu..." + RESET);
                std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            }
        }
        else if (paymentChoice == 5) {
            centerText(YELLOW + "Payment cancelled." + RESET);
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            return;
        }

        if (validPayment) {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            return;
        }

    } while (!validPayment);
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
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return true;
}


// ============= User input and menu   =============
User::User(std::string n, std::string e, std::string p, int id)
    : name(n), email(e), password(p), userID(id), phoneno(""), totalBill(0.0),
    isLoggedIn(false), joinDate(time(0)) {}


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

    int userChoice = 0;
    const double browsingRate = 0.50;
    const double gamingRate = 1.20;
    const double printCost = 0.70;
    const double scanCost = 0.52;
    bool hasUsedService = false;

    do {
        // Clear screen before showing menu
        clearConsole();
        ShowLogoAnimation();

        // Create menu items based on whether services have been used
        std::vector<std::string> menuItems = {
            "1. Internet Browsing ($0.50/min)",
            "2. Gaming ($1.20/min)",
            "3. Printing ($0.70/page)",
            "4. Scanning ($0.52/page)"
        };

        // Add appropriate final options based on service usage
        if (hasUsedService || user.totalBill > 0) {
            menuItems.push_back("5. View/Pay Total Bill");
            menuItems.push_back("6. Back to Main Menu");
        }
        else {
            menuItems.push_back("5. Back to Main Menu");
        }

        // Display the menu

        displayCenteredMenu(menuItems, "\tUser Menu");

        // Get and validate user input
        std::string input;
        const int maxOption = (hasUsedService || user.totalBill > 0) ? 6 : 5;
        centerText(YELLOW + "\t\t\tChoose an option (1-" + std::to_string(maxOption) + "):");
        centerText("");  // Empty line for spacing
        std::cout << std::string((80 - 20) / 2, ' ') << "\t\t> ";
        std::getline(std::cin, input);

        StopLogoAnimation();
        clearConsole();

        // Validate input
        if (!isInteger(input) || std::stoi(input) < 1 || std::stoi(input) > maxOption) {
            centerText(RED + "Invalid choice. Please enter a number between 1 and " +
                std::to_string(maxOption) + "." + RESET);
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            continue;
        }

        userChoice = std::stoi(input);
        ServiceUsage usage;
        usage.timestamp = time(0);

        switch (userChoice) {
        case 1: { // Internet Browsing
            usage.serviceType = "Internet Browsing";
            usage.rate = browsingRate;
            usage.quantity = 0;

            centerText(GREEN + "The rate is $0.50 per minute." + RESET);
            centerText("Special Offer: 15% off for 120 minutes or more!");

            std::string durationInput;
            bool validDuration = false;
            do {
                getUserInput("Enter minutes used:", durationInput);
                if (isInteger(durationInput) && std::stoi(durationInput) > 0) {
                    usage.duration = std::stoi(durationInput);
                    validDuration = true;
                }
                else {
                    centerText(RED + "Please enter a valid number of minutes." + RESET);
                    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                }
            } while (!validDuration);

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

            std::string durationInput;
            bool validDuration = false;
            do {
                getUserInput("Enter minutes played:", durationInput);
                if (isInteger(durationInput) && std::stoi(durationInput) > 0) {
                    usage.duration = std::stoi(durationInput);
                    validDuration = true;
                }
                else {
                    centerText(RED + "Please enter a valid number of minutes." + RESET);
                    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                }
            } while (!validDuration);

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

            std::string pageInput;
            bool validPages = false;
            do {
                getUserInput("Enter the number of pages to print:", pageInput);
                if (isInteger(pageInput) && std::stoi(pageInput) > 0) {
                    usage.quantity = std::stoi(pageInput);
                    validPages = true;
                }
                else {
                    centerText(RED + "Please enter a valid number of pages." + RESET);
                    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                }
            } while (!validPages);

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

            std::string pageInput;
            bool validPages = false;
            do {
                getUserInput("Enter the number of pages to scan:", pageInput);
                if (isInteger(pageInput) && std::stoi(pageInput) > 0) {
                    usage.quantity = std::stoi(pageInput);
                    validPages = true;
                }
                else {
                    centerText(RED + "Please enter a valid number of pages." + RESET);
                    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                }
            } while (!validPages);

            usage.cost = usage.quantity * scanCost;
            user.startSession();
            user.endSession(usage.cost);
            currentServices.push_back(usage);
            hasUsedService = true;
            break;
        }
        case 5: {
            if (hasUsedService || user.totalBill > 0) {
                // Handle bill payment
                clearConsole();
                displayInvoice(currentServices, user.totalBill, user);
                centerText("\nPress Enter to proceed to payment options...");
                std::cin.get();
                handlePayment(user, currentServices);  // Pass currentServices to handlePayment
                if (user.totalBill == 0) {
                    return; // Exit to main menu after successful payment
                }
            }
            else {
                // Exit to main menu if no services used
                centerText(CYAN + "Returning to main menu..." + RESET);
                return;
            }
            break;
        }
        case 6: {
            if (hasUsedService || user.totalBill > 0) {
                if (!logoutPermitted && user.totalBill > 0) {
                    centerText(RED + "Please pay your bill before logging out." + RESET);
                    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                    userChoice = 0; // Prevent logout
                }
                else {
                    centerText(CYAN + "Returning to main menu..." + RESET);
                    return;
                }
            }
            break;
        }
        }

        if (userChoice != 5 && userChoice != 6) {
            centerText("\nPress Enter to continue...");
            std::cin.get();
        }

    } while (true);
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

    centerText("Customer: " + user.name);
    centerText(GREEN + "ID: " + std::to_string(user.userID) + RESET);
    centerText(YELLOW + "Email: " + user.email + RESET);
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
    centerText(RED + "Total Amount Due: $" + std::to_string(totalAmount) + RESET);
    centerText(YELLOW + "=====================================" + RESET);
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
