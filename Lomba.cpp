#include "Lomba.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // For std::sort, std::transform
#include <limits>    // For std::numeric_limits
#include <iomanip>   // For std::setw, std::left
#include <sstream>   // For std::stringstream in generateNoReg and muatData (though muatData is in DataManager)
#include <functional> // For std::hash in generateNoReg (if kept as is)
// #include <cstdlib> // Already included in UIManager.cpp for system()

// Constructor
Lomba::Lomba(const std::string& pesertaFile, const std::string& lombaFile,
             const std::string& asciiArtFile, const std::string& asciiThanksFile)
    : dataManager(pesertaFile, lombaFile), 
      uiManager(asciiArtFile, asciiThanksFile) {
    pesertaMap = dataManager.muatData();
    daftarLomba = dataManager.muatDaftarLomba();
    // If daftarLomba is empty, add some default competitions
    if (daftarLomba.empty()) {
        daftarLomba.push_back("Pemrograman");
        daftarLomba.push_back("Debat");
        daftarLomba.push_back("Menulis Esai");
        dataManager.simpanDaftarLomba(daftarLomba); // Save defaults
    }
}

// Private helper method to generate registration number
std::string Lomba::generateNoReg(const Peserta& p) {
    // Using simple sequential generation for now, can be replaced with hash if needed
    // For hash-based generation:
    std::string combined = p.getNama() + p.getKategori() + std::to_string(p.getUmur()) + p.getJenisKelamin() + p.getAlamat() + p.getNoHp();
    std::size_t hash_value = std::hash<std::string>{}(combined);
    std::stringstream ss;
    ss << std::hex << hash_value;
    return ss.str().substr(0, 5); // Take first 5 chars of hex hash
}

// Private helper method to process the registration queue
void Lomba::processPesertaQueue() {
    int count = 0;
    uiManager.clearScreen();
    std::cout << "\nMemproses antrian peserta...\n";
    std::cout << "=============================\n";
    while (!pesertaQueue.empty()) {
        Peserta p = pesertaQueue.front(); // Get a copy
        pesertaQueue.pop();
        
        std::string newNoReg = generateNoReg(p);
        p.setNoReg(newNoReg); // Set the generated number
        
        pesertaMap[p.getNoReg()] = p; // Add to map
        
        std::cout << "\nPeserta " << p.getNama() << " (No. Reg: " << p.getNoReg() << ") berhasil didaftarkan!";
        count++;
        uiManager.pause(500); // 0.5 second pause
    }
    if (count > 0) {
        dataManager.simpanData(pesertaMap); // Save all new registrations
    }
    std::cout << "\n\n=============================\n";
    std::cout << count << " Peserta berhasil didaftarkan!\n";
    uiManager.pause(2000); // 2-second pause
}

