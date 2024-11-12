#include <stdio.h>
#include <string.h>

// S-box look-up table
/**
 * @brief AES暗号化で使用されるS-Box（Substitution Box）テーブル
 *
 * `g_sbox` は、AES暗号化のSubBytesステップで使用される代入ボックス（S-Box）です。
 * 各バイトはこのテーブルを通じて非線形な置換が行われ、暗号強度を向上させます。
 * この配列のインデックスを指定することで、対応する置換値を得ることができます。
 *
 * @note
 * S-Boxは、AES暗号のセキュリティを高めるために設計されており、事前に計算された
 * 256バイトの配列です。
 *
 * 例:
 * @code
 * unsigned char input_byte = 0x53;
 * unsigned char output_byte = g_sbox[input_byte]; // output_byte は 0xed になります
 * @endcode
 */
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


/**
 * @brief AES復号化で使用される逆S-Box（Inverse Substitution Box）テーブル
 *
 * `g_inv_sbox` は、AES復号化の逆SubBytesステップで使用される逆代入ボックスです。
 * 復号化時に暗号化されたデータを元のデータに戻すために、この逆S-Boxが使用されます。
 * 各バイトはこのテーブルを通じて逆変換が行われます。
 *
 * @note
 * 逆S-Boxは、S-Boxの逆操作を実現するために設計された256バイトの配列です。
 *
 * 例:
 * @code
 * unsigned char encrypted_byte = 0xed;
 * unsigned char original_byte = g_inv_sbox[encrypted_byte]; // original_byte は 0x53 になります
 * @endcode
 */
const unsigned char g_inv_sbox[256] = {
    0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38,
    0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
    0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87,
    0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
    0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D,
    0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
    0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2,
    0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
    0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16,
    0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
    0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA,
    0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
    0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A,
    0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
    0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02,
    0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
    0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA,
    0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
    0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85,
    0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
    0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89,
    0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
    0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20,
    0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
    0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31,
    0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
    0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D,
    0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
    0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0,
    0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26,
    0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D,
};

/**
 * @brief AESで使用されるガロア体 GF(2^8) 上での乗算を行う関数
 *
 * この関数は、有限体 GF(2^8) における2つの8ビット符号なし整数
 * `a` と `b` の乗算を、不可約多項式 \( x^8 + x^4 + x^3 + x + 1 \)
 * （16進数で `0x1b` と表される）を使って実行します。
 * GF(2^8) 上での `a` と `b` の積を計算します。
 *
 * @param a 1つ目のオペランド（8ビット符号なし整数）
 * @param b 2つ目のオペランド（8ビット符号なし整数）
 * @return GF(2^8) 上での `a` と `b` の積
 *
 * @details
 * この関数はAES暗号化処理で使用されるGF(2^8)上での乗算アルゴリズムを
 * 実装しています。乗算はビットシフトと条件付きXOR操作を使って行われ、
 * 各ループで`b`の最下位ビットを確認し、それが1であれば`a`と積み上げた
 * 結果`p`をXORします。その後、`a`の上位ビットがセットされている場合、
 * `0x1b`とXORして、ビットシフトによる次の演算に備えます。
 *
 * @note
 * この関数はGF(2^8)の範囲内に結果を収めるため、オーバーフローが発生する
 * 場合に不可約多項式 `0x1b` を使って結果を縮小します。
 *
 * 例:
 * @code
 * unsigned char result = gmul(0x57, 0x83); // result は 0xc1 になります
 * @endcode
 */
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

