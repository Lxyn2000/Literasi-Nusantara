#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUKU 100
#define MAX_HISTORY 100
#define MAX_STR 100

typedef struct
{
    char kode[10];
    char nama[MAX_STR];
    char jenis[MAX_STR];
    double harga;
} Buku;

typedef struct
{
    char kode_buku[10];
    int jumlah_terjual;
    double total_harga;
} History;

Buku daftarBuku[MAX_BUKU];
History daftarHistory[MAX_HISTORY];
int jumlahBuku = 0;
int jumlahHistory = 0;

// funngsi deklar

void clearBuffer();
void bacaString(const char *prompt, char *dest, int maxLen);
void generateKodeBuku(char *kode);
void bacaDataBuku();
void bacaHistory();
void simpanDataBuku();
void simpanHistory();
void tampilkanHeaderBuku();
void tampilkanBarisBuku(int no, Buku b);
void inputBukuBaru();
void viewHistory();
void viewBuku();
void deleteHistory();
void deleteBuku();
void inputTransaksi();
void bubbleSortNama(Buku arr[], int n);
void selectionSortHarga(Buku arr[], int n);
void sortBuku();
void tampilkanMenu();

void clearBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void bacaString(const char *prompt, char *dest, int maxLen)
{
    printf("%s", prompt);
    fgets(dest, maxLen, stdin);
    dest[strcspn(dest, "\n")] = '\0';
}

// kode buku

void generateKodeBuku(char *kode)
{
    sprintf(kode, "BK%03d", jumlahBuku + 1);
}

void bacaDataBuku()
{
    FILE *fp = fopen("databuku.txt", "r");
    if (fp == NULL)
    {
        printf("[INFO] databuku.txt tidak ditemukan. Memulai dengan data buku kosong.\n");
        return;
    }

    char line[256];
    while (jumlahBuku < MAX_BUKU && fgets(line, sizeof(line), fp))
    {
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0)
            continue;
        char *token = strtok(line, "|");
        if (!token)
            continue;
        strncpy(daftarBuku[jumlahBuku].kode, token, 9);

        token = strtok(NULL, "|");
        if (!token)
            continue;
        strncpy(daftarBuku[jumlahBuku].nama, token, MAX_STR - 1);

        token = strtok(NULL, "|");
        if (!token)
            continue;
        strncpy(daftarBuku[jumlahBuku].jenis, token, MAX_STR - 1);

        token = strtok(NULL, "|");
        if (!token)
            continue;
        daftarBuku[jumlahBuku].harga = atof(token);

        jumlahBuku++;
    }
    fclose(fp);
    printf("[INFO] %d data buku berhasil dimuat dari databuku.txt.\n", jumlahBuku);
}

void bacaHistory()
{
    FILE *fp = fopen("history.txt", "r");
    if (fp == NULL)
    {
        printf("[INFO] history.txt tidak ditemukan. Memulai dengan histori kosong.\n");
        return;
    }

    char line[256];
    while (jumlahHistory < MAX_HISTORY && fgets(line, sizeof(line), fp))
    {
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0)
            continue;

        char *token = strtok(line, "|");
        if (!token)
            continue;
        strncpy(daftarHistory[jumlahHistory].kode_buku, token, 9);

        token = strtok(NULL, "|");
        if (!token)
            continue;
        daftarHistory[jumlahHistory].jumlah_terjual = atoi(token);

        token = strtok(NULL, "|");
        if (!token)
            continue;
        daftarHistory[jumlahHistory].total_harga = atof(token);

        jumlahHistory++;
    }
    fclose(fp);
    printf("[INFO] %d data histori berhasil dimuat dari history.txt.\n", jumlahHistory);
}

void simpanDataBuku()
{
    FILE *fp = fopen("databuku.txt", "w");
    if (fp == NULL)
    {
        printf("[ERROR] Gagal membuat/membuka databuku.txt!\n");
        return;
    }
    for (int i = 0; i < jumlahBuku; i++)
    {
        fprintf(fp, "%s|%s|%s|%.2f\n",
                daftarBuku[i].kode,
                daftarBuku[i].nama,
                daftarBuku[i].jenis,
                daftarBuku[i].harga);
    }
    fclose(fp);
}

void simpanHistory()
{
    FILE *fp = fopen("history.txt", "w");
    if (fp == NULL)
    {
        printf("[ERROR] Gagal membuat/membuka history.txt!\n");
        return;
    }
    for (int i = 0; i < jumlahHistory; i++)
    {
        fprintf(fp, "%s|%d|%.2f\n",
                daftarHistory[i].kode_buku,
                daftarHistory[i].jumlah_terjual,
                daftarHistory[i].total_harga);
    }
    fclose(fp);
}

void tampilkanHeaderBuku()
{
    printf("\n%-4s  %-8s  %-28s  %-18s  %s\n",
           "No", "Kode", "Nama Buku", "Jenis", "Harga");
    printf("%-4s  %-8s  %-28s  %-18s  %s\n",
           "----", "--------", "----------------------------",
           "------------------", "-------------");
}

