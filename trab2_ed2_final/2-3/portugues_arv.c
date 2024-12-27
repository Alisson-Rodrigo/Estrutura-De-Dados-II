#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "portugues_arv.h"
#include "ingles_arv.h"

Info CriarInfo(char *palavra, char *palavraIngles, int unidade)
{
    Info info;

    info.palavraPortugues = malloc(strlen(palavra) + 1);
    strcpy(info.palavraPortugues, palavra);

    info.palavraIngles = NULL;
    info.palavraIngles = insertpalavraIngles(info.palavraIngles, palavraIngles, unidade);
    return info;
}

Arv_pt *CriarNoArvore23(const Info *informacao, Arv_pt *filhoesq, Arv_pt *filhocen)
{
    Arv_pt *no = (Arv_pt *)malloc(sizeof(Arv_pt));
    if (!no)
    {
        printf("Erro ao alocar memoria para no");
    }
    else
    {
        no->info1 = *informacao;
        memset(&(no->info2), 0, sizeof(Info)); 
        no->esq = filhoesq;
        no->cent = filhocen;
        no->dir = NULL;
        no->nInfos = 1;
    }

    return no;
}

Arv_pt *AdicionarInfo(Arv_pt *no, const Info *informacao, Arv_pt *filho)
{
    if (strcmp(informacao->palavraPortugues, no->info1.palavraPortugues) > 0)
    {
        no->info2 = *informacao;
        no->dir = filho;
    }
    else
    {
        no->info2 = no->info1;
        no->dir = no->cent;
        no->info1 = *informacao;
        no->cent = filho;
    }
    no->nInfos = 2;
    return no;
}

Arv_pt *QuebrarNo23(Arv_pt **no, const Info *informacao, Info *promove, Arv_pt **filho)
{
    Arv_pt *maior;

    if (strcmp(informacao->palavraPortugues, (*no)->info2.palavraPortugues) > 0)
    {
        *promove = (*no)->info2;
        maior = CriarNoArvore23(informacao, (*no)->dir, (filho ? *filho : NULL));
    }
    else if (strcmp(informacao->palavraPortugues, (*no)->info1.palavraPortugues) > 0)
    {
        *promove = *informacao;
        maior = CriarNoArvore23(&(*no)->info2, (filho ? *filho : NULL), (*no)->dir);
    }
    else
    {
        *promove = (*no)->info1;
        maior = CriarNoArvore23(&(*no)->info2, (*no)->cent, (*no)->dir);
        (*no)->info1 = *informacao;
        (*no)->cent = (filho ? *filho : NULL);
    }

    (*no)->nInfos = 1;
    return maior;
}

int NoEhFolha(const Arv_pt *no)
{
    return (no->esq == NULL);
}

Arv_pt *BuscarNoPorPalavra(Arv_pt **no, const char *palavraPortugues)
{
    Arv_pt *inserida = NULL; 

    if (no != NULL && *no != NULL)
    {
        if (strcmp(palavraPortugues, (*no)->info1.palavraPortugues) == 0)
        {
            inserida = (*no); 
        }
        else if ((*no)->nInfos == 2 && strcmp(palavraPortugues, (*no)->info2.palavraPortugues) == 0)
        {
            inserida = (*no);
        }
        else
        {
            
            if (strcmp(palavraPortugues, (*no)->info1.palavraPortugues) < 0)
            {
                inserida = BuscarNoPorPalavra(&(*no)->esq, palavraPortugues);
            }
            else if ((*no)->nInfos == 1 || strcmp(palavraPortugues, (*no)->info2.palavraPortugues) < 0)
            {
                inserida = BuscarNoPorPalavra(&(*no)->cent, palavraPortugues);
            }
            else
            {
                inserida = BuscarNoPorPalavra(&(*no)->dir, palavraPortugues);
            }
        }
    }

    return inserida;
}

void AdicionarTraducaoNo(Arv_pt *no, const char *palavraPortugues, const char *palavraIngles, int unidade)
{
    if (strcmp(palavraPortugues, (no)->info1.palavraPortugues) == 0)
    {
        InserirTraducaoIngles(&(no)->info1, palavraIngles, unidade);
    }
    else if (no->nInfos == 2 && strcmp(palavraPortugues, no->info2.palavraPortugues) == 0)
    {
        InserirTraducaoIngles(&(no)->info2, palavraIngles, unidade);
    }
}