/**
 * @brief AES暗号化で使用されるSubBytesステップを実行する関数
 *
 * この関数は、AES暗号化の各ラウンドで用いられるSubBytes操作を行います。
 * 入力データ `src` の各バイトをS-Box（代入ボックス）で置換し、その結果を
 * 出力バッファ `dst` に格納します。
 *
 * @param dst 置換後のデータを格納する出力バッファ（16バイト）
 * @param src 置換前の入力データ（16バイト）
 *
 * @details
 * SubBytesは、AES暗号化の1ラウンド内で非線形置換を行うステップです。
 * 各バイトはS-Box（`g_sbox`）を使用して別のバイトに置き換えられ、
 * 暗号強度を向上させます。この関数は、入力データ `src` の各バイトを
 * S-Boxで参照して変換し、結果を `dst` に格納します。
 *
 * @note
 * `src` と `dst` はどちらも16バイトで、暗号化対象の1ブロック（128ビット）
 * を表しています。`src` と `dst` が同じバッファでも動作します（インプレース
 * 実行可能）。
 *
 * 例:
 * @code
 * unsigned char src[16] = {0x32, 0x88, 0x31, 0xe0, ...};
 * unsigned char dst[16];
 * SubBytes(dst, src);
 * // dstにはSubBytesステップ後のデータが格納される
 * @endcode
 */
void SubBytes(unsigned char *dst, unsigned char *src) {
    for (int i = 0; i < 16; i++) {
        dst[i] = g_sbox[src[i]];
    }
}

/**
 * @brief AES復号化で使用されるinvSubBytesステップを実行する関数
 *
 * この関数は、AES復号化プロセスの各ラウンドで用いられる逆SubBytes操作を行います。
 * 入力データ `src` の各バイトを逆S-Box（逆代入ボックス）で置換し、その結果を
 * 出力バッファ `dst` に格納します。
 *
 * @param dst 逆置換後のデータを格納する出力バッファ（16バイト）
 * @param src 逆置換前の入力データ（16バイト）
 *
 * @details
 * invSubBytesは、AES復号化の1ラウンド内で行われる非線形逆置換ステップです。
 * 各バイトは逆S-Box（`g_inv_sbox`）を使用して元のバイトに戻されます。
 * この関数は、入力データ `src` の各バイトを逆S-Boxで参照して変換し、結果を `dst` に格納します。
 *
 * @note
 * `src` と `dst` はどちらも16バイトで、復号化対象の1ブロック（128ビット）を表しています。
 * `src` と `dst` が同じバッファでも動作します（インプレース実行可能）。
 *
 * 例:
 * @code
 * unsigned char src[16] = {0x7a, 0x89, 0x3b, 0xce, ...};
 * unsigned char dst[16];
 * invSubBytes(dst, src);
 * // dstには逆SubBytesステップ後のデータが格納される
 * @endcode
 */
void invSubBytes(unsigned char *dst, unsigned char *src) {
    for (int i = 0; i < 16; i++) {
        dst[i] = g_inv_sbox[src[i]];
    }
}


/**
 * @brief AES暗号化で使用されるShiftRowsステップを実行する関数
 *
 * この関数は、AES暗号化の各ラウンドで用いられるShiftRows操作を行います。
 * 入力データ `src` の各行を特定のバイト数だけ左にシフトし、その結果を
 * 出力バッファ `dst` に格納します。
 *
 * @param dst シフト後のデータを格納する出力バッファ（16バイト）
 * @param src シフト前の入力データ（16バイト）
 *
 * @details
 * ShiftRowsは、AES暗号化の1ラウンド内で行われる行のシフト操作です。
 * 4行構造の`state`配列に対し、1行目はそのまま、2行目は1バイト左に、
 * 3行目は2バイト左に、4行目は3バイト左にシフトします。
 * この操作により、AES暗号の拡散性が向上します。
 *
 * @note
 * `src` と `dst` はどちらも16バイトで、暗号化対象の1ブロック（128ビット）を表しています。
 * `src` と `dst` が同じバッファでも動作します（インプレース実行可能）。
 *
 * 例:
 * @code
 * unsigned char src[16] = {0x32, 0x88, 0x31, 0xe0, ...};
 * unsigned char dst[16];
 * ShiftRows(dst, src);
 * // dstにはShiftRowsステップ後のデータが格納される
 * @endcode
 */


