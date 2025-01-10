#ifndef ARVBIN_H
#define ARVBIN_H
#include "portugues.h"
#include "unidade.h"


typedef struct Inglesbin {
    char *englishWord;
    struct Inglesbin *rightChild, *leftChild;
    Unit *unitList; 
} Inglesbin;

typedef struct PortugueseTree PortugueseTree;



Inglesbin* initializeNode(const char* word, int unit);


Inglesbin* insertEnglishWord(Inglesbin* root, const char* word, int unit);

void insertTranslation(PortugueseTree *no, const char *palavraPortugues, const char *palavraIngles, int unidade);

int isLeafNode(Inglesbin *raiz);

Inglesbin *getSingleChild(Inglesbin *raiz);

Inglesbin *getMinimumChild(Inglesbin *raiz);

void printBinaryTree(Inglesbin *root);
int removeEnglishWord(Inglesbin **raiz, const char *englishWord,int unit);
void removeEnglishTranslation(PortugueseTree **rootNode, char *englishWord, int unit, PortugueseTree **parentNode);
Inglesbin *findEnglishWord(Inglesbin *raiz, const char *palavraIngles);

void clear_binary_tree(Inglesbin *raiz);
void showEnglishTranslations(Inglesbin *raiz);
#endif