#ifndef ARVRB_H
#define ARVRB_H
#include "structs.h"
#include "lista_encadeada.h"

int inserirPalavraPortugues(PortuguesRB **arvore, char *palavraPortugues, char *palavraIngles, char *unidade);

Info criaInfo(char *palavra, char *palavraIngles, char *unidade);

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

void imprimirPalavrasUnidade(PortuguesRB *arvore, char *unidade);

void exibir_traducao_Portugues(PortuguesRB **raiz, char *palavraPortugues);

void imprimirTraducoes(Inglesbin *node, char *unidade, char *palavraPortuguês);

void exibirArvore(PortuguesRB *raiz);


// void freeTree(PortuguesRB *no);

#endif