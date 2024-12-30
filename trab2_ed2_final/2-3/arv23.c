#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arv23.h"

Portugues23 *BuscarPalavra(Portugues23 **no, const char *palavraPortugues)
{
    Portugues23 *inserida = NULL; // Inicializa o retorno como NULL

    if (no != NULL && *no != NULL)
    {
        if (strcmp(palavraPortugues, (*no)->info1.portugueseWord) == 0)
        {
            inserida = (*no); // Palavra encontrada, retorna o nó
        }
        else if ((*no)->nInfos == 2 && strcmp(palavraPortugues, (*no)->info2.portugueseWord) == 0)
        {
            inserida = (*no);
        }
        else
        {
            // Continua a busca nos filhos
            if (strcmp(palavraPortugues, (*no)->info1.portugueseWord) < 0)
            {
                inserida = BuscarPalavra(&(*no)->esq, palavraPortugues);
            }
            else if ((*no)->nInfos == 1 || strcmp(palavraPortugues, (*no)->info2.portugueseWord) < 0)
            {
                inserida = BuscarPalavra(&(*no)->cent, palavraPortugues);
            }
            else
            {
                inserida = BuscarPalavra(&(*no)->dir, palavraPortugues);
            }
        }
    }

    return inserida;
}

void adicionarTraducao(Portugues23 *no, const char *palavraPortugues, const char *palavraIngles, int unidade)
{
    if (strcmp(palavraPortugues, (no)->info1.portugueseWord) == 0)
    {
        adicionarTraducaoEmIngles(&(no)->info1, palavraIngles, unidade);
    }
    else if (no->nInfos == 2 && strcmp(palavraPortugues, no->info2.portugueseWord) == 0)
    {
        adicionarTraducaoEmIngles(&(no)->info2, palavraIngles, unidade);
    }
}

int inserirPalavraPortugues(Portugues23 **arvore, char *palavraPortugues, char *palavraIngles, int unidade) {
    Info promove;
    Portugues23 *pai = NULL;
    int inseriu;

    // Busca a palavra na árvore
    Portugues23 *noExistente = NULL;
    noExistente =  BuscarPalavra(arvore, palavraPortugues);

    if (noExistente != NULL) {
        printf("A palavra já existe. Adicionando tradução...\n");
        adicionarTraducao(noExistente, palavraPortugues, palavraIngles, unidade);
        inseriu = 1;
    } else {
        Info novoInfo = createInfo(palavraPortugues, palavraIngles, unidade);
        inserirArv23(arvore, &novoInfo, &promove, &pai);
        inseriu = 0;
    }
    return inseriu;
}

Info createInfo(char *palavra, char *palavraIngles, int unidade)
{
    Info info;

    info.portugueseWord = malloc(strlen(palavra) + 1);
    strcpy(info.portugueseWord, palavra);

    info.palavraIngles = NULL;
    info.palavraIngles = insertpalavraIngles(info.palavraIngles, palavraIngles, unidade);
    return info;
}

Portugues23 *criaNo(const Info *informacao, Portugues23 *filhoesq, Portugues23 *filhocen)
{
    Portugues23 *no = (Portugues23 *)malloc(sizeof(Portugues23));
    no->info1 = *informacao;
    no->info2.palavraIngles = NULL;
    no->info2.portugueseWord = NULL;
    no->esq = filhoesq;
    no->cent = filhocen;
    no->dir = NULL;
    no->nInfos = 1;

    return no;
}

