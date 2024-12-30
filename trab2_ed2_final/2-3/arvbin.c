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

int removerPalavraIngles(Inglesbin **raiz, char *palavra)
{
    Inglesbin *endFilho;
    int existe = 0;

    if (*raiz)
    {
        // Se a palavra a ser removida é a mesma que a do nó atual
        if (strcmp(palavra, (*raiz)->palavraIngles) == 0)
        {
            existe = 1;
            printf("Removendo palavra: %s\n", palavra);

            // Libera a lista de unidades associada à palavra em inglês
            liberar_lista((*raiz)->unidades);

            Inglesbin *aux = *raiz;
            if (ehFolhas(*raiz))  // Caso o nó não tenha filhos
            {
                free(aux);
                *raiz = NULL;
            }
            else if ((endFilho = soUmFilho(*raiz)) != NULL)  // Caso tenha um filho
            {
                *raiz = endFilho;
                free(aux);
            }
            else  // Caso tenha dois filhos
            {
                // Substitui a palavra pelo menor filho à direita
                endFilho = menorFilho((*raiz)->dir);
                strcpy((*raiz)->palavraIngles, endFilho->palavraIngles);
                // Aqui não usamos 'unidade', já que o campo correto é 'unidades'
                (*raiz)->unidades = endFilho->unidades;  // Associa a lista de unidades do menor filho

                // Remove o nó com a palavra substituída
                removerPalavraIngles(&(*raiz)->dir, endFilho->palavraIngles);
            }
        }
        // Se a palavra for menor, busca na subárvore esquerda
        else if (strcmp(palavra, (*raiz)->palavraIngles) < 0)
        {
            existe = removerPalavraIngles(&(*raiz)->esq, palavra);
        }
        // Se a palavra for maior, busca na subárvore direita
        else
        {
            existe = removerPalavraIngles(&(*raiz)->dir, palavra);
        }
    }

    return existe;
}


void removerTraducaoIngles(Portugues23 **raiz, char *palavraIngles, int unidade, Portugues23 **pai)
{
    int removeu;
    if (*raiz != NULL)
    {
        // Busca na subárvore esquerda
        removerTraducaoIngles(&(*raiz)->esq, palavraIngles, unidade, pai);

        // Verifica se a palavra em inglês associada à primeira tradução corresponde à unidade
        if ((*raiz)->info1.palavraIngles != NULL && (*raiz)->info1.palavraIngles->unidades->unidade == unidade)
        {
            removeu = removerPalavraIngles(&(*raiz)->info1.palavraIngles, palavraIngles);
            if (removeu) 
                printf("A palavra %s foi removida com sucesso!\n\n", palavraIngles);
            
            // Se a palavra em inglês for removida e o nó for vazio, remove a palavra em português
            if ((*raiz)->info1.palavraIngles == NULL)
            {
                removeu = remover23(pai, raiz, (*raiz)->info1.portugueseWord);
                if (removeu) printf("Palavra em português removida: %s\n\n", (*raiz)->info1.portugueseWord);
            }
        }

        // Busca na subárvore central
        removerTraducaoIngles(&(*raiz)->cent, palavraIngles, unidade, raiz);

        // Verifica se a palavra em inglês associada à segunda tradução corresponde à unidade
        if ((*raiz)->nInfos == 2 && (*raiz)->info2.palavraIngles != NULL && (*raiz)->info2.palavraIngles->unidades->unidade == unidade)
        {
            removeu = removerPalavraIngles(&(*raiz)->info2.palavraIngles, palavraIngles);
            if (removeu) 
                printf("A palavra %s foi removida com sucesso!\n\n", palavraIngles);

            // Se a palavra em inglês for removida e o nó for vazio, remove a palavra em português
            if ((*raiz)->info2.palavraIngles == NULL)
            {
                removeu = remover23(pai, raiz, (*raiz)->info2.portugueseWord);
                if (removeu) printf("Palavra em português removida: %s\n\n", (*raiz)->info2.portugueseWord);
            }
        }

        // Se o nó tiver dois elementos e o segundo ainda tiver tradução, continua a busca na subárvore direita
        if ((*raiz)->nInfos == 2 && (*raiz)->info2.palavraIngles != NULL)
        {
            removerTraducaoIngles(&(*raiz)->dir, palavraIngles, unidade, raiz);
        }
    }
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

Inglesbin *buscarPalavraIngles(Inglesbin *raiz, const char *palavraIngles)
{
    if (raiz == NULL)
    {
        return NULL;
    }
    if (strcmp(palavraIngles, raiz->palavraIngles) == 0)
    {
        return raiz; // Palavra encontrada
    }
    else if (strcmp(palavraIngles, raiz->palavraIngles) < 0)
    {
        return buscarPalavraIngles(raiz->esq, palavraIngles);
    }
    else
    {
        return buscarPalavraIngles(raiz->dir, palavraIngles);
    }
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
