#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INPUTSIZE 30

typedef struct node Node;

struct node
{

    char* value;
    struct node* right;
    struct node* left;
};

void checkFile(FILE *fp, char* name)
{
    if(fp == 0)
    {
        printf("file %s does not exist", name);
        exit(EXIT_SUCCESS);
    }
}


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

void writeFile(FILE* outFp,char** words, int numWords)
{
    int i;
    for(i = 0; i < numWords; i++)
    {
        fputs(words[i], outFp);
        fputs(" ", outFp);
    }

    fclose(outFp);
}

Node* createNode(char* string)
{
    Node* newNode = (Node*) malloc(sizeof(Node));

    newNode->value = string;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

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

int compare(Node* node1, char* str)
{
    // printf("node value: %s string value: %s\n",node1->value,str);
    return strcasecmp(str,node1->value);
}


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

void freeStrings(char** strings, int size)
{
    int i;
    for(i = 0; i < size; i++)
    {
        free(strings[i]);
    }
    free(strings);
}

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

int findWord(char** words,int numWords, char* wordBuff)
{
    int i;
    for(i = 0; i < numWords; i++)
    {
        int cmp = strcmp(words[i],wordBuff);
        printf("%d\n",cmp);
        if(cmp == 0)
        {
            printf("word found\n");
            return i;
        }
    }

    printf("word not found\n");
    return -1;
}

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

void searchReplace(char** words, int numWords)
{
    char wordToFind[INPUTSIZE];
    memset(wordToFind,0,INPUTSIZE);
    printf("what word would you like to search for to replace: \n");
    fgets(wordToFind,INPUTSIZE,stdin);
    strtok(wordToFind, "\n");

    replace(words,numWords,wordToFind);
}

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

int main(int argc, char* argv[]) {

    FILE *inFp, *outFp, *dicFp;
    char **dictionaryArr;
    char **inputStringArr;
    char **incorrectWords;

    int dictionarySize = 0;
    int inputSize = 0;
    int numIncorrectWords = 0;

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