void ShiftRows(unsigned char *dst, unsigned char *src) {
    unsigned char tmp;

    // 1st row
    dst[0] = src[0]; //1行目はそのまま
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

/**
 * @brief AES復号化で使用されるinvShiftRowsステップを実行する関数
 *
 * この関数は、AES復号化の各ラウンドで用いられる逆ShiftRows操作を行います。
 * 入力データ `src` の各行を特定のバイト数だけ右にシフトし、その結果を
 * 出力バッファ `dst` に格納します。
 *
 * @param dst 逆シフト後のデータを格納する出力バッファ（16バイト）
 * @param src 逆シフト前の入力データ（16バイト）
 *
 * @details
 * invShiftRowsは、AES復号化の1ラウンド内で行われる行の逆シフト操作です。
 * 4行構造の`state`配列に対し、1行目はそのまま、2行目は1バイト右に、
 * 3行目は2バイト右に、4行目は3バイト右にシフトします。
 * この操作により、暗号化時のShiftRows操作を逆に戻すことができます。
 *
 * @note
 * `src` と `dst` はどちらも16バイトで、復号化対象の1ブロック（128ビット）を表しています。
 * `src` と `dst` が同じバッファでも動作します（インプレース実行可能）。
 *
 * 例:
 * @code
 * unsigned char src[16] = {0x7a, 0x89, 0x3b, 0xce, ...};
 * unsigned char dst[16];
 * invShiftRows(dst, src);
 * // dstにはinvShiftRowsステップ後のデータが格納される
 * @endcode
 */
void invShiftRows(unsigned char *dst, unsigned char *src) {
    unsigned char tmp;

    // 1st row
    dst[0] = src[0];
    dst[4] = src[4];
    dst[8] = src[8];
    dst[12] = src[12];

    // 2nd row
    tmp = src[13];
    dst[13] = src[9];
    dst[9] = src[5];
    dst[5] = src[1];
    dst[1] = tmp;

    // 3rd row
    tmp = src[2];
    dst[2] = src[10];
    dst[10] = tmp;
    tmp = src[6];
    dst[6] = src[14];
    dst[14] = tmp;

    // 4th row
    tmp = src[3];
    dst[3] = src[7];
    dst[7] = src[11];
    dst[11] = src[15];
    dst[15] = tmp;
}


/**
 * @brief AES暗号化で使用されるMixColumnsステップを実行する関数
 *
 * この関数は、AES暗号化の各ラウンドで用いられるMixColumns操作を行います。
 * 入力データ `src` の各列をガロア体 GF(2^8) 上で定義された特定の多項式で
 * 変換し、その結果を出力バッファ `dst` に格納します。
 *
 * @param dst MixColumns変換後のデータを格納する出力バッファ（16バイト）
 * @param src 変換前の入力データ（16バイト）
 *
 * @details
 * MixColumnsは、AES暗号化の1ラウンド内で行われる列変換ステップです。
 * 4バイトごとの列に対し、ガロア体 GF(2^8) 上で定義された行列乗算を行い、
 * 暗号の拡散性を高めます。各列に対し、次の行列乗算を行います:
 *
 * \[
 * \begin{bmatrix}
 * 2 & 3 & 1 & 1 \\
 * 1 & 2 & 3 & 1 \\
 * 1 & 1 & 2 & 3 \\
 * 3 & 1 & 1 & 2 \\
 * \end{bmatrix}
 * \]
 *
 * @note
 * `src` と `dst` はどちらも16バイトで、暗号化対象の1ブロック（128ビット）を表しています。
 * `src` と `dst` が同じバッファでも動作します（インプレース実行可能）。
 *
 * 例:
 * @code
 * unsigned char src[16] = {0x87, 0x6e, 0x46, 0xa6, ...};
 * unsigned char dst[16];
 * MixColumns(dst, src);
 * // dstにはMixColumnsステップ後のデータが格納される
 * @endcode
 */
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


/**
 * @brief AES復号化で使用されるinvMixColumnsステップを実行する関数
 *
 * この関数は、AES復号化の各ラウンドで用いられる逆MixColumns操作を行います。
 * 入力データ `src` の各列に対し、ガロア体 GF(2^8) 上で定義された特定の多項式で
 * 逆変換を実行し、その結果を出力バッファ `dst` に格納します。
 *
 * @param dst invMixColumns変換後のデータを格納する出力バッファ（16バイト）
 * @param src 逆変換前の入力データ（16バイト）
 *
 * @details
 * invMixColumnsは、AES復号化の1ラウンド内で行われる列の逆変換ステップです。
 * 4バイトごとの列に対し、ガロア体 GF(2^8) 上で定義された行列の逆行列を使用して
 * 変換を行います。各列に対し、次の逆行列を使用します:
 *
 * \[
 * \begin{bmatrix}
 * 14 & 11 & 13 & 9 \\
 * 9 & 14 & 11 & 13 \\
 * 13 & 9 & 14 & 11 \\
 * 11 & 13 & 9 & 14 \\
 * \end{bmatrix}
 * \]
 *
 * @note
 * `src` と `dst` はどちらも16バイトで、復号化対象の1ブロック（128ビット）を表しています。
 * `src` と `dst` が同じバッファでも動作します（インプレース実行可能）。
 *
 * 例:
 * @code
 * unsigned char src[16] = {0x7a, 0x89, 0x3b, 0xce, ...};
 * unsigned char dst[16];
 * invMixColumns(dst, src);
 * // dstにはinvMixColumnsステップ後のデータが格納される
 * @endcode
 */
void invMixColumns(unsigned char *dst, unsigned char *src) {
    unsigned char tmp[16];
    tmp[0] = gmul(src[0],14)^gmul(src[1],11)^gmul(src[2],13)^gmul(src[3],9);
    tmp[1] = gmul(src[0],9)^gmul(src[1],14)^gmul(src[2],11)^gmul(src[3],13);
    tmp[2] = gmul(src[0],13)^gmul(src[1],9)^gmul(src[2],14)^gmul(src[3],11);
    tmp[3] = gmul(src[0],11)^gmul(src[1],13)^gmul(src[2],9)^gmul(src[3],14);

    tmp[4] = gmul(src[4],14)^gmul(src[5],11)^gmul(src[6],13)^gmul(src[7],9);
    tmp[5] = gmul(src[4],9)^gmul(src[5],14)^gmul(src[6],11)^gmul(src[7],13);
    tmp[6] = gmul(src[4],13)^gmul(src[5],9)^gmul(src[6],14)^gmul(src[7],11);
    tmp[7] = gmul(src[4],11)^gmul(src[5],13)^gmul(src[6],9)^gmul(src[7],14);

    tmp[8] = gmul(src[8],14)^gmul(src[9],11)^gmul(src[10],13)^gmul(src[11],9);
    tmp[9] = gmul(src[8],9)^gmul(src[9],14)^gmul(src[10],11)^gmul(src[11],13);
    tmp[10] = gmul(src[8],13)^gmul(src[9],9)^gmul(src[10],14)^gmul(src[11],11);
    tmp[11] = gmul(src[8],11)^gmul(src[9],13)^gmul(src[10],9)^gmul(src[11],14);

    tmp[12] = gmul(src[12],14)^gmul(src[13],11)^gmul(src[14],13)^gmul(src[15],9);
    tmp[13] = gmul(src[12],9)^gmul(src[13],14)^gmul(src[14],11)^gmul(src[15],13);
    tmp[14] = gmul(src[12],13)^gmul(src[13],9)^gmul(src[14],14)^gmul(src[15],11);
    tmp[15] = gmul(src[12],11)^gmul(src[13],13)^gmul(src[14],9)^gmul(src[15],14);

    memcpy(dst, tmp, 16);
}


/**
 * @brief AES暗号化および復号化で使用されるAddRoundKeyステップを実行する関数
 *
 * この関数は、AESの各ラウンドで用いられるAddRoundKey操作を行います。
 * 入力データ `src` とラウンド鍵 `key` の各バイトをXOR演算し、その結果を
 * 出力バッファ `dst` に格納します。
 *
 * @param dst AddRoundKey処理後のデータを格納する出力バッファ（16バイト）
 * @param src AddRoundKey処理前の入力データ（16バイト）
 * @param key 現在のラウンドで使用するラウンド鍵（16バイト）
 *
 * @details
 * AddRoundKeyは、AES暗号化および復号化の各ラウンドで行われる操作です。
 * 入力データ `src` の各バイトに対し、ラウンド鍵 `key` の対応するバイトと
 * XOR演算を行い、その結果を `dst` に格納します。これは、暗号化および復号化の
 * 両方で使用され、各ラウンドでの鍵の影響を反映させるための重要なステップです。
 *
 * @note
 * `src`、`dst`、および `key` はそれぞれ16バイトで、AESブロック（128ビット）を表します。
 * `src` と `dst` が同じバッファでも動作します（インプレース実行可能）。
 *
 * 例:
 * @code
 * unsigned char src[16] = {0x32, 0x88, 0x31, 0xe0, ...};
 * unsigned char key[16] = {0x2b, 0x7e, 0x15, 0x16, ...};
 * unsigned char dst[16];
 * AddRoundKey(dst, src, key);
 * // dstにはAddRoundKey処理後のデータが格納される
 * @endcode
 */
void AddRoundKey(unsigned char *dst, unsigned char *src, unsigned char *key) {
    for (int i = 0; i < 16; i++) {
        dst[i] = src[i] ^ key[i];
    }
}


/**
 * @brief AES暗号化および復号化で使用されるラウンド鍵を生成する関数
 *
 * この関数は、AES暗号化および復号化で各ラウンドに使用されるラウンド鍵（キースケジュール）を生成します。
 * 初期鍵 `init_key` を入力として、拡張された鍵 `w` にラウンドごとの鍵を格納します。
 *
 * @param w 拡張された鍵（キースケジュール）を格納する配列（176バイト）
 * @param init_key 初期鍵（16バイト）
 *
 * @details
 * KeyExpansionは、AESの各ラウンドで使用される鍵を生成する重要なステップです。
 * 初期鍵 `init_key` を元にして、11ラウンド分の鍵が格納されたキースケジュール `w` を生成します。
 * この関数はRotWord、SubWord、Rcon（ラウンド定数）を使用してラウンドごとの変換を行い、
 * 各ラウンドで異なる鍵が得られるようにします。
 *
 * - **RotWord**: 4バイトのワードを左に1バイト回転
 * - **SubWord**: 各バイトをS-Boxで置換
 * - **Rcon**: 各ラウンドで定義された定数を最上位バイトに適用
 *
 * @note
 * `w` は176バイトの配列で、AES-128の11ラウンド分の鍵（各16バイト）を格納します。
 *
 * 例:
 * @code
 * unsigned char init_key[16] = {0x2b, 0x7e, 0x15, 0x16, ...};
 * unsigned char w[176];
 * KeyExpansion(w, init_key);
 * // wには各ラウンドで使用されるラウンド鍵が格納される
 * @endcode
 */
void KeyExpansion(unsigned char *w, unsigned char *init_key) {
    unsigned char Rcon[16] = {
        0x01,0x01,0x02,0x04,0x08,0x10,0x20,0x40,
        0x80,0x1b,0x36,0x6c,0xd8,0xab,0x4d,0x9a,
    };
	unsigned char temp,temp1,temp2,temp3,temp4;
	const int Nb = 4;
	const int Nk = 4;
	const int Nr = 10;

    memcpy(w, init_key, 16);

	int i = 4*Nk;
    while(i < 4*Nb*(Nr+1)) {
        temp1 = w[i-4];
        temp2 = w[i-3];
        temp3 = w[i-2];
        temp4 = w[i-1];
	    if (i % (4*Nk) == 0){
            // RotWord and SubWord
            temp = temp1;
            temp1 = g_sbox[temp2];
            temp2 = g_sbox[temp3];
            temp3 = g_sbox[temp4];
            temp4 = g_sbox[temp];
            // Rcon
            temp1 = temp1 ^ Rcon[i/(4*Nk)];
            temp2 = temp2 ^ 0;
            temp3 = temp3 ^ 0;
            temp4 = temp4 ^ 0;
	    }
	    else if (Nk > 6 && (i%Nk) == 4) {
            //SubWord
            temp1 = g_sbox[temp1];
            temp2 = g_sbox[temp2];
            temp3 = g_sbox[temp3];
            temp4 = g_sbox[temp4];
		}
        w[i]   = w[i-4*Nk]   ^ temp1;
        w[i+1] = w[i-4*Nk+1] ^ temp2;
        w[i+2] = w[i-4*Nk+2] ^ temp3;
        w[i+3] = w[i-4*Nk+3] ^ temp4;
        i += 4;
    }
}


/**
 * @brief AES-128暗号化を実行する関数
 *
 * この関数は、AES-128アルゴリズムを用いて、16バイトの平文を暗号化し、
 * 暗号文を出力バッファ `dst` に格納します。
 *
 * @param dst 暗号化されたデータ（暗号文）を格納する出力バッファ（16バイト）
 * @param src 暗号化する入力データ（平文）（16バイト）
 * @param init_key 暗号化に使用する初期鍵（16バイト）
 *
 * @details
 * Encrypt関数は、AES-128の暗号化手順に従って、入力データ `src` を暗号化します。
 * 初期鍵 `init_key` を用いてキースケジュール `w` を生成し、以下の処理を行います：
 *
 * - **AddRoundKey**: 最初に初期ラウンド鍵を適用。
 * - **9ラウンドのメイン処理**: 各ラウンドでSubBytes、ShiftRows、MixColumns、AddRoundKeyを適用。
 * - **最終ラウンド**: SubBytes、ShiftRows、およびAddRoundKeyのみ適用（MixColumnsは適用しない）。
 *
 * @note
 * AES-128では、10ラウンドで暗号化が完了します。`dst`、`src`、`init_key`はすべて16バイトで、
 * AESの1ブロック（128ビット）を表しています。
 *
 * 例:
 * @code
 * unsigned char src[16] = {0x32, 0x88, 0x31, 0xe0, ...};
 * unsigned char dst[16];
 * unsigned char key[16] = {0x2b, 0x7e, 0x15, 0x16, ...};
 * Encrypt(dst, src, key);
 * // dstにはAES暗号化後の暗号文が格納される
 * @endcode
 */
void Encrypt(unsigned char *dst, unsigned char *src, unsigned char *init_key) {
    unsigned char key[16];
    unsigned char w[176];          // キースケジュール用
    unsigned char state[16];       // 暗号化する状態 (16バイト)
    
    // 1. 初期化 - キーと状態の準備
    memcpy(key, init_key, 16);     // 初期キーをコピー
    KeyExpansion(w, key);          // キー拡張（KeyExpansion）
    memcpy(state, src, 16);        // 入力を状態 (state) にコピー

    // 2. 初回の AddRoundKey
    AddRoundKey(state, state, w);

    // 3. 9ラウンドのメイン暗号処理
    for (int round = 1; round <= 9; round++) {
        SubBytes(state, state);                     // SubBytes
        ShiftRows(state, state);                    // ShiftRows
        AddRoundKey(state, state, w + (round * 16)); // ラウンドキーの適用
    }

    // 4. 最終ラウンド (MixColumnsなし)
    SubBytes(state, state);                         // SubBytes
    ShiftRows(state, state);                        // ShiftRows
    AddRoundKey(state, state, w + 160);             // 最後のラウンドキーを適用

    // 5. 結果をdstにコピー
    memcpy(dst, state, 16);
}

int main() {
    unsigned char P[16] = {0};
    unsigned char C[16] = {0};
    unsigned char Key[16] = {0};

    // unsigned char P[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    //                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    // unsigned char C[16] = {0};
    // unsigned char Key[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    //                          0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};


    Encrypt(C, P, Key);
    printf("暗号化結果 (暗号文 C):\n");
    for (int i = 0; i < 16; i++) {
        printf("%02x ", C[i]);
    }
    printf("\n");

    return 0;
}
