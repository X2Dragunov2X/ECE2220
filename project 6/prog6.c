#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct HEADER Header;
typedef struct INFOHEADER InfoHeader;
typedef struct PIXEL Pixel;

#define INTSIZE 4
#define SHORTSIZE 2
#define CHARSIZE 1




struct HEADER
{
    unsigned short int Type;                 // magic identifier
    unsigned int Size;                       //file size in bytes
    unsigned short int Reserved1, Reserved2;
    unsigned int Offset;                     // offset to data in bytes
};

struct INFOHEADER
{
    unsigned int Size;
    int Width, Height;
    unsigned short int Planes;
    unsigned short int Bits;
    unsigned int Compression;
    unsigned int ImageSize;
    int xResolution, yResolution;
    unsigned int Colors;
    unsigned int ImportantColors;
};

struct PIXEL
{
    unsigned char Red, Green, Blue;
};



void printImage(InfoHeader* infoHeader, Pixel** pixelArr)
{
    int rows = infoHeader->Height;
    int cols = infoHeader->Width;
    int i;
    int j;

    for(i = 0; i < rows; i++)
    {
        for(j = 0; j < cols; j++)
        {
            printf("R: %3d",pixelArr[i][j].Red);
            printf(" G: %3d",pixelArr[i][j].Green);
            printf(" B: %3d",pixelArr[i][j].Blue);
        }
        printf("\n");
    }
}

Pixel** readFile(FILE* fp,Header* header,InfoHeader* infoHeader)
{

    if(fp == NULL)
    {
        printf("file not found\n");
    }

    fread(&header->Type,SHORTSIZE,1,fp);
    fread(&header->Size,INTSIZE,1,fp);
    fread(&header->Reserved1,SHORTSIZE,1,fp);
    fread(&header->Reserved2,SHORTSIZE,1,fp);
    fread(&header->Offset,INTSIZE,1,fp);

    fread(&infoHeader->Size,INTSIZE,1,fp);
    fread(&infoHeader->Width,INTSIZE,1,fp);
    fread(&infoHeader->Height,INTSIZE,1,fp);
    fread(&infoHeader->Planes,SHORTSIZE,1,fp);
    fread(&infoHeader->Bits,SHORTSIZE,1,fp);
    fread(&infoHeader->Compression,INTSIZE,1,fp);
    fread(&infoHeader->ImageSize,INTSIZE,1,fp);
    fread(&infoHeader->xResolution,INTSIZE,1,fp);
    fread(&infoHeader->yResolution,INTSIZE,1,fp);
    fread(&infoHeader->Colors,INTSIZE,1,fp);
    fread(&infoHeader->ImportantColors,INTSIZE,1,fp);

    int rows = infoHeader->Height;
    int cols = infoHeader->Width;


    Pixel** pixelArr = malloc(rows * sizeof(Pixel*));

    int i;
    for (i = 0; i < rows; i++)
    {
        pixelArr[i] = (Pixel*) malloc(cols * sizeof(Pixel));
    }


    int j;
    for(i = 0; i < rows; i++)
    {
        for(j = 0; j < cols; j++)
        {
            fread(&pixelArr[i][j].Blue,CHARSIZE,1,fp);
            fread(&pixelArr[i][j].Green,CHARSIZE,1,fp);
            fread(&pixelArr[i][j].Red,CHARSIZE,1,fp);
        }
    }

    fclose(fp);


    return pixelArr;

}

