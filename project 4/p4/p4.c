/*

Author Gavin Starnes
Last Updated: 3/11/2019

 this program reads two sets of floats and indicies from file then convolved them and writes the output to file;
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct TData tdata;

struct TData
{
    int MinIndex, MaxIndex, Points;
    float *pValue;

};

//takes in a file pointer a char pointer for mode and a tdata structure pointer to store the values it then reads the file and stores the values in the tdata structure.
void getData(char* file, char* mode,tdata *input)
{
    printf("\nreading data from file %s",file);

    int min, max;
    FILE *fp = fopen(file, mode);
    float temp;
    if(fp == 0)
    {
        printf("fuck");
    }
    fscanf(fp,"%d",&min);

    while(!feof(fp))
    {
        fscanf(fp, "%d", &max);
        fscanf(fp, "%f",&temp);
    }

    rewind(fp);

    int size = max - min;

    float *pvalue = malloc(size * sizeof(float));

    int i = 0;

   // printf("\nmin: %d\nmax: %d",min,max);

    for(i = 0;i < size; i++)
    {
        int temp;
        fscanf(fp,"%d%f",&temp,&pvalue[i]);
       // printf("\nindex: %d value: %f",i,pvalue[i]);
    }

    input->MinIndex = min;
    input->MaxIndex = max;
    input->pValue = pvalue;
    input->Points = size;



}

// takes in a tdata structure pointer and writes the contents of the tdata structure to the file given by the file pointer that is passed in
void writeData(tdata* output, const char* file)
{
    FILE *fp;
    printf("\nwriting data to output.txt");
    fp = fopen(file,"w+");
    int i;
    int max = output->MaxIndex;

    for(i = output->MinIndex;i < max+1; i++)
    {
        fprintf(fp, "%d\t%.6f\n", i, output->pValue[i]);
        fflush(fp);
    }
    fclose(fp);

}

// takes in 2 tdata pointers and convolved the first structures data around the second
tdata convolve(tdata* input1, tdata* input2)
{
    int size1 = input1->Points;
    int size2 = input2->Points;
    int sizeOutput = size1+size2-1;
    tdata output;

    float values[sizeOutput];

    int j;

    for(j = 0; j < sizeOutput; j++)
    {
        int min1;
        int max1;

        if(j >= size1-1)
        {
            min1 = j- size1 - 1;
        }else{

            min1 = 0;

        }

        if(j < size2-1)
        {
            max1 = j;
        }else{

            max1 = size2 -1;

        }

        values[j] = 0;

        int i;
        for (i = min1; i <= max1; i++)
        {
            values[j] += input1->pValue[i] * input2->pValue[j - i];
        }
    }

    int k = 0;




    output.Points = sizeOutput;
    output.pValue = malloc(sizeOutput* sizeof(float));

    for(k = 0; k < sizeOutput+2; k++) {
        output.pValue[k] = values[k];
    }

    for(k = 0; k < sizeOutput; k++) {
        output.pValue[k] = values[k];
        printf("\n%f", output.pValue[k]);
    }

    output.MaxIndex = sizeOutput-1;
    output.MinIndex = 0;
return output;


}

//main function of the program takes in command line arguments for files and calls axillary functions to get data convolve the data and write out the data from the files.
int main(int argc, char* argv[]) {
    tdata Input[2], Output;

    char* file1 = argv[1];
    char* file2 = argv[2];
    char* output = argv[3];
    char* mode = "r";

   int i;
   for(i = 0;i < argc;i++) {
       printf("\n%s", argv[i]);
   }

    getData(file1, mode,&Input[0]);
    getData(file2, mode,&Input[1]);


    Output = convolve(&Input[0],&Input[1]);

    writeData(&Output, output);
    return 0;
}