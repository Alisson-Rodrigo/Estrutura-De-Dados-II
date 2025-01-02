#ifndef ARVBIN_H
#define ARVBIN_H

#include "structs.h"

Inglesbin* createNode(const char* word, char *unidade);
int insertpalavraIngles(Inglesbin **root, Info *informacao);

void printBinaryTree(Inglesbin *root);

int eh_Folha(Inglesbin *raiz);
Inglesbin *soUmFilho(Inglesbin **raiz);
Inglesbin *menorFilho(Inglesbin *raiz);
int removerPalavraIngles(Inglesbin **raiz, const char *palavra);
void free_arvore_binaria(Inglesbin *raiz);

#endif