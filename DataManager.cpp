#include "DataManager.h"
#include <fstream>   // For file input/output operations
#include <sstream>   // For std::stringstream
#include <iostream>  // For std::cerr

// Constructor
DataManager::DataManager(const std::string& pesertaFile, const std::string& lombaFile)
    : dataPesertaFile(pesertaFile), daftarLombaFile(lombaFile) {}

// Method to save Peserta data to a CSV file
void DataManager::simpanData(const std::map<std::string, Peserta>& pesertaMap) {
    std::ofstream outFile(dataPesertaFile);

    if (!outFile.is_open()) {
        std::cerr << "Error: Tidak dapat membuka file untuk menyimpan data peserta: " << dataPesertaFile << std::endl;
        return;
    }

    // Optional: Write a header line (can be useful for CSV readability)
    // outFile << "no_reg,nama,kategori,umur,jenis_kelamin,alamat,no_hp\n";

    for (const auto& pair : pesertaMap) {
        const Peserta& p = pair.second;
        outFile << p.getNoReg() << ","
                << p.getNama() << ","
                << p.getKategori() << ","
                << p.getUmur() << ","
                << p.getJenisKelamin() << ","
                << p.getAlamat() << ","
                << p.getNoHp() << "\n";
    }
    outFile.close();
}

// Method to load Peserta data from a CSV file
std::map<std::string, Peserta> DataManager::muatData() {
    std::map<std::string, Peserta> pesertaMap;
    std::ifstream inFile(dataPesertaFile);

    if (!inFile.is_open()) {
        // If the file doesn't exist or cannot be opened, return an empty map.
        // This is not necessarily an error for the first run.
        // std::cerr << "Info: File data peserta tidak ditemukan atau tidak dapat dibuka: " << dataPesertaFile << std::endl;
        return pesertaMap;
    }

    std::string line;
    // Optional: Skip header line if you wrote one in simpanData
    // if (std::getline(inFile, line)) { /* Do nothing with header */ }

    while (std::getline(inFile, line)) {
        std::stringstream ss(line);
        std::string field;
        Peserta p;
        int fieldCount = 0;

        try {
            // no_reg
            if (std::getline(ss, field, ',')) {
                p.setNoReg(field);
                fieldCount++;
            } else continue; // Skip malformed line

            // nama
            if (std::getline(ss, field, ',')) {
                p.setNama(field);
                fieldCount++;
            } else continue;

            // kategori
            if (std::getline(ss, field, ',')) {
                p.setKategori(field);
                fieldCount++;
            } else continue;

            // umur
            if (std::getline(ss, field, ',')) {
                try {
                    p.setUmur(std::stoi(field));
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Peringatan: Format umur tidak valid untuk no_reg " << p.getNoReg() << ". Menggunakan umur default. Error: " << e.what() << std::endl;
                    // p.setUmur(0); // Or some other default / skip logic
                    continue; // Skip this record
                } catch (const std::out_of_range& e) {
                     std::cerr << "Peringatan: Nilai umur di luar jangkauan untuk no_reg " << p.getNoReg() << ". Menggunakan umur default. Error: " << e.what() << std::endl;
                     continue; // Skip this record
                }
                fieldCount++;
            } else continue;

            // jenis_kelamin
            if (std::getline(ss, field, ',')) {
                if (!field.empty()) {
                    try {
                        p.setJenisKelamin(field[0]);
                    } catch (const std::invalid_argument& e) {
                        std::cerr << "Peringatan: Format jenis kelamin tidak valid untuk no_reg " << p.getNoReg() << ". Menggunakan jenis kelamin default. Error: " << e.what() << std::endl;
                        // p.setJenisKelamin('L'); // Or some other default / skip logic
                        continue; // Skip this record
                    }
                } else {
                     std::cerr << "Peringatan: Jenis kelamin kosong untuk no_reg " << p.getNoReg() << ". Menggunakan jenis kelamin default." << std::endl;
                     continue; // Skip this record
                }
                fieldCount++;
            } else continue;

            // alamat
            if (std::getline(ss, field, ',')) {
                p.setAlamat(field);
                fieldCount++;
            } else continue;
            
            // no_hp (last field, so read until end of line)
            if (std::getline(ss, field)) { // No delimiter needed for the last field
                p.setNoHp(field);
                fieldCount++;
            } else continue;


            if (fieldCount == 7) { // Ensure all fields were read
                 pesertaMap[p.getNoReg()] = p;
            } else {
                std::cerr << "Peringatan: Baris data peserta tidak lengkap: " << line << std::endl;
            }

        } catch (const std::exception& e) {
            std::cerr << "Error memproses baris: " << line << " | Pesan: " << e.what() << std::endl;
            // Decide if you want to skip this line or handle error differently
        }
    }

    inFile.close();
    return pesertaMap;
}

// Method to save the list of competitions to a file
void DataManager::simpanDaftarLomba(const std::vector<std::string>& daftarLomba) {
    std::ofstream outFile(daftarLombaFile);

    if (!outFile.is_open()) {
        std::cerr << "Error: Tidak dapat membuka file untuk menyimpan daftar lomba: " << daftarLombaFile << std::endl;
        return;
    }

    for (const auto& lomba : daftarLomba) {
        outFile << lomba << "\n";
    }
    outFile.close();
}

// Method to load the list of competitions from a file
std::vector<std::string> DataManager::muatDaftarLomba() {
    std::vector<std::string> daftarLomba;
    std::ifstream inFile(daftarLombaFile);

    if (!inFile.is_open()) {
        // If the file doesn't exist or cannot be opened, return an empty vector.
        // std::cerr << "Info: File daftar lomba tidak ditemukan atau tidak dapat dibuka: " << daftarLombaFile << std::endl;
        return daftarLomba;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        if (!line.empty()) { // Avoid adding empty lines if any
            daftarLomba.push_back(line);
        }
    }

    inFile.close();
    return daftarLomba;
}