Portugues23 *adicionaChave(Portugues23 *no, const Info *informacao, Portugues23 *filho)
{
    if (strcmp(informacao->portugueseWord, no->info1.portugueseWord) > 0)
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

Portugues23 *quebraNo(Portugues23 **no, const Info *informacao, Info *promove, Portugues23 **filho)
{
    Portugues23 *maior;

    if (strcmp(informacao->portugueseWord, (*no)->info2.portugueseWord) > 0)
    {
        *promove = (*no)->info2;
        if(filho)
            maior = criaNo(informacao, (*no)->dir, *filho);
        else
            maior = criaNo(informacao, (*no)->dir, NULL);
    }
    else if (strcmp(informacao->portugueseWord, (*no)->info1.portugueseWord) > 0)
    {
        *promove = *informacao;
        if(filho)
            maior = criaNo(&(*no)->info2, *filho, (*no)->dir);
        else
            maior = criaNo(&(*no)->info2, NULL, (*no)->dir);
    }
    else
    {
        *promove = (*no)->info1;
        maior = criaNo(&(*no)->info2, (*no)->cent, (*no)->dir);
        (*no)->info1 = *informacao;
        if(filho)
            (*no)->cent = *filho;
        else
            (*no)->cent = NULL;
    }

    (*no)->nInfos = 1;
    return maior;
}

int ehFolha(const Portugues23 *no)
{
    return (no->esq == NULL);
}


Portugues23 *inserirArv23(Portugues23 **no, Info *informacao, Info *promove, Portugues23 **pai)
{
    Info promove1;
    Portugues23 *maiorNo = NULL;
    if (*no == NULL)
    {
        // Cria um novo nó caso seja nulo
        *no = criaNo(informacao, NULL, NULL);
    }
    else
    {
        if (ehFolha(*no))
        { // Caso seja folha
            if ((*no)->nInfos == 1)
            {
                // O nó tem espaço para a nova chave
                *no = adicionaChave(*no, informacao, NULL);
            }
            else
            {
                // O nó precisa ser quebrado
                maiorNo = quebraNo(no, informacao, promove, NULL);
                if (*pai == NULL)
                { // Se não há pai, criar nova raiz
                    *no = criaNo(promove, *no, maiorNo);
                    maiorNo = NULL;
                }
            }
        }
        else
        { // Nó não e folha
            // Navega para o filho apropriado
            if (strcmp(informacao->portugueseWord, (*no)->info1.portugueseWord) < 0)
            {
                maiorNo = inserirArv23(&((*no)->esq), informacao, promove, no);
            }
            else if ((*no)->nInfos == 1 || strcmp(informacao->portugueseWord, (*no)->info2.portugueseWord) < 0)
            {
                maiorNo = inserirArv23(&((*no)->cent), informacao, promove, no);
            }
            else
            {
                maiorNo = inserirArv23(&((*no)->dir), informacao, promove, no);
            }

            // Após inserir, verifica se houve promoção
            if (maiorNo)
            {
                if ((*no)->nInfos == 1)
                {
                    // Adiciona chave promovida no nó atual
                    *no = adicionaChave(*no, promove, maiorNo);
                    maiorNo = NULL;
                }
                else
                {
                    // O nó precisa ser quebrado
                    maiorNo = quebraNo(no, promove, &promove1, &maiorNo);
                    if (*pai == NULL)
                    {
                        *no = criaNo(&promove1, *no, maiorNo);
                        maiorNo = NULL;
                    }
                }
            }
        }
    }

    return maiorNo;
}



// ############################################## FUNÇOES PARA EXIBIR ##############################################

void exibir_tree23(const Portugues23 *raiz)
{
    if (raiz != NULL)
    {
        exibir_tree23(raiz->esq);
        printf("Palavra (PT): %s", raiz->info1.portugueseWord);
        if (raiz->info1.palavraIngles != NULL && raiz->info1.palavraIngles->palavraIngles != NULL)
        {
            printBinaryTree(raiz->info1.palavraIngles);
            printf("\n");
        }
        exibir_tree23(raiz->cent);
        // Se houver o segundo elemento (nInfos == 2), exibe o segundo filho
        if (raiz->nInfos == 2)
        {
            printf("Palavra (PT): %s", raiz->info2.portugueseWord);

            // Exibir a tradução em inglês, se houver
            if (raiz->info2.palavraIngles != NULL && raiz->info2.palavraIngles->palavraIngles != NULL)
                printBinaryTree(raiz->info2.palavraIngles);
            printf("\n");
        }

        if (raiz->nInfos == 2)
        {
            exibir_tree23(raiz->dir);
        }
    }
}

void imprimirPalavrasUnidade(Portugues23 *arvore, int unidade)
{
    if (arvore)
    {
        imprimirPalavrasUnidade(arvore->esq, unidade);
        imprimirTraducoes(arvore->info1.palavraIngles, unidade, arvore->info1.portugueseWord);

        imprimirPalavrasUnidade(arvore->cent, unidade);
        if (arvore->nInfos == 2)
        {
            imprimirTraducoes(arvore->info2.palavraIngles, unidade, arvore->info2.portugueseWord);
        }
        imprimirPalavrasUnidade(arvore->dir, unidade);
    }
}

void imprimirTraducoes(Inglesbin *node, int unidade, const char *palavraPortugues)
{
    if (node)
    {
        if (node->unidade == unidade)
        {
            printf("Palavra em Português: %s\n", palavraPortugues);
            printf("Palavra em inglês: %s\n", node->palavraIngles);
        }
        imprimirTraducoes(node->esq, unidade, palavraPortugues);
        imprimirTraducoes(node->dir, unidade, palavraPortugues);
    }
}

void exibir_traducao_Portugues(Portugues23 **raiz, const char *palavraPortugues)
{
    Portugues23 *resultado = NULL;
    if (raiz != NULL)
    {
        resultado = BuscarPalavra(raiz, palavraPortugues);
        if (resultado)
        {
            printf("Traduções em inglês para a palavra '%s':\n", palavraPortugues);

            if (strcmp(palavraPortugues, resultado->info1.portugueseWord) == 0)
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
            printf("A palavra '%s' não foi encontrada na árvore.\n", palavraPortugues);
        }
    }
}



// ############################################# REMOÇÃO ############################################


void menorInfoDir(Portugues23 *Raiz, Portugues23 **no, Portugues23 **paiNo)
{
    if (Raiz->esq != NULL)
    {
        *paiNo = Raiz;
        menorInfoDir(Raiz->esq, no, paiNo);
    }
    else
        *no = Raiz;
}

void maiorInfoEsq(Portugues23 *Raiz, Portugues23 **no, Portugues23 **paiNo)
{
    if (Raiz->dir != NULL)
    {
        *paiNo = Raiz;
        maiorInfoEsq(Raiz->dir, no, paiNo);
    }
    else
        *no = Raiz;
}

int remover23(Portugues23 **Pai, Portugues23 **Raiz, char *valor)
{
    int removeu = 0;
    Portugues23 *no = NULL, *no1, *paiNo = NULL, *paiNo1 = NULL, **aux;
    aux = (Portugues23 **)malloc(sizeof(Portugues23 *));
    no1 = (Portugues23 *)malloc(sizeof(Portugues23));

    if (*Raiz != NULL)
    {
        if (ehFolha(*Raiz) == 1)
        {
            if ((*Raiz)->nInfos == 2)
            {
                if (strcmp(valor, (*Raiz)->info2.portugueseWord) == 0)
                { // quando é folha, tem duas informações e o numero ta na segunda posição
                    (*Raiz)->info2.palavraIngles = NULL;
                    (*Raiz)->info2.portugueseWord = NULL;
                    (*Raiz)->nInfos = 1;
                    removeu = 1;
                }
                else if (strcmp(valor, (*Raiz)->info1.portugueseWord) == 0)
                { // quando é folha, tem duas informações e o numero ta na primeira posição do nó
                    (*Raiz)->info1 = (*Raiz)->info2;
                    (*Raiz)->info2.palavraIngles = NULL;
                    (*Raiz)->info2.portugueseWord = NULL;
                    (*Raiz)->nInfos = 1;
                    removeu = 1;
                }
            }
            else if (strcmp(valor, (*Raiz)->info1.portugueseWord) == 0)
            {
                if (*Pai == NULL)
                {
                    free(*Raiz);
                    *Raiz = NULL;
                    removeu = 1;
                }
                else if (*Raiz == (*Pai)->esq)
                {
                    (*Raiz)->info1 = (*Pai)->info1;
                    paiNo = *Pai;
                    menorInfoDir((*Pai)->cent, &no, &paiNo);
                    (*Pai)->info1 = no->info1;
                    removeu = 1;

                    if (no->nInfos == 2)
                    {
                        no->info1 = no->info2;
                        (*Raiz)->info2.palavraIngles = NULL;
                        (*Raiz)->info2.portugueseWord = NULL;
                        no->nInfos = 1;
                    }
                    else
                    {
                        if (paiNo->nInfos == 1)
                        {
                            (*Raiz)->info2 = no->info1;
                            (*Raiz)->nInfos = 2;
                            free(no);
                            *Pai = *Raiz;
                        }
                        else
                        {
                            no->info1 = paiNo->info2;
                            paiNo1 = paiNo;
                            menorInfoDir(paiNo->dir, &no1, &paiNo1);
                            paiNo->info2 = no1->info1;

                            if (no1->nInfos == 2)
                            {
                                no1->info1 = no1->info2;
                                no1->info2.palavraIngles = NULL;
                                no1->info2.portugueseWord = NULL;
                                no1->nInfos = 1;
                            }
                            else
                            {
                                no->info2 = paiNo->info2;
                                no->nInfos = 2;
                                paiNo->info2.palavraIngles = NULL;
                                paiNo->info2.portugueseWord = NULL;
                                paiNo->nInfos = 1;
                                free(no1);
                                paiNo1->dir = NULL;
                            }
                        }
                    }
                }
                else if ((*Raiz) == (*Pai)->cent)
                {
                    removeu = 1;
                    if ((*Pai)->nInfos == 1)
                    {
                        if (((*Pai)->esq)->nInfos == 2)
                        {
                            (*Raiz)->info1 = (*Pai)->info1;
                            (*Pai)->info1 = ((*Pai)->esq)->info2;
                            ((*Pai)->esq)->info2.palavraIngles = NULL;
                            ((*Pai)->esq)->info2.portugueseWord = NULL;
                            ((*Pai)->esq)->nInfos = 1;
                        }
                        else
                        {
                            ((*Pai)->esq)->info2 = (*Pai)->info1;
                            free(*Raiz);
                            ((*Pai)->esq)->nInfos = 2;
                            *aux = (*Pai)->esq;
                            free(*Pai);
                            *Pai = *aux;
                        }
                    }
                    else
                    {
                        (*Raiz)->info1 = (*Pai)->info2;
                        paiNo = *Pai;
                        menorInfoDir((*Pai)->dir, &no, &paiNo);
                        (*Pai)->info2 = no->info1;

                        if (no->nInfos == 2)
                        {
                            no->info1 = no->info2;
                            no->info2.palavraIngles = NULL;
                            no->info2.portugueseWord = NULL;
                            no->nInfos = 1;
                        }
                        else
                        {
                            (*Raiz)->nInfos = 2;
                            (*Raiz)->info2 = (*Pai)->info2;
                            (*Pai)->info2.palavraIngles = NULL;
                            (*Pai)->info2.portugueseWord = NULL;
                            (*Pai)->nInfos = 1;
                            free(no);
                            (*Pai)->dir = NULL;
                        }
                    }
                }
                else
                {
                    removeu = 1;
                    paiNo = *Pai;
                    maiorInfoEsq((*Pai)->cent, &no, &paiNo);

                    if (no->nInfos == 1)
                    {
                        no->info2 = (*Pai)->info2;
                        (*Pai)->info2.palavraIngles = NULL;
                        (*Pai)->info2.portugueseWord = NULL;
                        (*Pai)->nInfos = 1;
                        no->nInfos = 2;
                        free(*Raiz);
                        *Raiz = NULL;
                    }
                    else
                    {
                        (*Raiz)->info1 = (*Pai)->info2;
                        (*Pai)->info2 = no->info2;
                        no->info2.palavraIngles = NULL;
                        no->info2.portugueseWord = NULL;
                        no->nInfos = 1;
                    }
                }
            }
        }
        else
        { // se nao é folha
            if (strcmp(valor, (*Raiz)->info1.portugueseWord) < 0)
                removeu = remover23(Raiz, &(*Raiz)->esq, valor);
            else if (strcmp(valor, (*Raiz)->info1.portugueseWord) == 0)
            {
                paiNo = *Raiz;
                menorInfoDir((*Raiz)->cent, &no, &paiNo);
                (*Raiz)->info1 = no->info1;
                remover23(Raiz, &(*Raiz)->cent, (*Raiz)->info1.portugueseWord);
                removeu = 1;
            }
            else if (((*Raiz)->nInfos == 1) || (strcmp(valor, (*Raiz)->info1.portugueseWord) < 0))
            {
                removeu = remover23(Raiz, &(*Raiz)->cent, valor);
            }
            else if (strcmp(valor, (*Raiz)->info1.portugueseWord) == 0)
            {
                paiNo = *Pai;
                menorInfoDir((*Pai)->dir, &no, &paiNo);
                (*Raiz)->info2 = no->info1;
                remover23(Raiz, &(*Raiz)->dir, (*Raiz)->info2.portugueseWord);
                removeu = 1;
            }
            else
            {
                removeu = remover23(Raiz, &(*Raiz)->dir, valor);
            }
        }
    }
    return removeu;
}

/*#########################################FREE#######################################################*/

void freeInfo2_3(Info *info)
{
  free_arvore_binaria(info->palavraIngles);
  free(info->portugueseWord);
}

void freeTree(Portugues23 *no)
{
    if (no != NULL)
    {
        freeTree(no->esq);
        freeTree(no->cent);
        if (no->nInfos == 2)
            freeTree(no->dir);
        if (no->nInfos == 2)
            freeInfo2_3(&no->info2);
        freeInfo2_3(&no->info1);
        free(no);
    }
}

/*-----------------------------------------------------------------------------------------------------*/

/* (i) informar uma unidade e então imprima todas as palavras da unidade em português seguida das
equivalentes em inglês */

/*-----------------------------------------------------------------------------------------------------*/

/* (ii) informar uma palavraPortugues em português e então imprima todas as palavras em inglês equivalente à palavraPortugues em
português dada, independente da unidade */

/*-----------------------------------------------------------------------------------------------------*/

/* (iii) informar uma palavraPortugues em inglês e a unidade a qual a mesma pertence removÊ-la das arvores binarias
das quais ela pertence. Caso ela seja a única palavraPortugues em uma das arvores binarias, remover tambem da
arvore 2-3 */

/*-----------------------------------------------------------------------------------------------------*/

/* (iv)informar uma palavraPortugues em português e a unidade a qual a mesma pertence e então removê-la, para isto
deve remover a palavraPortugues em inglês da arvore binaria correspondente a palavraPortugues em português da mesma
unidade. Caso ela seja a única palavraPortugues na arvore binaria, a palavraPortugues em português deve ser removida da
arvore 2-3 */