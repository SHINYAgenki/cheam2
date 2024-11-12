#include <stdio.h>
#include <string.h>
// #include なんちゃらが必要

#define COMB_SIZE 1080

//鍵のペア(4)
typedef struct 
{
    unsigned char key_1;
    unsigned char key_6;
    unsigned char key_11;
    unsigned char key_12;
} Combination_4;

Combination_4 Comb4[COMB_SIZE];
int count=1;

void Detect_MixColumns_4(unsigned char *src1, unsigned char *src2, unsigned char *key){

    for (int i = 1; i < 16; i++)
    {
        for (int j = 1; j < 16; j++)
        {
            for (int k = 1; k < 16; k++)
            {
                for (int l = 1; l < 16; l++)
                {
                    First_Encrypt(src1, src1, key);
                    First_Encrypt(src2, src2, key);
                    if ((src1[0]==src2[0] && src1[1]==src2[1] && src1[2]==src2[2] && src1[3]!=src2[3]) ||
                        (src1[0]==src2[0] && src1[1]==src2[1] && src1[3]==src2[3] && src1[2]!=src2[2]) ||
                        (src1[0]==src2[0] && src1[2]==src2[2] && src1[3]==src2[3] && src1[1]!=src2[1]) ||
                        (src1[1]==src2[1] && src1[2]==src2[2] && src1[3]==src2[3] && src1[0]!=src2[0]))
                    {
                        count += 1;
                        Comb4[count].key_1 = key[1];
                        Comb4[count].key_6 = key[6];
                        Comb4[count].key_11 = key[11];
                        Comb4[count].key_12 = key[12];
                    }
                    key[12] += 0x01;
                }
                key[11] += 0x01;
            }
            key[6] += 0x01;
        }
        key[1] += 0x01;
    }
    
}