// Handler for participant registration
void Lomba::handleRegistrasi() {
    Peserta p_temp; // Temporary Peserta object to gather data
    char choice;

    do {
        uiManager.clearScreen();
        uiManager.displayAsciiArt(); // Or a specific registration header
        std::cout << "\n--- Pendaftaran Peserta Baru ---\n";
        std::cout << "Jumlah antrian saat ini: " << pesertaQueue.size() << "\n\n";

        // Kategori Lomba
        if (daftarLomba.empty()) {
            std::cout << "Belum ada kategori lomba yang tersedia. Silakan tambahkan lomba terlebih dahulu.\n";
            uiManager.pause(2000);
            return;
        }
        std::cout << "Pilihan Kategori Lomba:\n";
        for (size_t i = 0; i < daftarLomba.size(); ++i) {
            std::cout << i + 1 << ". " << daftarLomba[i] << "\n";
        }
        std::cout << daftarLomba.size() + 1 << ". Kembali ke Menu Utama\n";
        std::cout << "Pilihan: ";
        int kategoriPilihan;
        std::cin >> kategoriPilihan;

        if (std::cin.fail() || kategoriPilihan < 1 || kategoriPilihan > daftarLomba.size() + 1) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Pilihan tidak valid!\n";
            uiManager.pause(1000);
            continue;
        }
        if (kategoriPilihan == daftarLomba.size() + 1) break; // Kembali
        p_temp.setKategori(daftarLomba[kategoriPilihan - 1]);

        // Data Diri
        std::cout << "\n--- Isi Data Diri Peserta ---\n";
        std::cout << "Nama Lengkap: ";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear buffer
        std::string nama;
        std::getline(std::cin, nama);
        p_temp.setNama(nama);

        int umur;
        while (true) {
            std::cout << "Umur: ";
            std::cin >> umur;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Input umur tidak valid. Harap masukkan angka.\n";
            } else {
                try {
                    p_temp.setUmur(umur);
                    break;
                } catch (const std::invalid_argument& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
            }
        }
        
        char jk;
        while(true){
            std::cout << "Jenis Kelamin (L/P): ";
            std::cin >> jk;
            jk = toupper(jk);
            try {
                p_temp.setJenisKelamin(jk);
                break;
            } catch (const std::invalid_argument& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
        }

        std::cout << "Alamat: ";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::string alamat;
        std::getline(std::cin, alamat);
        p_temp.setAlamat(alamat);

        std::cout << "No. HP: ";
        std::string no_hp;
        std::getline(std::cin, no_hp);
        p_temp.setNoHp(no_hp);

        // Konfirmasi
        uiManager.clearScreen();
        std::cout << "\n--- Konfirmasi Data Peserta ---\n";
        std::cout << "Nama      : " << p_temp.getNama() << "\n";
        std::cout << "Kategori  : " << p_temp.getKategori() << "\n";
        std::cout << "Umur      : " << p_temp.getUmur() << "\n";
        std::cout << "Kelamin   : " << p_temp.getJenisKelamin() << "\n";
        std::cout << "Alamat    : " << p_temp.getAlamat() << "\n";
        std::cout << "No. HP    : " << p_temp.getNoHp() << "\n";
        std::cout << "---------------------------------\n";
        std::cout << "Apakah data di atas sudah benar? (Y/N): ";
        std::cin >> choice;

        if (toupper(choice) == 'Y') {
            pesertaQueue.push(p_temp); // Add to queue
            std::cout << "Peserta " << p_temp.getNama() << " berhasil ditambahkan ke antrian.\n";
        } else {
            std::cout << "Pendaftaran dibatalkan atau data akan diisi ulang.\n";
        }

        std::cout << "Tambah peserta lain? (Y/N): ";
        std::cin >> choice;
        if (toupper(choice) == 'N') {
            break; 
        }

    } while (toupper(choice) == 'Y');

    if (!pesertaQueue.empty()) {
        processPesertaQueue();
    }
}

// Handler for participant cancellation
void Lomba::handlePembatalan() {
    uiManager.clearScreen();
    std::cout << "\n--- Pembatalan Peserta ---\n";
    if (pesertaMap.empty()) {
        std::cout << "Belum ada peserta yang terdaftar.\n";
        uiManager.pause(1500);
        return;
    }

    std::string noRegInput;
    std::cout << "Masukkan No. Registrasi peserta yang akan dibatalkan: ";
    std::cin >> noRegInput;

    auto it = pesertaMap.find(noRegInput);
    if (it == pesertaMap.end()) {
        std::cout << "No. Registrasi tidak ditemukan.\n";
        uiManager.pause(1500);
        return;
    }

    const Peserta& p = it->second;
    std::cout << "\n--- Detail Peserta ---\n";
    std::cout << "Nama      : " << p.getNama() << "\n";
    std::cout << "No. Regis : " << p.getNoReg() << "\n";
    std::cout << "Kategori  : " << p.getKategori() << "\n";
    std::cout << "-----------------------\n";
    std::cout << "Yakin ingin membatalkan peserta ini? (Y/N): ";
    char confirmChoice;
    std::cin >> confirmChoice;

    if (toupper(confirmChoice) == 'Y') {
        pembatalanStack.push(p); // Push to cancellation stack
        pesertaMap.erase(it);    // Remove from map
        dataManager.simpanData(pesertaMap); // Save changes
        std::cout << "Peserta dengan No. Registrasi " << noRegInput << " berhasil dibatalkan.\n";
    } else {
        std::cout << "Pembatalan dibatalkan.\n";
    }
    uiManager.pause(2000);
}

