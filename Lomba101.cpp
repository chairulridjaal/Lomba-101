#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <iomanip>
#include <string>
#include <algorithm>
#include <limits>
#include <cstdlib>
#include <functional>
#include <chrono>
#include <thread>
#include <queue>
#include <stack>

#define GOLD "\033[33m"
#define RED "\033[31m"
#define RESET "\033[0m"

using namespace std;

struct Peserta
{
    string nama;
    string no_reg;
    string kategori;
    int umur;
    char jenis_kelamin;
    string alamat;
    string no_hp;
};

class Lomba
{
private:
    map<string, Peserta> peserta;
    vector<string> daftarLomba;
    queue<Peserta> pesertaQueue;
    stack<Peserta> pembatalanStack;

    int no_reg = 1;

    void ascii()
    {
        system("cls");
        string filename = "ascii_art.txt";
        ifstream file(filename);

        string line;
        while (getline(file, line))
        {
            cout << GOLD << line << RESET << endl;
        }

        file.close();
    }

    void ascii_thanks()
    {
        system("cls");
        string filename = "ascii_thanks.txt";
        ifstream file(filename);

        string line;
        while (getline(file, line))
        {
            cout << GOLD << line << RESET << endl;
            this_thread::sleep_for(chrono::milliseconds(100));
        }
        cout << "\nTerima kasih telah menggunakan aplikasi ini!\n";
        this_thread::sleep_for(chrono::milliseconds(2000));
        file.close();
    }

    string generateNoReg(const Peserta &p)
    {
        // Gabungkan atribut-atribut peserta menjadi satu string
        string combined = p.nama + p.kategori + to_string(p.umur) + p.jenis_kelamin + p.alamat + p.no_hp;

        // Gunakan std::hash untuk menghasilkan hash dari string gabungan
        size_t hash_value = hash<string>{}(combined);

        // Konversi hash_value menjadi string heksadesimal
        stringstream ss;
        ss << hex << hash_value;
        string hash_str = ss.str();

        // Ambil 5 karakter pertama dari string heksadesimal
        string no_reg = hash_str.substr(0, 5);

        return no_reg;
    }

    void simpanData()
    {
        ofstream file("data_peserta.txt");
        if (!file)
        {
            cerr << "Error: File tidak dapat dibuka!" << endl;
            return;
        }

        for (const auto &p : peserta)
        {
            file << p.second.no_reg << ","
                 << p.second.nama << ","
                 << p.second.kategori << ","
                 << p.second.umur << ","
                 << p.second.jenis_kelamin << ","
                 << p.second.alamat << ","
                 << p.second.no_hp << endl;
        }

        file.close();
    }

    void muatData()
    {
        ifstream file("data_peserta.txt");
        if (!file)
        {
            cerr << "Error: File tidak dapat dibuka!" << endl;
            return;
        }

        peserta.clear();
        string line;
        while (getline(file, line))
        {
            stringstream ss(line);
            string token;
            Peserta p;

            getline(ss, p.no_reg, ',');
            getline(ss, p.nama, ',');
            getline(ss, p.kategori, ',');
            getline(ss, token, ',');
            p.umur = stoi(token);
            getline(ss, token, ',');
            p.jenis_kelamin = token[0];
            getline(ss, p.alamat, ',');
            getline(ss, p.no_hp, ',');

            peserta[p.no_reg] = p;
        }

        file.close();
    }

    void processPesertaQueue()
    {
        int count = 0;
        cout << "\nMemproses antrian peserta...\n";
        cout << "=============================\n";
        while (!pesertaQueue.empty())
        {
            Peserta p = pesertaQueue.front();
            pesertaQueue.pop();
            p.no_reg = generateNoReg(p);
            peserta[p.no_reg] = p;
            simpanData();
            cout << "\nPeserta " << p.nama << " berhasil didaftarkan!";
            count++;
            this_thread::sleep_for(chrono::milliseconds(500));
        }
        cout << "\n\n=============================\n";
        cout << count << " Peserta berhasil didaftarkan!";
        this_thread::sleep_for(chrono::milliseconds(2000));
    }

    void tambahLomba()
    {
        string namaLomba;
        ascii();
        cout << "\nTambah Lomba Baru\n";
        cout << "=================\n";
        cout << "\nNama Lomba: ";
        cin.ignore();
        getline(cin, namaLomba);

        daftarLomba.push_back(namaLomba);
        cout << "\nLomba " << namaLomba << " berhasil ditambahkan!\n";
        this_thread::sleep_for(chrono::milliseconds(2000));

        simpanDaftarLomba();
    }