Arv_pt *InserirInfoArvore23(Arv_pt **no, Info *informacao, Info *promove, Arv_pt **pai)
{
    Info promove1;
    Arv_pt *maiorNo = NULL;
    if (*no == NULL)
    {
        
        *no = CriarNoArvore23(informacao, NULL, NULL);
    }
    else
    {
        if (NoEhFolha(*no))
        { 
            if ((*no)->nInfos == 1)
            {
                
                *no = AdicionarInfo(*no, informacao, NULL);
            }
            else
            {
                
                maiorNo = QuebrarNo23(no, informacao, promove, NULL);
                if (*pai == NULL)
                { 
                    *no = CriarNoArvore23(promove, *no, maiorNo);
                    maiorNo = NULL;
                }
            }
        }
        else
        { 
            
            if (strcmp(informacao->palavraPortugues, (*no)->info1.palavraPortugues) < 0)
            {
                maiorNo = InserirInfoArvore23(&((*no)->esq), informacao, promove, no);
            }
            else if ((*no)->nInfos == 1 || strcmp(informacao->palavraPortugues, (*no)->info2.palavraPortugues) < 0)
            {
                maiorNo = InserirInfoArvore23(&((*no)->cent), informacao, promove, no);
            }
            else
            {
                maiorNo = InserirInfoArvore23(&((*no)->dir), informacao, promove, no);
            }

          
            if (maiorNo)
            {
                if ((*no)->nInfos == 1)
                {
                    
                    *no = AdicionarInfo(*no, promove, maiorNo);
                    maiorNo = NULL;
                }
                else
                {
                    
                    maiorNo = QuebrarNo23(no, promove, &promove1, &maiorNo);
                    if (*pai == NULL)
                    {
                        *no = CriarNoArvore23(&promove1, *no, maiorNo);
                        maiorNo = NULL;
                    }
                }
            }
        }
    }

    return maiorNo;
}

void LiberarArvore23(Arv_pt *no)
{
    if (no != NULL)
    {
        LiberarArvore23(no->esq);
        LiberarArvore23(no->cent);
        LiberarArvore23(no->dir);
        free(no);
    }
}

void InserirTraducaoIngles(Info *info, const char *palavraIng, int unidade)
{
    info->palavraIngles = insertpalavraIngles(info->palavraIngles, palavraIng, unidade);
}


void ExibirArvore23(const Arv_pt *raiz)
{
    if (raiz != NULL)
    {
        ExibirArvore23(raiz->esq);
        printf("Palavra (PT): %s", raiz->info1.palavraPortugues);
        if (raiz->info1.palavraIngles != NULL && raiz->info1.palavraIngles->palavraIngles != NULL)
        {
            printBinaryTree(raiz->info1.palavraIngles);
            printf("\n");
        }
        ExibirArvore23(raiz->cent);
        if (raiz->nInfos == 2)
        {
            printf("Palavra (PT): %s", raiz->info2.palavraPortugues);

            if (raiz->info2.palavraIngles != NULL && raiz->info2.palavraIngles->palavraIngles != NULL)
                printBinaryTree(raiz->info2.palavraIngles);
            printf("\n");
        }

        if (raiz->nInfos == 2)
        {
            ExibirArvore23(raiz->dir);
        }
    }
}

void ImprimirPorUnidade(Arv_pt *arvore, int unidade)
{
    if (arvore)
    {
        ImprimirPorUnidade(arvore->esq, unidade);
        MostrarTraducoesUnidade(arvore->info1.palavraIngles, unidade, arvore->info1.palavraPortugues);

        ImprimirPorUnidade(arvore->cent, unidade);
        if (arvore->nInfos == 2)
        {
            MostrarTraducoesUnidade(arvore->info2.palavraIngles, unidade, arvore->info2.palavraPortugues);
        }
        ImprimirPorUnidade(arvore->dir, unidade);
    }
}

void MostrarTraducoesUnidade(Arv_en *node, int unidade, const char *palavraPortugues)
{
    if (node)
    {
        if (node->unidade.unidade == unidade)
        {
            printf("%s: %s;\n", palavraPortugues, node->palavraIngles);
        }
        MostrarTraducoesUnidade(node->esq, unidade, palavraPortugues);
        MostrarTraducoesUnidade(node->dir, unidade, palavraPortugues);
    }
}

void ExibirTraducoesPortugues(Arv_pt **raiz, const char *palavraPortugues)
{
    Arv_pt *resultado = NULL;
    if (raiz != NULL)
    {
        resultado = BuscarNoPorPalavra(raiz, palavraPortugues);
        if (resultado)
        {
            printf("Traducoes em ingles para '%s':\n", palavraPortugues);

            if (strcmp(palavraPortugues, resultado->info1.palavraPortugues) == 0)
            {
                printBinaryTree(resultado->info1.palavraIngles);
            }
            else 
            {
                printBinaryTree(resultado->info2.palavraIngles);
            }
        }
        else
        {
            printf("A palavra '%s' noo foi encontrada na arvore.\n", palavraPortugues);
        }
    }
}

