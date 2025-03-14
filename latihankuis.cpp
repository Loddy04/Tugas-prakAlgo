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
    
    Mahasiswa mahasiswa[100];
    int count = 0;
    while (fread(&mahasiswa[count], sizeof(Mahasiswa), 1, file)) {
        count++;
    }
    fclose(file);
    
    bubbleSort(mahasiswa, count);
    
    cout << "\nDaftar Mahasiswa (Diurutkan Berdasarkan NIM):\n\n";
    for (int i = 0; i < count; i++) {
        cout << "Nama     : " << mahasiswa[i].nama
             << "\nNIM      : " << mahasiswa[i].nim
             << "\nJurusan  : " << mahasiswa[i].jurusan
             << "\nAngkatan : " << mahasiswa[i].angkatan
             << "\nIPK      : " << mahasiswa[i].ipk
             << "\n------------------------\n";
    }
}

int partition(Mahasiswa arr[], int low, int high) {
    int pivot = arr[high].nim;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j].nim < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(Mahasiswa arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int binarySearch(Mahasiswa arr[], int left, int right, int nim) {
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid].nim == nim) return mid;
        if (arr[mid].nim < nim) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

void carinimmhs() {
    FILE *file = fopen("MahasiswaData.dat", "rb");
    if (!file) {
        cout << "Tidak ada data mahasiswa.\n";
        return;
    }
    
    Mahasiswa mahasiswa[100];
    int count = 0;
    while (fread(&mahasiswa[count], sizeof(Mahasiswa), 1, file)) {
        count++;
    }
    fclose(file);
    
    quickSort(mahasiswa, 0, count - 1);
    
    int nim;
    cout << "Masukkan NIM yang dicari: ";
    cin >> nim;
    
    int index = binarySearch(mahasiswa, 0, count - 1, nim);
    if (index != -1) {
        cout << "\nMahasiswa ditemukan!\nNama     : " << mahasiswa[index].nama
             << "\nJurusan  : " << mahasiswa[index].jurusan
             << "\nAngkatan : " << mahasiswa[index].angkatan
             << "\nIPK      : " << mahasiswa[index].ipk
             << "\n--------------------\n";
    } else {
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
            cout << "\nMahasiswa ditemukan!\nNama     : " << mhs.nama
                 << "\nJurusan  : " << mhs.jurusan
                 << "\nAngkatan : " << mhs.angkatan
                 << "\nIPK      : " << mhs.ipk
                 << "\n--------------------\n";
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
