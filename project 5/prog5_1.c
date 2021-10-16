#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*

Author Gavin Starnes
Last Updated: 4/2/2019

uses a input text file an output file and a dictionary file to spell check a text document
also allows search and replace functionality of the input file
as well as replacing of misspelled words from the input file or the ability to ignore these misspelled words
once ignore these words are added to the runtime dictionary not the dictionary file.

command line arguments are:
 arg 1: input file path
 arg 2: output file path
 arg 3: dictionary file path
*/

#define INPUTSIZE 30
#define MAX_CHARS_PER_LINE 80

typedef struct node Node;
/*
 * defines node structure for use in Binary Search Tree
 * @value: value at that node of the tree
 * @right: next node that is greater than current node
 * @left: next node that is less than current node
 * */
struct node
{

    char* value;
    struct node* right;
    struct node* left;
};

/*
 * checks if a file exists
 * @fp: pointer to the file
 * @name: path of the file whos existence is to be checked
 * */
void checkFile(FILE *fp, char* name)
{
    if(fp == 0)
    {
        printf("file %s does not exist", name);
        exit(EXIT_SUCCESS);
    }
}

/*
 * scans a text file and returns its size in terms of the number of strings contained in the file
 * @fp: pointer to the file
 * @return: type int, size of file
 * */
int getFileSize(FILE *fp)
{
    int size  = 0;
    char strBuffer[INPUTSIZE];

    while(!feof(fp))
    {

        fscanf(fp,"%s", strBuffer);
        size++;

    }

    fseek(fp , 0, SEEK_SET);

    return size-1;

}

/*
 * reads a text file and tokenizes into individual strings, closes the file on return
 * @fp: pointer to the file
 * @lines: number of lines
 * @return: type char**: an array of all the strings in the file
 * */
char** readFile(FILE *fp, int* lines)
{
    char strBuffer[INPUTSIZE];
    int strSize;
    int i = 0;

    *lines = getFileSize(fp);
    *lines = *lines;

    char** strings = (char**) malloc(*lines * sizeof(char*));

    for(i = 0; i < *lines; i++)
    {
        fscanf(fp,"%s",strBuffer);
        strSize = strlen(strBuffer) + 1;
        strings[i] = (char*) malloc(strSize * sizeof(char));
        strcpy(strings[i], strBuffer);
    }

    fclose(fp);

    return strings;

}

/*
 * writes out the modified input data to the specified output file
 * only allows for 80 char max per line including newline character
 * @fp: pointer to the file
 * @words: strings of modified input data
 * @numWords: number of strings in the input data array
 *
 * */
void writeFile(FILE* outFp,char** words, int numWords)
{
    int i;
    char lineBuffer[MAX_CHARS_PER_LINE];
    char wordBuffer[INPUTSIZE];
    char newline = '\n';
    char* space = " ";

    memset(lineBuffer, 0, MAX_CHARS_PER_LINE);

    for (i = 0; i < numWords; i++)
    {
        memset(wordBuffer,0,INPUTSIZE);
        strcat(wordBuffer, words[i]);
        strcat(wordBuffer, space);
        int bufferFilled = strlen(lineBuffer) + strlen(wordBuffer);

        if (bufferFilled < 79) {
            strcat(lineBuffer, wordBuffer);
        }
        if (bufferFilled >= 79 || !(i < (numWords - 1))) {
            lineBuffer[79] = newline;
            fputs(lineBuffer, outFp);
            fputc(newline, outFp);
            memset(lineBuffer, 0, MAX_CHARS_PER_LINE);
        }
    }

    fclose(outFp);
}

/*
 * creates new node with value of passed in string
 * @string: the string to be the value of the new node
 * @return: type Node*, newly created node
 * */
