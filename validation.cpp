#include "validation.h"
#include "display.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <algorithm>

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

bool isValidEmail(const std::string& email) {
    return isValidDetailedEmail(email);
}

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

    size_t atPos = email.find('@');
    size_t dotPos = email.find('.');

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

bool isValidPassword(const std::string& password) {
    return isValidDetailedPassword(password);
}

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