    void simpanDaftarLomba()
    {
        ofstream file("daftar_lomba.txt");
        if (!file)
        {
            cerr << "Error: Unable to open file for writing" << endl;
            return;
        }

        for (const auto &lomba : daftarLomba)
        {
            file << lomba << endl;
        }

        file.close();
    }

    void muatDaftarLomba()
    {
        ifstream file("daftar_lomba.txt");
        if (!file)
        {
            cerr << "Error: Unable to open file for reading" << endl;
            return;
        }

        daftarLomba.clear();
        string line;
        while (getline(file, line))
        {
            daftarLomba.push_back(line);
        }

        file.close();
    }

public:
    void registrasi()
    {
    start:
        Peserta p;
        int kategori;
        ascii();
        while (true)
        {
            cout << "\n Jumlah antrian peserta: " << pesertaQueue.size() << "\n";
            cout << "\nPilihan Kategori Lomba\n";
            cout << "=====================\n\n";
            for (size_t i = 0; i < daftarLomba.size(); ++i)
            {
                cout << i + 1 << ". Lomba " << daftarLomba[i] << "\n";
            }
            cout << daftarLomba.size() + 1 << ". Kembali\n";
            cout << "\n=====================\n";
            cout << "\nPilihan: ";
            cin >> kategori;
            if (cin.fail() || kategori < 1 || kategori > daftarLomba.size() + 1)
            {
                cin.clear();
                ascii();
                cout << "\nPilihan tidak valid!\n";
            }
            else
            {
                if (kategori == daftarLomba.size() + 1)
                {
                    goto end;
                }
                else
                {
                    ascii();
                    cout << "===========================\n";
                    cout << "\nKategori Lomba: " << daftarLomba[kategori - 1] << "\n";
                    cout << "\n========================\n";
                    cout << "Apakah Benar? (Y/N): ";
                    char choice;
                    cin >> choice;
                    if (toupper(choice) == 'Y')
                    {
                        p.kategori = daftarLomba[kategori - 1];
                    }
                    else
                    {
                        goto start;
                    }
                }
                break;
            }
        }
        ascii();
        while (true)
        {
            cout << "\n Jumlah antrian peserta: " << pesertaQueue.size() << "\n";
            cout << "\nIsi Data Diri Peserta\n";
            cout << "=====================\n";
            cout << "\nNama               : ";
            cin.ignore();
            getline(cin, p.nama);
            while (true)
            {
                cout << "Jenis Kelamin (L/P): ";
                cin >> p.jenis_kelamin;
                if (p.jenis_kelamin == 'L' || p.jenis_kelamin == 'P')
                {
                    break;
                }
                else
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "\033[A\33[2K\r";
                }
            }
            while (true)
            {
                cout << "Umur               : ";
                cin >> p.umur;
                if (cin.fail() || p.umur <= 0)
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "\033[A\33[2K\r";
                }
                else
                {
                    break;
                }
            }
            cout << "Alamat             : ";
            cin.ignore();
            getline(cin, p.alamat);
            cout << "No. HP             : ";
            getline(cin, p.no_hp);

