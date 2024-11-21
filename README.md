# Skyline Cyber Cafe Billing System

A comprehensive billing and management system for Skyline Cyber Cafe, designed to handle user sessions, service tracking, and administrative functions.

![image](https://github.com/user-attachments/assets/dafb5464-d2ba-4c26-a9c4-3e4873970d53)

##Flow Chart
![flow](https://github.com/user-attachments/assets/19ba4fd8-5b01-4a28-aacc-4d64a515d123)



## Overview

Skyline Cyber Cafe Billing System is a C++ application developed for Silver Star Enterprises Ltd.'s cyber cafe in Auckland CBD. The system manages user registrations, tracks service usage, calculates bills, and provides administrative controls for managing the cyber cafe operations.

## Features

### User Management
- Secure user registration with email verification
- Password validation following best practices
- Unique user ID generation
- Session tracking
- User authentication

### Service Tracking
- Internet browsing usage tracking
- Gaming session management
- Printing service billing
- Scanning service billing
- Real-time session monitoring

### Billing System
- Automated bill calculation
- Multi-service billing
- Session-based charging
- Receipt generation
- Payment processing with multiple payment methods
  - EFTPOS
  - Card Payment
  - Cash
  - Account Transfer

### Admin Features
- Secure admin authentication
- User management (View/Edit/Delete)
- Online user monitoring
- Bill payment status tracking
- System statistics and reports
- Add new admin users

## Technical Details

### Technologies Used
- C++
- JSON for data storage
- Object-Oriented Programming principles
- File I/O operations

### System Requirements
- C++ compiler with C++11 support or higher
- JSON library (nlohmann/json)
- Windows/Linux operating system

### Data Structures
- User class for customer management
- Admin class for administrative functions
- Session structure for tracking usage
- ServiceUsage structure for service details
- Receipt structure for billing information

## Installation

1. Clone the repository
```bash

```

2. Navigate to the project directory
```bash

```

3. Compile the project
```bash

```

## Usage

### Running the Application
```bash

```

### User Functions
1. Register new account
2. Login with credentials
3. Use services:
   - Internet browsing
   - Gaming
   - Printing
   - Scanning
4. View and pay bills

### Admin Functions
1. Login with admin credentials
2. View all users
3. Monitor online users
4. Check payment statuses
5. Edit user information
6. Delete users
7. View system statistics

## Security Features

- Password encryption
- Email validation
- Phone number validation
- Session tracking
- Secure payment processing
- Input validation and sanitization

## File Structure

```
skyline-cyber-cafe/
├── main.cpp
├── userdata.cpp
├── userdata.h
├── User_Registration.json
├── Admin_List.json
└── Receipts.json
```

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Authors

= PRADA
=FOX
SANMAIN

## Acknowledgments

- Silver Star Enterprises Ltd. for the project requirements
- Auckland CBD business community for support
- Contributors and testers

## Support

For support, please email support@skylinecybercafe.com or raise an issue in the GitHub repository.
