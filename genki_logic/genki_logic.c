#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * 平文の差分位置はこのどれかに当てはまる
 *  model1 |  model2 |  model3 |  model4
 * x 0 0 0 | 0 x 0 0 | 0 0 x 0 | 0 0 0 x
 * 0 x 0 0 | 0 0 x 0 | 0 0 0 x | x 0 0 0
 * 0 0 x 0 | 0 0 0 x | x 0 0 0 | 0 x 0 0
 * 0 0 0 x | x 0 0 0 | 0 x 0 0 | 0 0 x 0
 */

/* ここからグローバル変数*/
// 平文 mode1 ~ 4までの順 全部使わなくても良さそうで全てのモデルが検査できればおっけい
unsigned char PLAIN_TEXT[10][2][16] = {
    { // model1
        {0x3D, 0xCF, 0xD6, 0x74, 0xBD, 0xB2, 0xEF, 0x05, 0xC4, 0x66, 0x91, 0x68, 0x6F, 0x24, 0xA8, 0x9F},
        {0xAF, 0xCF, 0xD6, 0x74, 0xBD, 0x79, 0xEF, 0x05, 0xC4, 0x66, 0x59, 0x68, 0x6F, 0x24, 0xA8, 0x75}
    },
    {// model1
        {0x52, 0x71, 0x3F, 0xCF, 0x46, 0x07, 0x9F, 0xF8, 0x67, 0x24, 0x82, 0x60, 0x50, 0x66, 0x4A, 0x9C},
        {0xD6, 0x71, 0x3F, 0xCF, 0x46, 0x66, 0x9F, 0xF8, 0x67, 0x24, 0xE6, 0x60, 0x50, 0x66, 0x4A, 0xC5}
    },
    {//model2
        {0x3F, 0x2A, 0x26, 0x27, 0xFA, 0x16, 0xDB, 0x7A, 0xF4, 0xE6, 0x0F, 0x25, 0x70, 0xDA, 0x28, 0x7A},
        {0x3F, 0x2A, 0x26, 0x82, 0x10, 0x16, 0xDB, 0x7A, 0xF4, 0x0D, 0x0F, 0x25, 0x70, 0xDA, 0xA8, 0x7A}
    },
    {//model2
        {0x99, 0x88, 0x14, 0xD4, 0xD6, 0x37, 0xC2, 0xA8, 0xC7, 0x68, 0x24, 0x53, 0xE2, 0xB7, 0x57, 0xA7},
        {0x99, 0x88, 0x14, 0x61, 0xF7, 0x37, 0xC2, 0xA8, 0xC7, 0xF7, 0x24, 0x53, 0xE2, 0xB7, 0x2A, 0xA7}
    },
    {//model3
        {0xB9, 0x45, 0x15, 0x61, 0xB4, 0x78, 0xF1, 0x49, 0xAE, 0x2B, 0x4B, 0xE1, 0xD1, 0x5A, 0xC2, 0xDB},
        {0xB9, 0x45, 0xBD, 0x61, 0xB4, 0x78, 0xF1, 0xC0, 0xF7, 0x2B, 0x4B, 0xE1, 0xD1, 0x4E, 0xC2, 0xDB}
    },
    {//model3
        {0xDF, 0xDD, 0xCE, 0xF4, 0x09, 0xF8, 0x3B, 0xE3, 0x59, 0x2E, 0x46, 0xBE, 0x08, 0x1F, 0x57, 0x25},
        {0xDF, 0xDD, 0xA0, 0xF4, 0x09, 0xF8, 0x3B, 0x8E, 0x91, 0x2E, 0x46, 0xBE, 0x08, 0x4A, 0x57, 0x25}
    },
    {//model3
        {0x3E, 0x24, 0x56, 0xF2, 0xEA, 0xF1, 0xC6, 0x72, 0x45, 0x32, 0xD7, 0x75, 0x51, 0xAB, 0x7A, 0x8E},
        {0x3E, 0x24, 0xDC, 0xF2, 0xEA, 0xF1, 0xC6, 0x77, 0xAF, 0x32, 0xD7, 0x75, 0x51, 0xC9, 0x7A, 0x8E}
    },
    { //model4
        {0x19, 0x66, 0x50, 0x04, 0xF7, 0x6D, 0xA3, 0xAE, 0xB6, 0x24, 0xB5, 0xB8, 0x3C, 0x7E, 0xC7, 0x1F},
        {0x19, 0x57, 0x50, 0x04, 0xF7, 0x6D, 0x4B, 0xAE, 0xB6, 0x24, 0xB5, 0x48, 0x3E, 0x7E, 0xC7, 0x1F}
    },
    { //model4
        {0x55, 0x20, 0x32, 0x58, 0x65, 0x78, 0xEB, 0x5B, 0x63, 0xCF, 0xDD, 0x5B, 0xE6, 0x9C, 0x21, 0xF5},
        {0x55, 0x33, 0x32, 0x58, 0x65, 0x78, 0x7C, 0x5B, 0x63, 0xCF, 0xDD, 0x64, 0xC1, 0x9C, 0x21, 0xF5}
    },
    { //model4
        {0x3A, 0xA0, 0xF7, 0x4A, 0x6E, 0xCF, 0x38, 0x0D, 0xF9, 0x49, 0xCA, 0x10, 0x70, 0x6A, 0x7F, 0x6E},
        {0x3A, 0x89, 0xF7, 0x4A, 0x6E, 0xCF, 0x54, 0x0D, 0xF9, 0x49, 0xCA, 0x23, 0x73, 0x6A, 0x7F, 0x6E}
    }
};

