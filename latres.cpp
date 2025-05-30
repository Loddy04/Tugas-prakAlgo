#include <iostream>
#include <string>
using namespace std;

// Struktur Video terhubung dalam BST
struct Video {
    string judul;
    int durasi;
    string status;
    Video* left = nullptr;
    Video* right = nullptr;
};

// Node untuk playlist
struct PlaylistNode {
    Video* video;
    PlaylistNode* next = nullptr;
};

// Node untuk riwayat tontonan
struct RiwayatNode {
    Video* video;
    RiwayatNode* next = nullptr;
};

// Node untuk Undo stack
struct UndoNode {
    string aksi;
    Video* video;  // Menyimpan salinan video, bukan pointer asli
    UndoNode* next = nullptr;
};

// Global pointers
Video* root = nullptr;
PlaylistNode* headPlaylist = nullptr;
RiwayatNode* topRiwayat = nullptr;
UndoNode* topUndo = nullptr;

// Fungsi untuk membuat video baru
Video* buatVideo(string judul, int durasi, string status = "tersedia") {
    Video* v = new Video{judul, durasi, status};
    return v;
}

// Fungsi untuk menyalin video
Video* salinVideo(Video* original) {
    if (!original) return nullptr;
    return buatVideo(original->judul, original->durasi, original->status);
}

// Fungsi untuk memasukkan video ke BST
Video* insertBST(Video* node, Video* baru) {
    if (!node) return baru;
    if (baru->judul < node->judul)
        node->left = insertBST(node->left, baru);
    else if (baru->judul > node->judul)
        node->right = insertBST(node->right, baru);
    return node;
}

// Fungsi untuk mencari video dalam BST
Video* cariVideo(Video* node, string judul) {
    if (!node) return nullptr;
    if (node->judul == judul) return node;
    if (judul < node->judul) return cariVideo(node->left, judul);
    return cariVideo(node->right, judul);
}

// Fungsi untuk menampilkan video secara in-order
void tampilInOrder(Video* node) {
    if (!node) return;
    tampilInOrder(node->left);
    cout << "Judul: " << node->judul << ", Durasi: " << node->durasi 
         << " menit, Status: " << node->status << endl;
    tampilInOrder(node->right);
}

// Fungsi untuk menemukan video dengan judul terkecil
Video* findMin(Video* node) {
    while (node && node->left) node = node->left;
    return node;
}

// Fungsi untuk menghapus video dari BST
Video* deleteBST(Video* node, string judul) {
    if (!node) return nullptr;
    if (judul < node->judul)
        node->left = deleteBST(node->left, judul);
    else if (judul > node->judul)
        node->right = deleteBST(node->right, judul);
    else {
        if (!node->left) {
            Video* temp = node->right;
            delete node;
            return temp;
        } else if (!node->right) {
            Video* temp = node->left;
            delete node;
            return temp;
        } else {
            Video* temp = findMin(node->right);
            node->judul = temp->judul;
            node->durasi = temp->durasi;
            node->status = temp->status;
            node->right = deleteBST(node->right, temp->judul);
        }
    }
    return node;
}

// Fungsi untuk menambah video baru
void tambahVideo() {
    string judul;
    int durasi;
    cout << "Masukkan judul video: "; 
    cin.ignore();
    getline(cin, judul);
    
    if (cariVideo(root, judul)) {
        cout << "Judul sudah ada!\n";
        return;
    }
    
    cout << "Masukkan durasi (menit): "; 
    cin >> durasi;
    
    Video* baru = buatVideo(judul, durasi);
    root = insertBST(root, baru);
    
    // Simpan salinan video untuk undo
    topUndo = new UndoNode{"ADD", salinVideo(baru), topUndo};
    cout << "Video ditambahkan.\n";
}