            ascii();
            cout << "\nData Peserta\n";
            cout << "===========================\n";
            cout << "\nNama     : " << p.nama;
            cout << "\nNo. Regis: " << p.no_reg;
            cout << "\nKategori : " << p.kategori;
            cout << "\nUmur     : " << p.umur;
            cout << "\nSex      : " << p.jenis_kelamin;
            cout << "\nAlamat   : " << p.alamat;
            cout << "\nNo. HP   : " << p.no_hp << "\n";
            cout << "\n===========================\n";
            cout << "\nApakah data di atas sudah benar? (Y/N): ";
            char choice;
            cin >> choice;
            if (toupper(choice) == 'Y')
            {
                pesertaQueue.push(p);
            }
            else
            {
                goto start;
            }
            ascii();
            cout << "\nPeserta " << p.nama << " berhasil dimasukkan ke dalam antrian!\n";
            cout << "\n===========================\n";
            cout << "Tambahkan peserta lain? (Y/N): ";
            cin >> choice;
            if (toupper(choice) == 'N')
            {
                ascii();
                processPesertaQueue();
                break;
            }
            else if (toupper(choice) == 'Y')
            {
                ascii();
                goto start;
            }
            else
            {
            end:
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
        }
    }

    void pembatalan()
    {
        if (peserta.empty())
        {
            cout << "\nTidak ada peserta yang terdaftar!\n";
            cout << "================================\n";
            cout << "\nHapus peserta lain? (Y/N): ";
            char pilih;
            cin >> pilih;
            if (toupper(pilih) == 'Y')
            {
                pembatalan();
            }
            else
            {
                return;
            }
        }

        string no_reg_input;
        ascii();
        cout << "\nPembatalan Peserta\n";
        cout << "==================\n";
        cout << "\nNo. Registrasi Peserta yang akan dibatalkan: ";
        cin >> no_reg_input;

        if (peserta.find(no_reg_input) == peserta.end())
        {
            cout << "\nNo. Registrasi tidak ditemukan!\n";
            return;
        }

        ascii();
        cout << "\nData Peserta\n";
        cout << "==============================\n";
        cout << "\nNama     : " << peserta[no_reg_input].nama;
        cout << "\nNo. Regis: " << peserta[no_reg_input].no_reg;
        cout << "\nKategori : " << peserta[no_reg_input].kategori;
        cout << "\nUmur     : " << peserta[no_reg_input].umur;
        cout << "\nSex      : " << peserta[no_reg_input].jenis_kelamin;
        cout << "\nAlamat   : " << peserta[no_reg_input].alamat;
        cout << "\nNo. HP   : " << peserta[no_reg_input].no_hp << "\n";
        cout << "\n==============================\n";

        cout << "Apakah Anda yakin ingin membatalkan peserta ini? (Y/N): ";
        char choice;
        cin >> choice;
        if (toupper(choice) == 'Y')
        {
            pembatalanStack.push(peserta[no_reg_input]);
            peserta.erase(no_reg_input);
            ascii();
            cout << "================================\n";
            cout << "\nPeserta dengan No. Registrasi " << no_reg_input << " berhasil dibatalkan!\n";
            cout << "\nHapus peserta lain? (Y/N): ";
        }
        else
        {
            cout << "\nPembatalan dibatalkan!\n";
            cout << "\nHapus peserta lain? (Y/N): ";
        }
        char pilih;
        cin >> pilih;
        if (toupper(pilih) == 'Y')
        {
            pembatalan();
        }
        else
        {
            return;
        }
    }

    void list()
    {
        int totalPeserta = peserta.size();
        int pesertaPerHalaman = 10;
        int totalHalaman = (totalPeserta + pesertaPerHalaman - 1) / pesertaPerHalaman;
        int halaman = 1;
        int sorts = 0;

        if (peserta.empty())
        {
            cout << "\nTidak ada peserta yang terdaftar!\n";
            return;
        }

        vector<Peserta> listPeserta;
        listPeserta.reserve(totalPeserta);
        for (const auto &p : peserta)
        {
            listPeserta.push_back(p.second);
        }

    start:
        if (sorts == 1)
        {
            // sort berdasarkan nama ascending
            sort(listPeserta.begin(), listPeserta.end(), [](const Peserta &a, const Peserta &b)
                 {
                // Convert to lowercase copies
                string an = a.nama;
                string bn = b.nama;
                transform(an.begin(), an.end(), an.begin(), ::tolower);
                transform(bn.begin(), bn.end(), bn.begin(), ::tolower);
            
                // Compare the lowercase copies
                return an > bn; });
        }
        else if (sorts == 2)
        {
            // sort berdasarkan nama descending
            sort(listPeserta.begin(), listPeserta.end(), [](const Peserta &a, const Peserta &b)
                 {
                // Convert to lowercase copies
                string an = a.nama;
                string bn = b.nama;
                transform(an.begin(), an.end(), an.begin(), ::tolower);
                transform(bn.begin(), bn.end(), bn.begin(), ::tolower);
            
                // Compare the lowercase copies
                return an < bn; });
        }
        else if (sorts == 3)
        {
            // sort berdasarkan kategori
            sort(listPeserta.begin(), listPeserta.end(), [](const Peserta &a, const Peserta &b)
                 { return a.kategori < b.kategori; });
        }
        else if (sorts == 4)
        {
            // sort berdasarkan kategori
            sort(listPeserta.begin(), listPeserta.end(), [](const Peserta &a, const Peserta &b)
                 { return a.kategori > b.kategori; });
        }

        {
            char choice;
            ascii();
            cout << "\nDatabase Peserta (Halaman " << halaman << " dari " << totalHalaman << ") - " << totalPeserta << " Peserta\n";
            cout << "==============================================================================================\n";
            cout << left << setw(5) << "No." << "| " << setw(20) << "Nama" << "| " << setw(10) << "Kategori" << "| " << setw(5) << "Umur" << "|" << setw(10) << "Sex" << "|" << setw(21) << "Alamat" << "|" << setw(15) << "No. HP" << endl;
            cout << "==============================================================================================\n";

            int start = (halaman - 1) * pesertaPerHalaman;
            int end = min(start + pesertaPerHalaman, totalPeserta);

            for (int i = start; i < end; ++i)
            {
                cout << left << setw(5) << i + 1 << "| "
                     << setw(20) << listPeserta[i].nama << "| "
                     << setw(10) << listPeserta[i].kategori << "| "
                     << setw(5) << listPeserta[i].umur << "|"
                     << setw(10) << listPeserta[i].jenis_kelamin << "|"
                     << setw(21) << listPeserta[i].alamat << "|" << setw(15)
                     << listPeserta[i].no_hp << endl;
            }
            cout << "==============================================================================================\n";
            cout << "\n Menu\n";
            cout << "====================\n";
            cout << "\n1. Halaman Selanjutnya\n2. Halaman Sebelumnya\n3. Hapus Peserta\n4. Atur Filter\n5. Kembali\n";
            cout << "\n====================\n";
            cout << "\nPilihan: ";
            cin >> choice;
            if (toupper(choice) == '1')
            {
                if (halaman < totalHalaman)
                {
                    halaman++;
                    goto start;
                }
                else
                {
                    goto start;
                }
            }
            else if (toupper(choice) == '2')
            {
                if (halaman > 1)
                {
                    halaman--;
                    goto start;
                }
                else
                {
                    goto start;
                }
            }
            else if (toupper(choice) == '3')
            {
                pembatalan();
            }
            else if (toupper(choice) == '4')
            {
                ascii();
                cout << "\nAtur Filter\n";
                cout << "====================\n";
                cout << "\n1. Sort berdasarkan Nama Ascending\n2. Sort berdasarkan Nama Descending\n3. Sort berdasarkan Kategori Ascending\n4. Sort berdasarkan Kategori Descending\n5. Kembali\n";
                cout << "\n====================\n";
                cout << "\nPilihan: ";
                cin >> choice;
                if (toupper(choice) == '1')
                {
                    sorts = 1;
                    goto start;
                }
                else if (toupper(choice) == '2')
                {
                    sorts = 2;
                    goto start;
                }
                else if (toupper(choice) == '3')
                {
                    sorts = 3;
                    goto start;
                }
                else if (toupper(choice) == '4')
                {
                    sorts = 4;
                    goto start;
                }
                else if (toupper(choice) == '5')
                {
                    goto start;
                }
                else
                {
                    cout << "\nPilihan tidak valid!\n";
                    goto start;
                }
            }
            else if (toupper(choice) == '5')
            {
                return;
            }
            else
            {
                cout << "\nPilihan tidak valid!\n";
                goto start;
            }
        }
    }

    void cari()
    {
        if (peserta.empty())
        {
            cout << "\nTidak ada peserta yang terdaftar!\n";
            return;
        }

        ascii();
        string no_reg_input;
        cout << "\nPencarian Peserta\n";
        cout << "=================\n";
        cout << "\nNo. Registrasi Peserta: ";
        cin >> no_reg_input;

        if (peserta.find(no_reg_input) == peserta.end())
        {
            ascii();
            cout << "\nNo. Registrasi tidak ditemukan!\n";
            cout << "================================\n";
            goto end;
            return;
        }

        ascii();
        cout << "\nData Peserta\n";
        cout << "==============================\n";
        cout << "\nNama     : " << peserta[no_reg_input].nama;
        cout << "\nNo. Regis: " << peserta[no_reg_input].no_reg;
        cout << "\nKategori : " << peserta[no_reg_input].kategori;
        cout << "\nUmur     : " << peserta[no_reg_input].umur;
        cout << "\nSex      : " << peserta[no_reg_input].jenis_kelamin;
        cout << "\nAlamat   : " << peserta[no_reg_input].alamat;
        cout << "\nNo. HP   : " << peserta[no_reg_input].no_hp << "\n";
        cout << "\n==============================\n";

    end:
        cout << "\nCari Peserta Lain? (Y/N): ";
        char choice;
        cin >> choice;
        if (toupper(choice) == 'Y')
        {
            cari();
        }
        else
        {
            return;
        }
    }

    void run()
    {
        muatData();
        muatDaftarLomba();
        int choice;
        do
        {
        start:
            ascii();
            cout << "\nMenu Utama\n";
            cout << "====================\n";
            cout << "\n1. Daftar Peserta\n";
            cout << "2. Hapus Peserta\n";
            cout << "3. Cari Peserta\n";
            cout << "4. List Peserta\n";
            cout << "5. Tambah Lomba\n";
            cout << "6. Keluar\n";
            cout << "\n=================\n";
            cout << "\nPilihan: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
                registrasi();
                break;
            case 2:
                pembatalan();
                break;
            case 3:
                cari();
                break;
            case 4:
                list();
                break;
            case 5:
                tambahLomba();
                break;
            case 6:
                ascii_thanks();
                break;
            default:
                goto start;
            }
        } while (choice != 6);
    }
};

int main()
{
    Lomba lomba;
    lomba.run();
    return 0;
}