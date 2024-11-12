#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// S-box look-up table
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
    // irreducible polynomial: X^8 + X^4 + X^3 + X + 1
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

void SubBytes(unsigned char *dst, unsigned char *src) {
    for (int i = 0; i < 16; i++) {
        dst[i] = g_sbox[src[i]];
    }
}

void ShiftRows(unsigned char *dst, unsigned char *src) {
    unsigned char tmp;

    // 1st row
    dst[0] = src[0];
    dst[4] = src[4];
    dst[8] = src[8];
    dst[12] = src[12];

    // 2nd row
    tmp = src[1];
    dst[1] = src[5];
    dst[5] = src[9];
    dst[9] = src[13];
    dst[13] = tmp;

    // 3rd row
    tmp = src[2];
    dst[2] = src[10];
    dst[10] = tmp;
    tmp = src[6];
    dst[6] = src[14];
    dst[14] = tmp;

    // 4th row
    tmp = src[3];
    dst[3] = src[15];
    dst[15] = src[11];
    dst[11] = src[7];
    dst[7] = tmp;
}

void MixColumns(unsigned char *dst, unsigned char *src) {
    unsigned char tmp[16];
    tmp[0] = gmul(src[0],2)^gmul(src[1],3)^gmul(src[2],1)^gmul(src[3],1);
    tmp[1] = gmul(src[0],1)^gmul(src[1],2)^gmul(src[2],3)^gmul(src[3],1);
    tmp[2] = gmul(src[0],1)^gmul(src[1],1)^gmul(src[2],2)^gmul(src[3],3);
    tmp[3] = gmul(src[0],3)^gmul(src[1],1)^gmul(src[2],1)^gmul(src[3],2);

    tmp[4] = gmul(src[4],2)^gmul(src[5],3)^gmul(src[6],1)^gmul(src[7],1);
    tmp[5] = gmul(src[4],1)^gmul(src[5],2)^gmul(src[6],3)^gmul(src[7],1);
    tmp[6] = gmul(src[4],1)^gmul(src[5],1)^gmul(src[6],2)^gmul(src[7],3);
    tmp[7] = gmul(src[4],3)^gmul(src[5],1)^gmul(src[6],1)^gmul(src[7],2);

    tmp[8]  = gmul(src[8],2)^gmul(src[9],3)^gmul(src[10],1)^gmul(src[11],1);
    tmp[9]  = gmul(src[8],1)^gmul(src[9],2)^gmul(src[10],3)^gmul(src[11],1);
    tmp[10] = gmul(src[8],1)^gmul(src[9],1)^gmul(src[10],2)^gmul(src[11],3);
    tmp[11] = gmul(src[8],3)^gmul(src[9],1)^gmul(src[10],1)^gmul(src[11],2);

    tmp[12] = gmul(src[12],2)^gmul(src[13],3)^gmul(src[14],1)^gmul(src[15],1);
    tmp[13] = gmul(src[12],1)^gmul(src[13],2)^gmul(src[14],3)^gmul(src[15],1);
    tmp[14] = gmul(src[12],1)^gmul(src[13],1)^gmul(src[14],2)^gmul(src[15],3);
    tmp[15] = gmul(src[12],3)^gmul(src[13],1)^gmul(src[14],1)^gmul(src[15],2);

    memcpy(dst, tmp, 16);
}

void AddRoundKey(unsigned char *dst, unsigned char *src, unsigned char *key) {
    for (int i = 0; i < 16; i++) {
        dst[i] = src[i] ^ key[i];
    }
}

void Encrypt(unsigned char *dst, unsigned char *src, unsigned char *init_key) {
    unsigned char key[16]; //それぞれのラウンド鍵
    unsigned char state[16]; //16bite

    //暗号(Cipher)
    //0ラウンド
    AddRoundKey(state, src, init_key); 

    //1ラウンドのSubBytesまで
    SubBytes(dst, state);
    //ShiftRows(state, state);
    //MixColumns(dst, state);
    //AddRoundKey(dst, state, key);
}