// Fungsi untuk menampilkan daftar video
void tampilDaftar() {
    if (!root) {
        cout << "Tidak ada video tersedia.\n";
        return;
    }
    
    tampilInOrder(root);
    
    char pilih;
    cout << "Cari video? (y/t): "; 
    cin >> pilih;
    if (pilih == 'y') {
        string j;
        cout << "Masukkan judul: "; 
        cin.ignore();
        getline(cin, j);
        Video* v = cariVideo(root, j);
        if (v)
            cout << "Ditemukan: " << v->judul << ", " << v->durasi 
                 << " menit, Status: " << v->status << endl;
        else
            cout << "Tidak ditemukan.\n";
    }
}

// Fungsi untuk menambahkan video ke playlist
void tambahPlaylist() {
    if (!root) {
        cout << "Tidak ada video tersedia.\n";
        return;
    }
    
    string j;
    cout << "Judul video: "; 
    cin.ignore();
    getline(cin, j);
    
    Video* v = cariVideo(root, j);
    if (!v) {
        cout << "Video tidak ditemukan.\n";
        return;
    }
    
    if (v->status != "tersedia") {
        cout << "Video tidak tersedia untuk ditambahkan ke playlist.\n";
        return;
    }
    
    v->status = (!headPlaylist) ? "sedang diputar" : "dalam antrean";
    
    PlaylistNode* node = new PlaylistNode{v, nullptr};
    if (!headPlaylist) {
        headPlaylist = node;
    } else {
        PlaylistNode* temp = headPlaylist;
        while (temp->next) temp = temp->next;
        temp->next = node;
    }
    
    // Simpan salinan video untuk undo
    topUndo = new UndoNode{"ADD_TO_PLAYLIST", salinVideo(v), topUndo};
    cout << "Ditambahkan ke playlist.\n";
}

// Fungsi untuk menonton video dari playlist
void tonton() {
    if (!headPlaylist) {
        cout << "Playlist kosong.\n";
        return;
    }
    
    PlaylistNode* tonton = headPlaylist;
    headPlaylist = headPlaylist->next;
    
    cout << "Menonton: " << tonton->video->judul << endl;
    tonton->video->status = "tersedia";
    
    // Tambahkan ke riwayat
    topRiwayat = new RiwayatNode{tonton->video, topRiwayat};
    
    // Simpan salinan video untuk undo
    topUndo = new UndoNode{"WATCH", salinVideo(tonton->video), topUndo};
    
    // Update status video berikutnya jika ada
    if (headPlaylist) {
        headPlaylist->video->status = "sedang diputar";
    }
    
    delete tonton;
}

// Fungsi untuk menampilkan riwayat tontonan
void tampilRiwayat() {
    if (!topRiwayat) {
        cout << "Riwayat tontonan kosong.\n";
        return;
    }
    
    cout << "Riwayat:\n";
    RiwayatNode* temp = topRiwayat;
    while (temp) {
        cout << "- " << temp->video->judul << endl;
        temp = temp->next;
    }
}

// Fungsi untuk menghapus video
void hapusVideo() {
    if (!root) {
        cout << "Tidak ada video tersedia.\n";
        return;
    }
    
    string j;
    cout << "Judul video: "; 
    cin.ignore();
    getline(cin, j);
    
    Video* v = cariVideo(root, j);
    if (!v) {
        cout << "Video tidak ditemukan.\n";
        return;
    }
    
    if (v->status != "tersedia") {
        char yakin;
        cout << "Video dalam status '" << v->status 
             << "'. Yakin hapus? (y/t): "; 
        cin >> yakin;
        if (yakin != 'y') return;
    }
    
    // Buat salinan video sebelum menghapus
    Video* salinan = salinVideo(v);
    root = deleteBST(root, j);
    
    // Simpan salinan video untuk undo
    topUndo = new UndoNode{"DELETE", salinan, topUndo};
    cout << "Video dihapus.\n";
}

