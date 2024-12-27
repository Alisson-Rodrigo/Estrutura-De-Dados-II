#ifndef PORTUGUES_H
#define PORTUGUES_H

#include "ingles.h"

// Definição da estrutura que irá armazenar a informação de uma palavra e suas traduções
typedef struct Info
{
  Arv_ingles *palavraIngles;
  char *palavraPortugues;
} Info;

// Definição da estrutura do nó da árvore rubro-negra
typedef struct arv_portugues{
    Info info;
    int cor; // Cor do nó (1 = vermelho, 0 = preto)
    struct arv_portugues *esq, *dir; // Filhos esquerdo e direito
} Arv_portugues;



// Funções para manipulação da árvore rubro-negra
Info CriarInfo(char *palavra, char *palavraIngles, int unidade); // Cria a estrutura Info
Arv_portugues*criaNo(Info *informacao); // Cria um novo nó na árvore rubro-negra
int cor_no(Arv_portugues *raiz); // Retorna a cor de um nó (preto ou vermelho)
void trocar_cor(Arv_portugues **raiz); // Troca a cor do nó e seus filhos
void rotar_direita(Arv_portugues **raiz); // Realiza a rotação à direita
void rotar_esquerda(Arv_portugues **raiz); // Realiza a rotação à esquerda
void balancear(Arv_portugues **raiz); // Balanceia a árvore rubro-negra após inserções
int inserir_arv(Arv_portugues **raiz, Info *informacao); // Insere um nó na árvore rubro-negra
int inserirArvRec(Arv_portugues **raiz, Info *informacao); // Insere o nó e garante que a raiz seja preta
void move2EsqVermelha(Arv_portugues **raiz); // Move a cor vermelha para a esquerda
void move2DirVermelha(Arv_portugues **raiz); // Move a cor vermelha para a direita
void RemoverMenor(Arv_portugues **raiz); // Remove o menor nó da árvore
Arv_portugues *ProcurarMenor(Arv_portugues *raiz); // Busca o menor nó da árvore
int RemoverNo(Arv_portugues **raiz, char *valor); // Remove um nó com uma palavra portuguesa específica
void AdicionarTraducaoEN(Arv_portugues *raiz, char *palavraIng, int unidade); // Adiciona uma tradução em inglês
Arv_portugues *BuscarPalavra(Arv_portugues **arvore, char *palavraPortugues); // Busca uma palavra portuguesa na árvore

#endif // PORTUGUES_H
