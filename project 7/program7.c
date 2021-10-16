#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

/*
 * Author: Gavin Starnes
 * Date 4/28/2019
 * this program simulates a submarine base and
 * 3 submarines
 *
 * */

#define NUMTERMINALS 4
#define DATESIZE 50
#define NUMSUBS 3

int fuel;
int missels;
int distance;
char display[100];
FILE* fps[NUMTERMINALS];
int assignedTerminal = 0;
int counter = 0;
int returning = 0;
int pidParent;
int pid;
int parentTerminal = 0;

char** findTerminal()
{
    int terminals = 0;
    char terminalNum = 0;
    char terminalfile[30];
    memset(terminalfile,0,30);
    char** terminalList;
    FILE* fp;
    char* header = "/dev/pts/";

    terminalList =(char**) malloc(NUMTERMINALS*sizeof(char*));

    while(terminals != NUMTERMINALS)
    {

        sprintf(terminalfile,"%s%d",header,terminalNum);

        fp = fopen(terminalfile,"r");

        if(fp != NULL)
        {
            terminalList[terminals] = (char*) malloc(strlen(terminalfile) * sizeof(char));
            strcpy(terminalList[terminals],terminalfile);
            terminals++;
            fclose(fp);
        }

        terminalNum++;
    }

    return terminalList;
}

void openTerminals(FILE* fps[NUMTERMINALS], char** terminalList)
{
    int i;
    for(i = 0; i < NUMTERMINALS; i++)
    {
        fps[i] = fopen(terminalList[i], "a");
    }
}

struct tm* getDate()
{
    time_t currentTime = time(NULL);
    return localtime(&currentTime);
}

void handle_Alarm(int sigalrm) {
    if (counter % 3 == 0) {
        struct tm Date = *getDate();

        sprintf(display, "MISSION TIME: %02d:%02d:%02d\n", Date.tm_hour, Date.tm_min, Date.tm_sec);
        fputs(display, fps[assignedTerminal]);
        sprintf(display, "fuel: %d gallons\nBallistic missels: %d missiles\nDistance from base: %d miles\n", fuel,
                missels, distance);
        fputs(display, fps[assignedTerminal]);

    } else if (counter % 3 == 2) {
        if (returning) {
            distance -= rand() % 5 + 3;
        } else {
            distance += rand() % 5 + 5;
        }
    }
    if (fuel < 500) {
        sprintf(display, "sub %d running out of fuel\n", assignedTerminal);
        fputs(display, fps[assignedTerminal]);
    }
    if ((fuel - 300) <= 0) {
        fuel = 0;
        sprintf(display,"sub %d dead in the water\n",assignedTerminal);
        fputs(display, fps[assignedTerminal]);
        kill(pidParent,SIGUSR2);
        while(1);
    }else{
        fuel -= rand() % 200 + 100;
    }
    signal(SIGALRM, handle_Alarm);
    counter++;

    alarm(1);
}
void handle_SIGUSR1(int sigint)
{
    if(missels == 0)
    {
        sprintf(display, "sub out of ordinace returning to base\n");
        fputs(display,fps[assignedTerminal]);
        returning = 1;

    }else{
        missels--;
        sprintf(display,"SUB#: %d Has Launched Missile\n MISSILES REMAINING: %d\n",assignedTerminal, missels);
        fputs(display,fps[assignedTerminal]);
    }
}

void handle_SIGUSR2(int sigint)
{
    if(pidParent != pid)
    {
        sprintf(display,"sub refueled\n");
        fputs(display,fps[assignedTerminal]);
        fuel = 5000;
    }

    if(pidParent == getpid())
    {
        sprintf(display,"rescue is on the way sub\n");
        fputs(display,fps[0]);
    }
}

void submarine()
{
    srand((long int) fps[assignedTerminal]);
    fuel = ((rand() % 4000) + 1000);
    missels = ((rand() % 4) + 6);
    distance = 0;
    sprintf(display, "fuel: %d\nBallistic missels: %d\ndistance: %d\n", fuel, missels, distance);
    fputs(display, fps[assignedTerminal]);

    signal(SIGUSR1, handle_SIGUSR1);
    signal(SIGUSR2, handle_SIGUSR2);
    signal(SIGALRM, handle_Alarm);
    alarm(1);

    while(fuel > 0)
    {

    }
    sprintf(display,"sub %d dead in the water\n",assignedTerminal);
    fputs(display, fps[assignedTerminal]);
}

void base(int pids[NUMSUBS])
{
    char input;
    char temp;
    int subnumber;
    int done = 0;

    signal(SIGUSR2, handle_Alarm);


    while(!done)
    {
        sprintf(display, "subs awaiting orders\n");
        fputs(display,fps[parentTerminal]);
        input = getc(stdin);

        switch(input)
        {
            case 'l':
                temp = getc(stdin);
                subnumber = atoi(&temp);
                kill(pids[subnumber-1],SIGUSR1);
                break;
            case 'r':
                temp = getc(stdin);
                subnumber = atoi(&temp);
                kill(pids[subnumber-1],SIGUSR2);
                break;
            case 's':
                temp = getc(stdin);
                subnumber = atoi(&temp);
                kill(pids[subnumber-1],SIGKILL);
                sprintf(display,"scuttling sub %d\n",subnumber);
                fputs(display,fps[0]);
                break;
            case 'q':
                done = 1;
                kill(pids[0],SIGKILL);
                kill(pids[1],SIGKILL);
                kill(pids[2],SIGKILL);
                exit(0);
                break;
            default:
                sprintf(display,"incorrect order\n");
                fputs(display,fps[parentTerminal]);
        }

        fgetc(stdin);

    }
}

int main()
{
    char** terminalList;
    char missionDate[DATESIZE];
    pidParent = getpid();
    int childrenPid[NUMSUBS];


    terminalList = findTerminal();

    openTerminals(fps, terminalList);

    struct tm Date = *getDate();

    sprintf(missionDate,"MISSION DATE: %d-%d-%d %d:%d:%d\n", Date.tm_year + 1900, Date.tm_mon + 1, Date.tm_mday, Date.tm_hour, Date.tm_min, Date.tm_sec);
    fputs(missionDate,fps[0]);

    if(pidParent == getpid())
    {
        int i;
        for(i = 0; i < NUMSUBS; i++)
        {
            if(pidParent == getpid())
            {
                assignedTerminal++;
                pid = getpid();
                childrenPid[i] = fork();
            }
        }
    }

    if(pidParent != getpid())
    {
        pid = getpid();
    }


    if(pidParent == pid)
    {
        base(childrenPid);
    }



    if(pidParent != pid)
    {
        sprintf(display,"MISSION TIME: %02d:%02d:%02d\n",  Date.tm_hour, Date.tm_min, Date.tm_sec);
        fputs(display,fps[assignedTerminal]);
        sprintf(display,"parent pid %d\n",pid);
        fputs(display,fps[assignedTerminal]);
        submarine();
    }

    return 0;

}