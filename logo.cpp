//logo.cpp
#include "logo.h"
using namespace std;

#define RED 12
#define BLUE 9
#define WHITE 15

static unique_ptr<AsciiAnimator> currentAnimator;
static unique_ptr<thread> animationThread;
static const int logoHeight = 6;  // Height of the logo plus some padding

std::mutex AsciiAnimator::consoleMutex;

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void AsciiAnimator::clearScreen() {
    system("cls");
}

AsciiAnimator::AsciiAnimator() : gen(rd()), running(true) {
    art = {
        "\t ____  *  *___   ___     ___ *   * _____    ______   ______  _____ ____     ____    *    *____ _____ ",
        "\t / ___|| |/ /\\ \\ / / |   |_ *| \\ | | *___|  / ___\\ \\ / / ** )| **__|  * \\   / ***|  / \\  |  **_| ____|",
        "\t \\___ \\| ' /  \\ V /| |    | ||  \\| |  *|   | |    \\ V /|  * \\|  *| | |*) | | |     / * \\ | |*  |  _|  ",
        "\t  ___) | . \\   | | | |___ | || |\\  | |___  | |___  | | | |_) | |___|  * <  | |*__ / ___ \\|  *| | |*__ ",
        "\t |____/|_|\\_\\  |_| |_____|___|_| \\_|_____|  \\____| |_| |____/|_____|_| \\_\\  \\____/_/   \\_\\_|   |_____|"
    };

    for (int y = 0; y < art.size(); ++y) {
        for (size_t x = 0; x < art[y].length(); ++x) {
            if (art[y][x] != ' ') {
                validPositions.push_back({ static_cast<int>(x), y });
            }
        }
    }

    int numStars = 10;
    for (int i = 0; i < numStars; ++i) {
        int posIndex = uniform_int_distribution<>(0, validPositions.size() - 1)(gen);
        stars.push_back({
            validPositions[posIndex].first,
            validPositions[posIndex].second,
            true
            });
    }
}

void AsciiAnimator::clearLogoArea() {
    lock_guard<mutex> lock(consoleMutex);
    string emptyLine(120, ' ');
    for (int i = 0; i < logoHeight; ++i) {
        setCursorPosition(0, i);
        cout << emptyLine;
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