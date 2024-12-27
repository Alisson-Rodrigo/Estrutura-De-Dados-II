#ifndef arv23_H
#define arv23_H




#include "ingles_arv.h"

typedef struct info {
    char *palavraPortugues;
    Arv_en *palavraIngles;
} Info;

typedef struct Arv_pt{
    Info info1;
    Info info2;
    struct Arv_pt *cent;
    struct Arv_pt *dir;
    struct Arv_pt *esq;
    int nInfos;
} Arv_pt;




Arv_pt *CriarNoArvore23(const Info *informacao, Arv_pt *filhoesq, Arv_pt *filhocen);
Arv_pt *AdicionarInfo(Arv_pt *no, const Info *informacao, Arv_pt *filho);
Arv_pt *QuebrarNo23(Arv_pt **no, const Info *informacao, Info *promove, Arv_pt **filho);
int NoEhFolha(const Arv_pt *no);
Arv_pt *InserirInfoArvore23(Arv_pt **no, Info *informacao, Info *promove, Arv_pt **pai);
void LiberarArvore23(Arv_pt *no);
void ExibirArvore23(const Arv_pt *raiz);
void InserirTraducaoIngles(Info *info, const char *palavraIng, int unidade);
Info CriarInfo(char *palavra, char *palavraIngles, int unidade);


void ImprimirPorUnidade(Arv_pt *arvore, int unidade);
void ExibirTraducoesPortugues(Arv_pt **raiz, const char *palavraPortugues);
Arv_pt *BuscarNoPorPalavra(Arv_pt **no, const char *palavraPortugues);

Arv_en* createNode(const char* word, int unit);



void MostrarTraducoesUnidade(Arv_en *node, int unidade, const char *palavraPortugues);
void AdicionarTraducaoNo(Arv_pt *no, const char *palavraPortugues, const char *palavraIngles, int unidade);

int removerPalavraIngles(Arv_en **raiz, char *palavra);

void RemoverTraducoesInglesNaUnidade(Arv_pt **raiz, char *palavraIngles, int unidade);



int RemoverElementoArvore23(Arv_pt **raiz, const char *info, const Arv_pt *pai, Arv_pt **ref);

void RemoverPalavraArvore23(Arv_pt **raiz, const char *palavra);


#endif