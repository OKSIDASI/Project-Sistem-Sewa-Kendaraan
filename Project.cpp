#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <iomanip>

using namespace std;

const int MAX = 50;

struct Kendaraan {
    int    id;
    char   nama[50];
    char   jenis[10];
    char   plat[15];
    double harga;
    bool   tersedia;
};

struct Transaksi {
    int    id;
    int    idKendaraan;
    char   namaPenyewa[50];
    char   tanggalSewa[12];
    char   tanggalKembali[12];
    int    lamaSewa;
    double totalBayar;
    bool   sudahKembali;
};

// Global Array & Counter
Kendaraan kendaraan[MAX];
Transaksi transaksi[MAX];
int jmlKendaraan = 0;
int jmlTransaksi = 0;

// Fungsi Helper
void bersihkanBuffer() {
    cin.clear();
    char c;
    while (cin.get(c) && c != '\n');
}

void garis() {
    cout << "---------------------------------------------------------\n";
}

void tekanEnter() {
    cout << "\nTekan Enter untuk kembali ke menu...";
    cin.get();
}

int cariKendaraan(int id) {
    for (int i = 0; i < jmlKendaraan; i++) {
        if (kendaraan[i].id == id) return i;
    }
    return -1;
}

int cariTransaksi(int id) {
    for (int i = 0; i < jmlTransaksi; i++) {
        if (transaksi[i].id == id) return i;
    }
    return -1;
}

// ---------------------------------------------------------
// FILE HANDLING (SAVE & LOAD) - FIXED FOR LARGE NUMBERS
// ---------------------------------------------------------
void simpanData() {
    // 1. Simpan Kendaraan
    ofstream fk("kendaraan.txt", ios::trunc);
    if (!fk) { 
        cout << "Gagal membuka file kendaraan.txt untuk menulis!\n"; 
        return; 
    }
    fk << jmlKendaraan << "\n";
    for (int i = 0; i < jmlKendaraan; i++) {
        fk << kendaraan[i].id       << "\n";
        fk << kendaraan[i].nama     << "\n";
        fk << kendaraan[i].jenis    << "\n";
        fk << kendaraan[i].plat     << "\n";
        // FIX: Dipaksa berformat angka normal tanpa desimal saat ditulis ke file
        fk << fixed << setprecision(0) << kendaraan[i].harga    << "\n";
        fk << kendaraan[i].tersedia << "\n";
    }
    fk.close();

    // 2. Simpan Transaksi
    ofstream ft("transaksi.txt", ios::trunc);
    if (!ft) { 
        cout << "Gagal membuka file transaksi.txt untuk menulis!\n"; 
        return; 
    }
    ft << jmlTransaksi << "\n";
    for (int i = 0; i < jmlTransaksi; i++) {
        ft << transaksi[i].id           << "\n";
        ft << transaksi[i].idKendaraan  << "\n";
        ft << transaksi[i].namaPenyewa  << "\n";
        ft << transaksi[i].tanggalSewa  << "\n";
        ft << transaksi[i].tanggalKembali << "\n";
        ft << transaksi[i].lamaSewa     << "\n";
        // FIX: Dipaksa berformat angka normal tanpa desimal saat ditulis ke file
        ft << fixed << setprecision(0) << transaksi[i].totalBayar   << "\n";
        ft << transaksi[i].sudahKembali << "\n";
    }
    ft.close();
}

void muatData() {
    // 1. Muat Kendaraan
    ifstream fk("kendaraan.txt");
    if (fk) {
        fk >> jmlKendaraan;
        fk.ignore();
        for (int i = 0; i < jmlKendaraan; i++) {
            fk >> kendaraan[i].id;
            fk.ignore();
            fk.getline(kendaraan[i].nama, 50);
            fk.getline(kendaraan[i].jenis, 10);
            fk.getline(kendaraan[i].plat, 15);
            fk >> kendaraan[i].harga >> kendaraan[i].tersedia;
            fk.ignore();
        }
        fk.close();
    }

    // 2. Muat Transaksi
    ifstream ft("transaksi.txt");
    if (ft) {
        ft >> jmlTransaksi;
        ft.ignore();
        for (int i = 0; i < jmlTransaksi; i++) {
            ft >> transaksi[i].id >> transaksi[i].idKendaraan;
            ft.ignore();
            ft.getline(transaksi[i].namaPenyewa, 50);
            ft.getline(transaksi[i].tanggalSewa, 12);
            ft.getline(transaksi[i].tanggalKembali, 12);
            ft >> transaksi[i].lamaSewa >> transaksi[i].totalBayar >> transaksi[i].sudahKembali;
            ft.ignore();
        }
        ft.close();
    }
}