// Handler for listing participants
void Lomba::handleListPeserta() {
    if (pesertaMap.empty()) {
        uiManager.clearScreen();
        std::cout << "\nBelum ada peserta yang terdaftar.\n";
        uiManager.pause(1500);
        return;
    }

    std::vector<Peserta> listPesertaVec;
    for(const auto& pair : pesertaMap) {
        listPesertaVec.push_back(pair.second);
    }

    int sortOption = 0;
    // Simple sort for now: 1 for Name Asc, 2 for Name Desc, 3 for Category Asc, 4 for Category Desc
    // This part can be expanded with more sophisticated sorting UI

    char sortChoiceChar;
    uiManager.clearScreen();
    std::cout << "\n--- Daftar Peserta Terdaftar ---\n";
    std::cout << "Pilihan Urutan: \n"
              << "1. Nama (A-Z)\n"
              << "2. Nama (Z-A)\n"
              << "3. Kategori (A-Z)\n"
              << "4. Kategori (Z-A)\n"
              << "0. Default (Tanpa Urutan Spesifik)\n"
              << "Pilihan: ";
    std::cin >> sortChoiceChar;
    
    if(isdigit(sortChoiceChar)) {
        sortOption = sortChoiceChar - '0';
    }

    switch(sortOption) {
        case 1: // Nama Asc
            std::sort(listPesertaVec.begin(), listPesertaVec.end(), [](const Peserta& a, const Peserta& b) {
                std::string nameA = a.getNama(); std::transform(nameA.begin(), nameA.end(), nameA.begin(), ::tolower);
                std::string nameB = b.getNama(); std::transform(nameB.begin(), nameB.end(), nameB.begin(), ::tolower);
                return nameA < nameB;
            });
            break;
        case 2: // Nama Desc
             std::sort(listPesertaVec.begin(), listPesertaVec.end(), [](const Peserta& a, const Peserta& b) {
                std::string nameA = a.getNama(); std::transform(nameA.begin(), nameA.end(), nameA.begin(), ::tolower);
                std::string nameB = b.getNama(); std::transform(nameB.begin(), nameB.end(), nameB.begin(), ::tolower);
                return nameA > nameB;
            });
            break;
        case 3: // Kategori Asc
            std::sort(listPesertaVec.begin(), listPesertaVec.end(), [](const Peserta& a, const Peserta& b) {
                return a.getKategori() < b.getKategori();
            });
            break;
        case 4: // Kategori Desc
            std::sort(listPesertaVec.begin(), listPesertaVec.end(), [](const Peserta& a, const Peserta& b) {
                return a.getKategori() > b.getKategori();
            });
            break;
        default: // No sort / by registration order (map iteration order not guaranteed)
            break; 
    }


    int totalPeserta = listPesertaVec.size();
    int pesertaPerHalaman = 10;
    int totalHalaman = (totalPeserta + pesertaPerHalaman - 1) / pesertaPerHalaman;
    if (totalHalaman == 0) totalHalaman = 1; // Ensure at least one page even if empty
    int halamanSaatIni = 1;

    char navChoice;
    do {
        uiManager.clearScreen();
        std::cout << "\n--- Daftar Peserta (Halaman " << halamanSaatIni << "/" << totalHalaman << ") ---\n";
        std::cout << "Total Peserta: " << totalPeserta << "\n";
        std::cout << "------------------------------------------------------------------------------------------------------\n";
        std::cout << std::left << std::setw(7) << "No."
                  << std::setw(12) << "No.Reg"
                  << std::setw(25) << "Nama"
                  << std::setw(20) << "Kategori"
                  << std::setw(7) << "Umur"
                  << std::setw(5) << "JK"
                  << std::setw(20) << "Alamat"
                  << std::setw(15) << "No. HP" << std::endl;
        std::cout << "------------------------------------------------------------------------------------------------------\n";

        int startIndex = (halamanSaatIni - 1) * pesertaPerHalaman;
        int endIndex = std::min(startIndex + pesertaPerHalaman, totalPeserta);

        for (int i = startIndex; i < endIndex; ++i) {
            const auto& p = listPesertaVec[i];
            std::cout << std::left << std::setw(7) << i + 1
                      << std::setw(12) << p.getNoReg()
                      << std::setw(25) << p.getNama()
                      << std::setw(20) << p.getKategori()
                      << std::setw(7) << p.getUmur()
                      << std::setw(5) << p.getJenisKelamin()
                      << std::setw(20) << p.getAlamat()
                      << std::setw(15) << p.getNoHp() << std::endl;
        }
        std::cout << "------------------------------------------------------------------------------------------------------\n";

        if (totalHalaman > 1) {
            std::cout << "Navigasi: [N]ext, [P]revious, [E]xit to Menu: ";
            std::cin >> navChoice;
            navChoice = toupper(navChoice);
            if (navChoice == 'N' && halamanSaatIni < totalHalaman) {
                halamanSaatIni++;
            } else if (navChoice == 'P' && halamanSaatIni > 1) {
                halamanSaatIni--;
            } else if (navChoice != 'N' && navChoice != 'P') { // Any other key to exit pagination
                break;
            }
        } else {
            std::cout << "Tekan tombol apa saja untuk kembali ke menu...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n'); // Clear buffer before getline
            std::cin.get(); // Wait for user to press enter
            break;
        }
    } while (totalHalaman > 1);
}

