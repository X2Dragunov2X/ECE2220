#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
 * Author: Gavin Starnes
 * Date 2/19/2019
 * this program takes in two characters and turns them into a 21,16 hamming codeword
 *
 * */

unsigned int bin1(unsigned int num, int bit)
{
    if(num & bit)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//this function takes in 5 ptrs that will hold the sum mod2 that specific parity bit and the code word to check parity of.
int parity(unsigned int word, unsigned int *parity1ptr, unsigned int *parity2ptr, unsigned int *parity3ptr, unsigned int *parity4ptr, unsigned int *parity5ptr)
{
    unsigned int parity1 = 0;
    unsigned int parity2 = 0;
    unsigned int parity3 = 0;
    unsigned int parity4 = 0;
    unsigned int parity5 = 0;
    int temp = 0;
    unsigned i;

    for (i = 1 << 31; i > 0; i = i / 2) {
        int index = (int) (log(i) / log(2)); // uses lagarithms to get bit index of codeword
        temp = bin1(word, i);

        //printf("%u",temp);

        switch(index)
        {
            case 0:
                parity1 += temp;
                break;

            case 1:
                parity2 += temp;
                break;

            case 2:
                parity2 += temp;
                parity1 += temp;
                break;

            case 3:
                parity3 += temp;
                break;

            case 4:
                parity3 += temp;
                parity1 += temp;
                break;

            case 5:
                parity3 += temp;
                parity2 += temp;
                break;

            case 6:
                parity3 += temp;
                parity2 += temp;
                break;

            case 7:
                parity4 += temp;
                break;

            case 8:
                parity4 += temp;
                parity1 += temp;
                break;

            case 9:
                parity4 += temp;
                parity2 += temp;
                break;

            case 10:
                parity4 += temp;
                parity2 += temp;
                parity1 += temp;
                break;

            case 11:
                parity4 += temp;
                parity3 += temp;
                break;

            case 12:
                parity4 += temp;
                parity3 += temp;
                parity1 += temp;
                break;

            case 13:
                parity4 += temp;
                parity3 += temp;
                parity2 += temp;
                break;

            case 14:
                parity4 += temp;
                parity3 += temp;
                parity2 += temp;
                parity1 += temp;
                break;

            case 15:
                parity5 += temp;
                break;

            case 16:
                parity5 += temp;
                parity1 += temp;
                break;

            case 17:
                parity5 += temp;
                parity2 += temp;
                break;

            case 18:
                parity5 += temp;
                parity2 += temp;
                parity1 += temp;
                break;

            case 19:
                parity5 += temp;
                parity3 += temp;
                break;

            case 20:
                parity5 += temp;
                parity3 += temp;
                parity1 += temp;
                break;

            default: break;
        }

    }

    //printf("\nparity1: %u \nparity2: %u \nparity3: %u \nparity4: %u \nparity5: %u", parity1, parity2, parity3, parity4, parity5);

    *parity1ptr = parity1%2;
    *parity2ptr = parity2%2;
    *parity3ptr = parity3%2;
    *parity4ptr = parity4%2;
    *parity5ptr = parity5%2;

    return 0;
}

// the main fucntion where input is takin and bit masking is done on the code word
int main()
{
    unsigned int infoWord = 0b00000000000000000000000000000000;
    unsigned int codeword = 0b00000000000000000000000000000000;
    unsigned int tempword = 0b00000000000000000000000000000000;
    unsigned int bitmask1 = 0b00000000000000000000000000000100;
    unsigned int bitmask2 = 0b00000000000000000000000001110000;
    unsigned int bitmask3 = 0b00000000000000000111111100000000;
    unsigned int bitmask4 = 0b00000000000111110000000000000000;
    unsigned int parity1 = 0;
    unsigned int parity2 = 0;
    unsigned int parity3 = 0;
    unsigned int parity4 = 0;
    unsigned int parity5 = 0;

    char char1 = 0;
    char char2 = 0;
    char clear = 0;

    int done = 0;

    do {
        printf("\nplease enter a character, or a 0 for each character to quit: ");
        scanf("%c",&char1);
        scanf("%c",&clear);
        printf("\nplease enter a character: ");
        scanf("%c",&char2);

        if(char1 == '0' && char2 == '0')
        {
            done = 1;
        }
        else

            {
            infoWord = infoWord | char2;    //character 1 is loaded into the infoword
            infoWord <<= 8;                 //character 1 is shifted over
            infoWord = infoWord | char1;    //character 2 is loaded

            tempword = infoWord << 2;
            tempword = tempword & bitmask1; //the code word is built using the a tempword and a series of bitmasks
            codeword = tempword | codeword;

            tempword = infoWord << 3;
            tempword = tempword & bitmask2;
            codeword = tempword | codeword;

            tempword = infoWord << 4;
            tempword = tempword & bitmask3;
            codeword = tempword | codeword;

            tempword = infoWord << 5;
            tempword = tempword & bitmask4;
            codeword = tempword | codeword;

            parity(codeword, &parity1, &parity2, &parity3, &parity4, &parity5); //parity is checked

            //parity is then set for all 5 parity bits

            //parity bit 1
            if (parity1 == 1) {
                tempword = 0x0001;
                codeword = codeword | tempword;
                //bin(codeword);

            }

            //parity bit 2
            if (parity2 == 1) {
                tempword = 0x0002;
                codeword = codeword | tempword;
                //bin(codeword);

            }

            //parity bit 3
            if (parity3 == 1) {
                tempword = 0x0008;
                codeword = codeword | tempword;
                //bin(codeword);

            }

            //parity bit 4
            if (parity4 == 1) {
                tempword = 0x0080;
                codeword = codeword | tempword;
                //bin(codeword);

            }

            //parity bit 5
            if (parity5 == 1) {
                tempword = 0x8000;
                codeword = codeword | tempword;
                //bin(codeword);

            }

            printf("\nHamming codeword: %08X", codeword);
            scanf("%c",&clear);

        }
    }while(!done);


    return 0;
}