//差分し、差分された中間値について比を確認する関数
int sabun0(unsigned char *pt1, unsigned char *pt2, int a1, int a2){
    unsigned char pt[16] = {0};

    //PT1，PT2の差分をPTに格納
    for (int i=0; i<=15; i++){
        pt[i] = pt1[i]^pt2[i];
    }
    //PTのpt[a1]とpt[a2]について、比を確認する
    if( gmul( pt[a1], 0x09 ) == gmul( pt[a2], 0x0e ) || 
        gmul( pt[a1], 0x0e ) == gmul( pt[a2], 0x0b ) || 
        gmul( pt[a1], 0x0b ) == gmul( pt[a2], 0x0d ) || 
        gmul( pt[a1], 0x0d ) == gmul( pt[a2], 0x09 ) ){
        return 1;
    }
    return 0;
}


int main() {
    //1列目
    //平文ペア1つ目
    unsigned char P1[16] = {0xb6,0x0d,0x85,0x52, 0x63,0x95,0xcf,0x3f, 
                           0x02,0x5d,0xf6,0x8f, 0xea,0x49,0x8b,0xaa}; //平文P1
    unsigned char P2[16] = {0xa6,0x0d,0x85,0x52, 0x63,0x7a,0xcf,0x3f, 
                           0x02,0x5d,0x56,0x8f, 0xea,0x49,0x8b,0xd8}; //平文P2
    //平文ペア2つ目
    unsigned char P3[16] = {0x56,0x11,0xd8,0x06, 0x3f,0x37,0x1e,0x3d, 
                           0x3f,0x9e,0x8f,0x32, 0x47,0x62,0x34,0x5b}; //平文P3
    unsigned char P4[16] = {0x1d,0x11,0xd8,0x06, 0x3f,0x8a,0x1e,0x3d, 
                           0x3f,0x9e,0x7c,0x32, 0x47,0x62,0x34,0xac}; //平文P3

    //2列目
    //平文ペア1つ目
    unsigned char P5[16] = {0x43,0xB3,0xA2,0x5F,0x2B,0x9A,0xAB,0xAA,
                            0xCE,0x4B,0x16,0xD7,0x9A,0xA7,0xED,0xE3}; //平文P5
    unsigned char P6[16] = {0x43,0xB3,0xA2,0x62,0x06,0x9A,0xAB,0xAA,
                            0xCE,0xC7,0x16,0xD7,0x9A,0xA7,0x09,0xE3}; //平文P6
    //平文ペア2つ目
    unsigned char P7[16] = {0x85,0x01,0x4A,0x7F,0xC5,0xD8,0xB4,0x21,
                            0x0F,0x0D,0x32,0x45,0xC9,0x09,0xC9,0x6E}; //平文P7
    unsigned char P8[16] = {0x85,0x01,0x4A,0x30,0x40,0xD8,0xB4,0x21,
                            0x0F,0x38,0x32,0x45,0xC9,0x09,0xEB,0x6E}; //平文P8

    //3列目
    //平文ペア1つ目
    unsigned char P9[16] = {0x19,0x82,0x89,0xBD,0x27,0x44,0x09,0xFA,
                            0x4B,0xA3,0x7A,0xC5,0x5E,0x39,0x8D,0xA3}; //平文P9
    unsigned char P10[16] = {0x19,0x82,0x7B,0xBD,0x27,0x44,0x09,0x26,
                            0xDA,0xA3,0x7A,0xC5,0x5E,0x66,0x8D,0xA3}; //平文P10
    //平文ペア2つ目
    unsigned char P11[16] = {0x1A,0xFC,0xD2,0x31,0x3C,0xC2,0x60,0xF5,
                            0x07,0xB1,0xDD,0x9E,0x7B,0x23,0x01,0x0D}; //平文P11
    unsigned char P12[16] = {0x1A,0xFC,0xE6,0x31,0x3C,0xC2,0x60,0x30,
                            0x6D,0xB1,0xDD,0x9E,0x7B,0x85,0x01,0x0D}; //平文P12

    //4列目
    //平文ペア1つ目
    unsigned char P13[16] = {0x66,0x32,0x07,0x28,0x93,0x35,0xB7,0x79,
                            0x4C,0xE0,0x58,0x13,0x9C,0x5D,0xC0,0xDD}; //平文P13
    unsigned char P14[16] = {0x66,0xCE,0x07,0x28,0x93,0x35,0x4E,0x79,
                            0x4C,0xE0,0x58,0x81,0x15,0x5D,0xC0,0xDD}; //平文P14
    //平文ペア2つ目
    unsigned char P15[16] = {0xE1,0xCD,0xAD,0xD6,0xBB,0x4B,0x3D,0x0D,
                            0x02,0xD9,0xE7,0xC1,0x8A,0x19,0x07,0x0A}; //平文P15
    unsigned char P16[16] = {0xE1,0x64,0xAD,0xD6,0xBB,0x4B,0x15,0x0D,
                            0x02,0xD9,0xE7,0x19,0xA9,0x19,0x07,0x0A}; //平文P16

    //1ラウンド目SubBytes処理終了後の中間値
    unsigned char C1[16] = {};
    unsigned char C2[16] = {};
    unsigned char C3[16] = {};
    unsigned char C4[16] = {};

    unsigned char Key[16] = {0}; //候補鍵
    unsigned char last_key[16] = {0}; //条件に当てはまる鍵

    int state1, state2, state3, state4;
    int i;  

    //1~4列目に差分があるときSubytes処理後に影響がある位置をそれぞれ示す
    int type[4][4] = {{0, 5, 10, 15}, {4, 9, 14, 3}, {8, 13, 2, 7}, {12, 1, 6, 11}};
    //探索に使用する平文ペアを2ペアずつにまとめた 
    unsigned char *pList[4][4] = { {P1, P2, P3, P4},
                                   {P5, P6, P7, P8},
                                   {P9, P10, P11, P12},
                                   {P13, P14, P15, P16}};

    //行columnごとに探索を行う
    for (int column = 0; column <4; column++){
        int a = type[column][0];
        int b = type[column][1];
        int c = type[column][2];
        int d = type[column][3];

        unsigned char *X = pList[column][0];
        unsigned char *Y = pList[column][1];
        unsigned char *Z = pList[column][2];
        unsigned char *W = pList[column][3];

        //以下候補鍵を探す
        //鍵のKey[a],Key[b]にまず0から255までfor文をまわす
        for ( state1=0; state1<=255; state1++ ){
            for( state2=0; state2<=255; state2++ ){
                Key[a] = (unsigned char)state1;
                Key[b] = (unsigned char)state2;
                //Key[a],Key[b]が入力されたKeyについて、Encryptする
                Encrypt(C1, X, Key);
                Encrypt(C2, Y, Key);
                Encrypt(C3, Z, Key);
                Encrypt(C4, W, Key);

                //Subbytes後のC1,C2の差分,C3,C4の差分について、1行目と1行目の比がKeyとして適しているか判定する
                //適していない場合、次のKeyを試す
                if ( sabun0( C1, C2 , a, b ) == 1  && sabun0 ( C3, C4, a, b ) == 1 ){
                    //比が適していいた場合、Key[c]も0から255までfor文をまわす
                    for( state3=0; state3<=255; state3++ ){
                        Key[c] = (unsigned char)state3;
                        //Key[c]を追加しているため、再度Encryptする
                        Encrypt(C1, X, Key);
                        Encrypt(C2, Y, Key);
                        Encrypt(C3, Z, Key);
                        Encrypt(C4, W, Key);

                        //同様に、Subbytes後のC1,C2の差分,C3,C4の差分について、2行目と3行目の比がKeyとして適しているか判定する
                        if ( sabun0 ( C1, C2, b, c ) == 1 && sabun0 ( C3, C4, b, c ) == 1 ){
                            //比が適していいた場合、Key[d]も0から255までfor文をわます
                            for( state4=0; state4<=255; state4++ ){
                                Key[d] = (unsigned char)state4;
                                //Key[a]~[d]が入力されたKeyで、再度Encryptする
                                Encrypt(C1, X, Key);
                                Encrypt(C2, Y, Key);
                                Encrypt(C3, Z, Key);
                                Encrypt(C4, W, Key);

                                //同様に、Subbytes後のC1,C2の差分,C3,C4の差分について、3行目と4行目の比がKeyとして適しているか判定する
                                if ( sabun0 ( C1, C2, c, d ) == 1  && sabun0 ( C3, C4, c, d ) == 1 ){
                                    //比が適していた場合、候補鍵Keyではなく、鍵last_keyとして入力
                                    last_key[a] = (unsigned char)state1;
                                    last_key[b] = (unsigned char)state2;
                                    last_key[c] = (unsigned char)state3;
                                    last_key[d] = (unsigned char)state4;

                                    //条件に合った鍵last_keyの出力
                                    printf("鍵：0x");
                                    for (i=0; i<16; i++){
                                        printf("%X", last_key[i]);
                                    }
                                    printf("\n");
                                }
                            }
                        }
                    }
                }
            }
        }                     
    }           
}
