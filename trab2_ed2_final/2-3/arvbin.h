#ifndef ARVBIN_H
#define ARVBIN_H
#include "arv23.h"

typedef struct Inglesbin{
    char *palavraIngles;
    struct Inglesbin *dir,*esq;
    int unidade;
} Inglesbin;
typedef struct Portugues23 Portugues23; 


Inglesbin* createNode(const char* word, int unit);

// Função para inserir uma palavra em inglês na arvore binaria de busca
Inglesbin* insertpalavraIngles(Inglesbin* root, const char* word, int unit);

void adicionarTraducao(Portugues23 *no, const char *palavraPortugues, const char *palavraIngles, int unidade);

int ehFolhas(Inglesbin *raiz);

Inglesbin *soUmFilho(Inglesbin *raiz);

Inglesbin *menorFilho(Inglesbin *raiz);

void printBinaryTree(Inglesbin *root);

int removerPalavraIngles(Inglesbin **raiz, char *palavra);
void BuscarPalavraIngles(Portugues23 **raiz, char *palavraIngles, int unidade, Portugues23 **pai);

void free_arvore_binaria(Inglesbin *raiz);

#endif