void RemoverTraducoesInglesNaUnidade(Arv_pt **raiz, char *palavraIngles, int unidade) 
{ 
    if (*raiz != NULL) 
    {
        RemoverTraducoesInglesNaUnidade(&(*raiz)->esq, palavraIngles, unidade);

        if ((*raiz)->info1.palavraIngles != NULL && (*raiz)->info1.palavraIngles->unidade.unidade == unidade) 
        {
            removerPalavraIngles(&(*raiz)->info1.palavraIngles, palavraIngles);
        }
        RemoverTraducoesInglesNaUnidade(&(*raiz)->cent, palavraIngles, unidade);

        if ((*raiz)->nInfos == 2 && (*raiz)->info2.palavraIngles != NULL && (*raiz)->info2.palavraIngles->unidade.unidade == unidade) 
        {
            removerPalavraIngles(&(*raiz)->info2.palavraIngles, palavraIngles);
        }

        if ((*raiz)->nInfos == 2 && (*raiz)->info2.palavraIngles != NULL) 
        {
            RemoverTraducoesInglesNaUnidade(&(*raiz)->dir, palavraIngles, unidade);
        }
           
    }
}

static void ReorganizarArvore23(const Info info, Info *entrada, const Arv_pt *pai, Arv_pt **fonte, Arv_pt **raiz) {
    RemoverElementoArvore23(raiz, info.palavraPortugues, pai, fonte);
    if(entrada)
        *entrada = info;
}

static int PodeRemoverNo(const Arv_pt *raiz) {
    int pode = 0;

    if(raiz != NULL) {
        if(raiz->nInfos == 2)
            pode = 1;
        if(!pode) {
            pode = PodeRemoverNo(raiz->cent);
            if(!pode)
                pode = PodeRemoverNo(raiz->esq);
        }
    }
    return pode;
}

static Arv_pt *EncontrarMenorFilho(Arv_pt *no, Arv_pt **pai, Info *res){
    *pai = no;
    Arv_pt *filho = no->esq;

    while(filho != NULL && !NoEhFolha(filho)){
        *pai = filho;
        filho = filho->esq;
    }
    if(filho != NULL)
        *res = filho->info1;

    return filho;
}

static Arv_pt *EncontrarMaiorFilho(const Arv_pt *raiz) {
    return raiz->nInfos == 2 ? raiz->dir : raiz->cent;
}

static Arv_pt *EncontrarMaiorComPai(Arv_pt *raiz, Arv_pt **pai, Info *maiorinfo) {
    Arv_pt *filho = EncontrarMaiorFilho(raiz);
    *pai = raiz;

    while(filho != NULL && EncontrarMaiorFilho(filho) != NULL) {
        *pai = filho;
        filho = EncontrarMaiorFilho(filho);
    }
    if(filho != NULL)
        *maiorinfo = filho->nInfos ? filho->info2 : filho->info1;

    return filho;
}

static void LiberarNoArvore23(Arv_pt **no) {
    free(*no);
    *no = NULL;
}

static Arv_pt *UnirNos(Arv_pt *filho1, Arv_pt *filho2, Arv_pt **filho3) {
    Arv_pt *maior = NULL;
    if(!NoEhFolha(filho2)) {
        maior = UnirNos(filho2->esq, filho2->cent, &(filho1->dir));

        const Info aux = filho2->info1;
        filho2->info1 = maior->info1;
        maior->info1 = aux;
    }

    filho1->info2 = filho2->info1;
    maior = filho1;
    *filho3 = maior;
    LiberarNoArvore23(&filho2);

    return maior;
}

static int ehInfo1(const Arv_pt no, const char *palavra){
    int aux = 0;
    if(no.nInfos == 1 && palavra && no.info1.palavraPortugues)
        aux = strcmp(no.info1.palavraPortugues, palavra) == 0;
    return aux;
}

static int ehInfo2(const Arv_pt no, const char *palavra){
    int aux = 0;
    if(no.nInfos == 2 && palavra && no.info2.palavraPortugues)
        aux = strcmp(no.info2.palavraPortugues, palavra) == 0;
    return aux;
}

static Arv_pt *EncontrarPaiNo(Arv_pt *raiz, const char *palavra) {
    Arv_pt *pai = NULL;

    if(raiz != NULL) {
        if(!ehInfo1(*raiz, palavra) && !ehInfo2(*raiz, palavra)) {
            if(strcmp(palavra, raiz->info1.palavraPortugues) < 0)
                pai = EncontrarPaiNo(raiz->esq, palavra);
            else if(raiz->nInfos == 1 || strcmp(palavra, raiz->info1.palavraPortugues) < 0)
                pai = EncontrarPaiNo(raiz->cent, palavra);
            else
                pai = EncontrarPaiNo(raiz->dir, palavra);

            if(!pai)
                pai = raiz;
        }
    }
    return pai;
}

