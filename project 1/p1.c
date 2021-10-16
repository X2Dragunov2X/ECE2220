/*

Author Gavin Starnes
Last Updated: 1/22/2019

this program takes user input for a series of delta numbers
these delta numbers are then used to generate a series of 
lottery numbers.

random numbers are chosen for the winning values of the lottery numbers
and compared against the user's generated values to see if there is a match
earning a win. random winning numbers are generated 1million times and 
compared or until a win.

*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//takes in a string to display and returns an integer entered by the user
int getInt(const char* str)
{
    int num;
    printf("%s",str);
    scanf("%d", &num);

    return num;
}


// takes in a string, an upper bound, a lower bound, and a pointer to an array
// this funtion then calls a helper function to get input this input is then
// checked against the upper and lower bounds.
void getInput(const char* str, int *entry, int lowBound, int upBound)
{
    int done = 0;
    while(!done)
    {
        *entry = getInt(str);
        if(*entry <=upBound && *entry >=lowBound)
        {
            done = 1;
        }
        else
        {
            printf("\nincorrect entry please try again\n");
        }
    }
}

//swaps the values of to integer pointers
void swap(int *v1, int *v2)
{
    int temp = *v1;
    *v1 = *v2;
    *v2 = temp;
}
//takes in an integer array and an int of the size of the array
//uses bubbles sort  to sort an array of integers into ascending order, while
// using a helper function to swap the integer pointers
void sort(int arr[], int size)
{
    int i, j;
    j = 0;
    i = 0;
    for (; i < size-1; i++)
    {
        for (;j < size - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}


// takes in an array, its size, and a maximum value,
// the contents of the array are then summed and compared to the maximum value
// returns 1 for <= to max and 0 for > max.
int maxSum(int arr[], int size, int max)
{
    int total = 0;
    int i = 0;

    for(;i < size; i++)
    {
        total += arr[i];
    }

    if(total > max)
    {
        return 0;
    }

    return 1;

}

// takes in an integer array and its size
// randomizes the positions of the arrays contents
void shuffle(int arr[], int size)
{
    int i = size-1;
    srand(time(NULL));

    for(;i > 0; i--)
    {
        int index = rand() % (i+1);
        swap(&arr[i], &arr[index]);
    }


}

// takes in and array and its size
// prints the array
void print(int arr[], int size)
{
    int i = 0;
    for(;i < size; i++)
    {
        printf("%d, ", arr[i]);
    }
}

// takes in an array and its size
// calculates the delta numbers for the given array storing them in the
// array provided
void deltaNums(int arr[], int size)
{
    int i = 0;
    for(;i < size; i++)
    {
        if(i !=0)
        {
            arr[i] = arr[i] + arr[i-1];
        }
    }

}

// generates 5 random winning lottery numbers within the range of 0 and the
// maximum value
void winningNums(int arr[], int size, int MAX)
{
    srand(time(NULL));
    int i = 0;
    for(;i < size; i++)
    {
        int unique = 0;


        if(i == 0)
        {
            arr[i] = rand() % MAX;
        }
        else {
            while (!unique) {
                int repeat = 0;
                arr[i] = rand() % MAX;
		int j = i;
                for (;j >= 0; j--) {
                    if (arr[i] == arr[j - 1]) {
                        repeat++;
                    }
                }

                if (repeat == 0) {
                    unique = 1;
                }
            }
        }
    }

    sort(arr,size);
}

// the main funtion of the program
int main()
{


    int nums[] = {-1,-1,-1,-1,-1, -1};
    int lotteryNums[6];
    int size = sizeof(nums)/sizeof(nums[0]);
    int validInput;
    int won = 0;
    int count = 0;
    const int MAX = 50;

    do {
        getInput("please enter a number between 1 and 5 inclusive: ", &nums[0], 1, 5);
        getInput("\n please enter a number between 1 and 10 inclusive: ", &nums[1], 1, 10);
        getInput("\n please enter a number between 1 and 10 inclusive: ", &nums[2], 1, 10);
        getInput("\n please enter a number between 10 and 12 inclusive: ", &nums[3], 10, 12);
        getInput("\n please enter a number between 8 and 15 inclusive: ", &nums[4], 8, 15);
        getInput("\n please enter a number between 8 and 15 inclusive: ", &nums[5], 8, 15);

        validInput = maxSum(nums, size, MAX);

        if(!validInput)
        {
            printf("%s","input invalid, entries sum exceeds 50\n");
        }

    }while(!validInput);


    sort(nums, size);

    printf("\n your delta numbers are: ");

    print(nums, size);

    shuffle(nums,size);

    printf("\n your final delta sequence is: ");

    print(nums, size);

    deltaNums(nums, size);

    printf("\n Your lottery numbers are ");

    print(nums, size);

    while(!won)
    {

        winningNums(lotteryNums, size, MAX);
        int mismatch = 0;
	int i = 0;
        for(;i < size; i++)
        {
            if(lotteryNums[i] != nums[i])
            {
                mismatch++;
            }
        }

        if(mismatch == 0)
        {
            printf("\nwinning lottery numbers ");
            print(lotteryNums, size);
            printf("\nyouve won ");
            won = 1;
        }

        if(count == 1000000)
         {
	     printf("\n you havent wont any of a million lotterys\n");
             won = 1;
         }
        count++;
    }
    return 0;

}
