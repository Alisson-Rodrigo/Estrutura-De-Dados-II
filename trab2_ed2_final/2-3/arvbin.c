#include "arvbin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unidade.h"
Inglesbin *createNode(const char *palavraIngles, int unidade) {
    Inglesbin *novoNo = (Inglesbin *)malloc(sizeof(Inglesbin));
    if (novoNo != NULL) {
        novoNo->palavraIngles = (char *)malloc(strlen(palavraIngles) + 1);
        if (novoNo->palavraIngles != NULL) {
            strcpy(novoNo->palavraIngles, palavraIngles);
        }
        
        // Inicializar a lista ligada de unidades com o primeiro valor
        novoNo->unidades = criar_unidade(unidade);
        
        // Inicializar os ponteiros da árvore binária
        novoNo->esq = novoNo->dir = NULL;
    }
    return novoNo;
}


// Função para inserir uma palavra em inglês na arvore binaria de busca
Inglesbin *insertpalavraIngles(Inglesbin *root, const char *palavraIngles, int unidade) {
    Inglesbin *result;

    if (root == NULL) {
        // Cria um novo nó para a palavra em inglês e adiciona a unidade à lista
        result = createNode(palavraIngles, unidade);
    } else {
        // Se a palavra em inglês já existe, adiciona a unidade à lista ordenada
        if (strcmp(palavraIngles, root->palavraIngles) == 0) {
            root->unidades = adicionar_unidade_ordenada(root->unidades, criar_unidade(unidade));
            result = root;
        } else {
            // Caso contrário, insere recursivamente na árvore binária
            if (strcmp(palavraIngles, root->palavraIngles) < 0) {
                root->esq = insertpalavraIngles(root->esq, palavraIngles, unidade);
            } else {
                root->dir = insertpalavraIngles(root->dir, palavraIngles, unidade);
            }
            result = root;
        }
    }

    return result;
}

void adicionarTraducaoEmIngles(Info *info, const char *palavraIng, int unidade) {
    // Verifica se a árvore binária de traduções em inglês está vazia
    if (info->palavraIngles == NULL) {
        // Se a árvore estiver vazia, cria a palavra em inglês com a unidade associada
        info->palavraIngles = createNode(palavraIng, unidade);
    } else {
        // Se a árvore não estiver vazia, insere a palavra em inglês de maneira ordenada na árvore
        info->palavraIngles = insertpalavraIngles(info->palavraIngles, palavraIng, unidade);
    }

    // Agora associa a unidade à palavra em inglês, garantindo a ordem das unidades
    info->palavraIngles->unidades = adicionar_unidade_ordenada(info->palavraIngles->unidades, criar_unidade(unidade));
}


void printBinaryTree(Inglesbin *root)
{
    if (root != NULL)
    {
        // Percorre a árvore à esquerda
        printBinaryTree(root->esq);
        
        // Imprime a palavra em inglês e as unidades associadas
        printf("\nPalavra em Inglês: %s\n", root->palavraIngles);
        
        // Imprime as unidades associadas a essa palavra em inglês
        if (root->unidades != NULL) {
            printf("Unidades associadas: ");
            imprimir_unidades(root->unidades);  // Usa a função 'imprimir_unidades' para imprimir as unidades
        }
        else {
            printf("Nenhuma unidade associada.\n");
        }
        
        // Percorre a árvore à direita
        printBinaryTree(root->dir);
    }
}


int ehFolhas(Inglesbin *raiz){
    return (raiz->esq == NULL && raiz->dir == NULL);
}

Inglesbin *soUmFilho(Inglesbin *raiz){
    Inglesbin *aux;
    aux = NULL;

    if (raiz->dir == NULL)
    {
        aux = raiz->esq;
    }
    else if (raiz->esq == NULL)
    {
        aux = raiz->dir;
    }

    return aux;
}

Inglesbin *menorFilho(Inglesbin *raiz){
    Inglesbin *aux;
    aux = raiz;

    if (raiz)
    {
        if (raiz->esq)
            aux = menorFilho(raiz->esq);
    }

    return aux;
}

