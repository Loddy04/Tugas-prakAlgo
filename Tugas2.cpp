#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

struct Pesanan {
    string nama;
    string jenisRoti;
    int totalHarga;
    Pesanan* next;
};

Pesanan* headAntrean = NULL;
Pesanan* tailAntrean = NULL;
Pesanan* headRiwayat = NULL;
int jumlahAntrean = 0;
int jumlahRiwayat = 0;

void ambilAntrean() {
    string nama, jenisRoti;
    int totalHarga;

    cin.ignore();
    cout << "\n=== AMBIL ANTREAN ===" << endl;
    cout << "Nama Pembeli    : ";
    getline(cin, nama);
    cout << "Jenis Roti      : ";
    getline(cin, jenisRoti);
    cout << "Total Harga (Rp): ";
    cin >> totalHarga;

    Pesanan* pesananBaru = new Pesanan{nama, jenisRoti, totalHarga, NULL};

    if (headAntrean == NULL) {
        headAntrean = tailAntrean = pesananBaru;
    } else {
        tailAntrean->next = pesananBaru;
        tailAntrean = pesananBaru;
    }

    jumlahAntrean++;
    cout << "\nPesanan " << nama << " berhasil ditambahkan ke antrean!" << endl;
    cout << "Nomor Antrean: " << jumlahAntrean << endl;
}

void layaniPembeli() {
    if (headAntrean == NULL) {
        cout << "\nAntrean kosong. Tidak ada pesanan yang dapat dilayani." << endl;
        return;
    }

    Pesanan* pesananDilayani = headAntrean;
    headAntrean = headAntrean->next;
    if (headAntrean == NULL) {
        tailAntrean = NULL;
    }

    pesananDilayani->next = headRiwayat;
    headRiwayat = pesananDilayani;

    jumlahAntrean--;
    jumlahRiwayat++;

    cout << "\n=== LAYANI PEMBELI ===" << endl;
    cout << "Pesanan atas nama " << pesananDilayani->nama << " telah dilayani." << endl;
    cout << "Jenis Roti    : " << pesananDilayani->jenisRoti << endl;
    cout << "Total Harga   : Rp" << pesananDilayani->totalHarga << endl;
}

void tampilkanPesanan() {
    if (headAntrean == NULL) {
        cout << "\nAntrean kosong. Tidak ada pesanan yang dapat ditampilkan." << endl;
        return;
    }

    cout << "\n=== DAFTAR ANTREAN PESANAN ===" << endl;
    cout << "Jumlah antrean: " << jumlahAntrean << endl;
    cout << "-----------------------------------------------" << endl;
    cout << "No. Nama Pembeli      Jenis Roti       Total Harga" << endl;
    cout << "-----------------------------------------------" << endl;

    Pesanan* current = headAntrean;
    int nomor = 1;

    while (current != NULL) {
        cout << nomor << ". " << current->nama << " - " << current->jenisRoti
             << " - Rp" << current->totalHarga << endl;
        current = current->next;
        nomor++;
    }
}

void batalkanPesanan() {
    if (headAntrean == NULL) {
        cout << "\nAntrean kosong. Tidak ada pesanan yang dapat dibatalkan." << endl;
        return;
    }

    if (headAntrean == tailAntrean) {
        cout << "\nPesanan atas nama " << headAntrean->nama << " telah dibatalkan." << endl;
        delete headAntrean;
        headAntrean = tailAntrean = NULL;
    } else {
        Pesanan* current = headAntrean;
        while (current->next != tailAntrean) {
            current = current->next;
        }

        cout << "\nPesanan atas nama " << tailAntrean->nama << " telah dibatalkan." << endl;
        delete tailAntrean;
        tailAntrean = current;
        tailAntrean->next = NULL;
    }

    jumlahAntrean--;
}

void tampilkanHistoryPesanan() {
    if (headRiwayat == NULL) {
        cout << "\nBelum ada riwayat pesanan." << endl;
        return;
    }

    cout << "\n=== RIWAYAT PESANAN ===" << endl;
    cout << "Jumlah pesanan yang telah dilayani: " << jumlahRiwayat << endl;
    cout << "-----------------------------------------------" << endl;
    cout << "No. Nama Pembeli - Jenis Roti - Total Harga" << endl;
    cout << "-----------------------------------------------" << endl;

    Pesanan* current = headRiwayat;
    int nomor = 1;

    while (current != NULL) {
        cout << nomor << ". " << current->nama << " - "
             << current->jenisRoti << " - Rp" << current->totalHarga << endl;
        current = current->next;
        nomor++;
    }

    cout << "-----------------------------------------------" << endl;
}

void tampilkanMenu() {
    cout << "\n=== SISTEM ANTREAN TOKO ROTI ===" << endl;
    cout << "1. Ambil Antrean" << endl;
    cout << "2. Layani Pembeli" << endl;
    cout << "3. Tampilkan Antrean Pesanan" << endl;
    cout << "4. Batalkan Pesanan Terakhir" << endl;
    cout << "5. Tampilkan Riwayat Pesanan" << endl;
    cout << "0. Keluar" << endl;
    cout << "Pilihan: ";
}

int main() {
    int pilihan;

    do {
        tampilkanMenu();
        cin >> pilihan;

        switch (pilihan) {
            case 1: ambilAntrean(); break;
            case 2: layaniPembeli(); break;
            case 3: tampilkanPesanan(); break;
            case 4: batalkanPesanan(); break;
            case 5: tampilkanHistoryPesanan(); break;
            case 0:
                cout << "\nTerima kasih telah menggunakan Sistem Antrean Toko Roti Manis Lezat!" << endl;
                break;
            default:
                cout << "\nPilihan tidak valid. Silakan coba lagi." << endl;
        }

    } while (pilihan != 0);

    return 0;
}
