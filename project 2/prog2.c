
/*

Author Gavin Starnes
Last Updated: 2/4/2019

 this program takes in a number of inputs regarding a politician and displays them, clears them
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define true 1
#define false 0




// prints the menu for the program
void printMenu()
{
    const char* menuItems ="\n1) Enter name:\n"
                           "2) Enter Years, Party, Office, and State:\n"
                           "3) Enter Age and Sex:\n"
                           "4) Enter Contacts:\n"
                           "5) Enter contributions and Lies:\n"
                           "6) Display the Data:\n"
                           "7) Clear All Data:\n"
                           "8) Quit:\n";
    printf("%s", menuItems);
}


//displays the politicians information
void displayInfo(char* middleInitial,char sex,char* state,char* suffix,char* twitter,char* firstName,char* lastName,char* office,int years,int age,int party, long int phone, double contributions, double lies)
{
    double contribPerLie = contributions/lies;
    printf("\nName: %-s, %-s, %-s, %-s \n", lastName, firstName, middleInitial, suffix);
    printf("Years in office: %-2d\nParty: %-d\nOffice: %-32s\nState: %-2s\n", years, party, office, state);
    printf("Age: %-3d Sex: %c\n", age, sex);
    printf("Phone: %ld Twitter: @%s\n", phone, twitter);
    printf("Contributions: %-lf Lies Told: %-lf\n", contributions, lies);
    printf("%-.2lf$/lie\n", contribPerLie);

}

//takes in 4 char* for the first and last name middle initial and suffix then gets user input for each and assigns it to that pointer
void getName(char* firstName,char* lastName,char* middleInitial,char* suffix)
{
    printf("please enter your first name \nyour last name \nyour middle initial or -1 if you dont have one \nthen your suffix or -1 if you dont have one\n ignore the above menu");
    fflush(stdin);
    scanf("%31[^\n, ] %31[^\n, ] %c %3[^\n, ]", firstName,lastName, middleInitial, suffix);
}

//takes in 4 pointers one for years in office one for party one for office and one for state then gets user input for each and assigns it to that pointer
void getPoliInfo(int* years, int* party, char* office, char* state)
{
    int done = false;
    char *states = "AL, AK, AZ, AR, CA, CO, CT, DE, FL, GA\nHI, ID, IL, IN, IA, KS, KY, LA, ME, MD\nMA, MI, MN, MS, MO, MT, NE, NV, NH, NJ\nNM, NY, NC, ND, OH, OK, OR, PA, RI, SC\nSD, TN, TX, UT, VT, VA, WA, WV, WI, WY: ";

    do {
        printf("please enter the number of years spent in office: ");
        scanf("%d", years);
        if(*years < 0 || *years > 99)
        {
            printf("incorrect entry please enter a number between 0 - 99\n");
        }
        else
        {
            done = true;
        }
    }while(!done);
    done = false;

    do {
        printf("please enter the party 1-Constitution, 2-Democrat, 3-Green, 4-Libertarian, 5-Republican: ");
        scanf("%d", party);
        if(*party <= 0 || *party > 99)
        {
            printf("incorrect entry please enter a number between 1 - 5 to indicate the corresponding party\n");
        }
        else
        {
            done = true;
        }
    }while(!done);


    printf("please enter the office held: ");
    scanf("%31s", office);

    printf("please enter the state from the list provided: \n");
    printf("%s", states);
    scanf("%2s", state);


}

//gets an int from the user for age checking its within bounds and returns it
int getAge()
{
    int age;
    int done = false;
    do {
        printf("please enter your age between 21 - 99: ");
        scanf("%d", &age);
        if(age < 21 || age > 99)
        {
            printf("incorrect age please enter an age between 21 - 99\n");
        }
        else
        {
            done = true;
        }
    }while(!done);
    return age;
}
//gets a char from the user for sex checking its within bounds and returns it
char getSex()
{
    char sex;
    int done = false;
    do {

        printf("please enter your sex M or F: ");
        scanf("%c", &sex);

        if(sex == 'M' || sex == 'm' || sex == 'f' || sex =='F')
        {
            done = true;
        }
        else
        {
            printf("incorrect entry, please enter either M or F\n");
        }
    }while(!done);
    return sex;
}
//takes in a char* gets user input for their twitter handle and assigns it to the pointer
void getTwitter(char* twitter)
{
    printf("please enter your twitter handle excluding the @: ");
    scanf("%15s",twitter);
}
//gets an int from the user for the phone number checking its within bounds and returns it
long int getPhone()
{
   long int phone;
    int done = false;
    do {
        printf("please enter your phone number: ");
        scanf("%ld", &phone);
        if(phone < 1000000000 || phone > 9999999999)
        {
            printf("incorrect phone number\n");
        }
        else
        {
            done = true;
        }
    }while(!done);
    return phone;
}
//gets a double from the user for contributions checking its within bounds and returns it
double getContributions()
{
    double contributions;
    int done = false;
    do {
        printf("please enter your contributions");
        scanf("%lf", &contributions);
        if(contributions < 0 || contributions > pow(10,50))
        {
            printf("incorrect entry, please enter a value between 0 - 10^50");
        }
        else
        {
            done = true;
        }
    }while(!done);
    return contributions;
}
//gets a double from the user for lies checking its within bounds and returns it
double getLies()
{
    double lies;
    int done = false;
    do {
        printf("please enter your number lies");
        scanf("%lf", &lies);
        if(lies < 0 || lies > pow(10,100))
        {
            printf("incorrect entry, please enter a value between 0 - 10^100");
        }
        else
        {
            done = true;
        }
    }while(!done);
    return lies;
}

// takes in all the state variables as pointers and then clears all thier values;
void clearData(char* middleInitial, char* sex, char* state, char* suffix, char* twitter, char *firstName, char* lastName, char* office, int* years, int* age, int* party, long int* phone, double* contributions, double* lies)
{
  int size = sizeof(*middleInitial);
  memset(middleInitial,0,size);

  size = sizeof(sex);
  memset(sex, 0, size);

  size = sizeof(state);
  memset(state, 0, size);

  size = sizeof(suffix);
  memset(suffix, 0, size);

  size = sizeof(twitter);
  memset(twitter, 0, size);

  size = sizeof(firstName);
  memset(firstName, 0, size);

  size = sizeof(lastName);
  memset(lastName, 0, size);

  size = sizeof(office);
  memset(office, 0, size);

  size = sizeof(*years);
  memset(years, 0, size);

  size = sizeof(*age);
  memset(age, 0, size);

  size = sizeof(*party);
  memset(party, 0, size);

  size = sizeof(*phone);
  memset(phone, 0, size);

  size = sizeof(*contributions);
  memset(contributions, 0, size);

  size = sizeof(*lies);
  memset(lies, 0, size);


}

// the main loop of the function
int main() {

    char middleInitial = -1;
    char sex = -1;
    char state[3] ;
    char suffix[4];
    char twitter[16];
    char firstName[33];
    char lastName[33];
    char office[33];

    int done = 0;
    int years = -1;
    int age = -1;
    int party =-1;
    long int phone = -1;
    int input = -1;

    double contributions = -1;
    double lies = -1;


    do{
        printMenu();
        scanf("%d", &input);


        switch(input)
        {
            case 1: {
                        getName(firstName,lastName,&middleInitial,suffix);
                        //getLastName(lastName);
                        //getMiddleInitial(&middleInitial);
                        //getSuffix(suffix);
                    }
            break;

            case 2: {
                        getPoliInfo(&years, &party, office, state);
                    }
            break;

            case 3: {
                        age = getAge();
                        sex = getSex();
                    }
            break;

            case 4: {
                        getTwitter(twitter);
                        phone = getPhone();
                    }
            break;

            case 5: {
                        contributions = getContributions();
                        lies = getLies();
                    }
                    break;
            case 6: {
                        displayInfo(&middleInitial, sex, state, suffix,twitter,firstName,lastName,office,years,age,party,phone,contributions,lies);
                    }
                    break;

            case 7: {
                        clearData(&middleInitial, &sex, state, suffix, twitter,firstName,lastName,office,&years,&age,&party,&phone,&contributions,&lies);
                    }
            break;

            case 8: {
                        done = 1;
                    }
                    break;

            default:printf("incorrect entry please enter one of the selections listed below\n");

        }
    }while(!done);

    return 0;
}