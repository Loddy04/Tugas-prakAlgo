#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
using namespace std;

struct Mahasiswa {
    char nama[50];
    char jurusan[50];
    int nim, angkatan;
    float ipk;
}; Mahasiswa mhs;

void tambahmhs() {
    FILE *file = fopen("MahasiswaData.dat", "ab");
    if (!file) {
        cout << "Gagal membuka file!\n";
        return;
    }
    cout << "      INPUT DATA";
    cout << "\n=======================";
    cout << "\nMasukkan Nama: "; cin.ignore(); cin.getline(mhs.nama, 50);
    cout << "Masukkan NIM: "; cin >> mhs.nim; cin.ignore();
    cout << "Masukkan Jurusan: "; cin.getline(mhs.jurusan, 50);
    cout << "Masukkan Angkatan: "; cin >> mhs.angkatan;
    cout << "=======================";
    mhs.ipk = 0.0;

    fwrite(&mhs, sizeof(Mahasiswa), 1, file);
    fclose(file);

    cout << "\nData berhasil disimpan!\n\n";
}

void tampilmhs() {
    FILE *file = fopen("MahasiswaData.dat", "rb");
    if (!file) {
        cout << "Tidak ada data mahasiswa.\n";
        return;
    }

    Mahasiswa mhs;
    cout << "\nDaftar Mahasiswa:\n\n";
    while (fread(&mhs, sizeof(Mahasiswa), 1, file)) {
            cout << "Nama     : " << mhs.nama; 
            cout << "\nNIM      : " << mhs.nim;
            cout << "\nJurusan  : " << mhs.jurusan; 
            cout << "\nAngkatan : " << mhs.angkatan; 
            cout << "\nIPK      : " << mhs.ipk; 
            cout << "\n------------------------\n";
    }
    fclose(file);
}

void carinimmhs() {
    FILE *file = fopen("MahasiswaData.dat", "rb");
    if (!file) {
        cout << "Tidak ada data mahasiswa.\n";
        cout << "-------------------------\n";
        return;
    }

    int nim;
    cout << "Masukkan NIM yang dicari: ";
    cin >> nim;

    Mahasiswa mhs;
    bool found = false;
    while (fread(&mhs, sizeof(Mahasiswa), 1, file)) {
        if (mhs.nim == nim) {
            cout << "\nMahasiswa ditemukan!";
            cout << "\n--------------------";
            cout << "\nNama     :"  << mhs.nama; 
            cout << "\nJurusan  : " << mhs.jurusan; 
            cout << "\nAngkatan : " << mhs.angkatan; 
            cout << "\nIPK      : " << mhs.ipk;
            cout << "\n--------------------\n";
            found = true;
            break;
        }
    }
    fclose(file);

    if (!found) {
        cout << "Mahasiswa dengan NIM " << nim << " tidak ditemukan!\n";
    }
}

void carijurusanmhs() {
    FILE *file = fopen("MahasiswaData.dat", "rb");
    if (!file) {
        cout << "Tidak ada data mahasiswa.\n";
        return;
    }

    char jurusan[50];
    cout << "Masukkan jurusan yang dicari: ";
    cin.ignore();
    cin.getline(jurusan, 50);

    Mahasiswa mhs;
    bool found = false;
    while (fread(&mhs, sizeof(Mahasiswa), 1, file)) {
        if (strcmp(mhs.jurusan, jurusan) == 0) {
            cout << "\nMahasiswa ditemukan!";
            cout << "\nNama     :"  << mhs.nama; 
            cout << "\nJurusan  : " << mhs.jurusan; 
            cout << "\nAngkatan : " << mhs.angkatan; 
            cout << "\nIPK      : " << mhs.ipk;
            cout << "\n";
            found = true;
        }
    }
    fclose(file);

    if (!found) cout << "Mahasiswa dengan jurusan " << jurusan << " tidak ditemukan!\n";
}

void gantiipk() {
    FILE *file = fopen("MahasiswaData.dat", "rb");
    if (!file) {
        cout << "Tidak ada data mahasiswa.\n";
        return;
    }

    FILE *tempFile = fopen("TempData.dat", "wb");
    if (!tempFile) {
        cout << "Gagal membuat file sementara!\n";
        fclose(file);
        return;
    }

    int nim;
    cout << "Masukkan NIM yang IPK-nya ingin diperbarui: ";
    cin >> nim;

    Mahasiswa mhs;
    bool found = false;
    while (fread(&mhs, sizeof(Mahasiswa), 1, file)) {
        if (mhs.nim == nim) {
            found = true;
            cout << "Masukkan IPK baru: ";
            float ipkBaru;
            cin >> ipkBaru;

            if (ipkBaru < 0.0 || ipkBaru > 4.0) {
                cout << "IPK harus dalam rentang 0.0 - 4.0!\n";
            } else {
                mhs.ipk = ipkBaru;
                cout << "IPK berhasil diperbarui!\n";
            }
        }
        fwrite(&mhs, sizeof(Mahasiswa), 1, tempFile);
    }

    fclose(file);
    fclose(tempFile);
    remove("MahasiswaData.dat");
    rename("TempData.dat", "MahasiswaData.dat");

    if (!found) cout << "Mahasiswa tidak ditemukan!\n";
}

void hapusmhs() {
    FILE *file = fopen("MahasiswaData.dat", "rb");
    if (!file) {
        cout << "Tidak ada data mahasiswa untuk dihapus.\n";
        return;
    }

    FILE *tempFile = fopen("TempData.dat", "wb");
    int nim;
    cout << "Masukkan NIM mahasiswa yang akan dihapus: ";
    cin >> nim;

    Mahasiswa mhs;
    bool found = false;
    while (fread(&mhs, sizeof(Mahasiswa), 1, file)) {
        if (mhs.nim == nim) {
            found = true;
        } else {
            fwrite(&mhs, sizeof(Mahasiswa), 1, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);
    remove("MahasiswaData.dat");
    rename("TempData.dat", "MahasiswaData.dat");

    if (found) {
        cout << "\nMahasiswa dengan NIM " << nim << " berhasil dihapus!";
        cout << "\n-----------------------------------------------\n";
    } else {
        cout << "\nMahasiswa tidak ditemukan!";
        cout << "\n-----------------------------------------------\n";
    }
}

int main() {
    int pilihan;
    do {
        cout <<"\n============================="<<endl;
        cout <<"       Smart Campus ++"<<endl;
        cout <<"============================="<<endl;
        cout <<"1. Tambah Mahasiswa"<<endl;
        cout <<"2. Tampil Mahasiswa"<<endl;
        cout <<"3. Cari Nama Berdasar Nim"<<endl;
        cout <<"4. Cari Nama Berdasar Jurusan"<<endl;
        cout <<"5. Perbarui IPK"<<endl;
        cout <<"6. Hapus Mahasiswa"<<endl;
        cout <<"7. Keluar"<<endl;
        cout <<"============================="<<endl;
        cout <<"Pilih : "; cin >> pilihan;
        cout << endl;

        switch (pilihan) {
            case 1: tambahmhs();
            system("pause"); 
            break;

            case 2: tampilmhs();
            system("pause"); 
            break;

            case 3: carinimmhs();
            system("pause"); 
            break;

            case 4: carijurusanmhs();
            system("pause"); 
            break;

            case 5: gantiipk();
            system("pause"); 
            break;

            case 6: hapusmhs();
            system("pause"); 
            break;

            case 7: cout << "Terima kasih!\n"; break;
        }
    } while (pilihan != 7);
}