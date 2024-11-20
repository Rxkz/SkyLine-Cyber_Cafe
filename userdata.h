#ifndef USERDATA_H
#define USERDATA_H

#include "display.h"
#include "validation.h"
#include <string>
#include <vector>
#include <iostream>
#include <ctime>
#include "nlohmann/json.hpp"

// Forward declarations
class User;
class Admin;
struct Receipt;
struct ServiceUsage;
struct Session;
struct UserRegistration;


// Function declaration for receipt loading
std::vector<Receipt> loadReceiptsFromJson();

// Session structure
struct Session {
    time_t startTime;
    time_t endTime;
    double cost;
    bool isPaid;

    Session() : startTime(0), endTime(0), cost(0.0), isPaid(false) {}
};

// User Registration structure
struct UserRegistration {
    std::string username;
    std::string password;
    std::string fullname;
    std::string email;
    std::string phoneno;
    time_t joinDate;
};

// Service Usage structure
struct ServiceUsage {
    std::string serviceType;
    int duration;          // For time-based services
    int quantity;         // For page-based services
    double rate;
    double cost;
    time_t timestamp;
};

// Receipt structure
struct Receipt {
    int userID;
    std::string userName;
    std::string userEmail;
    std::vector<ServiceUsage> services;
    double totalAmount;
    time_t receiptDate;
    std::string paymentMethod;
    bool isPaid;
};

// User class
class User {
public:
    std::string name;
    std::string email;
    std::string password;
    int userID;
    std::string phoneno;
    double totalBill;
    bool isLoggedIn;
    time_t joinDate;
    std::vector<Session> sessions;

    User(std::string n, std::string e, std::string p, int id);
    void startSession();
    void endSession(double sessionBill);
    int getTotalTime() const;
    bool hasPaidBill() const;
};

// Admin class
class Admin {
public:
    std::string email;
    std::string password;

    Admin(std::string e, std::string p);
    void viewAllUsers(const std::vector<User>& users);
    void viewOnlineUsers(const std::vector<User>& users);
    void viewUsersByPaymentStatus(const std::vector<User>& users, bool paid);
    void searchAndEditUser(std::vector<User>& users);
    void searchAndDeleteUser(std::vector<User>& users);
    void viewTotalStats(const std::vector<User>& users);
};

// Receipt and invoice functions
void saveReceiptToJson(const Receipt& receipt);
void displayReceipt(const Receipt& receipt);
void displayInvoice(const std::vector<ServiceUsage>& services, double totalAmount, const User& user);
void handleReceiptEmail(const Receipt& receipt);

// JSON operations
void updateUserInJson(const User& user, const std::string& originalEmail);
void saveUserToJson(const UserRegistration& user);
void loadAdminsFromJson(std::vector<Admin>& admins);
void saveAdminToJson(const std::string& email, const std::string& password);
void loadUsersFromJson(std::vector<User>& users, int& userIDCounter);

// Menu handling functions
void handleUserMenu(User& user);
void handleAdminMenu(Admin& admin, std::vector<User>& users, std::vector<Admin>& admins);
void handleNewAdminRegistration(std::vector<Admin>& admins);
void handleRegistration(std::vector<User>& users, int& userIDCounter);
void handlePayment(User& user);
bool handleCardPayment();

#endif // USERDATA_H