int removerPalavraIngles(Inglesbin **raiz, char *palavra, int unidade) {
    int existe = 0; // Indicador de sucesso da remoção

    if (*raiz != NULL) {
        if (strcmp(palavra, (*raiz)->palavraIngles) == 0) {
            existe = 1;
            printf("Removendo a palavra %s da unidade %d\n", palavra, unidade);

            // Remove apenas a unidade especificada
            (*raiz)->unidades = remover_unidade((*raiz)->unidades, unidade);

            // Se a lista de unidades estiver vazia, decide remover o nó
            if ((*raiz)->unidades == NULL) {
                Inglesbin *aux = *raiz;

                if (ehFolhas(*raiz)) { // Caso o nó não tenha filhos
                    free(aux->palavraIngles);
                    free(aux);
                    *raiz = NULL;
                } else if (soUmFilho(*raiz)) { // Caso tenha apenas um filho
                    Inglesbin *endFilho = soUmFilho(*raiz);
                    *raiz = endFilho;
                    free(aux->palavraIngles);
                    free(aux);
                } else { // Caso tenha dois filhos
                    // Substitui a palavra pelo menor filho à direita
                    Inglesbin *endFilho = menorFilho((*raiz)->dir);
                    free((*raiz)->palavraIngles);
                    (*raiz)->palavraIngles = strdup(endFilho->palavraIngles);
                    (*raiz)->unidades = endFilho->unidades; // Atualiza a lista de unidades
                    removerPalavraIngles(&(*raiz)->dir, endFilho->palavraIngles, unidade);
                }
            }
        } else if (strcmp(palavra, (*raiz)->palavraIngles) < 0) {
            existe = removerPalavraIngles(&(*raiz)->esq, palavra, unidade);
        } else {
            existe = removerPalavraIngles(&(*raiz)->dir, palavra, unidade);
        }
    }

    return existe;
}






void free_arvore_binaria(Inglesbin *raiz)
{
  if (raiz)
  {
    free_arvore_binaria(raiz->esq);
    free_arvore_binaria(raiz->dir);
    free(raiz->palavraIngles);
    free(raiz);
  }
}
void imprimirTraducoes(Inglesbin *node, int unidade, const char *palavraPortugues)
{
    if (node)
    {
        // Percorre a subárvore esquerda
        imprimirTraducoes(node->esq, unidade, palavraPortugues);

        // Verifica se a unidade está associada à palavra em inglês
        if (buscar_unidade(node->unidades, unidade)) // Verifica se a unidade existe na lista
        {
            printf("Palavra em Português: %s\n", palavraPortugues);
            printf("Tradução em Inglês: %s\n", node->palavraIngles);
        }

        // Percorre a subárvore direita
        imprimirTraducoes(node->dir, unidade, palavraPortugues);
    }
}

Inglesbin *buscarPalavraIngles(Inglesbin *raiz, const char *palavraIngles) {
    Inglesbin *resultado = NULL;

    if (raiz != NULL) {
        int comparacao = strcmp(palavraIngles, raiz->palavraIngles);

        if (comparacao == 0) {
            resultado = raiz; // Palavra encontrada
        } else if (comparacao < 0) {
            resultado = buscarPalavraIngles(raiz->esq, palavraIngles);
        } else {
            resultado = buscarPalavraIngles(raiz->dir, palavraIngles);
        }
    }

    return resultado;
}


void BuscarPalavraIngles(Portugues23 **raiz, char *palavraIngles, int unidade, Portugues23 **pai)
{
    if (*raiz != NULL)
    {
        // Verifica na subárvore esquerda
        BuscarPalavraIngles(&(*raiz)->esq, palavraIngles, unidade, pai);

        // Verifica no primeiro elemento do nó
        if ((*raiz)->info1.palavraIngles != NULL)
        {
            Inglesbin *traducaoEncontrada = buscarPalavraIngles((*raiz)->info1.palavraIngles, palavraIngles);
            if (traducaoEncontrada != NULL && buscar_unidade(traducaoEncontrada->unidades, unidade))
            {
                printf("Palavra em Inglês: %s encontrada na Unidade: %d\n", palavraIngles, unidade);
            }
        }

        // Verifica na subárvore central
        BuscarPalavraIngles(&(*raiz)->cent, palavraIngles, unidade, raiz);

        // Se o nó tem dois elementos, verifica o segundo
        if ((*raiz)->nInfos == 2 && (*raiz)->info2.palavraIngles != NULL)
        {
            Inglesbin *traducaoEncontrada = buscarPalavraIngles((*raiz)->info2.palavraIngles, palavraIngles);
            if (traducaoEncontrada != NULL && buscar_unidade(traducaoEncontrada->unidades, unidade))
            {
                printf("Palavra em Inglês: %s encontrada na Unidade: %d\n", palavraIngles, unidade);
            }
        }

        // Verifica na subárvore direita
        if ((*raiz)->nInfos == 2)
        {
            BuscarPalavraIngles(&(*raiz)->dir, palavraIngles, unidade, raiz);
        }
    }
}

void exibirTraducoesIngles(Inglesbin *raiz)
{
    if (raiz)
    {
        // Percorre a subárvore esquerda
        exibirTraducoesIngles(raiz->esq);

        // Exibe a tradução em inglês
        printf("- %s\n", raiz->palavraIngles);

        // Percorre a subárvore direita
        exibirTraducoesIngles(raiz->dir);
    }
}