// ---------------------------------------------------------
// FITUR UTAMA
// ---------------------------------------------------------

// 1. TAMBAH KENDARAAN
void tambahKendaraan() {
    system("cls");
    cout << "=== TAMBAH KENDARAAN ===\n\n";
    if (jmlKendaraan >= MAX) {
        cout << "Data penuh! Tidak bisa menambah kendaraan baru.\n";
        tekanEnter();
        return;
    }

    Kendaraan k;
    k.id = (jmlKendaraan == 0) ? 1 : kendaraan[jmlKendaraan - 1].id + 1;
    k.tersedia = true;

    cout << "Nama Kendaraan           : ";
    cin.getline(k.nama, 50);
    if (strlen(k.nama) == 0) { cout << "Nama tidak boleh kosong!\n"; tekanEnter(); return; }

    int pilJenis;
    cout << "Jenis (1=Mobil, 2=Motor) : ";
    if (!(cin >> pilJenis) || (pilJenis != 1 && pilJenis != 2)) {
        cout << "Pilihan jenis tidak valid!\n";
        bersihkanBuffer(); tekanEnter(); return;
    }
    bersihkanBuffer();
    if (pilJenis == 1) strcpy(k.jenis, "Mobil");
    else               strcpy(k.jenis, "Motor");

    cout << "Nomor Plat               : ";
    cin.getline(k.plat, 15);
    if (strlen(k.plat) == 0) { cout << "Plat tidak boleh kosong!\n"; tekanEnter(); return; }

    cout << "Harga Sewa / Hari (Rp)   : ";
    if (!(cin >> k.harga) || k.harga <= 0) {
        cout << "Harga tidak valid!\n";
        bersihkanBuffer(); tekanEnter(); return;
    }
    bersihkanBuffer();

    kendaraan[jmlKendaraan++] = k;
    simpanData(); 
    cout << "\n[SUKSES] Kendaraan berhasil disimpan ke file (ID: " << k.id << ")\n";
    tekanEnter();
}

// 2. TAMPILKAN DATA KENDARAAN
void tampilkanData() {
    system("cls");
    cout << "=== DATA SEMUA KENDARAAN ===\n\n";
    if (jmlKendaraan == 0) { 
        cout << "Belum ada data kendaraan di database/file.\n"; 
    } else {
        garis();
        cout << left << setw(5) << "ID" << setw(20) << "Nama" << setw(10) << "Jenis" << setw(12) << "Plat" << setw(15) << "Harga" << "Status\n";
        garis();
        for (int i = 0; i < jmlKendaraan; i++) {
            cout << left << setw(5) << kendaraan[i].id 
                 << setw(20) << kendaraan[i].nama 
                 << setw(10) << kendaraan[i].jenis 
                 << setw(12) << kendaraan[i].plat 
                 << "Rp " << setw(12) << fixed << setprecision(0) << kendaraan[i].harga // FIX TAMPILAN LAYAR
                 << (kendaraan[i].tersedia ? "Tersedia" : "Sedang Disewa") << "\n";
        }
        garis();
        cout << "Total Kendaraan Terdaftar: " << jmlKendaraan << "\n";
    }
    tekanEnter();
}

// 3. CARI KENDARAAN
void cariKendaraanMenu() {
    system("cls");
    cout << "=== CARI KENDARAAN (DATABASE) ===\n\n";
    if (jmlKendaraan == 0) { cout << "Database kosong.\n"; tekanEnter(); return; }

    int idCari;
    cout << "Masukkan ID kendaraan yang dicari: ";
    if (!(cin >> idCari)) {
        cout << "Input harus berupa angka!\n";
        bersihkanBuffer(); tekanEnter(); return;
    }
    bersihkanBuffer();

    int idx = cariKendaraan(idCari);
    if (idx == -1) {
        cout << "\n[NOT FOUND] Kendaraan dengan ID " << idCari << " tidak ditemukan di file.\n";
    } else {
        cout << "\n[DITEMUKAN] Detail Kendaraan:\n";
        garis();
        cout << "ID       : " << kendaraan[idx].id << "\n";
        cout << "Nama     : " << kendaraan[idx].nama << "\n";
        cout << "Jenis    : " << kendaraan[idx].jenis << "\n";
        cout << "Plat     : " << kendaraan[idx].plat << "\n";
        cout << "Harga    : Rp " << fixed << setprecision(0) << kendaraan[idx].harga << " / hari\n"; // FIX TAMPILAN LAYAR
        cout << "Status   : " << (kendaraan[idx].tersedia ? "Tersedia" : "Sedang Disewa") << "\n";
        garis();
    }
    tekanEnter();
}

