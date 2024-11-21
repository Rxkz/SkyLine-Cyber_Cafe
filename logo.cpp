//logo.cpp
#include "logo.h"
using namespace std;

#define RED 12
#define BLUE 9
#define WHITE 15

// Current animation controller
static unique_ptr<AsciiAnimator> currentAnimator;    // Manages ASCII animations, auto-deletes when done

// Thread for running animations
static unique_ptr<thread> animationThread;           // Handles animation in separate thread, auto-cleanup

// Height of logo in characters
static const int logoHeight = 6;                     // Defines logo size for display calculations

// Thread synchronization for console access
std::mutex AsciiAnimator::consoleMutex;   // Prevents multiple threads from writing to console simultaneously

/*
Key features:
- Static mutex shared by all AsciiAnimator instances
- Ensures thread-safe console output
- Prevents text garbling from concurrent access
- Used with lock_guard for safe locking/unlocking
*/

// Function to change console text color
void setColor(int color) {
    // Get handle to console window
    GetStdHandle(STD_OUTPUT_HANDLE);      // Access console output

    // Set the text color attribute
    SetConsoleTextAttribute(              // Changes text properties
        GetStdHandle(STD_OUTPUT_HANDLE),  // Console handle
        color                             // Color value to set
    );
}


// Moves console cursor to specified coordinates
void setCursorPosition(int x, int y) {
    COORD coord;                                     // Structure to hold coordinates
    coord.X = x;                                     // Set X (horizontal) position
    coord.Y = y;                                     // Set Y (vertical) position
    SetConsoleCursorPosition(                        // Windows API call
        GetStdHandle(STD_OUTPUT_HANDLE),            // Get console handle
        coord                                       // Pass coordinates
    );
}

// Member function of AsciiAnimator class to clear console screen
void AsciiAnimator::clearScreen() {
    system("cls");    // Windows command to clear entire console screen
}

// Constructor for AsciiAnimator class - initializes animation system
AsciiAnimator::AsciiAnimator() : gen(rd()), running(true) {
    // Define ASCII art logo
    art = {
        "\t ____  *  *___   ___     ___ *   * _____    ______   ______  _____ ____     ____    *    *____ _____ ",
        "\t / ___|| |/ /\\ \\ / / |   |_ *| \\ | | *___|  / ___\\ \\ / / ** )| **__|  * \\   / ***|  / \\  |  **_| ____|",
        "\t \\___ \\| ' /  \\ V /| |    | ||  \\| |  *|   | |    \\ V /|  * \\|  *| | |*) | | |     / * \\ | |*  |  _|  ",
        "\t  ___) | . \\   | | | |___ | || |\\  | |___  | |___  | | | |_) | |___|  * <  | |*__ / ___ \\|  *| | |*__ ",
        "\t |____/|_|\\_\\  |_| |_____|___|_| \\_|_____|  \\____| |_| |____/|_____|_| \\_\\  \\____/_/   \\_\\_|   |_____|"
    };

    // Find all non-space positions for star placement
    for (int y = 0; y < art.size(); ++y) {
        for (size_t x = 0; x < art[y].length(); ++x) {
            if (art[y][x] != ' ') {
                validPositions.push_back({ static_cast<int>(x), y });  // Store valid positions
            }
        }
    }

    // Create initial stars at random valid positions
    int numStars = 10;                                     // Number of stars to create
    for (int i = 0; i < numStars; ++i) {
        // Generate random index for position
        int posIndex = uniform_int_distribution<>(0, validPositions.size() - 1)(gen);

        // Add star with position and active state
        stars.push_back({
            validPositions[posIndex].first,              // X position
            validPositions[posIndex].second,             // Y position
            true                                         // Active state
            });
    }
}

void AsciiAnimator::clearLogoArea() {
    lock_guard<mutex> lock(consoleMutex);    // Lock console for thread safety

    string emptyLine(120, ' ');              // Create line of 120 spaces

    // Clear each line of logo area
    for (int i = 0; i < logoHeight; ++i) {
        setCursorPosition(0, i);             // Move to start of each line
        cout << emptyLine;                   // Print spaces to clear line
    }
}

void AsciiAnimator::drawFrame() {
    lock_guard<mutex> lock(consoleMutex);

    // Save current cursor position
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    COORD savedPosition = csbi.dwCursorPosition;

    // Draw the base art
    for (size_t i = 0; i < art.size(); ++i) {
        setCursorPosition(0, i);
        setColor(BLUE);
        cout << art[i];
    }

    // Draw the stars
    setColor(RED);
    for (const auto& star : stars) {
        if (star.visible) {
            setCursorPosition(star.x, star.y);
            cout << "*";
        }
    }

    // Restore cursor position
    SetConsoleCursorPosition(hConsole, savedPosition);
    setColor(WHITE);
}

void AsciiAnimator::updateStars() {
    for (auto& star : stars) {
        if (uniform_int_distribution<>(0, 4)(gen) == 0) {
            star.visible = !star.visible;
        }
    }
}

void AsciiAnimator::initialDraw() {
    clearScreen();
    for (size_t i = 0; i < art.size(); ++i) {
        setColor(BLUE);
        cout << art[i] << endl;
    }
}

void AsciiAnimator::startAnimation() {
    while (running) {
        drawFrame();
        updateStars();
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

void ShowLogoAnimation() {
    // Configure console
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info{ 100, TRUE };
    SetConsoleCursorInfo(consoleHandle, &info);
    system("mode con: cols=120 lines=40");

    // Create new animator and start animation
    currentAnimator = make_unique<AsciiAnimator>();
    currentAnimator->clearLogoArea();

    animationThread = make_unique<thread>([&]() {
        currentAnimator->startAnimation();
        });

    // Position cursor below logo area for menu
    setCursorPosition(0, logoHeight);
}

void StopLogoAnimation() {
    if (currentAnimator) {
        currentAnimator->stopAnimation();
    }
    if (animationThread && animationThread->joinable()) {
        animationThread->join();
    }
    currentAnimator.reset();
    animationThread.reset();
}