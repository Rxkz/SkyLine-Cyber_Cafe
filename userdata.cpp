//userdata.cpp
// Disables warnings about secure C runtime functions
#define _CRT_SECURE_NO_WARNINGS

// Custom header files for project functionality
#include "userdata.h"    // User class and data management functions
#include "display.h"     // Screen display and formatting functions
#include "validation.h"  // Input validation functions
#include "logo.h"       // Cafe logo and animation

// Standard C++ libraries
#include <iostream>    // For input/output operations
#include <fstream>     // For file handling operations
#include <conio.h>     // For password hiding and keyboard input
#include <iomanip>     // For output formatting
#include <string>      // For string handling
#include <algorithm>   // For general algorithms (like find, sort)
#include <ctime>      // For time-based operations
#include <map>        // For storing key-value pairs
#include <thread>     // For sleep/delay functions

// Conditional compilation for Windows-specific code
#ifdef _WIN32
#include <windows.h>   // For Windows-specific functions (like Sleep)
#endif


using namespace std;

// ============= Global Dec =============

int userIDCounter = 1;




// ============= User Sessions  =============

// Function to start a new user session
void User::startSession() {
    // Create new session object
    Session newSession;

    // Set start time to current time
    newSession.startTime = time(0);

    // Add new session to user's sessions vector
    sessions.push_back(newSession);

    // Set user's login status to true
    isLoggedIn = true;

    // Display success message in green
    centerText(GREEN + "Session started successfully!" + RESET);
}

// Function to end current user session and calculate bill
void User::endSession(double sessionBill) {
    // Check if there are sessions and user is logged in
    if (!sessions.empty() && isLoggedIn) {
        // Set end time of current session to now
        sessions.back().endTime = time(0);

        // Record cost of this session
        sessions.back().cost = sessionBill;

        // Add session cost to user's total bill
        totalBill += sessionBill;

        // Set login status to false
        isLoggedIn = false;
    }

    // Display session end message in blue
    centerText(BLUE + "Session ended successfully!" + RESET);

    // Display session cost
    centerText("Session Bill: NZD " + std::to_string(sessionBill));
}

// Function to calculate total time spent across all sessions
int User::getTotalTime() const {
    // Initialize counter for total minutes
    int totalMinutes = 0;

    // Loop through all user sessions
    for (const auto& session : sessions) {
        // For completed sessions (with end time)
        if (session.endTime > 0) {
            // Calculate duration between end and start time
            // difftime gives seconds, divide by 60 for minutes
            totalMinutes += static_cast<int>(difftime(session.endTime, session.startTime) / 60);
        }
        // For active session (user still logged in)
        else if (isLoggedIn) {
            // Calculate duration from start until now
            totalMinutes += static_cast<int>(difftime(time(0), session.startTime) / 60);
        }
    }

    // Return total duration in minutes
    return totalMinutes;
}

// ============= USER REGISTRATION  =============

/*
FUNCTION: handleRegistration
PURPOSE: Handles new user registration with validation and security checks
PARAMETERS:
- users: Vector storing all registered users
- userIDCounter: Counter for assigning unique user IDs
*/
void handleRegistration(std::vector<User>& users, int& userIDCounter) {
    // Clear screen for fresh start
    clearConsole();

    // Create new user object and set join timestamp
    UserRegistration newUser;
    newUser.joinDate = time(0);  // Current system time

    /*
    PART 1: USERNAME VALIDATION AND INPUT
    Features:
    - Character-by-character input control
    - Real-time display
    - Backspace handling
    - Format validation
    */
    do {
        displayLogo(2);  // Show cafe logo

        // Center username prompt with yellow highlight
        centerText(YELLOW + "\t\t\tEnter Username:" + RESET);
        cout << string((80 - 20) / 2, ' ') << "> ";  // Create input cursor

        // Character-by-character input handling
        string input;
        char ch;
        while ((ch = _getch()) != 13) {  // Loop until Enter key (ASCII 13)
            if (ch == 8) {  // Handle backspace (ASCII 8)
                if (!input.empty()) {
                    input.pop_back();          // Remove last character
                    cout << "\b \b";           // Move cursor back, erase, move back
                }
            }
            // Accept printable ASCII characters (32-126)
            else if (ch >= 32 && ch <= 126) {
                input += ch;                   // Add to input string
                cout << ch;                    // Show character
            }
        }
        cout << endl;

        newUser.username = input;

        // Validate username format and show error if invalid
        if (!isValidUserName(newUser.username)) {
            centerText(RED + "Invalid username format. It must be at least 6 characters long, start with a letter," + RESET);
            centerText(RED + "and can only include the special characters: @, ., -, and _. Please try again..." + RESET);
            centerText("");
            Sleep(1500);  // Pause for error message reading
        }
    } while (!isValidUserName(newUser.username));

    /*
    PART 2: PASSWORD INPUT AND VALIDATION
    Features:
    - Hidden password input (masked with *)
    - Password confirmation
    - Complexity validation
    - Secure input handling
    */
    std::string confirmPassword;
    do {
        centerText(YELLOW + "\t\t\tEnter Password:" + RESET);
        cout << string((80 - 20) / 2, ' ') << "> ";

        // Secure password input with masking
        string password;
        char ch;
        while ((ch = _getch()) != 13) {
            if (ch == 8) {  // Backspace handling
                if (!password.empty()) {
                    password.pop_back();
                    cout << "\b \b";  // Remove * from display
                }
            }
            else if (ch >= 32 && ch <= 126) {
                password += ch;        // Add character to password
                cout << "*";          // Display asterisk instead
            }
        }
        cout << endl;
        newUser.password = password;

        // Validate password complexity
        if (!isValidDetailedPassword(newUser.password)) {
            centerText(RED + "Invalid password format. Must be 8+ characters, include uppercase, lowercase," + RESET);
            centerText(RED + "number, and special character. Try again.." + RESET);
            Sleep(1500);
            continue;
        }

        // Password confirmation
        centerText(YELLOW + "\t\t\tConfirm Password:" + RESET);
        cout << string((80 - 20) / 2, ' ') << "> ";

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

        // Check if passwords match
        if (newUser.password != confirmPassword) {
            centerText(RED + "Passwords do not match. Please try again." + RESET);
            Sleep(1500);
        }
    } while (!isValidDetailedPassword(newUser.password) || newUser.password != confirmPassword);

    /*
    PART 3: FULL NAME INPUT
    Features:
    - 50 character limit
    - Non-empty validation
    - Visible character input
    */
    do {
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
            // Accept characters with length limit
            else if ((ch >= 32 && ch <= 126) && input.length() < 50) {
                input += ch;
                cout << ch;
            }
        }
        cout << endl;
        newUser.fullname = input;

        // Validate non-empty name
        if (newUser.fullname.empty()) {
            centerText(RED + "Name cannot be empty. Please try again." + RESET);
            Sleep(1500);
        }
    } while (newUser.fullname.empty());

    /*
    PART 4: EMAIL VALIDATION AND CONFIRMATION
    Features:
    - Format validation
    - Duplicate checking
    - Email confirmation
    - Multiple validation layers
    */
    std::string confirmEmail;
    bool validEmail = false;
    do {
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

        // Email format validation
        if (!isValidDetailedEmail(newUser.email)) {
            centerText(RED + "Invalid email format. Please try again." + RESET);
            Sleep(1500);
            continue;
        }

        // Check for duplicate email
        if (isDuplicateEmail(newUser.email)) {
            centerText(RED + "Email already registered. Please login or use a different email." + RESET);
            Sleep(1500);
            continue;
        }

        // Email confirmation
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

        // Check if emails match
        if (newUser.email != confirmEmail) {
            centerText(RED + "Emails do not match. Please try again." + RESET);
            Sleep(1500);
            continue;
        }
        validEmail = true;
    } while (!validEmail);

    /*
    PART 5: PHONE NUMBER VALIDATION
    Features:
    - Format validation
    - Number cleaning
    - Duplicate checking
    - Length limitation
    */
    bool validPhone = false;
    do {
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
            // Accept characters with 15 char limit
            else if ((ch >= 32 && ch <= 126) && input.length() < 15) {
                input += ch;
                cout << ch;
            }
        }
        cout << endl;

        std::string phoneStr = input;

        // Validate phone format
        if (!isValidPhoneNumber(phoneStr)) {
            centerText(RED + "Invalid phone number format. Must be 10-13 digits and may include spaces, +, -, (, )." + RESET);
            centerText(RED + "Please enter a valid number" + RESET);
            Sleep(1500);
            continue;
        }

        // Clean phone number (remove non-digits)
        std::string cleanNumber;
        for (char c : phoneStr) {
            if (isdigit(c)) {
                cleanNumber += c;
            }
        }

        // Check for duplicate phone
        if (isDuplicatePhone(cleanNumber)) {
            centerText(RED + "Phone number already registered. Please use a different number." + RESET);
            Sleep(1500);
            continue;
        }
        validPhone = true;
        newUser.phoneno = cleanNumber;
    } while (!validPhone);

    /*
    PART 6: USER DATA STORAGE
    Features:
    - JSON file storage
    - Memory storage
    - Unique ID assignment
    - Welcome message
    */
    // Save to JSON file
    saveUserToJson(newUser);

    // Add to active users with new ID
    users.emplace_back(newUser.fullname, newUser.email, newUser.password, userIDCounter++);

    // Display welcome message
    centerText(YELLOW + "Welcome to Skyline Cyber Cafe, " + newUser.fullname + "!" + RESET);
    Sleep(2000);  // Pause to show welcome message
}

