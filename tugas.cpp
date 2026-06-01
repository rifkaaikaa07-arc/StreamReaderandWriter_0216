#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <limits>
using namespace std;

// 1. Struktur Kelas TokoElektronik
class TokoElektronik {
private:
    // Atribut Private: Array berkapasitas 3 elemen (Enkapsulasi)
    array<string, 3> etalase;

public:
    // Constructor: Mengisi data awal ke dalam array secara otomatis
    TokoElektronik() {
        etalase[0] = "Laptop Gaming";
        etalase[1] = "Keyboard Mekanikal";
        etalase[2] = "Mouse Wireless";
    }

    // Method Public: Mengambil produk berdasarkan nomor rak
    string ambilProduk(size_t nomorRak) {
        try {
            // Wajib menggunakan fungsi .at()
            return etalase.at(nomorRak);
        } 
        // Menangkap error bawaan out_of_range
        catch (const out_of_range& e) {
            // Melemparkan kembali (throw) pesan error kustom
            string errorMsg = "Gagal Mengambil Barang : Rak nomor " + to_string(nomorRak) + " kosong atau tidak tersedia!";
            throw runtime_error(errorMsg);
        }
    }
};

// 3. Fitur Manajemen Gudang Berbasis File (File I/O & CRUD)
class ManajemenGudang {
private:
    const string namaFile = "gudang.txt";

public:
    // CREATE: Menambahkan data barang baru
    void tambahBarang(const string& namaBarang) {
        ofstream file(namaFile, ios::app); // Mode append
        if (file.is_open()) {
            file << namaBarang << "\n";
            file.close();
            cout << "[+] Barang berhasil ditambahkan ke gudang.\n";
        } else {
            cout << "[!] Gagal membuka file gudang.txt\n";
        }
    }

    // READ: Membaca dan menampilkan daftar barang
    void tampilkanGudang() {
        ifstream file(namaFile);
        cout << "\n=== Daftar Barang di Gudang (gudang.txt) ===\n";
        if (!file.is_open() || file.peek() == ifstream::traits_type::eof()) {
            cout << "  (Gudang kosong atau file belum terbuat)\n";
            return;
        }

        string baris;
        int id = 1;
        while (getline(file, baris)) {
            cout << "  " << id++ << ". " << baris << "\n";
        }
        file.close();
    }

    // UPDATE: Memperbarui data barang tertentu
    void perbaruiBarang(int idTarget, const string& namaBarangBaru) {
        vector<string> daftarBarang;
        ifstream fileIn(namaFile);
        string baris;
        bool ditemukan = false;

        if (fileIn.is_open()) {
            while (getline(fileIn, baris)) {
                daftarBarang.push_back(baris);
            }
            fileIn.close();
        }

        if (idTarget > 0 && idTarget <= (int)daftarBarang.size()) {
            daftarBarang[idTarget - 1] = namaBarangBaru;
            ditemukan = true;
        }

        if (ditemukan) {
            ofstream fileOut(namaFile);
            for (const auto& barang : daftarBarang) {
                fileOut << barang << "\n";
            }
            fileOut.close();
            cout << "[+] Barang berhasil diperbarui.\n";
        } else {
            cout << "[!] Gagal Update: ID Barang tidak ditemukan!\n";
        }
    }

    // DELETE: Menghapus data barang
    void hapusBarang(int idTarget) {
        vector<string> daftarBarang;
        ifstream fileIn(namaFile);
        string baris;
        bool ditemukan = false;

        if (fileIn.is_open()) {
            while (getline(fileIn, baris)) {
                daftarBarang.push_back(baris);
            }
            fileIn.close();
        }

        if (idTarget > 0 && idTarget <= (int)daftarBarang.size()) {
            daftarBarang.erase(daftarBarang.begin() + idTarget - 1);
            ditemukan = true;
        }

        if (ditemukan) {
            ofstream fileOut(namaFile);
            for (const auto& barang : daftarBarang) {
                fileOut << barang << "\n";
            }
            fileOut.close();
            cout << "[-] Barang berhasil dihapus.\n";
        } else {
            cout << "[!] Gagal Hapus: ID Barang tidak ditemukan!\n";
        }
    }
};

// Fungsi bantuan untuk membersihkan input buffer (mencegah infinite loop bila salah input)
void bersihkanBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// 4. Pengujian pada Fungsi main()
int main() {
    TokoElektronik etalaseToko;
    ManajemenGudang gudang;
    int pilihan;
    string inputNama;
    int inputId;

    // Looping sistem Menu Utama interaktif (CLI)
    do {
        // Read: Tampil otomatis saat menu dibuka
        gudang.tampilkanGudang();

        cout << "\n=== MENU UTAMA TOKO ELEKTRONIK GIBRAN JAYA ===\n";
        cout << "1. Tambah Barang Baru (Create)\n";
        cout << "2. Update Barang (Update)\n";
        cout << "3. Hapus Barang (Delete)\n";
        cout << "4. Simulasi Etalase (Test Exception Handling)\n";
        cout << "0. Keluar\n";
        cout << "Pilih menu (0-4): ";
        cin >> pilihan;

        if (cin.fail()) {
            cout << "\n[!] Input tidak valid. Masukkan angka.\n";
            bersihkanBuffer();
            continue;
        }
        bersihkanBuffer();

        switch (pilihan) {
            case 1:
                cout << "\nMasukkan nama barang baru: ";
                getline(cin, inputNama);
                gudang.tambahBarang(inputNama);
                break;

            case 2:
                cout << "\nMasukkan ID barang yang akan diupdate: ";
                cin >> inputId;
                bersihkanBuffer();
                cout << "Masukkan nama barang pengganti: ";
                getline(cin, inputNama);
                gudang.perbaruiBarang(inputId, inputNama);
                break;

            case 3:
                cout << "\nMasukkan ID barang yang akan dihapus: ";
                cin >> inputId;
                bersihkanBuffer();
                gudang.hapusBarang(inputId);
                break;

            case 4:
                cout << "\n--- SIMULASI ETALASE ---\n";
                // Skenario 1: Sukses (indeks 1 = elemen kedua "Keyboard Mekanikal")
                cout << "> Skenario 1: Pengambilan rak indeks ke-1\n";
                try {
                    string barang = etalaseToko.ambilProduk(1);
                    cout << "  Sukses! Anda mengambil: " << barang << "\n";
                } catch (const exception& e) {
                    cout << "  Error: " << e.what() << "\n";
                }

                // Skenario 2: Gagal (Custom Error)
                cout << "\n> Skenario 2: Pengambilan rak indeks ke-5\n";
                try {
                    string barang = etalaseToko.ambilProduk(5);
                    cout << "  Sukses! Anda mengambil: " << barang << "\n";
                } catch (const exception& e) {
                    // Penangkapan hasil re-throw kustom dan program tetap lanjut
                    cout << "  Tertangkap Exception: " << e.what() << "\n";
                }
                
                cout << "\nTekan Enter untuk kembali ke menu...";
                cin.get();
                break;

            case 0:
                cout << "\nTerima kasih telah menggunakan sistem manajemen Gibran Jaya.\n";
                break;

            default:
                cout << "\n[!] Pilihan tidak valid.\n";
                break;
        }
    } while (pilihan != 0);

    return 0;
}