Node* createNode(char* string)
{
    Node* newNode = (Node*) malloc(sizeof(Node));

    newNode->value = string;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

/*
 * creates a Binary Search Tree recursively
 * @strings: string array that will be the value of the nodes in the BST
 * @start: the beginning element for that section of the BST
 * @end: the last element for that section of the BST
 * @return: type Node*, returns the root of the tree
 * */
Node* createTree(char** strings, int start, int end)
{
    int middle = (start + end) /2;

    if(start > end)
    {
        return  (Node*) NULL;
    }

    Node* head = createNode(strings[middle]);

    Node* _right = createTree(strings, middle + 1, end);
    Node* _left = createTree(strings, start, middle - 1);

    head->right = _right;
    head->left = _left;

    return head;

}

/*
 * compares a Node string value with another string returning their difference
 * @node1: the Node whos value is to be compared
 * @str: the string whos value is to be compared
 * return: type int, the difference of the compared strings
 * */
int compare(Node* node1, char* str)
{
    return strcasecmp(str,node1->value);
}

/*
 * traverses the BST for the specified value
 * @str: the string value to search the BST for
 * @root: the root of the BST
 * @return: type int, 1 if value was found 0 if value was not found in the tree
 * */
int searchTree(char* str, Node* root)
{

    int cmp = compare(root,str);

    if(cmp < 0){

        root = root->left;

        if(root == NULL)
        {
            return 0;
        }else{
           return searchTree(str, root);
        }

    }else if(cmp > 0){

        root = root->right;

        if(root == NULL)
        {
            return 0;
        }else{
          return searchTree(str, root);
        }
    }
    return 1;
}

/*
 * frees the memory of a string array
 * @strings: the string array to be free'ed
 * @size: the size of the string array
 * */
void freeStrings(char** strings, int size)
{
    int i;
    for(i = 0; i < size; i++)
    {
        free(strings[i]);
    }
    free(strings);
}

/*
 * compares the value of the input file to that of the dictionary recording misspelled strings
 * @inputStringArr: the string array that will be compared to the dictionary
 * @inputSize: the size of the string array
 * @dictionaryTree: the BST of the dictionary
 * @numMisspelledWords: pointer to the number of misspelled words
 * @return: type char**, string array of misspelled words
 * */
char** spellcheck(char** inputStringArr, int inputSize, Node* dictionaryTree, int *numMispelledWords)
{
    int i;
    *numMispelledWords = 0;
    char** incorrectWords = (char**) malloc(sizeof(char*));

    for(i = 0; i < inputSize; i++)
    {
        int result = searchTree(inputStringArr[i], dictionaryTree);
        if(result == 0)
        {
            *numMispelledWords += 1;
            incorrectWords = (char**) realloc(incorrectWords, *numMispelledWords * sizeof(char*));
            incorrectWords[*numMispelledWords-1] = inputStringArr[i];
        }
    }
    return incorrectWords;
}

/*
 * searchs a string array for a specified string
 * @words: string array to search through
 * @numWords: size of string array
 * @wordToFind: string to be searched for
 * @return:
 * */
int findWord(char** words,int numWords, char* wordToFind)
{
    int i;
    for(i = 0; i < numWords; i++)
    {
        int cmp = strcmp(words[i],wordToFind);
        if(cmp == 0)
        {
            printf("word found\n");
            return i;
        }
    }

    printf("word not found\n");
    return -1;
}

/*
 * displays misspelled words to the console
 * @words: array of misspelled words
 * @size: number of strings in array
 * */
void displayMisspelledWords(char** words, int size) {
    if (size > 0)
    {
        int i;

        printf("\nMisspelled words found:\n");
        for (i = 0; i < size; i++) {
            printf("word %d: %s\n", i, words[i]);
        }
    }
}

/*
 * replaces a word with a specified new string
 * @words: string array of words containing the word to be replaced
 * @numWords: size of string array
 * @wordToFind: the word to be replaced
 * */
void replace(char** words, int numWords, char* wordToFind)
{
    char replacement[INPUTSIZE];
    memset(replacement,0,INPUTSIZE);

    int wordindex = findWord(words, numWords, wordToFind);
    if(wordindex == -1)
    {
        return;
    }

    printf("please enter the replacement for this word:\n");
    fgets(replacement,INPUTSIZE,stdin);
    strtok(replacement, "\n");
    words[wordindex] = (char*) realloc(words[wordindex], sizeof(strlen(replacement)));
    strcpy(words[wordindex],replacement);

}

/*
 * takes user input to and searches for a specified word to replace
 * @words: string array to search
 * @numWords: size of string array
 * */
void searchReplace(char** words, int numWords)
{
    char wordToFind[INPUTSIZE];
    memset(wordToFind,0,INPUTSIZE);
    printf("what word would you like to search for to replace: \n");
    fgets(wordToFind,INPUTSIZE,stdin);
    strtok(wordToFind, "\n");

    replace(words,numWords,wordToFind);
}

/*
 * adds a node to the BST
 * @root: pointer to the root of the BST
 * @str: string value of the new node
 * @return: type int, 1 if successful 0 if unsuccessful
 * */
int addNode(Node *root, char* str)
{
    int cmp = compare(root, str);
    Node* newNode = createNode(str);

    if(cmp < 0){

        root = root->left;

        if(root == NULL)
        {
            root = newNode;
            return 1;
        }else{
          return addNode(root,str);
        }

    }else if(cmp > 0){

        root = root->right;

        if(root == NULL)
        {
            root = newNode;
            return 1;
        }else{
           return addNode(root,str);
        }
    }
    return 0;
}

/*
 * ignores or replaces on a per string basis of a string array containing misspelled words
 * @inputStringArr: string array contain all input strings
 * @numwords: size of input string array
 * @incorrectWords: string array of incorrect words
 * @numMisspelled: size of incorrect word string array
 * @dictionaryTree: pointer to root of BST
 * */
void ignoreOrReplace(char** inputStringArr,int numwords, char** incorrectWords, int numMisspelled, Node *dictionaryTree)
{

    int i;
    int done = 0;
    for (i = 0; i < numMisspelled; ++i)
    {
        char userinput[INPUTSIZE];
        printf("would you like to 1) ignore the word: %s \nor \n2)replace it\n", incorrectWords[i]);
        fgets(userinput,INPUTSIZE,stdin);
        strtok(userinput, "\n");

        while(!done) {
            if (strcmp(userinput, "2") == 0) {

                replace(inputStringArr, numwords, incorrectWords[i]);
                done = 1;

            } else if (strcmp(userinput, "1") == 0) {

                addNode(dictionaryTree,incorrectWords[i]);
               done = 1;

            } else{
                printf("\n%s is not a valid input please choose one of the following options\n", userinput);
            }
        }
    }
}

/*
 * displays the menu for the main program
 * @inputStringArr: input string array for use in menu functions
 * @inputSize: input string array size
 * @incorrectWords: string array containing incorrect words
 * @numMisspelled: size of string array containing misspelled words
 * @dictionaryTree: root of BST containing the dictionary
 * @outFp: pointer to the output file
 * */
void menu(char ** inputStringArr, int inputSize, char** incorrectWords, int numMisspelled, Node *dictionaryTree,FILE* outFp) {

    int done = 0;

    do {
        printf("\nplease select one of the following:\n1) search for and replace a word\n2) replace/ignore Misspelled words\n3) display words\n0) quit\n");
        char userinput[INPUTSIZE];
        fgets(userinput, INPUTSIZE, stdin);
        strtok(userinput, "\n");

        if (strcmp(userinput, "0") == 0) {
            done = 1;
            writeFile(outFp, inputStringArr, inputSize);
        } else if (strcmp(userinput, "1") == 0) {

            searchReplace(inputStringArr, inputSize);

        } else if (strcmp(userinput, "2") == 0) {

            ignoreOrReplace(inputStringArr,inputSize, incorrectWords, numMisspelled, dictionaryTree);

        } else if (strcmp(userinput, "3") == 0) {
            int i;
            for (i = 0; i < inputSize; i++) {
                printf("%s ", inputStringArr[i]);
            }
            printf("\n");

        } else {
            printf("\n%s is not a valid input please choose one of the following options\n", userinput);
        }
    }while(!done);
}

/*
 * main function of the program
 * @argc: number of command line arguments entered
 * @arcv: string array containing file paths from commandline
 * */
int main(int argc, char* argv[]) {

    FILE *inFp, *outFp, *dicFp;
    char **dictionaryArr;
    char **inputStringArr;
    char **incorrectWords;

    int dictionarySize = 0;
    int inputSize = 0;
    int numIncorrectWords = 0;
    printf("arg1: %s, arg2: %s, arg3: %s\n",argv[0],argv[1],argv[2]);

    inFp = fopen(argv[1], "r");
    outFp = fopen(argv[2], "w");
    dicFp = fopen(argv[3], "r");

    checkFile(inFp, argv[1]);
    checkFile(dicFp, argv[2]);

    dictionaryArr = readFile(dicFp, &dictionarySize);
    Node* dictionaryTree = createTree(dictionaryArr, 0, dictionarySize - 1);

    inputStringArr = readFile(inFp, &inputSize);

    incorrectWords = spellcheck(inputStringArr, inputSize, dictionaryTree, &numIncorrectWords);

    displayMisspelledWords(incorrectWords, numIncorrectWords);

    menu(inputStringArr,inputSize, incorrectWords, numIncorrectWords, dictionaryTree, outFp);

    freeStrings(inputStringArr,inputSize);
    freeStrings(dictionaryArr,dictionarySize);
    freeStrings(incorrectWords,numIncorrectWords);

    return 0;
}
