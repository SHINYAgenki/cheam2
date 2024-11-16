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


/**
 * @def MODE_1
 * @brief Δ1 : Δ2 の計算モード
 */

/**
 * @def MODE_2
 * @brief Δ2 : Δ3 の計算モード
 */

/**
 * @def MODE_3
 * @brief Δ3 : Δ4 の計算モード
 */
#define MODE_1 1 
#define MODE_2 2
#define MODE_3 3

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

// sbox用の配列を用意
unsigned char g_sbox[256];

void load_sbox_from_csv(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "CSVファイルを開けません: %s\n", filename);
        exit(1);
    }

    for (int i = 0; i < 256; i++) {
        unsigned int value;
        if (fscanf(file, "%2x,", &value) != 1) {
            fprintf(stderr, "CSVファイルの形式が正しくありません。\n");
            fclose(file);
            exit(1);
        }
        g_sbox[i] = (unsigned char)value;
    }

    fclose(file);
}

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


void FirstEncrypt(unsigned char dst[4][16], unsigned char src[4][16], unsigned char *init_key) {
    unsigned char state[4][16]; //16bite

    for (int i = 0; i < 4; i++) {
        AddRoundKey(state[i], src[i], init_key);
        SubBytes(dst[i], state[i]);
    }

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
            return 0;
    }
    return 1;
}

/* check_key_candidates */
int check_key_candidates(unsigned char temp_encrypted[4][16], unsigned char temp_value[4][16], unsigned char *key, int diffPositions[4], int mode) {
    FirstEncrypt(temp_encrypted, temp_value, key);
    return Ratio_Difference(temp_encrypted[0], temp_encrypted[1], diffPositions[mode - 1], diffPositions[mode], mode) &&
           Ratio_Difference(temp_encrypted[2], temp_encrypted[3], diffPositions[mode - 1], diffPositions[mode], mode);
}

/* find_key */
void find_key(unsigned char temp_encrypted[4][16], unsigned char temp_value[4][16], unsigned char *key, int diffPositions[4], unsigned char *mostPrimekey) {
    for (int k_1 = 0; k_1 < 256; k_1++) {
        key[diffPositions[0]] = (unsigned char)k_1;
        for (int k_2 = 0; k_2 < 256; k_2++) {
            key[diffPositions[1]] = (unsigned char)k_2;
            if (!check_key_candidates(temp_encrypted, temp_value, key, diffPositions, MODE_1)) continue;

            for (int k_3 = 0; k_3 < 256; k_3++) {
                key[diffPositions[2]] = (unsigned char)k_3;
                if (!check_key_candidates(temp_encrypted, temp_value, key, diffPositions, MODE_2)) continue;

                for (int k_4 = 0; k_4 < 256; k_4++) {
                    key[diffPositions[3]] = (unsigned char)k_4;
                    if (!check_key_candidates(temp_encrypted, temp_value, key, diffPositions, MODE_3)) continue;

                    mostPrimekey[diffPositions[0]] = (unsigned char)k_1;
                    mostPrimekey[diffPositions[1]] = (unsigned char)k_2;
                    mostPrimekey[diffPositions[2]] = (unsigned char)k_3;
                    mostPrimekey[diffPositions[3]] = (unsigned char)k_4;
                }
            }
        }
    }
}

/* main */
int main() {
    unsigned char temp_encrypted[4][16] = {};
    unsigned char key[16] = {0};
    unsigned char mostPrimekey[16] = {0};
    load_sbox_from_csv("g_sbox.csv");

    unsigned char *targetText[4][4] = {
        {PLAIN_TEXT[0][0], PLAIN_TEXT[0][1], PLAIN_TEXT[1][0], PLAIN_TEXT[1][1]},
        {PLAIN_TEXT[2][0], PLAIN_TEXT[2][1], PLAIN_TEXT[3][0], PLAIN_TEXT[3][1]},
        {PLAIN_TEXT[4][0], PLAIN_TEXT[4][1], PLAIN_TEXT[5][0], PLAIN_TEXT[5][1]},
        {PLAIN_TEXT[8][0], PLAIN_TEXT[8][1], PLAIN_TEXT[9][0], PLAIN_TEXT[9][1]}
    };

    int model[4][4] = {{0, 5, 10, 15}, {3, 4, 9, 14}, {2, 7, 8, 13}, {1, 6, 11, 12}};

    for (int col = 0; col < 4; col++) {
        int diffPositions[4];
        for (int i = 0; i < 4; i++) {
            diffPositions[i] = model[col][i];
        }

        unsigned char temp_value[4][16];
        for (int i = 0; i < 4; i++) {
            memcpy(temp_value[i], targetText[col][i], 16);
        }

        find_key(temp_encrypted, temp_value, key, diffPositions, mostPrimekey);
    }

    printf("やったーーー鍵が見つかった!!!! \n0x");
    for (int i = 0; i < 16; i++) {
        printf("%02X", mostPrimekey[i]);
    }
    printf("\n");

    return 0;
}