// 4. DAFTAR HARGA KENDARAAN
void hargaKendaraan() {
    system("cls");
    cout << "=== DAFTAR HARGA SEWA KENDARAAN ===\n\n";
    if (jmlKendaraan == 0) { cout << "Belum ada data kendaraan.\n"; tekanEnter(); return; }

    garis();
    cout << left << setw(5) << "ID" << setw(25) << "Nama Kendaraan" << "Harga Sewa / Hari\n";
    garis();
    for (int i = 0; i < jmlKendaraan; i++) {
        cout << left << setw(5) << kendaraan[i].id 
             << setw(25) << kendaraan[i].nama 
             << "Rp " << fixed << setprecision(0) << kendaraan[i].harga << "\n"; // FIX TAMPILAN LAYAR
    }
    garis();
    tekanEnter(); 
}

// 5. SEWA KENDARAAN (CRUCIAL CHECKOUT)
void sewaKendaraan() {
    system("cls");
    cout << "=== SEWA KENDARAAN & CHECKOUT ===\n\n";
    if (jmlKendaraan == 0) { cout << "Belum ada kendaraan yang terdaftar.\n"; tekanEnter(); return; }
    if (jmlTransaksi >= MAX) { cout << "Memori transaksi penuh!\n"; tekanEnter(); return; }

    bool adaTersedia = false;
    cout << "Kendaraan Ready untuk Disewa:\n";
    garis();
    for (int i = 0; i < jmlKendaraan; i++) {
        if (kendaraan[i].tersedia) {
            cout << "ID: " << kendaraan[i].id << " | " << kendaraan[i].nama 
                 << " [" << kendaraan[i].plat << "] | Rp " << fixed << setprecision(0) << kendaraan[i].harga << "/hari\n"; // FIX TAMPILAN LAYAR
            adaTersedia = true;
        }
    }
    garis();

    if (!adaTersedia) {
        cout << "Maaf, semua kendaraan saat ini sedang disewa / tidak tersedia.\n";
        tekanEnter();
        return;
    }

    Transaksi t;
    cout << "Pilih ID Kendaraan yang disewa : ";
    if (!(cin >> t.idKendaraan)) {
        cout << "Input tidak valid!\n";
        bersihkanBuffer(); tekanEnter(); return;
    }
    bersihkanBuffer();

    int idx = cariKendaraan(t.idKendaraan);
    if (idx == -1) { cout << "ID Kendaraan tidak ditemukan!\n"; tekanEnter(); return; }
    if (!kendaraan[idx].tersedia) { 
        cout << "KENDARAAN BLOCKED! Kendaraan sedang disewa orang lain.\n"; 
        tekanEnter(); return; 
    }

    cout << "Nama Penyewa                   : "; cin.getline(t.namaPenyewa, 50);
    cout << "Tanggal Mulai Sewa (DD/MM/YY)  : "; cin.getline(t.tanggalSewa, 12);
    cout << "Tanggal Kembali    (DD/MM/YY)  : "; cin.getline(t.tanggalKembali, 12);
    cout << "Lama Sewa (Hari)               : ";
    if (!(cin >> t.lamaSewa) || t.lamaSewa <= 0) {
        cout << "Durasi sewa tidak valid!\n";
        bersihkanBuffer(); tekanEnter(); return;
    }
    bersihkanBuffer();

    t.totalBayar = kendaraan[idx].harga * t.lamaSewa;

    system("cls");
    cout << "=== NOTA & BILL CHECKOUT ===\n";
    garis();
    cout << "Penyewa       : " << t.namaPenyewa << "\n";
    cout << "Kendaraan     : " << kendaraan[idx].nama << " (" << kendaraan[idx].plat << ")\n";
    cout << "Durasi        : " << t.tanggalSewa << " s/d " << t.tanggalKembali << " (" << t.lamaSewa << " Hari)\n";
    cout << "TOTAL BILL    : Rp " << fixed << setprecision(0) << t.totalBayar << "\n"; // FIX TAMPILAN LAYAR
    garis();

    double uangBayar;
    cout << "Masukkan Uang Pembayaran (Rp)  : ";
    if (!(cin >> uangBayar) || uangBayar < t.totalBayar) {
        cout << "Transaksi Gagal! Uang tidak cukup atau input salah.\n";
        bersihkanBuffer(); tekanEnter(); return;
    }
    bersihkanBuffer();

    cout << "Kembalian Anda                 : Rp " << fixed << setprecision(0) << (uangBayar - t.totalBayar) << "\n"; // FIX TAMPILAN LAYAR
    
    int konfirm;
    cout << "\nKonfirmasi Cetak Transaksi? (1=Ya, 2=Batal): ";
    cin >> konfirm; bersihkanBuffer();

    if (konfirm == 1) {
        t.id = (jmlTransaksi == 0) ? 1 : transaksi[jmlTransaksi - 1].id + 1;
        t.sudahKembali = false;
        
        transaksi[jmlTransaksi++] = t;
        kendaraan[idx].tersedia = false; 
        
        simpanData(); 
        cout << "\n[SUKSES] Sewa berhasil dicatat dan status kendaraan dikunci!\n";
    } else {
        cout << "\nTransaksi dibatalkan.\n";
    }
    tekanEnter();
}

