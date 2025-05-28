#ifndef PESERTA_H
#define PESERTA_H

#include <string>
#include <stdexcept> // Required for std::invalid_argument

class Peserta {
private:
    std::string nama;
    std::string no_reg;
    std::string kategori;
    int umur;
    char jenis_kelamin;
    std::string alamat;
    std::string no_hp;

public:
    // Default constructor
    Peserta();

    // Getter methods
    std::string getNama() const { return nama; }
    std::string getNoReg() const { return no_reg; }
    std::string getKategori() const { return kategori; }
    int getUmur() const { return umur; }
    char getJenisKelamin() const { return jenis_kelamin; }
    std::string getAlamat() const { return alamat; }
    std::string getNoHp() const { return no_hp; }

    // Setter methods
    void setNama(const std::string& nama);
    void setNoReg(const std::string& no_reg);
    void setKategori(const std::string& kategori);
    void setUmur(int umur);
    void setJenisKelamin(char jenis_kelamin);
    void setAlamat(const std::string& alamat);
    void setNoHp(const std::string& no_hp);
};

#endif // PESERTA_H
