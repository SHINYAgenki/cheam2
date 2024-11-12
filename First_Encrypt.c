#include <stdio.h>
#include <string.h>
// #include なんちゃらが必要

void First_Encrypt(unsigned char *dst, unsigned char *src, unsigned char *init_key) {
    unsigned char key[16]; // 初期鍵
    unsigned char state[16]; // ステート

    // ステートと鍵の初期化
    memcpy(state, src, 16);
    memcpy(key, init_key, 16);
    
    // 0 ラウンド目（初期鍵を加える）
    AddRoundKey(state, state, key);

    // 1 ラウンド目（MixColumns まで）
    for (int i = 1; i < 10; i++)
    {
        SubBytes(state, state);
        ShiftRows(state,state);
        MixColumns(state, state);
    }
    
    // dst への出力
    memcpy(dst, state, 16);
}