// 6. PENGEMBALIAN KENDARAAN
void pengembalianKendaraan() {
    system("cls");
    cout << "=== PENGEMBALIAN KENDARAAN ===\n\n";
    if (jmlTransaksi == 0) { cout << "Belum ada riwayat transaksi aktif.\n"; tekanEnter(); return; }

    bool adaAktif = false;
    cout << "Daftar Transaksi Pinjam Aktif:\n";
    garis();
    for (int i = 0; i < jmlTransaksi; i++) {
        if (!transaksi[i].sudahKembali) {
            int kIdx = cariKendaraan(transaksi[i].idKendaraan);
            cout << "ID Trx: " << transaksi[i].id << " | Nama: " << transaksi[i].namaPenyewa 
                 << " | Unit: " << (kIdx != -1 ? kendaraan[kIdx].nama : "Tidak Diketahui") << "\n";
            adaAktif = true;
        }
    }
    garis();

    if (!adaAktif) { cout << "Tidak ada kendaraan yang sedang keluar (0 rental aktif).\n"; tekanEnter(); return; }

    int idTrx;
    cout << "Masukkan ID Transaksi Pengembalian: ";
    if (!(cin >> idTrx)) { cout << "Input salah.\n"; bersihkanBuffer(); tekanEnter(); return; }
    bersihkanBuffer();

    int tIdx = cariTransaksi(idTrx);
    if (tIdx == -1 || transaksi[tIdx].sudahKembali) {
        cout << "ID Transaksi tidak ditemukan atau kendaraan sudah lama kembali.\n";
        tekanEnter(); return;
    }

    int kIdx = cariKendaraan(transaksi[tIdx].idKendaraan);
    
    transaksi[tIdx].sudahKembali = true;
    if (kIdx != -1) {
        kendaraan[kIdx].tersedia = true; 
    }

    simpanData(); 
    cout << "\n[SUKSES] Kendaraan berhasil dikembalikan! Status unit sekarang: TERSEDIA.\n";
    tekanEnter();
}

// 7. SORTING KENDARAAN
void sortingKendaraan() {
    system("cls");
    cout << "=== METODE SORTING KENDARAAN ===\n\n";
    if (jmlKendaraan == 0) { cout << "Data kosong.\n"; tekanEnter(); return; }

    cout << "1. Urutkan berdasarkan Abjad (A-Z)\n";
    cout << "2. Urutkan berdasarkan Harga (Termurah ke Termahal)\n";
    cout << "Pilih metode sorting: ";
    int pil;
    if (!(cin >> pil) || (pil != 1 && pil != 2)) {
        cout << "Pilihan tidak valid.\n";
        bersihkanBuffer(); tekanEnter(); return;
    }
    bersihkanBuffer();

    Kendaraan temp[MAX];
    for (int i = 0; i < jmlKendaraan; i++) temp[i] = kendaraan[i];

    for (int i = 0; i < jmlKendaraan - 1; i++) {
        for (int j = 0; j < jmlKendaraan - 1 - i; j++) {
            bool tukar = false;
            if (pil == 1) {
                if (strcmp(temp[j].nama, temp[j+1].nama) > 0) tukar = true;
            } else {
                if (temp[j].harga > temp[j+1].harga) tukar = true;
            }

            if (tukar) {
                Kendaraan swap = temp[j];
                temp[j] = temp[j+1];
                temp[j+1] = swap;
            }
        }
    }

    cout << "\nHasil Pengurutan:\n";
    garis();
    for (int i = 0; i < jmlKendaraan; i++) {
        cout << i+1 << ". " << temp[i].nama << " [" << temp[i].jenis << "] | Rp " << fixed << setprecision(0) << temp[i].harga << "/hari\n"; // FIX TAMPILAN LAYAR
    }
    garis();
    tekanEnter();
}