void writeOut(FILE* fp, Header* header, InfoHeader* infoHeader, Pixel** pixelArr)
{
    if(fp == NULL)
    {
        printf("file not found\n");
    }

    fwrite(&header->Type,SHORTSIZE,1,fp);
    fwrite(&header->Size,INTSIZE,1,fp);
    fwrite(&header->Reserved1,SHORTSIZE,1,fp);
    fwrite(&header->Reserved2,SHORTSIZE,1,fp);
    fwrite(&header->Offset,INTSIZE,1,fp);

    fwrite(&infoHeader->Size,INTSIZE,1,fp);
    fwrite(&infoHeader->Width,INTSIZE,1,fp);
    fwrite(&infoHeader->Height,INTSIZE,1,fp);
    fwrite(&infoHeader->Planes,SHORTSIZE,1,fp);
    fwrite(&infoHeader->Bits,SHORTSIZE,1,fp);
    fwrite(&infoHeader->Compression,INTSIZE,1,fp);
    fwrite(&infoHeader->ImageSize,INTSIZE,1,fp);
    fwrite(&infoHeader->xResolution,INTSIZE,1,fp);
    fwrite(&infoHeader->yResolution,INTSIZE,1,fp);
    fwrite(&infoHeader->Colors,INTSIZE,1,fp);
    fwrite(&infoHeader->ImportantColors,INTSIZE,1,fp);

    int rows = infoHeader->Height;
    int cols = infoHeader->Width;


    int i;
    int j;
    for(i = 0; i < rows; i++)
    {
        for(j = 0; j < cols; j++)
        {
            fwrite(&pixelArr[i][j].Red,CHARSIZE,1,fp);
            fwrite(&pixelArr[i][j].Green,CHARSIZE,1,fp);
            fwrite(&pixelArr[i][j].Blue,CHARSIZE,1,fp);
        }
    }

     fclose(fp);

}

void printHeaders(const char* argv, Header* header, InfoHeader* infoHeader)
{
    printf("string: %s\n",argv);
    printf("type: %hu\n", header->Type);
    printf("size: %u\n", header->Size);
    printf("reserved1: %hu\n", header->Reserved1);
    printf("reserved2: %hu\n", header->Reserved1);
    printf("offset: %u\n", header->Offset);
    printf("size2 : %u\n", infoHeader->Size);
    printf("Width : %d\n", infoHeader->Width);
    printf("Height : %d\n", infoHeader->Height);
    printf("Planes : %hu\n", infoHeader->Planes);
    printf("Bits : %hu\n", infoHeader->Bits);
    printf("Compression : %u\n", infoHeader->Compression);
    printf("ImageSize : %u\n", infoHeader->ImageSize);
    printf("xResolution : %d\n", infoHeader->xResolution);
    printf("yResolution : %d\n", infoHeader->yResolution);
    printf("Colors : %u\n", infoHeader->Colors);
    printf("Importantcolors : %u\n", infoHeader->ImportantColors);
}

void shader(Pixel** pixelArr,int rows, int cols, short int R, short int G, short int B)
{
    int i;
    int j;



    for(i = 0; i < rows; i++) {
        for (j =0; j < cols; j++)
            {

                if ((pixelArr[i][j].Red + R >= 255))
                {
                    pixelArr[i][j].Red = 255;
                } else if((pixelArr[i][j].Red + R <= 0))
                {
                    pixelArr[i][j].Red = 0;
                }else
                {
                    pixelArr[i][j].Red += R;
                }


                if ((pixelArr[i][j].Green + G >= 255))
                {
                    pixelArr[i][j].Green = 255;
                } else if((pixelArr[i][j].Green + G <= 0))
                {
                    pixelArr[i][j].Green = 0;
                }else
                {
                    pixelArr[i][j].Green += G;
                }


                if ((pixelArr[i][j].Blue + B >= 255))
                {
                    pixelArr[i][j].Blue = 255;
                } else if((pixelArr[i][j].Blue + B <= 0))
                {
                    pixelArr[i][j].Blue = 0;
                }else
                {
                    pixelArr[i][j].Blue += B;
                }


        }
    }
}

