#include <iostream>
using namespace std;

struct mahasiswa {
    string nama;
    int nim;
    float ipk;
} mhs [10] = {
    {"Akali", 124240120, 3.9},
    {"Garen", 124240116, 2.9},
    {"Heimdal", 124240118, 4},
    {"Katarina", 124240113, 3.6},
    {"Lux", 124240111, 3},
    {"Omen", 124240119, 2.8},
    {"Paimon", 124240117, 3.8},
    {"Vector", 124240112, 3.7},
    {"Yasuo", 124240115, 3.3},
    {"Zed", 124240114, 3.4}
};
int n = sizeof(mhs) / sizeof(mhs[0]);
mahasiswa *ptr = mhs;

void tampilmhs(){
    for (int i = 0; i < n; i++){
        cout <<"Nama = "<<(*(ptr + i)).nama<<", Nim = "<<(*(ptr + i)).nim<<", IPK = "<<(*(ptr + i)).ipk;
        cout << endl;
    }
}

void carinimmhs(){
    int carinim;
    bool ditemukan = false;
    cout << "Masukan nim yang ingin dicari : "; cin >> carinim;
    for(int i = 0; i < n; i++){
        if(mhs[i].nim == carinim){
            cout << "Data mahasiswa ditemukan!" << endl;
            cout << "Nama   : "<< mhs[i].nama << endl;
            cout << "Nim    : "<< mhs[i].nim << endl;
            cout << "IPK    : "<< mhs[i].ipk << endl;
        ditemukan = true;
        break;
        } 
    } if (!ditemukan) {
        cout << "Data mahasiswa tidak ditemukan" << endl;
    }
}
void carinamamhs(){
    int i, j, t;
    string x;  
    bool ditemukan = false;

    cout << "Nama yang dicari = ";
    cin >> x; 

    i = 0;  
    j = 9;  

    while((!ditemukan) && (i <= j)) { 
        t = (i + j) / 2;

    if(mhs[t].nama == x) {
        ditemukan = true;  
    } else if (x < mhs[t].nama) {
        j = t - 1;  
    } else {
        i = t + 1; 
    }
    }
    if(ditemukan) {
        cout << "Data mahasiswa ditemukan!" << endl;
        cout << "Nama   : " << mhs[t].nama << endl;  
        cout << "NIM    : " << mhs[t].nim << endl;
        cout << "IPK    : " << mhs[t].ipk << endl;
    } else {
        cout << "Data mahasiswa tidak ditemukan" << endl;
    }
}

void sortipkmhsasc(int awal, int akhir) {
    int low = awal, high = akhir;
    float pivot = mhs[(awal + akhir) / 2].ipk; 

    do {
        while (mhs[low].ipk < pivot) low++;
        while (mhs[high].ipk > pivot) high--;

        if (low <= high) {
            swap(mhs[low], mhs[high]);
            low++;
            high--;
        }
    } while (low <= high);

    if (awal < high) sortipkmhsasc(awal, high);
    if (low < akhir) sortipkmhsasc(low, akhir);

    if (awal == 0 && akhir == n - 1) {
        for (int i = 0; i < n; i++) {
            cout << "Name: " << mhs[i].nama << ", NIM: " << mhs[i].nim << ", IPK: " << mhs[i].ipk << endl;
        }
    }
}

void sortipkmhsdesc(){
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (mhs[j].ipk < mhs[j + 1].ipk) {  
                swap(mhs[j], mhs[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;  
    } for (int i = 0; i < n; i++) {
        cout << "Name: " << mhs[i].nama << ", NIM: " << mhs[i].nim << ", IPK: " << mhs[i].ipk << endl;
    }
}

int main(){
    int pilmenu;
    system ("cls");
    while (true){
        cout << "----------------------------\n";
        cout << "            MENU\n";
        cout << "----------------------------\n";
        cout << "1. Tampilkan mahasiswa\n";
        cout << "2. Cari berdasarkan nim\n";
        cout << "3. Cari berdasarkan nama\n";
        cout << "4. Sort IPK mahasiswa (asc)\n";
        cout << "5. Sort IPK mahasiswa (desc)\n";
        cout << "6. Keluar Dari Program\n";
        cout << "-----------------------------\n";
        cout << "Pilih menu = ";
        cin >> pilmenu;
        cout << endl;

        switch (pilmenu){
            case 1:
                tampilmhs();
                system("pause");
                break;

            case 2:
                carinimmhs();
                system("pause");
                break;

            case 3:
                carinamamhs();
                system("pause");
                break;

            case 4:
                sortipkmhsasc(0, n - 1);
                cout << "Mahasiswa telah diurutkan berdasarkan IPK (ascending)" << endl;
                system("pause");
                break;

            case 5:
                sortipkmhsdesc();
                cout << "Mahasiswa telah diurutkan berdasarkan IPK (descending)" << endl;
                system("pause");
                break;

            case 6:
                exit(1);

            default:
                cout << "Tidak ada dalam pilihan!" << endl;
                system("pause");
                break;
        }
    }
}