// 8. HAPUS DATA KENDARAAN
void hapusData() {
    system("cls");
    cout << "=== HAPUS DATA KENDARAAN ===\n\n";
    if (jmlKendaraan == 0) { cout << "Belum ada data kendaraan untuk dihapus.\n"; tekanEnter(); return; }

    cout << "Daftar unit saat ini:\n";
    garis();
    for (int i = 0; i < jmlKendaraan; i++) {
        cout << "ID: " << kendaraan[i].id << " - " << kendaraan[i].nama 
             << " [" << (kendaraan[i].tersedia ? "Ready" : "Sedang Disewa") << "]\n";
    }
    garis();

    int idHapus;
    cout << "Masukkan ID Kendaraan yang ingin dihapus (Mogok/Rusak): ";
    if (!(cin >> idHapus)) { cout << "Input salah.\n"; bersihkanBuffer(); tekanEnter(); return; }
    bersihkanBuffer();

    int idx = cariKendaraan(idHapus);
    if (idx == -1) { cout << "Kendaraan tidak ditemukan.\n"; tekanEnter(); return; }
    
    if (!kendaraan[idx].tersedia) {
        cout << "Gagal! Kendaraan tidak boleh dihapus karena sedang berjalan dalam masa sewa.\n";
        tekanEnter(); return;
    }

    cout << "Apakah Anda yakin menghapus \"" << kendaraan[idx].nama << "\"? (1=Ya, 2=Batal): ";
    int konf; cin >> konf; bersihkanBuffer();
    if (konf != 1) { cout << "Penghapusan dibatalkan.\n"; tekanEnter(); return; }

    for (int i = idx; i < jmlKendaraan - 1; i++) {
        kendaraan[i] = kendaraan[i + 1];
    }
    jmlKendaraan--;

    simpanData(); 
    cout << "\n[SUKSES] Unit kendaraan berhasil dihapus dari database file.\n";
    tekanEnter();
}

// ---------------------------------------------------------
// MAIN SYSTEM CONTROL
// ---------------------------------------------------------
int main() {
    muatData();

    int pilih;
    do {
        system("cls");
        cout << "=========================================================\n";
        cout << "                 SISTEM RENTAL KENDARAAN C++                \n";
        cout << "=========================================================\n";
        cout << " 1. Tambah Kendaraan Baru (Save to File)\n";
        cout << " 2. Tampilkan Semua List Kendaraan\n";
        cout << " 3. Cari Kendaraan via File\n";
        cout << " 4. Cek Daftar Harga Sewa (Menu Back)\n";
        cout << " 5. Sewa Kendaraan (Checkout Pembayaran)\n";
        cout << " 6. Pengembalian Unit Rental\n";
        cout << " 7. Urutkan Kendaraan (Abjad / Harga)\n";
        cout << " 8. Hapus Unit Kendaraan (Mogok / Rusak)\n";
        cout << " 9. Keluar Sistem\n";
        cout << "=========================================================\n";
        cout << "Pilih Opsi (1-9): ";

        if (!(cin >> pilih)) {
            pilih = 0;
            bersihkanBuffer();
            continue;
        }
        bersihkanBuffer();

        switch (pilih) {
            case 1: tambahKendaraan();       break;
            case 2: tampilkanData();         break;
            case 3: cariKendaraanMenu();     break;
            case 4: hargaKendaraan();        break;
            case 5: sewaKendaraan();         break;
            case 6: pengembalianKendaraan(); break;
            case 7: sortingKendaraan();      break;
            case 8: hapusData();             break;
            case 9: cout << "\nTerima kasih! Sesi program ditutup.\n"; break;
            default: 
                cout << "\nOpsi salah! Masukkan angka 1 sampai 9.\n"; 
                tekanEnter();
                break;
        }
    } while (pilih != 9);

    return 0;
}