void tampilkanBarisBuku(int no, Buku b)
{
    printf("%-4d  %-8s  %-28s  %-18s  Rp %.2f\n",
           no, b.kode, b.nama, b.jenis, b.harga);
}

// function menu

void inputBukuBaru()
{
    if (jumlahBuku >= MAX_BUKU)
    {
        printf("[ERROR] Data buku sudah penuh! Maksimum %d buku.\n", MAX_BUKU);
        return;
    }

    Buku baru;
    generateKodeBuku(baru.kode);

    printf("\n=== INPUT DATA BUKU BARU ===\n");
    printf("Kode Buku (otomatis) : %s\n", baru.kode);

    bacaString("Nama Buku            : ", baru.nama, MAX_STR);
    bacaString("Jenis Buku           : ", baru.jenis, MAX_STR);

    printf("Harga Buku           : Rp ");
    scanf("%lf", &baru.harga);
    clearBuffer();

    if (baru.harga < 0)
    {
        printf("[ERROR] Harga tidak boleh negatif!\n");
        return;
    }

    daftarBuku[jumlahBuku++] = baru;
    simpanDataBuku();
    printf("\n[SUCCESS] Buku '%s' berhasil ditambahkan dengan kode %s!\n",
           baru.nama, baru.kode);
}

void viewHistory()
{
    printf("\n=== HISTORY TRANSAKSI PENJUALAN ===\n");
    if (jumlahHistory == 0)
    {
        printf("Belum ada histori transaksi.\n");
        return;
    }

    printf("%-4s  %-10s  %-16s  %s\n",
           "No", "Kode Buku", "Jumlah Terjual", "Total Harga");
    printf("%-4s  %-10s  %-16s  %s\n",
           "----", "----------", "----------------", "-------------");
    for (int i = 0; i < jumlahHistory; i++)
    {
        printf("%-4d  %-10s  %-16d  Rp %.2f\n",
               i + 1,
               daftarHistory[i].kode_buku,
               daftarHistory[i].jumlah_terjual,
               daftarHistory[i].total_harga);
    }
}

void viewBuku()
{
    printf("\n=== DATA BUKU TERSEDIA ===\n");
    if (jumlahBuku == 0)
    {
        printf("Belum ada data buku.\n");
        return;
    }
    tampilkanHeaderBuku();
    for (int i = 0; i < jumlahBuku; i++)
    {
        tampilkanBarisBuku(i + 1, daftarBuku[i]);
    }
}

void deleteHistory()
{
    viewHistory();
    if (jumlahHistory == 0)
        return;

    int index;
    printf("\nMasukkan index histori yang ingin dihapus (1 - %d): ", jumlahHistory);
    scanf("%d", &index);
    clearBuffer();

    if (index < 1 || index > jumlahHistory)
    {
        printf("[ERROR] Index tidak valid! Harus antara 1 dan %d.\n", jumlahHistory);
        return;
    }

    for (int i = index - 1; i < jumlahHistory - 1; i++)
    {
        daftarHistory[i] = daftarHistory[i + 1];
    }
    jumlahHistory--;
    simpanHistory();

    printf("[SUCCESS] Data Successfully delete..\n");
}

void deleteBuku()
{
    viewBuku();
    if (jumlahBuku == 0)
        return;

    int index;
    printf("\nMasukkan index buku yang ingin dihapus (1 - %d): ", jumlahBuku);
    scanf("%d", &index);
    clearBuffer();

    if (index < 1 || index > jumlahBuku)
    {
        printf("[ERROR] Index tidak valid! Harus antara 1 dan %d.\n", jumlahBuku);
        return;
    }

    for (int i = index - 1; i < jumlahBuku - 1; i++)
    {
        daftarBuku[i] = daftarBuku[i + 1];
    }
    jumlahBuku--;
    simpanDataBuku();

    printf("[SUCCESS] Data Successfully delete..\n");
}

void inputTransaksi()
{
    printf("\n=== INPUT TRANSAKSI PENJUALAN ===\n");

    if (jumlahBuku == 0)
    {
        printf("[INFO] Tidak ada data buku. Tambahkan buku terlebih dahulu (Menu 1).\n");
        return;
    }

    viewBuku();

    char kodeBuku[10];
    bacaString("\nKode Buku     : ", kodeBuku, 10);

    int found = -1;
    for (int i = 0; i < jumlahBuku; i++)
    {
        if (strcmp(daftarBuku[i].kode, kodeBuku) == 0)
        {
            found = i;
            break;
        }
    }

    if (found == -1)
    {
        printf("[ERROR] Kode buku '%s' tidak ditemukan!\n", kodeBuku);
        return;
    }

    int jumlah;
    printf("Jumlah Terjual: ");
    scanf("%d", &jumlah);
    clearBuffer();

    if (jumlah <= 0)
    {
        printf("[ERROR] Jumlah terjual harus lebih dari 0!\n");
        return;
    }

    double totalHarga = (double)jumlah * daftarBuku[found].harga;

    strncpy(daftarHistory[jumlahHistory].kode_buku, kodeBuku, 9);
    daftarHistory[jumlahHistory].jumlah_terjual = jumlah;
    daftarHistory[jumlahHistory].total_harga = totalHarga;
    jumlahHistory++;

    simpanHistory();

    printf("\n--- Detail Transaksi ---\n");
    printf("Buku         : %s\n", daftarBuku[found].nama);
    printf("Kode         : %s\n", daftarBuku[found].kode);
    printf("Harga Satuan : Rp %.2f\n", daftarBuku[found].harga);
    printf("Jumlah       : %d\n", jumlah);
    printf("Total Harga  : Rp %.2f\n", totalHarga);
    printf("Transaksi berhasil dicatat.\n");
}

