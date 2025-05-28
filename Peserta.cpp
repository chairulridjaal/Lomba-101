#include "Peserta.h"
#include <stdexcept> // Required for std::invalid_argument
#include <iostream> // For potential debugging, not strictly necessary for functionality

// Default constructor
Peserta::Peserta() : umur(0), jenis_kelamin('L') {
    // Initialize string members to empty strings by default
    // nama, no_reg, kategori, alamat, no_hp are default-initialized to empty
}

// Setter methods implementation
void Peserta::setNama(const std::string& nama) {
    this->nama = nama;
}

void Peserta::setNoReg(const std::string& no_reg) {
    this->no_reg = no_reg;
}

void Peserta::setKategori(const std::string& kategori) {
    this->kategori = kategori;
}

void Peserta::setUmur(int umur) {
    if (umur <= 0) {
        throw std::invalid_argument("Umur harus positif.");
    }
    this->umur = umur;
}

void Peserta::setJenisKelamin(char jenis_kelamin) {
    if (jenis_kelamin != 'L' && jenis_kelamin != 'P') {
        throw std::invalid_argument("Jenis kelamin harus 'L' atau 'P'.");
    }
    this->jenis_kelamin = jenis_kelamin;
}

void Peserta::setAlamat(const std::string& alamat) {
    this->alamat = alamat;
}

void Peserta::setNoHp(const std::string& no_hp) {
    this->no_hp = no_hp;
}