// ============= JSON FUNCTIONS  =============

// Function to save new user registration to JSON file
void saveUserToJson(const UserRegistration& user) {
    // Create main JSON object for user data
    nlohmann::json userData;

    // Set filename for user registration data
    const std::string filename = "User_Registration.json";

    // Try to open existing user file
    std::ifstream inFile(filename);
    if (inFile.good()) {
        // If file exists, read current data
        userData = nlohmann::json::parse(inFile);
    }
    else {
        // If no file exists, create new users array
        userData["users"] = nlohmann::json::array();
    }
    inFile.close();  // Close input file

    // Create JSON object for new user
    nlohmann::json newUser;

    // Add all user registration details
    newUser["username"] = user.username;   // Login username
    newUser["password"] = user.password;   // User password
    newUser["fullname"] = user.fullname;   // Full name
    newUser["email"] = user.email;         // Email address
    newUser["phoneno"] = user.phoneno;     // Phone number
    newUser["joinDate"] = user.joinDate;   // Registration date

    // Add new user to users array
    userData["users"].push_back(newUser);

    // Open file for writing updated data
    std::ofstream outFile(filename);
    outFile << userData.dump(2);  // Write with formatting (indent=2)
    outFile.close();

    // Display success message in green
    centerText(GREEN + "Registration successful!  ");
    std::cout << "\n";  // Add blank line
}

// Function to update existing user's information in the JSON file
void updateUserInJson(const User& user, const std::string& originalEmail) {
    // Store the filename as a constant for file operations
    const std::string filename = "User_Registration.json";

    // Main try block to catch any unexpected errors during the entire process
    try {
        // SECTION 1: OPEN AND READ FILE
        //=================================
        // Attempt to open the JSON file for reading
        std::ifstream inFile(filename);

        // Check if file opened successfully
        if (!inFile.good()) {
            // Display error in red if file couldn't be opened
            centerText(RED + "Error: Could not open user file" + RESET);
            return; // Exit function if file can't be opened
        }

        // SECTION 2: PARSE JSON
        //=================================
        // Create JSON object to hold file data
        nlohmann::json userData;

        // Try to parse JSON from file
        try {
            // Parse file content into userData object
            userData = nlohmann::json::parse(inFile);
        }
        catch (const nlohmann::json::exception& e) {
            // Handle JSON parsing errors (malformed JSON)
            centerText(RED + "Error parsing JSON: " + e.what() + RESET);
            inFile.close(); // Close file before returning
            return;
        }

        // Close input file after successful reading
        inFile.close();

        // SECTION 3: VALIDATE JSON STRUCTURE
        //=================================
        // Check if JSON has required structure
        if (!userData.contains("users") || !userData["users"].is_array()) {
            // Show error if JSON structure is invalid
            centerText(RED + "Error: Invalid JSON structure" + RESET);
            return;
        }

        // SECTION 4: UPDATE USER DATA
        //=================================
        // Flag to track if we found the user to update
        bool userFound = false;

        // Loop through all users in JSON
        for (auto& jsonUser : userData["users"]) {
            try {
                // Check if current user matches the one we want to update
                if (jsonUser["email"].get<std::string>() == originalEmail) {
                    // APPLY SIZE LIMITS TO NEW DATA
                    // Limit name to 100 characters
                    std::string newName = user.name.substr(0, 100);

                    // Limit email to 100 characters
                    std::string newEmail = user.email.substr(0, 100);

                    // Limit password to 50 characters
                    std::string newPassword = user.password.substr(0, 50);

                    // Limit phone to 15 characters
                    std::string newPhone = user.phoneno.substr(0, 15);

                    // UPDATE USER FIELDS
                    jsonUser["fullname"] = newName;    // Update name
                    jsonUser["email"] = newEmail;      // Update email
                    jsonUser["password"] = newPassword;// Update password
                    jsonUser["phoneno"] = newPhone;    // Update phone number

                    // Mark that we found and updated the user
                    userFound = true;
                    break; // Exit loop after updating
                }
            }
            catch (const std::exception& e) {
                // Handle any errors during user data processing
                centerText(RED + "Error processing user data: " + e.what() + RESET);
                continue; // Skip to next user if error occurs
            }
        }

        // Check if we found the user to update
        if (!userFound) {
            // Show warning if user wasn't found
            centerText(YELLOW + "Warning: User not found in JSON file" + RESET);
            return;
        }

        // SECTION 5: SAVE UPDATED DATA
        //=================================
        // Open file for writing (will overwrite existing file)
        std::ofstream outFile(filename);

        // Check if file opened successfully for writing
        if (!outFile.good()) {
            // Show error if can't open file for writing
            centerText(RED + "Error: Could not open file for writing" + RESET);
            return;
        }

        try {
            // Write the updated JSON to file with formatting (indent=2)
            outFile << userData.dump(2);

            // Close the output file
            outFile.close();

            // Show success message in green
            centerText(GREEN + "User data updated successfully!" + RESET);
        }
        catch (const std::exception& e) {
            // Handle any errors during file writing
            centerText(RED + "Error writing to file: " + e.what() + RESET);
            outFile.close(); // Ensure file is closed even if error occurs
        }
    }
    catch (const std::exception& e) {
        // Handle any unexpected errors in the entire process
        centerText(RED + "An unexpected error occurred: " + e.what() + RESET);
    }
}


