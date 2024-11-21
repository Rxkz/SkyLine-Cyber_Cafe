#ifndef LOGO_H
#define LOGO_H

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <random>
#include <windows.h>
#include <conio.h>
#include <mutex> // Provides synchronization primitives to prevent data races in multithreaded programs.


struct Star {
    int x, y;
    bool visible;
};

class AsciiAnimator {
private:
    std::vector<std::string> art;
    std::vector<Star> stars;
    std::random_device rd;
    std::mt19937 gen;
    bool running;
    std::vector<std::pair<int, int>> validPositions;
    static std::mutex consoleMutex;

    void updateStars();
    void drawFrame();
    void clearScreen();

public:
    AsciiAnimator();
    void startAnimation();
    void stopAnimation() { running = false; }
    void initialDraw();
    void clearLogoArea();  // Added this declaration
};

void ShowLogoAnimation();
void StopLogoAnimation();

#endif // LOGO_H