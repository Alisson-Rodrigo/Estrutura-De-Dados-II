#ifndef arv23_H
#define arv23_H


#include "ingles.h"
typedef struct info {
    char *portugueseWord;
    Inglesbin *englishWord;
} NodeInfo;

typedef struct PortugueseTree{
    NodeInfo info1;
    NodeInfo info2;
    struct PortugueseTree *cent;
    struct PortugueseTree *right;
    struct PortugueseTree *left;
    int nInfos;
} PortugueseTree;

typedef struct Inglesbin Inglesbin;
PortugueseTree *FindWord(PortugueseTree **no, const char *palavraPortugues);
int insertPortugueseTerm(PortugueseTree **arvore, char *palavraPortugues, char *palavraIngles, int unidade);
void addEnglishTranslation(NodeInfo *info, const char *palavraIng, int unidade);

NodeInfo createInfo(char *palavra, char *palavraIngles, int unidade);
PortugueseTree *createNode(NodeInfo *informacao, PortugueseTree *filhoesq, PortugueseTree *filhocen);
PortugueseTree *updateNode23(PortugueseTree *no,  NodeInfo *informacao, PortugueseTree *filho);
PortugueseTree *splitNode(PortugueseTree **no, NodeInfo *informacao, NodeInfo *promove, PortugueseTree **filho);
int isNodeLeaf(PortugueseTree *no);
PortugueseTree *insertNode23(PortugueseTree **no, NodeInfo *informacao, NodeInfo *promove, PortugueseTree **pai);


void deleteInfo2_3(NodeInfo *info);
void deallocateTree(PortugueseTree **no);
int remove_palavra_ingles_unidade(PortugueseTree *raiz, const char *palavraIngles, int unidade, PortugueseTree **top);
void print_tree23(PortugueseTree *raiz);
void printWordsAtUnit(PortugueseTree *arvore, int unidade, int *unidadeImpressa);

void printPortugueseTranslation(PortugueseTree **raiz, const char *palavraPortugues);
void printTranslations(Inglesbin *node, int unidade, const char *palavraPortugues);


int removeNodeFrom23Tree(PortugueseTree **raiz, char *info, PortugueseTree *pai, PortugueseTree **origem, PortugueseTree **maior);

void printTranslationsInFormat(Inglesbin *node, int unidade, const char *palavraPortugues);
int deleteFrom23Tree(PortugueseTree **raiz, char *info, PortugueseTree *pai, PortugueseTree **origem, PortugueseTree **maior);
#endif