/*逆行列から求まる比*/
const unsigned char RATIO_PATTERN[4] = {0x0e, 0x09, 0x0d, 0xb};

// S-box look-up table この辺グローバル変数が長すぎてうざいから別ファイルにまとめたい
const unsigned char g_sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
    0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
    0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
    0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
    0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
    0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
    0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
    0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
    0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
    0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
    0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
    0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
    0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
    0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
    0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
    0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
    0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16,
};

unsigned char gmul(unsigned char a, unsigned char b) {
    unsigned char p = 0;
    unsigned char counter;
    unsigned char hi_bit_set;
    for(counter = 0; counter < 8; counter++) {
        if((b & 1) == 1)
            p ^= a;
        hi_bit_set = (a & 0x80);
        a <<= 1;
        if(hi_bit_set == 0x80)
            a ^= 0x1b;
        b >>= 1;
    }
    return p;
}

void AddRoundKey(unsigned char *dst, unsigned char *src, unsigned char *key) {
    for (int i = 0; i < 16; i++) {
        dst[i] = src[i] ^ key[i];
    }
}


void SubBytes(unsigned char *dst, unsigned char *src) {
    for (int i = 0; i < 16; i++) {
        dst[i] = g_sbox[src[i]];
    }
}


void FirstEncrypt(unsigned char *dst, unsigned char *src, unsigned char *init_key) {
    unsigned char key[16];
    unsigned char state[16]; //16bite
    int i;

    //0ラウンドにぶち込む初期キー, 以降は鍵は使わない
    AddRoundKey(state, src, init_key); 

    //1ラウンドのSubBytesまで
    SubBytes(dst, state);
}


/*
    Ratio_Difference_* を複数個作ってうんちコードなのは知ってる。あとでスマートに誰かまとめてくれ

    あと思ったんだけど比率が関係が決まってるから、もっとスマートに割り出せるかもしれない。
    まぁ実行速度早いからいいんだけどね
*/

int Ratio_Difference(unsigned char *pt1, unsigned char *pt2, int row1, int row2, int mode) {
    unsigned char diff[16] = {0};

    for (int i = 0; i < 16; i++) {
        diff[i] = pt1[i] ^ pt2[i];
        //ちゃんと差分が指定のモデルの位置のみに0以外の値があるかをチェック
        // printf("%02X ", diff[i]);
    }
        // printf("\n");

    switch (mode) {
        case 1:
            return (gmul(diff[row1], 0x09) == gmul(diff[row2], 0x0e) ||
                    gmul(diff[row1], 0x0e) == gmul(diff[row2], 0x0b) ||
                    gmul(diff[row1], 0x0b) == gmul(diff[row2], 0x0d) ||
                    gmul(diff[row1], 0x0d) == gmul(diff[row2], 0x09));
        case 2:
            return (gmul(diff[row1], 0x0d) == gmul(diff[row2], 0x09) ||
                    gmul(diff[row1], 0x09) == gmul(diff[row2], 0x0e) ||
                    gmul(diff[row1], 0x0e) == gmul(diff[row2], 0x0b) ||
                    gmul(diff[row1], 0x0b) == gmul(diff[row2], 0x0d));
        case 3:
            return (gmul(diff[row1], 0x0b) == gmul(diff[row2], 0x0d) ||
                    gmul(diff[row1], 0x0d) == gmul(diff[row2], 0x09) ||
                    gmul(diff[row1], 0x09) == gmul(diff[row2], 0x0e) ||
                    gmul(diff[row1], 0x0e) == gmul(diff[row2], 0x0b));
        default:
            return 0; // モードが無効な場合は0を返す
    }
    return 1;
}


