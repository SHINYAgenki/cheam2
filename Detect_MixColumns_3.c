#include <stdio.h>
#include <string.h>
// #include なんちゃらが必要

#define COMB_SIZE 1080

//鍵のペア(3)
typedef struct 
{
    unsigned char key_2;
    unsigned char key_7;
    unsigned char key_8;
    unsigned char key_13;
} Combination_3;

Combination_3 Comb3[COMB_SIZE];
int count=1;

void Detect_MixColumns_3(unsigned char *src1, unsigned char *src2, unsigned char *key){

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
                        Comb3[count].key_2 = key[2];
                        Comb3[count].key_7 = key[7];
                        Comb3[count].key_8 = key[8];
                        Comb3[count].key_13 = key[13];
                    }
                    key[13] += 0x01;
                }
                key[8] += 0x01;
            }
            key[7] += 0x01;
        }
        key[2] += 0x01;
    }
    
}