void loadAdminsFromJson(std::vector<Admin>& admins) { // made function to load admins from json file 
    const std::string filename = "Admin_List.json"; // named the file with string datatype
    std::ifstream file(filename);// used ifstream so that the json made can open 

    if (!file.good()) {// this line is only to check if the file exists and can be opened 
        return;// if it dosent exist the exit this load admin function
    }

    try {
        //Parse JSON file into adminData object
        nlohmann::json adminData = nlohmann::json::parse(file);

        for (const auto& admin : adminData["admins"]) { // loop thorught each and every admin in the json file 
            // for the admin details that has been entered currently on lofin as admin :
            std::string email = admin["email"]; // get email 
            std::string password = admin["password"];// get password 
            bool exists = false;// checking if admins email already existes 
            for (const auto& existingAdmin : admins) {// check in vector
                if (existingAdmin.email == email) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {// if new email then 
                admins.emplace_back(email, password);//create new admin object e,p in constoctor
            }
        }
    }
    catch (const std::exception& e) {// if there is error 
        centerText(RED + "Error loading admins: " + e.what() + RESET);
    }
    file.close();// close json file
}

// Function that saves admin email and password to a JSON file
void saveAdminToJson(const std::string& email, const std::string& password) {
    // Create a new JSON object to store admin data
    nlohmann::json adminData;

    // Set the name of the file where we'll save admin information
    const std::string filename = "Admin_List.json";

    // Try to open the existing admin file for reading
    std::ifstream inFile(filename);

    // Check if the file exists and can be opened
    if (inFile.good()) {
        // If file exists, read its contents into our adminData object
        adminData = nlohmann::json::parse(inFile);
    }
    else {
        // If file doesn't exist, create a new empty array called "admins"
        adminData["admins"] = nlohmann::json::array();
    }

    // Close the input file since we're done reading
    inFile.close();

    // Create a new JSON object for the admin we want to add
    nlohmann::json newAdmin;

    // Add email and password to the new admin object
    newAdmin["email"] = email;
    newAdmin["password"] = password;

    // Add the new admin to our list of admins
    adminData["admins"].push_back(newAdmin);

    // Open the file for writing (this will overwrite the old file)
    std::ofstream outFile(filename);

    // Write the updated admin data to file with nice formatting (indent = 2)
    outFile << adminData.dump(2);

    // Close the output file
    outFile.close();

    // Show a success message in green text
    centerText(GREEN + "Admin added successfully!" + RESET);
}

// Function to load all registered users from a JSON file into our program
void loadUsersFromJson(std::vector<User>& users, int& userIDCounter) {
    // Set the name of our user data file
    const std::string filename = "User_Registration.json";

    // Try to open the file for reading
    std::ifstream file(filename);

    // If file doesn't exist or can't be opened, exit the function
    if (!file.good()) {
        return;
    }

    try {
        // Read the JSON file contents into userData
        nlohmann::json userData = nlohmann::json::parse(file);

        // Remove all existing users from our vector to start fresh
        users.clear();

        // Reset the counter that gives each user a unique ID
        userIDCounter = 1;

        // Loop through each user in the JSON file
        for (const auto& user : userData["users"]) {
            // Get the user's information from JSON
            std::string fullname = user["fullname"];
            std::string email = user["email"];
            std::string password = user["password"];

            // Create a new User object and add it to our vector
            // userIDCounter++ means: use current value, then increase by 1
            users.emplace_back(fullname, email, password, userIDCounter++);
        }
    }
    // If anything goes wrong while reading the file
    catch (const std::exception& e) {
        // Show error message in red text
        centerText(RED + "Error loading users: " + e.what() + RESET);
    }

    // Always close the file when we're done
    file.close();
}

// Function to register a new admin - takes a vector to store all admin data
void handleNewAdminRegistration(std::vector<Admin>& admins) {
    // Clear the console screen for a clean display
    clearConsole();

    // Create variables to store the new admin's email and password
    std::string email, password;

    // Ask for and store the admin's email
    getUserInput("Enter new admin email:", email);

    // Keep asking for email until a valid format is provided
    while (!isValidEmail(email)) {
        // Show error message in red if email format is invalid
        centerText(RED + "Invalid email format." + RESET);
        // Ask for email again
        getUserInput("Enter new admin email:", email);
    }

    // Ask for and store the admin's password
    getUserInput("Enter password (min 8 characters):", password);

    // Keep asking for password until it meets minimum requirements
    while (!isValidPassword(password)) {
        // Show error message in red if password is too short
        centerText(RED + "Password must be at least 8 characters." + RESET);
        // Ask for password again
        getUserInput("Enter password (min 8 characters):", password);
    }

    // Add the new admin to our vector of admins
    // emplace_back creates a new Admin object with email and password
    admins.emplace_back(email, password);

    // Save the new admin's information to our JSON file
    saveAdminToJson(email, password);
}


//// ============= Admin  =============


// Constructor for Admin class - creates a new admin with email and password
Admin::Admin(std::string e, std::string p) : email(e), password(p) {}

// Function to handle the admin menu interface and options
void handleAdminMenu(Admin& admin, std::vector<User>& users, std::vector<Admin>& admins) {
    // Initialize the user's menu choice to 0
    int choice = 0;

    // Create a list of menu options to display
    std::vector<std::string> menuItems = {
        "1. View All Users",          // Show all registered users
        "2. View Online Users",       // Show currently active users
        "3. View Paid Bills",         // Show completed payments
        "4. View Unpaid Bills",       // Show pending payments
        "5. Search/Edit User",        // Modify user information
        "6. Delete User",             // Remove user from system
        "7. System Stats",            // View usage statistics
        "8. Add New Admin",           // Register new admin
        "9. Return to Main Menu"      // Exit admin menu
    };

    // Start menu loop - continues until admin chooses to exit
    do {
        // Show the animated logo
        ShowLogoAnimation();

        // Display the menu options centered on screen
        displayCenteredMenu(menuItems, "\tAdmin Menu");

        // Get admin's choice
        std::string inputStr;
        centerText(YELLOW + "\t\t\tEnter choice (1-9):");
        centerText("");  // Add blank line for spacing
        // Create space for input cursor
        std::cout << std::string((80 - 20) / 2, ' ') << "\t\t> ";
        // Get admin's input
        std::getline(std::cin, inputStr);

        // Clear screen and stop animation for next display
        clearConsole();
        StopLogoAnimation();

        // Validate admin's input
        if (!isInteger(inputStr) || std::stoi(inputStr) < 1 || std::stoi(inputStr) > 9) {
            // Show error if input is invalid
            centerText(RED + "\t\tInvalid choice. Please enter a number between 1 and 9." + RESET);
            // Wait 1.5 seconds before showing menu again
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            continue;  // Restart loop
        }

        // Convert validated input string to integer
        choice = std::stoi(inputStr);

        // Handle admin's choice
        switch (choice) {
        case 1: admin.viewAllUsers(users); break;                    // Show all users
        case 2: admin.viewOnlineUsers(users); break;                 // Show active users
        case 3: admin.viewUsersByPaymentStatus(users, true); break;  // Show paid bills
        case 4: admin.viewUsersByPaymentStatus(users, false); break; // Show unpaid bills
        case 5: admin.searchAndEditUser(users); break;               // Edit user info
        case 6: admin.searchAndDeleteUser(users); break;             // Delete user
        case 7: admin.viewTotalStats(users); break;                  // View statistics
        case 8: handleNewAdminRegistration(admins); break;           // Add new admin
        case 9: centerText(CYAN + "Logging out...\n" + RESET); break;// Exit menu
        }

        // If not exiting, wait for user to press Enter before continuing
        if (choice != 9) {
            centerText("\nPress Enter to continue...");
            std::cin.get();
            clearConsole();
        }
    } while (choice != 9);  // Continue loop until exit choice selected
}

// Function to display a list of all registered users in the system
void Admin::viewAllUsers(const std::vector<User>& users) {
    // Clear the console screen for a fresh display
    clearConsole();

    // Display header for user list with blue color
    centerText(BLUE + "\n---- Complete User List ----" + RESET);

    // Initialize counter for numbering users in display
    int count = 1;

    // Loop through each user in the system
    for (const auto& user : users) {
        // Convert the join date from time_t to readable string
        std::string joinDateStr = std::ctime(&user.joinDate);

        // Display user information, each line centered on screen
        // Show user number in yellow
        centerText(YELLOW + "User #" + std::to_string(count++) + RESET);

        // Display user's unique ID
        centerText("User ID: " + std::to_string(user.userID));

        // Display user's full name
        centerText("Name: " + user.name);

        // Display user's email address
        centerText("Email: " + user.email);

        // Display when user registered
        centerText("Join Date: " + joinDateStr);

        // Display user's current bill amount
        centerText("Total Bill: NZD " + std::to_string(user.totalBill));

        // Show if user is currently using the system
        centerText("Status: " + std::string(user.isLoggedIn ? "Online" : "Offline"));

        // Show total time user has spent in the system
        centerText("Total Time: " + std::to_string(user.getTotalTime()) + " minutes");

        // Add separator line between users
        centerText("-------------------");
    }
}

// Function to display list of currently active users in the system
void Admin::viewOnlineUsers(const std::vector<User>& users) {
    // Clear screen for fresh display
    clearConsole();

    // Display header in green color
    centerText(GREEN + "\n---- Currently Online Users ----" + RESET);

    // Flag to track if we found any online users
    bool foundOnline = false;

    // Loop through all users in the system
    for (const auto& user : users) {
        // Check if user is currently logged in
        if (user.isLoggedIn) {
            // Display user's ID number
            centerText("User ID: " + std::to_string(user.userID));

            // Display user's name
            centerText("Name: " + user.name);

            // Show how long they've been using the system
            centerText("Current Session Time: " + std::to_string(user.getTotalTime()) + " minutes");

            // Display their current unpaid bill
            centerText("Current Bill: NZD " + std::to_string(user.totalBill));

            // Add separator line between users
            centerText("-------------------");

            // Set flag to true since we found an online user
            foundOnline = true;
        }
    }

    // If no online users were found, display message
    if (!foundOnline) {
        centerText("No users currently online.");
    }
}


// admin to search and edit a user
void Admin::searchAndEditUser(std::vector<User>& users) {
    // Clear screen for new operation
    clearConsole();

    // Variables for search input
    std::string searchTerm;
    const size_t MAX_INPUT_LENGTH = 100;  // Maximum characters allowed for input

    // Get and validate search term
    while (true) {
        getUserInput("Enter user ID, name, or email to search:", searchTerm);

        // Check if search term is empty
        if (searchTerm.empty()) {
            centerText(RED + "Search term cannot be empty. Please try again." + RESET);
            continue;
        }

        // Check if search term is too long
        if (searchTerm.length() > MAX_INPUT_LENGTH) {
            centerText(RED + "Search term is too long. Maximum length is " +
                std::to_string(MAX_INPUT_LENGTH) + " characters." + RESET);
            continue;
        }
        break;  // Exit loop if input is valid
    }

    // Try-catch block to handle any errors during search/edit
    try {
        bool userFound = false;
        // Loop through all users to find a match
        for (auto& user : users) {
            // Check if search term matches user ID, name, or email
            if (std::to_string(user.userID) == searchTerm ||
                user.name == searchTerm ||
                user.email == searchTerm) {
                userFound = true;

                // Create menu showing current user data
                std::vector<std::string> menuItems = {
                    "1. Name: " + user.name,
                    "2. Email: " + user.email,
                    "3. Password",
                    "4. Phone Number",
                    "5. Cancel"
                };

                // Display edit options menu
                displayCenteredMenu(menuItems, "Edit User");

                // Store original email for JSON update
                std::string originalEmail = user.email;
                int choice;
                getUserInput("What would you like to edit? (1-5):", choice);

                // Handle different edit options
                switch (choice) {
                case 1: { // Edit Name
                    std::string newName;
                    while (true) {
                        getUserInput("Enter new name:", newName);
                        // Validate new name
                        if (newName.empty()) {
                            centerText(RED + "Name cannot be empty." + RESET);
                            continue;
                        }
                        if (newName.length() > MAX_INPUT_LENGTH) {
                            centerText(RED + "Name is too long. Maximum length is " +
                                std::to_string(MAX_INPUT_LENGTH) + " characters." + RESET);
                            continue;
                        }
                        // Update name in memory and JSON
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
                        // Validate new email
                        if (newEmail.length() > MAX_INPUT_LENGTH) {
                            centerText(RED + "Email is too long" + RESET);
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
                        // Update email in memory and JSON
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
                        newPassword = getHiddenPassword();  // Hide password while typing
                        // Validate new password
                        if (newPassword.length() > MAX_INPUT_LENGTH) {
                            centerText(RED + "Password is too long" + RESET);
                            continue;
                        }
                        if (!isValidPassword(newPassword)) {
                            centerText(RED + "Invalid password format." + RESET);
                            continue;
                        }
                        // Update password in memory and JSON
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
                        // Validate new phone number
                        if (newPhone.length() > MAX_INPUT_LENGTH) {
                            centerText(RED + "Phone number is too long" + RESET);
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
                        // Update phone in memory and JSON
                        user.phoneno = newPhone;
                        updateUserInJson(user, originalEmail);
                        centerText(GREEN + "Phone number updated successfully!" + RESET);
                        break;
                    }
                    break;
                }
                case 5:  // Cancel operation
                    centerText(YELLOW + "Edit cancelled." + RESET);
                    break;
                default:  // Invalid menu choice
                    centerText(RED + "Invalid choice." + RESET);
                }
                break;  // Exit user loop after edit
            }
        }

        // Show message if user not found
        if (!userFound) {
            centerText(RED + "User not found." + RESET);
        }
    }
    // Handle any errors that occur during the process
    catch (const std::exception& e) {
        centerText(RED + "An error occurred: " + e.what() + RESET);
        centerText(YELLOW + "Please try again." + RESET);
    }
}

//search and delete user
void Admin::searchAndDeleteUser(std::vector<User>& users) {
    // Clear screen for clean display
    clearConsole();

    // Get search term from admin
    std::string searchTerm;
    getUserInput("Enter user ID, name, or email to search:", searchTerm);

    // Search for user using find_if with lambda function
    auto it = std::find_if(users.begin(), users.end(),
        [&searchTerm](const User& user) {
            // Check if searchTerm matches any user identifier
            return std::to_string(user.userID) == searchTerm ||
                user.name == searchTerm ||
                user.email == searchTerm;
        });

    // If user is found
    if (it != users.end()) {
        // Display found user info
        centerText("User found: " + it->name + " (ID: " + std::to_string(it->userID) + ")");

        // Get deletion confirmation
        std::string confirmation;
        getUserInput("Are you sure you want to delete? (y/n):", confirmation);

        // If admin confirms deletion (enters 'y' or 'Y')
        if (!confirmation.empty() && (confirmation[0] == 'y' || confirmation[0] == 'Y')) {
            // Store email for finding user in JSON
            std::string emailToDelete = it->email;

            // Remove user from memory vector
            users.erase(it);

            // Start JSON file update process
            const std::string filename = "User_Registration.json";
            try {
                // Open and read JSON file
                std::ifstream inFile(filename);
                if (!inFile.good()) {
                    centerText(RED + "Error: Could not open user file" + RESET);
                    return;
                }

                // Parse JSON data
                nlohmann::json userData = nlohmann::json::parse(inFile);
                inFile.close();

                // Get users array from JSON
                auto& jsonUsers = userData["users"];

                // Find user in JSON array using lambda function
                auto jsonIt = std::find_if(jsonUsers.begin(), jsonUsers.end(),
                    [&emailToDelete](const nlohmann::json& user) {
                        return user["email"] == emailToDelete;
                    });

                // If user found in JSON
                if (jsonIt != jsonUsers.end()) {
                    // Remove user from JSON array
                    jsonUsers.erase(jsonIt);

                    // Write updated JSON back to file
                    std::ofstream outFile(filename);
                    outFile << userData.dump(2);  // dump(2) for pretty formatting
                    outFile.close();

                    // Show success message
                    centerText(GREEN + "User deleted successfully from both memory and file!" + RESET);
                }
                else {
                    // User not found in JSON (shouldn't normally happen)
                    centerText(YELLOW + "Warning: User not found in JSON file" + RESET);
                }
            }
            // Handle any errors during file operations
            catch (const std::exception& e) {
                centerText(RED + "Error updating JSON file: " + e.what() + RESET);
                // Reload users from JSON to ensure data consistency
                loadUsersFromJson(users, userIDCounter);
            }
        }
        else {
            // If admin cancels deletion
            centerText(YELLOW + "Deletion cancelled." + RESET);
        }
    }
    else {
        // If user not found
        centerText(RED + "User not found." + RESET);
    }
}

// admin to get bill data 
std::vector<Receipt> loadReceiptsFromJson() {
    // Create vector to store receipts
    std::vector<Receipt> receipts;

    // Define JSON file name
    const std::string filename = "Receipts.json";

    // Try to open the receipts file
    std::ifstream file(filename);

    // Return empty vector if file doesn't exist
    if (!file.good()) {
        return receipts;
    }

    try {
        // Parse JSON data from file
        nlohmann::json receiptData = nlohmann::json::parse(file);

        // Loop through each receipt in JSON
        for (const auto& receipt : receiptData["receipts"]) {
            // Create new Receipt object
            Receipt r;

            // Load basic receipt information
            r.userID = receipt["userID"];
            r.userName = receipt["userName"];
            r.userEmail = receipt["userEmail"];
            r.totalAmount = receipt["totalAmount"];
            r.receiptDate = receipt["receiptDate"];
            r.paymentMethod = receipt["paymentMethod"];
            r.isPaid = receipt["isPaid"];

            // Load all services associated with this receipt
            for (const auto& service : receipt["services"]) {
                // Create new ServiceUsage object
                ServiceUsage s;

                // Load service details
                s.serviceType = service["type"];      // Type of service (internet, printing, etc)
                s.duration = service["duration"];     // Time-based services duration
                s.quantity = service["quantity"];     // Quantity-based services amount
                s.rate = service["rate"];            // Cost per unit
                s.cost = service["cost"];            // Total cost of service
                s.timestamp = service["timestamp"];   // When service was used

                // Add service to receipt's services list
                r.services.push_back(s);
            }

            // Add complete receipt to vector
            receipts.push_back(r);
        }
    }
    // Handle any errors during loading
    catch (const std::exception& e) {
        centerText(RED + "Error loading receipts: " + e.what() + RESET);
    }

    // Close file and return loaded receipts
    file.close();
    return receipts;
}

// Function to display users based on their payment status (paid/unpaid)
void Admin::viewUsersByPaymentStatus(const std::vector<User>& users, bool paid) {
    // Clear screen for fresh display
    clearConsole();

    // Load all receipts from JSON file
    std::vector<Receipt> receipts = loadReceiptsFromJson();

    // Display header with appropriate status
    centerText(YELLOW + "\n---- Users with " + (paid ? "Paid" : "Unpaid") + " Bills ----" + RESET);

    // Flag to track if any matching receipts are found
    bool found = false;

    // Loop through all receipts
    for (const auto& receipt : receipts) {
        // Check if receipt matches requested payment status
        if (receipt.isPaid == paid) {
            // Display basic user information
            centerText(YELLOW + "User ID: " + std::to_string(receipt.userID) + RESET);
            centerText("Name: " + receipt.userName);
            centerText("Email: " + receipt.userEmail);

            // Display payment information
            centerText("Total Amount: NZD " + std::to_string(receipt.totalAmount));
            centerText("Date: " + std::string(ctime(&receipt.receiptDate)));

            // Show payment method only for paid bills
            if (paid) {
                centerText("Payment Method: " + receipt.paymentMethod);
            }

            // Display services section header
            centerText("Services Used:");

            // Loop through each service in the receipt
            for (const auto& service : receipt.services) {
                // Start building service details string
                std::string serviceDetails = " - " + service.serviceType;

                // Add duration for time-based services
                if (service.duration > 0) {
                    serviceDetails += " (" + std::to_string(service.duration) + " minutes)";
                }
                // Add quantity for page-based services
                else if (service.quantity > 0) {
                    serviceDetails += " (" + std::to_string(service.quantity) + " pages)";
                }

                // Add cost information
                serviceDetails += ": NZD " + std::to_string(service.cost);

                // Display complete service details
                centerText(serviceDetails);
            }

            // Add separator between receipts
            centerText("-------------------");

            // Set flag to indicate matching receipt found
            found = true;
        }
    }

    // If no matching receipts found, display appropriate message
    if (!found) {
        centerText("No " + std::string(paid ? "paid" : "unpaid") + " bills found.");
    }
}

// Function to display comprehensive system statistics
void Admin::viewTotalStats(const std::vector<User>& users) {
    // Clear screen for fresh display
    clearConsole();

    // Load all receipt data
    std::vector<Receipt> receipts = loadReceiptsFromJson();

    // Define structure to track service statistics
    struct ServiceStats {
        int userCount = 0;        // Number of users per service
        int totalDuration = 0;    // Total minutes for time-based services
        int totalQuantity = 0;    // Total units for quantity-based services
        double totalRevenue = 0.0; // Total money earned from service
    };

    // Create map to store stats for each service type
    std::map<std::string, ServiceStats> serviceStats;

    // Initialize general statistics counters
    int totalUsers = users.size();        // Total registered users
    int onlineUsers = 0;                  // Currently active users
    double totalRevenue = 0.0;            // Total system revenue
    int totalTransactions = 0;            // Total completed transactions

    // Count currently online users
    for (const auto& user : users) {
        if (user.isLoggedIn) onlineUsers++;
    }

    // Process each receipt for statistics
    for (const auto& receipt : receipts) {
        if (receipt.isPaid) {
            // Update general totals
            totalRevenue += receipt.totalAmount;
            totalTransactions++;

            // Process each service in receipt
            for (const auto& service : receipt.services) {
                // Get reference to stats for this service type
                ServiceStats& stats = serviceStats[service.serviceType];
                stats.userCount++;
                stats.totalRevenue += service.cost;

                // Update time-based service stats
                if (service.duration > 0) {
                    stats.totalDuration += service.duration;
                }
                // Update quantity-based service stats
                if (service.quantity > 0) {
                    stats.totalQuantity += service.quantity;
                }
            }
        }
    }

    // Display General Statistics
    centerText(CYAN + "\t\t===== SKYLINE CYBER CAFE SYSTEM STATISTICS =====" + RESET);
    centerText(YELLOW + "----------------------------------------");
    centerText(YELLOW + "General Statistics:" + RESET);
    centerText("Total Registered Users: " + std::to_string(totalUsers));
    centerText("Currently Online Users: " + std::to_string(onlineUsers));
    centerText("Total Completed Transactions: " + std::to_string(totalTransactions));
    centerText("Total Revenue: $" + std::to_string(totalRevenue));
    centerText(YELLOW + "----------------------------------------");

    // Display Service-specific Statistics
    cout << "\n";
    centerText(CYAN + "  Service-wise Statistics:" + RESET);

    // Display Internet Browsing Statistics
    if (serviceStats.count("Internet Browsing")) {
        const auto& stats = serviceStats["Internet Browsing"];
        centerText(GREEN + "\nInternet Browsing:" + RESET);
        centerText("Total Users Logged-In: " + std::to_string(stats.userCount));
        centerText("Total Minutes Used: " + std::to_string(stats.totalDuration));
        // Calculate and display average session length
        centerText("Average Session Length: " +
            std::to_string(stats.userCount > 0 ? stats.totalDuration / stats.userCount : 0) + " minutes");
        centerText("Total Revenue: $" + std::to_string(stats.totalRevenue));
    }

    // Display Gaming Statistics
    if (serviceStats.count("Gaming")) {
        const auto& stats = serviceStats["Gaming"];
        centerText(GREEN + "\nGaming:" + RESET);
        centerText("Total Users: " + std::to_string(stats.userCount));
        centerText("Total Gaming Minutes: " + std::to_string(stats.totalDuration));
        // Calculate and display average gaming session
        centerText("Average Gaming Session: " +
            std::to_string(stats.userCount > 0 ? stats.totalDuration / stats.userCount : 0) + " minutes");
        centerText("Total Revenue: $" + std::to_string(stats.totalRevenue));
    }

    // Display Printing Statistics
    if (serviceStats.count("Printing")) {
        const auto& stats = serviceStats["Printing"];
        centerText(GREEN + "\nPrinting:" + RESET);
        centerText("Total Users: " + std::to_string(stats.userCount));
        centerText("Total Pages Printed: " + std::to_string(stats.totalQuantity));
        // Calculate and display average pages per user
        centerText("Average Pages per User: " +
            std::to_string(stats.userCount > 0 ? stats.totalQuantity / stats.userCount : 0));
        centerText("Total Revenue: $" + std::to_string(stats.totalRevenue));
    }

    // Display Scanning Statistics
    if (serviceStats.count("Scanning")) {
        const auto& stats = serviceStats["Scanning"];
        centerText(GREEN + "\nScanning:" + RESET);
        centerText("Total Users: " + std::to_string(stats.userCount));
        centerText("Total Pages Scanned: " + std::to_string(stats.totalQuantity));
        // Calculate and display average pages per user
        centerText("Average Pages per User: " +
            std::to_string(stats.userCount > 0 ? stats.totalQuantity / stats.userCount : 0));
        centerText("Total Revenue: $" + std::to_string(stats.totalRevenue));
    }

    centerText(YELLOW + "----------------------------------------");
}


// ============= Payments   =============

// Function to check if all sessions have been paid for
bool User::hasPaidBill() const {
    // Use std::all_of to check every session
    return std::all_of(sessions.begin(), sessions.end(),
        // Lambda function to check each session's payment status
        // A lambda function is like a mini-function you can write right where you need it
        // Instead of creating a separate function, you can define it on the spot
        [](const Session& s) {
            return s.isPaid;  // Returns true if session is paid
        });
}

// Function to handle payment processing and receipt generation
void handlePayment(User& user, std::vector<ServiceUsage>& currentServices) {
    // Initialize payment status and create receipt
    bool validPayment = false;
    Receipt receipt;

    // Set up receipt basic information
    receipt.userID = user.userID;
    receipt.userName = user.name;
    receipt.userEmail = user.email;
    receipt.receiptDate = time(0);
    receipt.isPaid = false;
    receipt.services = currentServices;
    receipt.totalAmount = user.totalBill;

    // Start payment loop
    do {
        clearConsole();
        ShowLogoAnimation();// main logo

        // Display total bill amount
        centerText(GREEN + "Your total bill is: NZD " + std::to_string(user.totalBill) + RESET);

        // show payment options
        std::vector<std::string> paymentMenuItems = {
            "1. EFTPOS",
            "2. Card Payment",
            "3. Cash",
            "4. Account Transfer",
            "5. Cancel Payment"
        };

        // Show payment menu
        displayCenteredMenu(paymentMenuItems, "\tPayment Methods");// displayy cennter menu is disgin to oraginse menuss in center in  this case the vector itmes 

        // Get user's payment choice
        std::string input;
        centerText(YELLOW + "\t\t\tChoose payment method (1-5):");
        centerText("");
        std::cout << std::string((80 - 20) / 2, ' ') << "\t\t> ";// this line is used ofr constistant cursor position 80 is the total assumed windoe lenght 20 is total input length after subteaction result is /2 for postion 
        std::getline(std::cin, input);

        StopLogoAnimation();// main logo

        // Validate user input
        if (!isInteger(input) || std::stoi(input) < 1 || std::stoi(input) > 5) {// cjeck for 3 possible cases and stoi is used to convert string input to int input 
            centerText(RED + "Invalid choice. Please enter a number between 1 and 5." + RESET);
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            continue;
        }

        int paymentChoice = std::stoi(input);//  int payment input is same as stoi input

        // Handle payment methods 1-4
        if (paymentChoice >= 1 && paymentChoice <= 4) {
            // Get confirmation from user
            clearConsole();
            centerText("Are you sure you want to proceed with this payment method? (y/n):");
            std::string confirmInput;
            std::cout << std::string((80 - 20) / 2, ' ') << "> ";
            std::getline(std::cin, confirmInput);

            if (!confirmInput.empty() && (confirmInput[0] == 'y' || confirmInput[0] == 'Y')) {
                switch (paymentChoice) {
                case 1: { // EFTPOS Payment
                    centerText(BLUE + "Processing payment...");
                    std::this_thread::sleep_for(std::chrono::seconds(3));// Pause for 3 seconds to show error message
                    centerText(GREEN + "Your payment via EFTPOS has been completed successfully." + RESET);
                    receipt.paymentMethod = "EFTPOS";
                    receipt.isPaid = true;
                    user.totalBill = 0.0;
                    validPayment = true;
                    break;
                }
                case 2: { // Card Payment
                    if (handleCardPayment()) {  // Call card payment handler
                        std::this_thread::sleep_for(std::chrono::seconds(5));// Pause for 5 seconds to show error message
                        centerText(GREEN + "Payment successful!" + RESET);
                        receipt.paymentMethod = "Card";
                        receipt.isPaid = true;
                        user.totalBill = 0.0;
                        validPayment = true;
                    }
                    break;
                }
                case 3: { // Cash Payment
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
                    std::this_thread::sleep_for(std::chrono::seconds(5));// Pause for 5 seconds to show error message
                    centerText(GREEN + "Payment received via account transfer." + RESET);
                    receipt.paymentMethod = "Account Transfer";
                    receipt.isPaid = true;
                    user.totalBill = 0.0;
                    validPayment = true;
                    break;
                }
                }

                // Handle successful payment
                if (validPayment) {
                    saveReceiptToJson(receipt);      // Save receipt to file
                    displayReceipt(receipt);         // Show receipt to user
                    handleReceiptEmail(receipt);     // Handle email receipt option
                }
            }
            else {
                // Handle payment cancellation
                centerText(YELLOW + "Payment cancelled. Returning to payment menu..." + RESET);
                std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            }
        }
        else if (paymentChoice == 5) {
            // Handle payment cancellation
            centerText(YELLOW + "Payment cancelled." + RESET);
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            return;
        }

        // Exit if payment was successful
        if (validPayment) {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            return;
        }

    } while (!validPayment);  // Continue until valid payment is made
}

// Helper function for card payment processing
// Function to handle credit card payment process
bool handleCardPayment() {
    // Declare variables for card details
    std::string cardNumber, cardHolder, expiryDate, cvv;
    bool validInput;

    // CARD NUMBER VALIDATION
    do {
        // Get card number from user
        getUserInput("Enter Card Number (16 digits):", cardNumber);

        // Check if number is 16 digits
        if (cardNumber.length() == 16 && std::all_of(cardNumber.begin(), cardNumber.end(), ::isdigit)) {
            // Format card number with dashes for readability: XX-XXXX-XXXXXXX-XXX
            std::string formattedCard = cardNumber.substr(0, 2) + "-" +
                cardNumber.substr(2, 4) + "-" +
                cardNumber.substr(6, 7) + "-" +
                cardNumber.substr(13, 3);

            // Ask user to confirm formatted number
            centerText(YELLOW + "Confirm card number: " + formattedCard + " (y/n):" + RESET);
            char confirm;
            std::cin >> confirm;
            std::cin.ignore();  // Clear input buffer
            validInput = (confirm == 'y' || confirm == 'Y');
        }
        else {
            // Show error if format is invalid
            centerText(RED + "Invalid card number format." + RESET);
            validInput = false;
        }
    } while (!validInput);  // Repeat until valid input received

    // CARDHOLDER NAME VALIDATION
    do {
        // Get cardholder name
        getUserInput("Enter Cardholder Name:", cardHolder);

        // Confirm name with user
        centerText(YELLOW + "Confirm name: " + cardHolder + " (y/n):" + RESET);
        char confirm;
        std::cin >> confirm;
        std::cin.ignore();
        validInput = (confirm == 'y' || confirm == 'Y');
    } while (!validInput);

    // EXPIRY DATE VALIDATION
    do {
        // Get expiry date in MM/YY format
        getUserInput("Enter Expiry Date (MM/YY):", expiryDate);

        // Check format: 5 chars, middle is '/', all others are digits
        if (expiryDate.length() == 5 && expiryDate[2] == '/' &&
            std::isdigit(expiryDate[0]) && std::isdigit(expiryDate[1]) &&
            std::isdigit(expiryDate[3]) && std::isdigit(expiryDate[4])) {

            // Extract and validate month (1-12)
            int month = std::stoi(expiryDate.substr(0, 2));
            if (month >= 1 && month <= 12) {
                // Confirm date with user
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

    // CVV VALIDATION
    do {
        // Get 3-digit CVV
        getUserInput("Enter CVV (3 digits):", cvv);

        // Check if CVV is 3 digits and not '000'
        if (cvv.length() == 3 && cvv != "000" && std::all_of(cvv.begin(), cvv.end(), ::isdigit)) {
            // Confirm CVV with user
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

    // Process payment (simulation)
    centerText("Processing payment...");
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Return success
    return true;
}


// ============= User input and menu   =============
// Constructor for User class - initializes a new user with provided information
User::User(std::string n, std::string e, std::string p, int id)
// Initialize all member variables using initializer list
    : name(n),          // Set user's name
    email(e),         // Set user's email
    password(p),      // Set user's password
    userID(id),       // Set unique user ID
    phoneno(""),      // Initialize phone number as empty
    totalBill(0.0),   // Initialize bill to zero
    isLoggedIn(false),// Set initial login status as not logged in
    joinDate(time(0)) // Set join date to current time
{}

/* This constructor:
1. Takes 4 parameters:
   - n: user's name
   - e: user's email
   - p: user's password
   - id: unique identifier

2. Uses initializer list (faster than assigning in constructor body)

3. Sets default values:
   - Empty phone number
   - Zero bill
   - Not logged in
   - Current time as join date

4. time(0) gets current system time for join date
*/

// Function to handle all user interactions and service selections
void handleUserMenu(User& user) {
    // Initialize control variables
    bool logoutPermitted = false;  // Controls if user can exit
    std::vector<ServiceUsage> currentServices;  // Tracks services used in this session

    // Set up receipt for current session
    Receipt receipt;
    receipt.userID = user.userID;
    receipt.userName = user.name;
    receipt.userEmail = user.email;
    receipt.receiptDate = time(0);  // Current timestamp
    receipt.isPaid = false;
    receipt.paymentMethod = "";

    // Define service rates (constants)
    int userChoice = 0;
    const double browsingRate = 0.50;   // Per minute
    const double gamingRate = 1.20;     // Per minute
    const double printCost = 0.70;      // Per page
    const double scanCost = 0.52;       // Per page
    bool hasUsedService = false;        // Tracks if any service was used

    // Main menu loop
    do {
        clearConsole();
        ShowLogoAnimation();

        // Define available services
        std::vector<std::string> menuItems = {
            "1. Internet Browsing ($0.50/min)",
            "2. Gaming ($1.20/min)",
            "3. Printing ($0.70/page)",
            "4. Scanning ($0.52/page)"
        };

        // Add payment/exit options based on usage
        if (hasUsedService || user.totalBill > 0) {
            menuItems.push_back("5. View/Pay Total Bill");
            menuItems.push_back("6. Back to Main Menu");
        }
        else {
            menuItems.push_back("5. Back to Main Menu");
        }

        // Display menu and get user choice
        displayCenteredMenu(menuItems, "\tUser Menu");

        // Handle user input
        std::string input;
        const int maxOption = (hasUsedService || user.totalBill > 0) ? 6 : 5;
        centerText(YELLOW + "\t\t\tChoose an option (1-" + std::to_string(maxOption) + "):");
        centerText("");
        std::cout << std::string((80 - 20) / 2, ' ') << "\t\t> ";
        std::getline(std::cin, input);

        StopLogoAnimation();
        clearConsole();

        // Validate user input
        if (!isInteger(input) || std::stoi(input) < 1 || std::stoi(input) > maxOption) {
            centerText(RED + "Invalid choice. Please enter a number between 1 and " +
                std::to_string(maxOption) + "." + RESET);
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            continue;
        }

        // Create service usage object for selected service
        userChoice = std::stoi(input);
        ServiceUsage usage;
        usage.timestamp = time(0);

        // Handle different service choices
        switch (userChoice) {
        case 1: { // Internet Browsing Service
            usage.serviceType = "Internet Browsing";
            usage.rate = browsingRate;
            usage.quantity = 0;  // Not used for time-based service

            // Show rates and special offers
            centerText(GREEN + "The rate is $0.50 per minute." + RESET);
            centerText("Special Offer: 15% off for 120 minutes or more!");

            // Get and validate duration
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

            // Calculate cost with possible discount
            usage.cost = usage.duration * browsingRate;
            if (usage.duration >= 120) {
                usage.cost *= 0.85;  // Apply 15% discount
                centerText(GREEN + "Discount applied! 15% off for using 120 minutes or more!" + RESET);
            }

            // Record service usage
            user.startSession();
            user.endSession(usage.cost);
            currentServices.push_back(usage);
            hasUsedService = true;
            break;
        }

        case 2: { // Gaming Service
            // Similar structure to Internet Browsing but with different rates
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
                usage.cost *= 0.8;  // Apply 20% discount
                centerText(GREEN + "Discount applied! 20% off for playing 120 minutes or more!" + RESET);
            }

            user.startSession();
            user.endSession(usage.cost);
            currentServices.push_back(usage);
            hasUsedService = true;
            break;
        }

        case 3: { // Printing Service
            usage.serviceType = "Printing";
            usage.rate = printCost;
            usage.duration = 0;  // Not time-based

            centerText(GREEN + "The cost for printing is $0.70 per page." + RESET);

            // Get and validate number of pages
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

        case 4: { // Scanning Service
            // Similar to printing but different rate
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

        case 5: { // Bill Payment or Exit
            if (hasUsedService || user.totalBill > 0) {
                // Show bill and handle payment
                clearConsole();
                displayInvoice(currentServices, user.totalBill, user);
                centerText("\nPress Enter to proceed to payment options...");
                std::cin.get();
                handlePayment(user, currentServices);
                if (user.totalBill == 0) {
                    return;  // Exit after successful payment
                }
            }
            else {
                // Exit if no services used
                centerText(CYAN + "Returning to main menu..." + RESET);
                return;
            }
            break;
        }

        case 6: { // Alternative Exit Option
            if (hasUsedService || user.totalBill > 0) {
                // Prevent logout if bill unpaid
                if (!logoutPermitted && user.totalBill > 0) {
                    centerText(RED + "Please pay your bill before logging out." + RESET);
                    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                    userChoice = 0;  // Reset choice to prevent exit
                }
                else {
                    centerText(CYAN + "Returning to main menu..." + RESET);
                    return;
                }
            }
            break;
        }
        }

        // Wait for user acknowledgment except for payment/exit options
        if (userChoice != 5 && userChoice != 6) {
            centerText("\nPress Enter to continue...");
            std::cin.get();
        }

    } while (true);  // Continue until valid exit condition met
}


// ============= Receipts   =============

// Function to save a receipt to the JSON file system
void saveReceiptToJson(const Receipt& receipt) {
    // Create main JSON object that will hold all receipt data
    nlohmann::json receiptData;  // Empty JSON container

    // Name of file where receipts are stored
    const std::string filename = "Receipts.json";

    /* STEP 1: READ EXISTING FILE */
    // Open file for reading
    std::ifstream inFile(filename);

    // Check if file exists and can be opened
    if (inFile.good()) {
        // Parse existing JSON data into receiptData
        receiptData = nlohmann::json::parse(inFile);
    }
    else {
        // Create new empty array if file doesn't exist
        receiptData["receipts"] = nlohmann::json::array();
    }
    // Close the input file stream
    inFile.close();

    /* STEP 2: CREATE NEW RECEIPT OBJECT */
    // Create JSON object for this specific receipt
    nlohmann::json newReceipt;

    // Store basic user information
    newReceipt["userID"] = receipt.userID;           // Unique user identifier
    newReceipt["userName"] = receipt.userName;        // User's full name
    newReceipt["userEmail"] = receipt.userEmail;      // User's email address

    // Store transaction details
    newReceipt["totalAmount"] = receipt.totalAmount;  // Total bill amount
    newReceipt["receiptDate"] = receipt.receiptDate;  // When transaction occurred
    newReceipt["paymentMethod"] = receipt.paymentMethod; // How user paid
    newReceipt["isPaid"] = receipt.isPaid;           // Payment status

    /* STEP 3: ADD SERVICES TO RECEIPT */
    // Create array to store services used
    newReceipt["services"] = nlohmann::json::array();

    // Loop through each service in the receipt
    for (const auto& service : receipt.services) {
        // Create JSON object for current service
        nlohmann::json serviceObj;

        // Add all service details
        serviceObj["type"] = service.serviceType;     // What service was used
        serviceObj["duration"] = service.duration;    // Minutes used (for internet/gaming)
        serviceObj["quantity"] = service.quantity;    // Pages used (for printing/scanning)
        serviceObj["rate"] = service.rate;           // Price per unit
        serviceObj["cost"] = service.cost;           // Total cost for this service
        serviceObj["timestamp"] = service.timestamp;  // When service was used

        // Add this service to receipt's services array
        newReceipt["services"].push_back(serviceObj);
    }

    /* STEP 4: UPDATE MAIN RECEIPT DATA */
    // Add the complete new receipt to main receipts array
    receiptData["receipts"].push_back(newReceipt);

    /* STEP 5: SAVE TO FILE */
    // Open file for writing (overwrites existing file)
    std::ofstream outFile(filename);

    // Write formatted JSON to file (indent=2 for readability)
    outFile << receiptData.dump(2);

    // Close the output file
    outFile.close();
}

// Function to display a formatted receipt to the user
void displayReceipt(const Receipt& receipt) {
    // Clear screen for clean display
    clearConsole();

    // Display header section
    centerText(BLUE + "=== SKYLINE CYBER CAFE RECEIPT ===" + RESET);

    // Display receipt timestamp
    // ctime converts time_t to readable string
    centerText("Receipt Date: " + std::string(ctime(&receipt.receiptDate)));

    // Display customer information
    centerText("Customer: " + receipt.userName);          // Customer name
    centerText("ID: " + std::to_string(receipt.userID)); // Customer ID
    centerText("Email: " + receipt.userEmail);           // Customer email

    // Separator line
    centerText("----------------------------------------");

    // Services section header
    centerText("Services Used:");

    // Loop through each service used
    for (const auto& service : receipt.services) {
        // Start building service description
        std::string serviceDetails = service.serviceType;

        // Add duration for time-based services (internet/gaming)
        if (service.duration > 0) {
            serviceDetails += " (" + std::to_string(service.duration) + " minutes)";
        }
        // Add quantity for unit-based services (printing/scanning)
        else if (service.quantity > 0) {
            serviceDetails += " (" + std::to_string(service.quantity) + " pages)";
        }

        // Create full cost line with rate and total
        std::string costLine = serviceDetails +
            " @ $" + std::to_string(service.rate) +   // Rate per unit
            " = $" + std::to_string(service.cost);    // Total cost

        // Display service line
        centerText(costLine);
    }

    // Bottom section separator
    centerText("----------------------------------------");

    // Display total amount
    centerText("Total Amount: $" + std::to_string(receipt.totalAmount));

    // Show payment status (PAID/UNPAID)
    centerText("Payment Status: " + std::string(receipt.isPaid ? "PAID" : "UNPAID"));

    // Show payment method if paid
    if (receipt.isPaid) {
        centerText("Payment Method: " + receipt.paymentMethod);
    }

    // Bottom border
    centerText("========================================");
}

// Function to display a formatted invoice before payment
void displayInvoice(const std::vector<ServiceUsage>& services, double totalAmount, const User& user) {
    // Clear screen for fresh display
    clearConsole();

    // Display header with cafe name in yellow
    centerText(YELLOW + "=== SKYLINE CYBER CAFE INVOICE ===" + RESET);

    // Display customer information
    centerText("Customer: " + user.name);                           // Customer name
    centerText(GREEN + "ID: " + std::to_string(user.userID) + RESET); // Customer ID in green
    centerText(YELLOW + "Email: " + user.email + RESET);             // Email in yellow

    // Separator line
    centerText("----------------------------------------");

    // Services section header
    centerText("Services Used:");

    // Initialize subtotal for calculations
    double subtotal = 0.0;

    // Loop through each service used
    for (const auto& service : services) {
        // Start building service description
        std::string serviceDetails = service.serviceType;

        // Handle time-based services (internet/gaming)
        if (service.duration > 0) {
            serviceDetails += " (" + std::to_string(service.duration) + " minutes @ $" +
                std::to_string(service.rate) + "/min)";
        }
        // Handle quantity-based services (printing/scanning)
        else if (service.quantity > 0) {
            serviceDetails += " (" + std::to_string(service.quantity) + " pages @ $" +
                std::to_string(service.rate) + "/page)";
        }

        // Add service cost to subtotal
        subtotal += service.cost;

        // Create and display cost line showing service details and cost
        std::string costLine = serviceDetails + " = $" + std::to_string(service.cost);
        centerText(costLine);
    }

    // Bottom section with totals
    centerText("----------------------------------------");
    centerText("Subtotal: $" + std::to_string(subtotal));

    // Display total amount due in red for emphasis
    centerText(RED + "Total Amount Due: $" + std::to_string(totalAmount) + RESET);

    // Bottom border in yellow
    centerText(YELLOW + "=====================================" + RESET);
}

// Function to handle emailing receipt to customer
void handleReceiptEmail(const Receipt& receipt) {
    // Variable to store user's email choice
    char emailChoice;

    // Ask user if they want receipt emailed
    centerText("\nWould you like a copy of the receipt emailed to " + receipt.userEmail + "? (y/n):");

    // Get user input
    std::cin >> emailChoice;
    std::cin.ignore();  // Clear input buffer

    // If user wants email (y or Y)
    if (emailChoice == 'y' || emailChoice == 'Y') {
        // Show sending status in blue
        centerText(BLUE + "Sending receipt to " + receipt.userEmail + "..." + RESET);

        // Simulate email sending with 2-second delay
        std::this_thread::sleep_for(std::chrono::seconds(2));

        // Show success message in green
        centerText(GREEN + "Receipt has been sent to your email!" + RESET);
    }

    // Display farewell messages
    centerText(GREEN + "\nThank you for choosing Skyline Cyber Cafe!" + RESET);
    centerText(CYAN + "We hope to see you again soon!" + RESET);

    // Pause for 2 seconds before continuing
    std::this_thread::sleep_for(std::chrono::seconds(2));
}