//デバッグ用
void print_target_text(unsigned char *targetText[4][4]) {
    for (int i = 0; i < 4; i++) {
        printf("Model %d:\n", i + 1);
        printf("  Text 1: ");
        for (int j = 0; j < 16; j++) {
            printf("%02X ", targetText[i][0][j]);
        }
        printf("\n");
        
        printf("  Text 2: ");
        for (int j = 0; j < 16; j++) {
            printf("%02X ", targetText[i][1][j]);
        }
        printf("\n");
        printf("  Text 3: ");

        for (int j = 0; j < 16; j++) {
            printf("%02X ", targetText[i][2][j]);
        }

        printf("\n");
        printf("  Text 3: ");

        for (int j = 0; j < 16; j++) {
            printf("%02X ", targetText[i][3][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    //SubBytes後の各値
    unsigned char p1[16] = {};
    unsigned char p2[16] = {};
    unsigned char p3[16] = {};
    unsigned char p4[16] = {};

    unsigned char key[16] = {0};
    unsigned char mostPrimekey[16] = {0}; // 見つかった鍵を入れる配列
    int i;
    
    //配列[i][j] i にそれぞれのモデル　j に各モデルの平文
    unsigned char *targetText[4][4] = {
        {PLAIN_TEXT[0][0],PLAIN_TEXT[0][1], PLAIN_TEXT[1][0], PLAIN_TEXT[1][1]}, 
        {PLAIN_TEXT[2][0],PLAIN_TEXT[2][1], PLAIN_TEXT[3][0],PLAIN_TEXT[3][1] },
        {PLAIN_TEXT[4][0], PLAIN_TEXT[4][1], PLAIN_TEXT[5][0], PLAIN_TEXT[5][1]},
        {PLAIN_TEXT[8][0], PLAIN_TEXT[8][1], PLAIN_TEXT[9][0],PLAIN_TEXT[9][1]}
    };

    // デバッグ用
    print_target_text(targetText);

    int model[4][4] = {{0, 5, 10, 15}, {3, 4, 9, 14}, {2, 7, 8, 13}, {1, 6, 11, 12}}; //モデル1 ~ 4で捜査対象のインデックス

    for (int col = 0; col < 4; col++) {

        //各モデルの差分が生じる部分 ex) 0 5 10 15
        int a = model[col][0];
        int b = model[col][1];
        int c = model[col][2];
        int d = model[col][3];

        unsigned char *x = targetText[col][0];
        unsigned char *y = targetText[col][1];
        unsigned char *w = targetText[col][2];
        unsigned char *z = targetText[col][3];
    
        //  x と y の内容を出力して確認 デバッグ用
        printf("col %d:\n", col);
        printf("  x (Text 1): ");
        for (int j = 0; j < 16; j++) {
            printf("%02X ", x[j]);
        }
        printf("\n  y (Text 2): ");
        for (int j = 0; j < 16; j++) {
            printf("%02X ", y[j]);
        }
        printf("\n\n");

        // w と zもついでにデバッグ
        printf("col %d:\n", col);
        printf("  w (Text 1): ");
        for (int j = 0; j < 16; j++) {
            printf("%02X ", w[j]);
        }
        printf("\n  z (Text 2): ");
        for (int j = 0; j < 16; j++) {
            printf("%02X ", z[j]);
        }
        printf("\n\n");


        for (int k_1 = 0; k_1 < 256; k_1++) {
            for (int k_2 = 0; k_2 < 256; k_2++) {
                key[a] = (unsigned char)k_1;
                key[b] = (unsigned char)k_2;

                FirstEncrypt(p1, x, key);
                FirstEncrypt(p2, y, key);
                FirstEncrypt(p3, w, key);
                FirstEncrypt(p4, z, key);


                if (Ratio_Difference(p1, p2, a, b, 1) == 1 &&Ratio_Difference(p3, p4, a, b, 1) == 1) {
                    //鍵の中間報告
                    // printf("key_1: %02X key_2: %02X \n", (unsigned char)k_1, (unsigned char)k_2);
                    for (int k_3 = 0; k_3 < 256; k_3++) {
                        key[c] = (unsigned char)k_3;
                        FirstEncrypt(p1, x, key);
                        FirstEncrypt(p2, y, key);
                        FirstEncrypt(p3, w, key);
                        FirstEncrypt(p4, z, key);

                        if (Ratio_Difference(p1, p2, b, c, 2) == 1 &&Ratio_Difference(p3, p4, b, c, 2) == 1) {
                            //鍵の中間報告
                            // printf("key_1: %02X key_2: %02X, key_3: %02X \n", (unsigned char)k_1, (unsigned char)k_2, (unsigned char)k_3);

                            for (int k_4 = 0; k_4 < 256; k_4++) {
                                key[d] = (unsigned char)k_4;
                                FirstEncrypt(p1, x, key);
                                FirstEncrypt(p2, y, key);
                                FirstEncrypt(p3, w, key);
                                FirstEncrypt(p4, z, key);

                                if (Ratio_Difference(p1, p2, c, d, 3) == 1 && Ratio_Difference(p3, p4, c, d, 3) == 1) {
                                    //鍵の中間報告
                                    // printf("key_1: %02X key_2: %02X, key_3: %02X  key_4: %02X \n", (unsigned char)k_1, (unsigned char)k_2, (unsigned char)k_3, (unsigned char)k_4);
                                    mostPrimekey[a] = (unsigned char)k_1;
                                    mostPrimekey[b] = (unsigned char)k_2;
                                    mostPrimekey[c] = (unsigned char)k_3;
                                    mostPrimekey[d] = (unsigned char)k_4;
                                }
                            }
                        }
                    }
                    

                }
            }
        }
    }

    printf("やったーーー鍵が見つかった!!!! \n0x");
    for (i=0; i<16; i++){
        printf("%02X", mostPrimekey[i]);
    }
    printf("\n");

    return 0;
}