// Handler for searching participant
void Lomba::handleCariPeserta() {
    uiManager.clearScreen();
    std::cout << "\n--- Cari Peserta ---\n";
    if (pesertaMap.empty()) {
        std::cout << "Belum ada peserta yang terdaftar.\n";
        uiManager.pause(1500);
        return;
    }

    std::string noRegInput;
    std::cout << "Masukkan No. Registrasi peserta yang dicari: ";
    std::cin >> noRegInput;

    auto it = pesertaMap.find(noRegInput);
    if (it == pesertaMap.end()) {
        std::cout << "No. Registrasi tidak ditemukan.\n";
    } else {
        const Peserta& p = it->second;
        std::cout << "\n--- Detail Peserta Ditemukan ---\n";
        std::cout << "Nama      : " << p.getNama() << "\n";
        std::cout << "No. Regis : " << p.getNoReg() << "\n";
        std::cout << "Kategori  : " << p.getKategori() << "\n";
        std::cout << "Umur      : " << p.getUmur() << "\n";
        std::cout << "Kelamin   : " << p.getJenisKelamin() << "\n";
        std::cout << "Alamat    : " << p.getAlamat() << "\n";
        std::cout << "No. HP    : " << p.getNoHp() << "\n";
        std::cout << "---------------------------------\n";
    }
    uiManager.pause(3000); // Pause to show result
}

// Handler for adding a new competition category
void Lomba::handleTambahLomba() {
    uiManager.clearScreen();
    std::cout << "\n--- Tambah Kategori Lomba Baru ---\n";
    std::string namaLombaBaru;
    std::cout << "Masukkan nama kategori lomba baru: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear buffer
    std::getline(std::cin, namaLombaBaru);

    if (namaLombaBaru.empty()) {
        std::cout << "Nama lomba tidak boleh kosong.\n";
    } else {
        // Check if lomba already exists (case-insensitive for robustness, optional)
        bool exists = false;
        std::string lowerNamaLombaBaru = namaLombaBaru;
        std::transform(lowerNamaLombaBaru.begin(), lowerNamaLombaBaru.end(), lowerNamaLombaBaru.begin(), ::tolower);
        for(const auto& l : daftarLomba) {
            std::string currentLomba = l;
            std::transform(currentLomba.begin(), currentLomba.end(), currentLomba.begin(), ::tolower);
            if (currentLomba == lowerNamaLombaBaru) {
                exists = true;
                break;
            }
        }

        if (exists) {
            std::cout << "Kategori lomba '" << namaLombaBaru << "' sudah ada.\n";
        } else {
            daftarLomba.push_back(namaLombaBaru);
            dataManager.simpanDaftarLomba(daftarLomba);
            std::cout << "Kategori lomba '" << namaLombaBaru << "' berhasil ditambahkan.\n";
        }
    }
    uiManager.pause(2000);
}