int RemoverElementoArvore23(Arv_pt **raiz, const char *info, const Arv_pt *pai, Arv_pt **ref) {
    int removeu = 0;

    if(*raiz != NULL) {
        const int info1 = ehInfo1(**raiz, info);
        const int info2 = ehInfo2(**raiz, info);
        if(info1 || info2) {
            if(NoEhFolha(*raiz)) {
                if((*raiz)->nInfos == 2) {
                    if(info1)
                        (*raiz)->info1 = (*raiz)->info2;
                    (*raiz)->nInfos = 1;
                    removeu = 1;
                }
                else {
                    if(pai != NULL) {
                        if((*raiz) == pai->esq)
                            ReorganizarArvore23(pai->info1, &((*raiz)->info1), NULL, ref, ref);
                        else {
                            if(pai->nInfos == 2) {
                                if(*raiz == pai->cent)
                                    ReorganizarArvore23(pai->info2, &((*raiz)->info1), NULL, ref, ref);
                                else
                                    ReorganizarArvore23(pai->info2, &(pai->cent->info2), NULL, ref, ref);
                            }
                            else
                                ReorganizarArvore23(pai->info1, &(pai->esq->info2), NULL, ref, ref);
                        }
                        removeu = 1;
                    }
                    else {
                        free(*raiz);
                        *raiz = NULL;
                        removeu = 1;
                    }
                }
            }
            else {
                removeu = 1;
                Arv_pt *filho, *auxpai;
                Info auxinfo;
                int juntou = 0;
                if(info2) {
                    if(PodeRemoverNo((*raiz)->dir))
                        filho = EncontrarMenorFilho((*raiz)->dir, &auxpai, &auxinfo);
                    else if(PodeRemoverNo((*raiz)->cent))
                        filho = EncontrarMaiorComPai((*raiz)->cent, &auxpai, &auxinfo);
                    else {
                        UnirNos((*raiz)->cent, (*raiz)->dir, &(*raiz)->cent);
                        juntou = 1;
                    }
                    if(!juntou)
                        ReorganizarArvore23(auxinfo, &((*raiz)->info2), auxpai, &filho, raiz);
                }
                if(info1){
                    if(PodeRemoverNo((*raiz)->esq))
                        filho = EncontrarMaiorComPai((*raiz)->esq, &auxpai, &auxinfo);
                    else if(PodeRemoverNo((*raiz)->cent))
                        filho = EncontrarMaiorComPai((*raiz)->cent, &auxpai, &auxinfo);
                    else if((*raiz)->nInfos == 1) {
                        if(pai != NULL) {
                            if(*raiz == pai->esq || (pai->nInfos == 2 && (*raiz == pai->cent))) {
                                filho = EncontrarMenorFilho((*raiz)->cent, &auxpai, &auxinfo);
                                auxpai = EncontrarPaiNo(*ref, pai->info1.palavraPortugues);

                                if(*raiz == pai->esq)
                                    ReorganizarArvore23(pai->info1, &(filho->info2), auxpai, ref, ref);
                                else
                                    ReorganizarArvore23(pai->info2, &(filho->info2), auxpai, ref, ref);
                            }
                            else {
                                filho = EncontrarMaiorComPai((*raiz)->esq, &auxpai, &auxinfo);
                                auxpai = EncontrarPaiNo(*ref, pai->info1.palavraPortugues);
                                filho->info2 = filho->info1;
                                ReorganizarArvore23((pai->nInfos == 2 && (*raiz == pai->dir)) ? pai->info2 : pai->info1, &((*raiz)->info1), auxpai, ref, ref);
                            }
                        }
                        else {
                            Arv_pt *aux = *raiz;
                            UnirNos((*raiz)->esq, (*raiz)->cent, raiz);
                            juntou = 1;
                            LiberarNoArvore23(&aux);
                        }
                    }
                    if(pai != NULL && !juntou)
                        ReorganizarArvore23(auxinfo, &((*raiz)->info1), auxpai, ref, &filho);
                }
            }
        }
        else {
            if(strcmp(info, (*raiz)->info1.palavraPortugues) < 0)
                removeu = RemoverElementoArvore23((&(*raiz)->esq), info, *raiz, ref);
            else if((*raiz)->nInfos == 1 || strcmp(info, (*raiz)->info2.palavraPortugues) < 0)
                removeu = RemoverElementoArvore23((&(*raiz)->cent), info, *raiz, ref);
            else
                removeu = RemoverElementoArvore23((&(*raiz)->dir), info, *raiz, ref);
        }
    }

    return removeu;
}

void RemoverPalavraArvore23(Arv_pt **raiz, const char *palavra) {
    if (RemoverElementoArvore23(raiz, palavra, NULL, raiz))
        printf("Elemento '%s' removido com sucesso.\n", palavra);
    else
        printf("Elemento '%s' nao encontrado.\n", palavra);
}

