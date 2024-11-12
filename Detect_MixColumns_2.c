#include <stdio.h>
#include <string.h>
// #include なんちゃらが必要

#define COMB_SIZE 1080

//鍵のペア(2)
typedef struct 
{
    unsigned char key_3;
    unsigned char key_4;
    unsigned char key_9;
    unsigned char key_14;
} Combination_2;

Combination_2 Comb2[COMB_SIZE];
int count=1;

void Detect_MixColumns_2(unsigned char *src1, unsigned char *src2, unsigned char *key){

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
                        Comb2[count].key_3 = key[3];
                        Comb2[count].key_4 = key[4];
                        Comb2[count].key_9 = key[9];
                        Comb2[count].key_14 = key[14];
                    }
                    key[14] += 0x01;
                }
                key[9] += 0x01;
            }
            key[4] += 0x01;
        }
        key[3] += 0x01;
    }
    
}
