#include <iostream>
#include <vector>


extern "C"{
// Fungsi untuk melakukan operasi XOR antara dua blok 128-bit
void xorBlocks(const unsigned char *block1, const unsigned char *block2, unsigned char *result) {
    for (int i = 0; i < 16; ++i) {
        result[i] = block1[i] ^ block2[i];
    }
}

// Fungsi substitusi byte berdasarkan tabel S-box
void subBytes(unsigned char *block) {
    static const unsigned char sBox[256] = {
        0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
        0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
        0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
        0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
        0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
        0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
        0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
        0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
        0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
        0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
        0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
        0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
        0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
        0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
        0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
        0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
    };

    for (int i = 0; i < 16; ++i) {
        block[i] = sBox[block[i]];
    }
}

// Fungsi shift rows
void shiftRows(unsigned char *block) {
    unsigned char temp[16];

    // Baris 0 tetap tidak berubah
    temp[0] = block[0];
    temp[4] = block[4];
    temp[8] = block[8];
    temp[12] = block[12];

    // Geser baris 1
    temp[1] = block[5];
    temp[5] = block[9];
    temp[9] = block[13];
    temp[13] = block[1];

    // Geser baris 2
    temp[2] = block[10];
    temp[6] = block[14];
    temp[10] = block[2];
    temp[14] = block[6];

    // Geser baris 3
    temp[3] = block[15];
    temp[7] = block[3];
    temp[11] = block[7];
    temp[15] = block[11];

    // Salin kembali ke blok asli
    for (int i = 0; i < 16; ++i) {
        block[i] = temp[i];
    }
}

// Fungsi untuk mengalikan bilangan dalam Galois Field (GF(2^8))
unsigned char gf_mul(unsigned char a, unsigned char b) {
    unsigned char p = 0;
    while (b) {
        if (b & 1) {
            p ^= a;
        }
        unsigned char carry = a & 0x80; // Cek apakah MSB adalah 1
        a <<= 1;
        if (carry) {
            a ^= 0x1b; // Konstanta irreducible polinomial untuk GF(2^8)
        }
        b >>= 1;
    }
    return p;
}

// Fungsi mix columns
void mixColumns(unsigned char *block) {
    static const unsigned char matrix[4][4] = {
        {0x02, 0x03, 0x01, 0x01},
        {0x01, 0x02, 0x03, 0x01},
        {0x01, 0x01, 0x02, 0x03},
        {0x03, 0x01, 0x01, 0x02}
    };

    unsigned char temp[4][4];
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            temp[i][j] = 0;
            for (int k = 0; k < 4; ++k) {
                temp[i][j] ^= gf_mul(matrix[i][k], block[k * 4 + j]);
            }
        }
    }

    // Salin kembali ke blok asli
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            block[i * 4 + j] = temp[i][j];
        }
    }
}

// Fungsi untuk melakukan operasi XOR antara blok 128-bit dengan kunci putaran
void addRoundKey(unsigned char *block, const unsigned char *roundKey) {
    for (int i = 0; i < 16; ++i) {
        block[i] ^= roundKey[i];
    }
}


    // Fungsi enkripsi AES
    void encryptAES(const unsigned char *plainText, const unsigned char *key, unsigned char *cipherText) {
        // Inisialisasi kunci dan blok
        unsigned char state[16];
        for (int i = 0; i < 16; ++i) {
            state[i] = plainText[i];
        }

        // Tambahkan putaran kunci pertama
        addRoundKey(state, key);

        // Lakukan putaran enkripsi (10 putaran untuk AES-128)
        for (int round = 1; round <= 10; ++round) {
            subBytes(state);
            shiftRows(state);
            if (round < 10) {
                mixColumns(state);
            }
            addRoundKey(state, key + round * 16); // Tambahkan kunci putaran
        }

        // Salin hasil enkripsi ke blok chiper
        for (int i = 0; i < 16; ++i) {
            cipherText[i] = state[i];
        }
    }
}


// int main() {
//     // Data yang akan dienkripsi
//     unsigned char plainText[] = "Ini adalah contoh teks yang akan dienkripsi dengan AES.";
//     int sizePt = sizeof(plainText)/sizeof(plainText[0]);
//     std::cout << sizePt << std::endl;

//     // Kunci enkripsi (128-bit)
//     unsigned char key[] = "admin";

//     // Hasil enkripsi
//     unsigned char cipherText[16];
//     encryptAES(plainText, key, cipherText);

//     std::cout << "Kata yang akan di enkripsi: ";
//     std::cout << plainText << std::endl;
//     // Menampilkan hasil enkripsi
//     std::cout << "Hasil Enkripsi: ";
//     for (int i = 0; i < 16; ++i) {
//         std::cout << std::hex << (int)cipherText[i];
//     }
//     std::cout << std::endl;

//     return 0;
// }
