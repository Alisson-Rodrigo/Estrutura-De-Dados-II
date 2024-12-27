#ifndef ARVRB_H
#define ARVRB_H
#include "structs.h"

int inserirPalavraPortugues(PortuguesRB **arvore, char *palavraPortugues, char *palavraIngles, int unidade);

Info criaInfo(char *palavra, char *palavraIngles, int unidade);

PortuguesRB *criaNo(Info *informacao);
int cor(PortuguesRB *raiz);

void troca_cor(PortuguesRB **raiz);

void rotacao_direita(PortuguesRB **raiz);

void rotacao_esquerda(PortuguesRB **raiz);

void balancear(PortuguesRB **raiz);

int inserirRB(PortuguesRB **raiz, Info *informacao);

int inserirArvRB(PortuguesRB **raiz, Info *informacao);

void moveEsqVermelha(PortuguesRB **raiz);

void moveDirVermelha(PortuguesRB **raiz);

void removeMenor(PortuguesRB **raiz);

PortuguesRB *procuraMenor(PortuguesRB *raiz);

int removerNoArvVP(PortuguesRB **raiz, char *valor);

int removerArvRB(PortuguesRB **raiz, char *valor);

PortuguesRB *BuscarPalavra(PortuguesRB **arvore, char *palavraPortugues);

void imprimirPalavrasUnidade(PortuguesRB *arvore, int unidade);

void exibir_traducao_Portugues(PortuguesRB **raiz, char *palavraPortugues);

void imprimirTraducoes(Inglesbin *node, int unidade, char *palavraPortugues);

void exibirArvore(PortuguesRB *raiz);


// void freeTree(PortuguesRB *no);

#endif