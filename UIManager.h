#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <string>
#include <iostream> // For std::cout, std::cerr
#include <fstream>  // For std::ifstream
#include <chrono>   // For std::chrono::milliseconds
#include <thread>   // For std::this_thread::sleep_for

// Define color codes (optional, can be removed if not used or handled differently)
#define GOLD "\033[33m" 
#define RESET "\033[0m"

class UIManager {
private:
    std::string asciiArtFile;
    std::string asciiThanksFile;

    // Helper function to display content from a file
    void displayFileContent(const std::string& filePath, bool animate);

public:
    // Constructor
    UIManager(const std::string& asciiArtFile, const std::string& asciiThanksFile);

    // Public methods
    void displayAsciiArt();
    void displayAsciiThanks();
    void clearScreen();
    void pause(int milliseconds);
};

#endif // UIMANAGER_H
