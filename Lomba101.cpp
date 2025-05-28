#include <iostream> // For std::cout, std::cerr (primarily for error handling in main)
#include <string>   // For std::string
#include "Lomba.h"  // For the Lomba class, which encapsulates the application logic

int main() {
    // Define file paths for data storage and UI assets.
    // These files are expected to be in the same directory as the executable,
    // or the paths should be adjusted (e.g., made configurable or relative to a known location).
    std::string dataPesertaFile = "data_peserta.txt";
    std::string daftarLombaFile = "daftar_lomba.txt";
    std::string asciiArtFile = "ascii_art.txt";        
    std::string asciiThanksFile = "ascii_thanks.txt";  

    try {
        // Instantiate the Lomba application object.
        // The constructor of Lomba is responsible for initializing all necessary
        // components, including DataManager and UIManager, and loading initial data.
        Lomba lomba(dataPesertaFile, daftarLombaFile, asciiArtFile, asciiThanksFile);

        // Start the main application loop.
        // The run() method contains the primary user interaction logic (main menu, etc.).
        lomba.run();

    } catch (const std::exception& e) {
        // Catch standard exceptions that might propagate to main.
        std::cerr << "Terjadi kesalahan (exception) pada level aplikasi: " << e.what() << std::endl;
        return 1; // Return a non-zero value to indicate an error.
    } catch (...) {
        // Catch any other unknown exceptions.
        std::cerr << "Terjadi kesalahan (unknown exception) pada level aplikasi." << std::endl;
        return 1; // Return a non-zero value to indicate an error.
    }

    // Return 0 to indicate successful execution.
    return 0;
}