// Handler for editing participant data
void Lomba::handleEditPeserta() {
    uiManager.clearScreen();
    std::cout << "\n--- Edit Data Peserta ---\n";
    
    std::string noRegInput;
    std::cout << "Masukkan No. Registrasi Peserta yang akan diedit: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear buffer before getline, just in case
    std::getline(std::cin, noRegInput); // Use getline to read the whole line, including potential spaces

    if (noRegInput.empty()) {
        std::cout << "No. Registrasi tidak boleh kosong.\n";
        uiManager.pause(2000);
        return;
    }

    auto it = pesertaMap.find(noRegInput);
    if (it == pesertaMap.end()) {
        std::cout << "No. Registrasi '" << noRegInput << "' tidak ditemukan!\n";
        uiManager.pause(2000);
        return;
    }

    // Participant found, display current details
    Peserta& p = it->second; // Get a reference to the Peserta object
    std::cout << "\n--- Data Peserta Saat Ini ---\n";
    std::cout << "Nama     : " << p.getNama() << "\n";
    std::cout << "No. Regis: " << p.getNoReg() << "\n";
    std::cout << "Kategori : " << p.getKategori() << "\n";
    std::cout << "Umur     : " << p.getUmur() << "\n";
    std::cout << "Sex      : " << p.getJenisKelamin() << "\n";
    std::cout << "Alamat   : " << p.getAlamat() << "\n";
    std::cout << "No. HP   : " << p.getNoHp() << "\n";
    std::cout << "---------------------------------\n";

    std::cout << "\n--- Masukkan Data Baru (kosongkan jika tidak ingin diubah) ---\n";

    std::string newNamaStr, newKategoriStr, newUmurStr, newJKStr, newAlamatStr, newNoHPStr;

    // Nama
    std::cout << "Nama (" << p.getNama() << "): ";
    // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ensure buffer is clear before first getline if needed
    std::getline(std::cin, newNamaStr);

    // Kategori
    std::cout << "Kategori Saat Ini: " << p.getKategori() << std::endl;
    if (!daftarLomba.empty()) {
        std::cout << "Pilihan Kategori Baru:\n";
        for (size_t i = 0; i < daftarLomba.size(); ++i) {
            std::cout << i + 1 << ". " << daftarLomba[i] << "\n";
        }
        std::cout << "Masukkan nomor pilihan (atau tekan Enter untuk tidak mengubah): ";
        std::string kategoriChoiceStr;
        std::getline(std::cin, kategoriChoiceStr);
        if (!kategoriChoiceStr.empty()) {
            try {
                int kategoriPilihan = std::stoi(kategoriChoiceStr);
                if (kategoriPilihan >= 1 && kategoriPilihan <= static_cast<int>(daftarLomba.size())) {
                    newKategoriStr = daftarLomba[kategoriPilihan - 1];
                } else {
                    std::cout << "Pilihan kategori tidak valid. Kategori tidak akan diubah.\n";
                }
            } catch (const std::invalid_argument& ia) {
                std::cout << "Input kategori tidak valid. Kategori tidak akan diubah.\n";
            } catch (const std::out_of_range& oor) {
                std::cout << "Input kategori di luar jangkauan. Kategori tidak akan diubah.\n";
            }
        }
    } else {
        std::cout << "Tidak ada kategori lomba lain yang tersedia.\n";
    }
    
    // Umur
    std::cout << "Umur (" << p.getUmur() << "): ";
    std::getline(std::cin, newUmurStr);

    // Jenis Kelamin
    std::cout << "Jenis Kelamin (L/P) (" << p.getJenisKelamin() << "): ";
    std::getline(std::cin, newJKStr);

    // Alamat
    std::cout << "Alamat (" << p.getAlamat() << "): ";
    std::getline(std::cin, newAlamatStr);

    // No. HP
    std::cout << "No. HP (" << p.getNoHp() << "): ";
    std::getline(std::cin, newNoHPStr);

    bool dataChanged = false;

    // Update Nama
    if (!newNamaStr.empty()) {
        p.setNama(newNamaStr);
        dataChanged = true;
    }

    // Update Kategori
    if (!newKategoriStr.empty()) {
        p.setKategori(newKategoriStr); // Assumes setKategori is a simple setter
        dataChanged = true;
    }

    // Update Umur
    if (!newUmurStr.empty()) {
        try {
            int umurInt = std::stoi(newUmurStr);
            p.setUmur(umurInt); // This might throw if validation within setUmur fails
            dataChanged = true;
        } catch (const std::invalid_argument& e) {
            std::cout << "Peringatan: Input umur tidak valid ('" << newUmurStr << "'). Umur tidak diubah. (" << e.what() << ")\n";
        } catch (const std::out_of_range& e) {
            std::cout << "Peringatan: Input umur di luar jangkauan ('" << newUmurStr << "'). Umur tidak diubah. (" << e.what() << ")\n";
        }
    }

    // Update Jenis Kelamin
    if (!newJKStr.empty()) {
        if (newJKStr.length() == 1) {
            try {
                p.setJenisKelamin(toupper(newJKStr[0])); // This might throw if validation fails
                dataChanged = true;
            } catch (const std::invalid_argument& e) {
                std::cout << "Peringatan: Input jenis kelamin tidak valid ('" << newJKStr[0] << "'). Jenis kelamin tidak diubah. (" << e.what() << ")\n";
            }
        } else {
            std::cout << "Peringatan: Input jenis kelamin tidak valid (harus satu karakter). Jenis kelamin tidak diubah.\n";
        }
    }

    // Update Alamat
    if (!newAlamatStr.empty()) {
        p.setAlamat(newAlamatStr);
        dataChanged = true;
    }

    // Update No. HP
    if (!newNoHPStr.empty()) {
        p.setNoHp(newNoHPStr);
        dataChanged = true;
    }

    if (dataChanged) {
        dataManager.simpanData(pesertaMap); // Save changes to file
        std::cout << "\nData peserta telah berhasil diperbarui.\n";
    } else {
        std::cout << "\nTidak ada perubahan data yang dilakukan.\n";
    }
    
    uiManager.pause(2000); 
}

