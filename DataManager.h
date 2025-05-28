#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <string>
#include <vector>
#include <map>
#include "Peserta.h" // Assuming Peserta.h is in the same directory or include path

class DataManager {
private:
    std::string dataPesertaFile;
    std::string daftarLombaFile;

public:
    // Constructor
    DataManager(const std::string& pesertaFile, const std::string& lombaFile);

    // Methods for Peserta data
    void simpanData(const std::map<std::string, Peserta>& pesertaMap);
    std::map<std::string, Peserta> muatData();

    // Methods for daftar lomba
    void simpanDaftarLomba(const std::vector<std::string>& daftarLomba);
    std::vector<std::string> muatDaftarLomba();
};

#endif // DATAMANAGER_H
