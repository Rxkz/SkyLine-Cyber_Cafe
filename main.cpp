#include "userdata.h"
#include "display.h"
#include "logo.h"
#include "validation.h"
#include <iostream>
#include <windows.h>

using namespace std;

int main() {
    vector<User> users;
    vector<Admin> admins = { Admin("123", "456") };
    int userIDCounter = 1;
    loadUsersFromJson(users, userIDCounter);
    loadAdminsFromJson(admins);


    while (true) {
        system("cls");
        ShowLogoAnimation();  // Show the animated logo

        // Add spacing after logo
        cout << "\n\n\n\n\n\n\n";

        // Menu items
        vector<string> mainMenuItems = {
            "1. Register",
            "2. Login as User",
            "3. Login as Admin",
            "4. Exit"
        };

        displayCenteredMenu(mainMenuItems, "Main Menu");

        // Get user input with validation
        int choice = 0;
        string input;
        centerText("Choose an option (1-4):");
        centerText("");  // Empty line for spacing
        cout << string((80 - 20) / 2, ' ') << "> ";
        getline(cin, input);

        // Validate input
        if (!isInteger(input) || stoi(input) < 1 || stoi(input) > 4) {
            StopLogoAnimation();
            centerText(RED + "Please enter a valid number." + RESET);
            this_thread::sleep_for(chrono::milliseconds(1500));
            continue;  // This will restart the loop and show everything again
        }

        choice = stoi(input);
        StopLogoAnimation();
        system("cls");

        switch (choice) {
        case 1:
            handleRegistration(users, userIDCounter);
            cout << "\nPress Enter to continue...";
            cin.get();
            break;
        case 2: {
            displayLogo(1);
            string email, password;
            getUserInput(YELLOW + "Enter Email:" + RESET, email);
            centerText(YELLOW + "Enter Password:" + RESET);
            password = getHiddenPassword();
            bool userFound = false;
            for (auto& user : users) {
                if (user.email == email && user.password == password) {
                    userFound = true;
                    centerText(CYAN + "Welcome " + user.name + "!" + RESET);
                    user.startSession();
                    handleUserMenu(user);
                    break;
                }
            }
            if (!userFound) {
                centerText(RED + "Invalid email or password." + RESET);
            }
            cout << "\nPress Enter to continue...";
            cin.get();
            break;
        }
        case 3: {
            displayLogo(3);
            string email, password;
            getUserInput("Enter Admin Email:", email);
            centerText("Enter Password:");
            password = getHiddenPassword();
            bool adminFound = false;
            for (auto& admin : admins) {
                if (admin.email == email && admin.password == password) {
                    adminFound = true;
                    centerText(CYAN + "Welcome Admin!" + RESET);
                    handleAdminMenu(admin, users, admins);
                    break;
                }
            }
            if (!adminFound) {
                centerText(RED + "Invalid admin credentials." + RESET);
            }
            cout << "\nPress Enter to continue...";
            cin.get();
            break;
        }
        case 4:
            centerText(RED + "Exiting..." + RESET);
            cout << "\nPress Enter to exit...";
            cin.get();
            return 0;
        }
    }
}