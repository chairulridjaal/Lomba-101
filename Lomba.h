#ifndef LOMBA_H
#define LOMBA_H

#include <string>
#include <vector>
#include <map>
#include "Peserta.h"
#include "DataStructures.h"
#include "DataManager.h"
#include "UIManager.h"

class Lomba {
private:
    std::map<std::string, Peserta> pesertaMap;
    std::vector<std::string> daftarLomba;
    queuePeserta pesertaQueue;
    stackPeserta pembatalanStack;
    
    DataManager dataManager;
    UIManager uiManager;

    // Private helper methods
    std::string generateNoReg(const Peserta& p);
    void processPesertaQueue();
    
    void handleRegistrasi();
    void handlePembatalan();
    void handleListPeserta();
    void handleCariPeserta();
    void handleTambahLomba();
    void handleEditPeserta(); // New method declaration

public:
    // Constructor
    Lomba(const std::string& pesertaFile, const std::string& lombaFile, 
          const std::string& asciiArtFile, const std::string& asciiThanksFile);

    // Main application loop
    void run();
};

#endif // LOMBA_H
