#include <stdio.h>
#include <string.h>
// #include なんちゃらが必要

#define COMB_SIZE 1080

//鍵のペア(1)
typedef struct 
{
    unsigned char key_0;
    unsigned char key_5;
    unsigned char key_10;
    unsigned char key_15;
} Combination_1;

Combination_1 Comb1[COMB_SIZE];
int count=1;

void Detect_MixColumns_1(unsigned char *src1, unsigned char *src2, unsigned char *key){

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
                        Comb1[count].key_0 = key[0];
                        Comb1[count].key_5 = key[5];
                        Comb1[count].key_10 = key[10];
                        Comb1[count].key_15 = key[15];
                    }
                    key[15] += 0x01;
                }
                key[10] += 0x01;
            }
            key[5] += 0x01;
        }
        key[0] += 0x01;
    }
    
}