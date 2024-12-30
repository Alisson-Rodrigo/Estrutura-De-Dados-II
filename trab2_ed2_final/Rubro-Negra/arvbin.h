#ifndef ARVBIN_H
#define ARVBIN_H

#include "structs.h"


Inglesbin* createNode(const char* word, char *unidade);

// Função para inserir uma palavra em inglês na arvore binaria de busca
int insertpalavraIngles(Inglesbin **root, Info *informacao);

int ehFolhas(Inglesbin *raiz);

Inglesbin *soUmFilho(Inglesbin *raiz);

Inglesbin *menorFilho(Inglesbin *raiz);

void printBinaryTree(Inglesbin *root);

int removerPalavraIngles(Inglesbin **raiz, const char *palavra);

void free_arvore_binaria(Inglesbin *raiz);

#endif