void edgeDectect(InfoHeader* infoHeader, Pixel** pixelArr)
{
    char Matrix[3][3] =
            {
                    { 0,-1, 0},
                    {-1, 4,-1},
                    { 0,-1, 0}
            };

    int rows = infoHeader->Height;
    int cols = infoHeader->Width;
    char R = 0;
    char G = 0;
    char B = 0;

    int i;
    int j;

    for(i = 0; i < rows; i++) {
        for ( j = 0; j < cols; j++) {

            if(i < 1 || j < 1)
            {
                continue;
            }

            if(i > 198 || j > 299)
            {
                continue;
            }



            R += R + (pixelArr[i - 1][j - 1].Red * Matrix[0][0]);
            G += G + (pixelArr[i - 1][j - 1].Green * Matrix[0][0]);
            B += B + (pixelArr[i - 1][j - 1].Blue * Matrix[0][0]);

            R += R + (pixelArr[i - 1][j].Red * Matrix[0][1]);
            G += G + (pixelArr[i - 1][j].Green * Matrix[0][1]);
            B += B + (pixelArr[i - 1][j].Blue * Matrix[0][1]);

            R += R + (pixelArr[i - 1][j + 1].Red * Matrix[0][2]);
            G += G + (pixelArr[i - 1][j + 1].Green * Matrix[0][2]);
            B += B + (pixelArr[i - 1][j + 1].Blue * Matrix[0][2]);




            R += R + (pixelArr[i][j - 1].Red * Matrix[1][0]);
            G += G + (pixelArr[i][j - 1].Green * Matrix[1][0]);
            B += B + (pixelArr[i][j - 1].Blue * Matrix[1][0]);

            R += R + (pixelArr[i][j].Red * Matrix[1][1]);
            G += G + (pixelArr[i][j].Green * Matrix[1][1]);
            B += B + (pixelArr[i][j].Blue * Matrix[1][1]);

            R += R + (pixelArr[i][j + 1].Red * Matrix[1][2]);
            G += G + (pixelArr[i][j + 1].Green * Matrix[1][2]);
            B += B + (pixelArr[i][j + 1].Blue * Matrix[1][2]);




            R += R + (pixelArr[i + 1][j - 1].Red * Matrix[2][0]);
            G += G + (pixelArr[i + 1][j - 1].Green * Matrix[2][0]);
            B += B + (pixelArr[i + 1][j - 1].Blue * Matrix[2][0]);

            R += R + (pixelArr[i + 1][j].Red * Matrix[2][1]);
            G += G + (pixelArr[i + 1][j].Green * Matrix[2][1]);
            B += B + (pixelArr[i + 1][j].Blue * Matrix[2][1]);

            R = R + (pixelArr[i + 1][j + 1].Red * Matrix[2][2]);
            G = G + (pixelArr[i + 1][j + 1].Green * Matrix[2][2]);
            B = B + (pixelArr[i + 1][j + 1].Blue * Matrix[2][2]);

            pixelArr[i][j].Red = R;
            pixelArr[i][j].Green = G;
            pixelArr[i][j].Blue = B;
        }
    }
}



int main(int argc, char** argv)
{
    Header *header =(Header*) malloc(14);
    InfoHeader *infoHeader =(InfoHeader*) malloc(40);
    short int B = atoi(argv[2]);
    short int G = atoi(argv[3]);
    short int R = atoi(argv[4]);
    const char* inFp;
    inFp = argv[1];


    FILE *fp = fopen(inFp,"rb");
    printf("\nreading in file %s\n", inFp);
    Pixel** pixelArr = readFile(fp,header,infoHeader);
    fp = fopen(inFp,"rb");
    Pixel** pixelArr2 = readFile(fp,header,infoHeader);

    printf("\nshading image\n");
    shader(pixelArr, infoHeader->Height, infoHeader->Width, R, G, B);

    char* outshade =argv[1];
    strcpy(&outshade[strlen(outshade)-4],"(shade).bmp");

    printf("\nwriting out file %s\n", outshade);
    FILE* outfp = fopen(outshade, "wb");
    writeOut(outfp, header, infoHeader, pixelArr);

    printf("\nedge detecting image\n");
    edgeDectect(infoHeader,pixelArr2);

    char* outedge = argv[1];
    strcpy(&outedge[strlen(outedge)-11],"(edge).bmp");

    printf("\nwriting out file %s\n", outedge);
    outfp = fopen(outedge, "wb");
    writeOut(outfp, header, infoHeader, pixelArr2);

    printf("\nDone!\n");

    return 0;

}