// Main application loop
void Lomba::run() {
    int choice;
    do {
        uiManager.clearScreen();
        uiManager.displayAsciiArt();
        std::cout << "\n--- Menu Utama Lomba 101 ---\n";
        std::cout << "1. Daftar Peserta\n";
        std::cout << "2. Hapus Peserta (Pembatalan)\n";
        std::cout << "3. Cari Peserta\n";
        std::cout << "4. List Peserta Terdaftar\n";
        std::cout << "5. Tambah Kategori Lomba\n";
        std::cout << "6. Edit Peserta\n"; // New option
        std::cout << "7. Keluar\n";       // Keluar is now 7
        std::cout << "----------------------------\n";
        std::cout << "Pilihan Anda: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            choice = 0; // Set to a default that will show "Pilihan tidak valid"
        }

        switch (choice) {
            case 1:
                handleRegistrasi();
                break;
            case 2:
                handlePembatalan();
                break;
            case 3:
                handleCariPeserta();
                break;
            case 4:
                handleListPeserta();
                break;
            case 5:
                handleTambahLomba();
                break;
            case 6: // Call the new handler
                handleEditPeserta();
                break;
            case 7: // Keluar is now 7
                uiManager.displayAsciiThanks();
                break;
            default:
                std::cout << "Pilihan tidak valid. Silakan coba lagi.\n";
                uiManager.pause(1000);
                break;
        }
    } while (choice != 7); // Loop condition updated to 7
}
