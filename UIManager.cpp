#include "UIManager.h"
#include <cstdlib> // For system()

// Constructor
UIManager::UIManager(const std::string& artFile, const std::string& thanksFile)
    : asciiArtFile(artFile), asciiThanksFile(thanksFile) {}

// Helper function to display content from a file
void UIManager::displayFileContent(const std::string& filePath, bool animate) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Tidak dapat membuka file: " << filePath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Assuming color codes are in the file itself or not used here.
        std::cout << line << std::endl;
        if (animate) {
            pause(100); // Animate with 100ms delay per line
        }
    }
    file.close();
}

// Public methods
void UIManager::displayAsciiArt() {
    // clearScreen(); // Optional: clear screen before displaying art
    displayFileContent(asciiArtFile, false);
}

void UIManager::displayAsciiThanks() {
    // clearScreen(); // Optional: clear screen before displaying thanks
    displayFileContent(asciiThanksFile, true);
    std::cout << "\nTerima kasih telah menggunakan aplikasi ini!\n"; // Additional message
    pause(2000); // Pause for 2 seconds after thanks
}

void UIManager::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear"); // POSIX
#endif
}

void UIManager::pause(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}
