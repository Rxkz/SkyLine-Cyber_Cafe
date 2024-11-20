#ifndef VALIDATION_H
#define VALIDATION_H

#include <string>
#include <vector>
#include <regex>

// Validation function declarations
bool isValidUserName(const std::string& username);
bool isValidEmail(const std::string& email);
bool isValidDetailedEmail(const std::string& email);
bool isValidPassword(const std::string& password);
bool isValidDetailedPassword(const std::string& password);
bool isValidPhoneNumber(const std::string& phone);

// Duplicate checking functions
bool isDuplicateEmail(const std::string& email);
bool isDuplicatePhone(const std::string& phone);


#endif // VALIDATION_H#pragma once