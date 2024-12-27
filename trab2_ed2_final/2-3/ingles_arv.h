#ifndef ARVORE_BINARIA_H
#define ARVORE_BINARIA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "portugues_arv.h" 

typedef struct listUnit {
    int unidade;
    struct ListUnit *prox;
}ListUnit;

typedef struct Arv_en {
    char palavraIngles[50];
    struct Arv_en *dir, *esq;
    ListUnit unidade;
} Arv_en;


Arv_en *createNode(const char *palavraIngles, int unidade);
Arv_en *insertpalavraIngles(Arv_en *root, const char *palavraIngles, int unidade);
void printBinaryTree(Arv_en *root);
int ehFolhas(Arv_en *raiz);
Arv_en *soUmFilho(Arv_en *raiz);
Arv_en *menorFilho(Arv_en *raiz);
int removerPalavraIngles(Arv_en **raiz, char *palavra);


#endif 