// bagian sorting

void bubbleSortNama(Buku arr[], int n)
{
    Buku temp;
    int i, j;
    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - i - 1; j++)
        {
            if (strcmp(arr[j].nama, arr[j + 1].nama) > 0)
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void selectionSortHarga(Buku arr[], int n)
{
    Buku temp;
    int i, j, maxIdx;
    for (i = 0; i < n - 1; i++)
    {
        maxIdx = i;
        for (j = i + 1; j < n; j++)
        {
            if (arr[j].harga > arr[maxIdx].harga)
            {
                maxIdx = j;
            }
        }
        if (maxIdx != i)
        {
            temp = arr[i];
            arr[i] = arr[maxIdx];
            arr[maxIdx] = temp;
        }
    }
}

void sortBuku()
{
    if (jumlahBuku == 0)
    {
        printf("Tidak ada data buku untuk diurutkan.\n");
        return;
    }

    Buku salinan[MAX_BUKU];
    int i;
    for (i = 0; i < jumlahBuku; i++)
    {
        salinan[i] = daftarBuku[i];
    }

    printf("\n=== SORT BUKU ===\n");
    printf("(a) Urutkan berdasarkan Nama Buku  [Ascending  - Bubble Sort]\n");
    printf("(b) Urutkan berdasarkan Harga Buku [Descending - Selection Sort]\n");
    printf("(c) Kembali ke Menu Utama\n");
    printf("Pilihan: ");

    char pilih;
    scanf(" %c", &pilih);
    clearBuffer();

    if (pilih == 'a' || pilih == 'A')
    {
        bubbleSortNama(salinan, jumlahBuku);
        printf("\n=== HASIL SORT: NAMA BUKU (A → Z) ===\n");
        tampilkanHeaderBuku();
        for (i = 0; i < jumlahBuku; i++)
        {
            tampilkanBarisBuku(i + 1, salinan[i]);
        }
    }
    else if (pilih == 'b' || pilih == 'B')
    {
        selectionSortHarga(salinan, jumlahBuku);
        printf("\n=== HASIL SORT: HARGA BUKU (Tertinggi → Terendah) ===\n");
        tampilkanHeaderBuku();
        for (i = 0; i < jumlahBuku; i++)
        {
            tampilkanBarisBuku(i + 1, salinan[i]);
        }
    }
    else if (pilih == 'c' || pilih == 'C')
    {
        return;
    }
    else
    {
        printf("[ERROR] Pilihan tidak valid! Masukkan a, b, atau c.\n");
    }
}

// menu utama

void tampilkanMenu()
{
    printf("\n==========================================\n");
    printf("    TOKO BUKU LITERASI NUSANTARA JAYA    \n");
    printf("==========================================\n");
    printf("  [1] Input Data Buku Baru\n");
    printf("  [2] View History Transaksi Penjualan\n");
    printf("  [3] View Buku (Seluruh Data Buku)\n");
    printf("  [4] Delete History (Hapus Histori Penjualan)\n");
    printf("  [5] Delete Buku (Hapus Data Buku)\n");
    printf("  [6] Exit (Simpan & Keluar)\n");
    printf("  [7] Input Transaksi Penjualan\n");
    printf("  [8] Sort Buku (Nama / Harga)\n");
    printf("==========================================\n");
    printf("  Pilihan Anda: ");
}

// Main
int main()
{
    printf("\n==========================================\n");
    printf("    TOKO BUKU LITERASI NUSANTARA JAYA    \n");
    printf("       Sistem Monitoring Transaksi        \n");
    printf("==========================================\n\n");

    bacaDataBuku();
    bacaHistory();

    int pilihan;

    do
    {
        tampilkanMenu();
        scanf("%d", &pilihan);
        clearBuffer();

        switch (pilihan)
        {
        case 1:
            inputBukuBaru();
            break;
        case 2:
            viewHistory();
            break;
        case 3:
            viewBuku();
            break;
        case 4:
            deleteHistory();
            break;
        case 5:
            deleteBuku();
            break;
        case 6:
            simpanDataBuku();
            simpanHistory();
            printf("\n[SUCCESS] Data berhasil disimpan ke databuku.txt dan history.txt.\n");
            printf("Terima kasih! Sampai jumpa.\n");
            break;
        case 7:
            inputTransaksi();
            break;
        case 8:
            sortBuku();
            break;
        default:
            printf("[ERROR] Pilihan tidak valid! Masukkan angka 1 - 8.\n");
        }

    } while (pilihan != 6);

    return 0;
}