// Fungsi untuk melakukan undo
void undo() {
    if (!topUndo) {
        cout << "Tidak ada aksi yang dapat di-undo.\n";
        return;
    }
    
    UndoNode* aksi = topUndo;
    topUndo = topUndo->next;
    
    if (aksi->aksi == "ADD") {
        // Undo penambahan video = hapus video
        root = deleteBST(root, aksi->video->judul);
        cout << "Undo tambah video: " << aksi->video->judul << endl;
    } 
    else if (aksi->aksi == "DELETE") {
        // Undo penghapusan video = tambahkan kembali
        root = insertBST(root, salinVideo(aksi->video));
        cout << "Undo hapus video: " << aksi->video->judul << endl;
    } 
    else if (aksi->aksi == "ADD_TO_PLAYLIST") {
        // Undo penambahan ke playlist
        Video* asli = cariVideo(root, aksi->video->judul);
        if (asli) {
            asli->status = "tersedia";
            
            // Hapus dari playlist
            PlaylistNode *curr = headPlaylist, *prev = nullptr;
            while (curr && curr->video->judul != aksi->video->judul) {
                prev = curr;
                curr = curr->next;
            }
            
            if (curr) {
                if (prev) prev->next = curr->next;
                else headPlaylist = curr->next;
                delete curr;
                
                // Update status video yang sedang diputar
                if (headPlaylist) {
                    headPlaylist->video->status = "sedang diputar";
                }
            }
        }
        cout << "Undo tambah ke playlist: " << aksi->video->judul << endl;
    } 
    else if (aksi->aksi == "WATCH") {
        // Undo menonton video = kembalikan ke playlist
        Video* asli = cariVideo(root, aksi->video->judul);
        if (asli) {
            asli->status = "sedang diputar";
            
            // Tambahkan ke awal playlist
            PlaylistNode* p = new PlaylistNode{asli, headPlaylist};
            headPlaylist = p;
            
            // Hapus dari riwayat
            if (topRiwayat && topRiwayat->video->judul == aksi->video->judul) {
                RiwayatNode* temp = topRiwayat;
                topRiwayat = topRiwayat->next;
                delete temp;
            }
        }
        cout << "Undo tonton video: " << aksi->video->judul << endl;
    }
    
    delete aksi->video;  // Hapus salinan video
    delete aksi;         // Hapus node undo
}

// Fungsi untuk membersihkan memori
void bersihkanMemori() {
    // Hapus BST
    while (root) {
        root = deleteBST(root, root->judul);
    }
    
    // Hapus playlist
    while (headPlaylist) {
        PlaylistNode* temp = headPlaylist;
        headPlaylist = headPlaylist->next;
        delete temp;
    }
    
    // Hapus riwayat
    while (topRiwayat) {
        RiwayatNode* temp = topRiwayat;
        topRiwayat = topRiwayat->next;
        delete temp;
    }
    
    // Hapus stack undo
    while (topUndo) {
        UndoNode* temp = topUndo;
        topUndo = topUndo->next;
        delete temp->video;
        delete temp;
    }
}

// Menu utama
void menu() {
    int pilih;
    do {
        cout << "\n=== MENU IDLIX ===\n";
        cout << "1. Tambah Video\n";
        cout << "2. Tampilkan Video\n";
        cout << "3. Tambah ke Playlist\n";
        cout << "4. Tonton Video\n";
        cout << "5. Riwayat Tontonan\n";
        cout << "6. Hapus Video\n";
        cout << "7. Undo\n";
        cout << "8. Keluar\n";
        cout << "Pilih: "; 
        cin >> pilih;
        
        switch (pilih) {
            case 1: tambahVideo(); break;
            case 2: tampilDaftar(); break;
            case 3: tambahPlaylist(); break;
            case 4: tonton(); break;
            case 5: tampilRiwayat(); break;
            case 6: hapusVideo(); break;
            case 7: undo(); break;
            case 8: cout << "Keluar...\n"; break;
            default: cout << "Pilihan tidak valid!\n";
        }
    } while (pilih != 8);
    
    bersihkanMemori();
}

int main() {
    menu();
    return 0;
}