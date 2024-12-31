#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvbin.h"
#include "arvrb.h"
#include "unidade.h"
#define RED 1
#define BLACK 0

Info createInfo(char *palavra, char *palavraIngles, int unidade) {
    Info info;

    // Aloca e copia a palavra em português
    info.portugueseWord = malloc(strlen(palavra) + 1);
    if (info.portugueseWord == NULL) {
        perror("Erro ao alocar memória para a palavra em português.");
        exit(EXIT_FAILURE);
    }
    strcpy(info.portugueseWord, palavra);

    // Inicializa a árvore binária de palavras em inglês
    info.englishWordNode = NULL;

    // Insere a palavra em inglês e a unidade na árvore binária
    info.englishWordNode = insertEnglishWord(info.englishWordNode, palavraIngles, unidade);

    return info;
}

int insertPortugueseWord(RedBlackTreePT **arvore, char *palavraPortugues, char *palavraIngles, int unidade) {
    int inseriu = 0; // Variável para controlar o estado da inserção
    RedBlackTreePT *noExistente = NULL;

    // Busca a palavra na árvore rubro-negra
    noExistente = SearchWordInTree(arvore, palavraPortugues);

    if (noExistente != NULL) {
        // Adiciona a palavra em inglês e unidade no nó existente
        addEnglishTranslation(noExistente, palavraIngles, unidade);
        inseriu = 1;
    } else {
        // Cria uma nova informação e insere na árvore rubro-negra
        Info novoInfo = createInfo(palavraPortugues, palavraIngles, unidade);
        if (insertRedBlackTree(arvore, &novoInfo)) {
            inseriu = 1;
        }
    }

    return inseriu; // Único ponto de retorno
}

RedBlackTreePT *createNode(Info *informacao)
{
    RedBlackTreePT *novo = (RedBlackTreePT *)malloc(sizeof(RedBlackTreePT));
    novo->info = *informacao;
    novo->color = 1;
    novo->left = NULL;
    novo->right = NULL;
    return novo;
}
int getNodeColor(RedBlackTreePT *raiz)
{
    int cor;

    if (raiz == NULL)
        cor = BLACK;
    else
        cor = raiz->color;

    return cor;
}

void switch_node_color(RedBlackTreePT **raiz)
{
    (*raiz)->color = !(*raiz)->color;
    if ((*raiz)->left)
        (*raiz)->left->color = !(*raiz)->left->color;
    if ((*raiz)->right)
        (*raiz)->right->color = !(*raiz)->right->color;
}

void rotate_right(RedBlackTreePT **raiz)
{
    RedBlackTreePT *aux = (*raiz)->left;
    (*raiz)->left = aux->right;
    aux->right = *raiz;
    aux->color = (*raiz)->color;
    (*raiz)->color = RED;
    (*raiz) = aux;
}

void left_rotate(RedBlackTreePT **raiz)
{
    RedBlackTreePT *aux = (*raiz)->right;
    (*raiz)->right = aux->left;
    aux->left = *raiz;
    aux->color = (*raiz)->color;
    (*raiz)->color = RED;
    (*raiz) = aux;
}

void balanceTree(RedBlackTreePT **raiz)
{
    if (*raiz)
    {
        if (getNodeColor((*raiz)->right) == RED && getNodeColor((*raiz)->left) == BLACK)
            left_rotate(raiz);
        if (getNodeColor((*raiz)->left) == RED && getNodeColor((*raiz)->left->left) == RED)
            rotate_right(raiz);
        if (getNodeColor((*raiz)->left) == RED && getNodeColor((*raiz)->right) == RED)
            switch_node_color(raiz);
    }
}

int insertRedBlackNode(RedBlackTreePT **raiz, Info *informacao)
{

    int inseriu = 1;
    if (*raiz == NULL)
    {
        *raiz = createNode(informacao);
    }
    else
    {
        if (strcmp(informacao->portugueseWord, (*raiz)->info.portugueseWord) < 0)
        {
            inseriu = insertRedBlackNode(&(*raiz)->left, informacao);
        }
        else
        {
            inseriu = insertRedBlackNode(&(*raiz)->right, informacao);
        }
        balanceTree(&(*raiz));
    }

    return inseriu;
}

int insertRedBlackTree(RedBlackTreePT **raiz, Info *informacao)
{
    int inseriu = insertRedBlackNode(raiz, informacao);
    if (inseriu)
    {
        (*raiz)->color = BLACK;
    }
    return inseriu;
}

void shiftLeftRed(RedBlackTreePT **raiz)
{
    switch_node_color(raiz);

    if ((*raiz)->right && getNodeColor((*raiz)->right->left) == RED)
    {
        rotate_right(&(*raiz)->right);
        left_rotate((raiz));
        switch_node_color(raiz);
    }
}

void rotateRedRight(RedBlackTreePT **raiz)
{
    switch_node_color(raiz);

    if ((*raiz)->left && getNodeColor((*raiz)->left->left) == RED)
    {
        rotate_right(raiz);
        switch_node_color(raiz);
    }
}

void removeMinimum(RedBlackTreePT **raiz)
{
    if (!((*raiz)->left))
    {
        free(*raiz);
        *raiz = NULL;
    }
    else
    {
        if (getNodeColor((*raiz)->left) == BLACK && getNodeColor((*raiz)->left->left) == BLACK)
            shiftLeftRed(raiz);

        removeMinimum(&(*raiz)->left);
        balanceTree(raiz);
    }
}

RedBlackTreePT *findMinimum(RedBlackTreePT *raiz)
{
    RedBlackTreePT *menor;
    menor = raiz;

    if (raiz)
        if (raiz->left)
            menor = findMinimum(raiz->left);

    return menor;
}

int removeNodeFromRBTree(RedBlackTreePT **raiz, char *valor)
{
    int existe = 0;

    if (*raiz)
    {
        if (strcmp(valor, (*raiz)->info.portugueseWord) < 0)
        {
            if ((*raiz)->left && getNodeColor((*raiz)->left) == BLACK && getNodeColor((*raiz)->left->left) == BLACK)
                shiftLeftRed(raiz);

            existe = removeNodeFromRBTree(&(*raiz)->left, valor);
        }
        else
        {
            if (getNodeColor((*raiz)->left) == RED)
                rotate_right(raiz);

            if (strcmp(valor, (*raiz)->info.portugueseWord) == 0 && (*raiz)->right == NULL)
            {
                free(*raiz);
                *raiz = NULL;

                existe = 1;
            }else{
                if ((*raiz)->right && getNodeColor((*raiz)->right) == BLACK && getNodeColor((*raiz)->right->left) == BLACK)
                rotateRedRight(raiz);

                if (strcmp(valor, (*raiz)->info.portugueseWord) == 0)
                {
                    RedBlackTreePT *aux;
                    aux = findMinimum((*raiz)->right);
                    (*raiz)->info = aux->info;
                    removeMinimum(&(*raiz)->right);

                    existe = 1;
                }
                else
                {
                    existe = removeNodeFromRBTree(&(*raiz)->right, valor);
                }
            }
        }
    }
    balanceTree(raiz);
    return existe;
}

int removeRBTreeNode(RedBlackTreePT **raiz, char *valor)
{
    int removeu = removeNodeFromRBTree(raiz, valor);
    if (removeu)
    {
        (*raiz)->color = BLACK;
    }
    return removeu;
}


RedBlackTreePT *SearchWordInTree(RedBlackTreePT **arvore, char *palavraPortugues) {
    RedBlackTreePT *atual = NULL;

    if (*arvore != NULL) {
        // Exibe a palavra com a qual está comparando
        printf("[DEBUG] Comparando '%s' com '%s'\n", palavraPortugues, (*arvore)->info.portugueseWord);

        if (strcmp(palavraPortugues, (*arvore)->info.portugueseWord) == 0) {
            printf("[DEBUG] Palavra encontrada: '%s'\n", (*arvore)->info.portugueseWord);
            atual = *arvore;
        } else if (strcmp(palavraPortugues, (*arvore)->info.portugueseWord) < 0) {
            printf("[DEBUG] Indo para a esquerda.\n");
            atual = SearchWordInTree(&(*arvore)->left, palavraPortugues);
        } else {
            printf("[DEBUG] Indo para a direita.\n");
            atual = SearchWordInTree(&(*arvore)->right, palavraPortugues);
        }
    } else {
        printf("[DEBUG] Chegou a um nó NULL.\n");
    }

    return atual;
}


void printWordsByUnit(RedBlackTreePT *arvore, int unidade)
{
    if (arvore)
    {
        printWordsByUnit(arvore->left, unidade);
        printTranslations(arvore->info.englishWordNode, unidade, arvore->info.portugueseWord);
        printWordsByUnit(arvore->right, unidade);
    }
}

void printTranslations(BinaryTreeNode *node, int unidade, char *palavraPortugues) {
    if (node) {
        // Verifica se a unidade fornecida está presente na lista de unidades
        Unidade *currentUnit = node->unitValues;
        int unidadeEncontrada = 0;

        while (currentUnit != NULL) {
            if (currentUnit->unidade == unidade) {
                unidadeEncontrada = 1;
                break;
            }
            currentUnit = currentUnit->prox;
        }

        // Se a unidade foi encontrada, imprime a tradução
        if (unidadeEncontrada) {
            printf("Palavra em Português: %s\n", palavraPortugues);
            printf("Palavra em inglês: %s\n", node->englishWord);
        }

        // Percorre os subnós da árvore binária
        printTranslations(node->left, unidade, palavraPortugues);
        printTranslations(node->right, unidade, palavraPortugues);
    }
}

void showPortugueseTranslation(RedBlackTreePT **raiz, char *palavraPortugues) {
    if (*raiz != NULL) {
        // Busca a palavra na árvore rubro-negra
        printf("[DEBUG] Buscando a palavra em português: '%s'\n", palavraPortugues);
        RedBlackTreePT *resultado = SearchWordInTree(raiz, palavraPortugues);

        if (resultado != NULL) {
            printf("[DEBUG] Palavra encontrada na árvore rubro-negra.\n");
            printf("Traduções em inglês para a palavra '%s':\n", palavraPortugues);

            // Percorre a árvore binária associada e imprime as traduções
            BinaryTreeNode *node = resultado->info.englishWordNode;
            if (node == NULL) {
                printf("Nenhuma tradução encontrada.\n");
            } else {
                printBinaryTree(node);
            }
        } else {
            printf("[DEBUG] Palavra '%s' não encontrada na árvore rubro-negra.\n", palavraPortugues);
            printf("Nenhuma tradução encontrada para '%s'.\n", palavraPortugues);
        }
    } else {
        printf("[DEBUG] A árvore rubro-negra está vazia.\n");
    }
}


void showRedBlackTree(RedBlackTreePT *raiz)
{
    if (raiz)
    {
        showRedBlackTree(raiz->left);
        printf("Cor - %d\n", raiz->color);
        printf("Palavra em Português - %s\n", raiz->info.portugueseWord);
        printBinaryTree(raiz->info.englishWordNode);
        printf("\n");
        showRedBlackTree(raiz->right);
    }
}

RedBlackTreePT *SearchEnglishWordInRBTree(RedBlackTreePT *raiz, char *palavraIngles, int unidade) {
    RedBlackTreePT *result = NULL; // Variável para armazenar o retorno

    if (raiz != NULL) {
        // Verifica o nó atual da árvore vermelho-preto
        BinaryTreeNode *currentNode = raiz->info.englishWordNode;
        while (currentNode != NULL) {
            printf("Verificando palavra: '%s' na unidade %d\n", currentNode->englishWord, unidade);

            Unidade *currentUnit = currentNode->unitValues;
            int unidadeEncontrada = 0;

            // Verifica se a unidade existe na lista
            while (currentUnit != NULL) {
                if (currentUnit->unidade == unidade) {
                    unidadeEncontrada = 1;
                    break;
                }
                currentUnit = currentUnit->prox;
            }

            if (unidadeEncontrada && strcmp(currentNode->englishWord, palavraIngles) == 0) {
                printf("Palavra encontrada na árvore binária associada ao nó português: '%s'\n", raiz->info.portugueseWord);
                result = raiz;
                break;
            }

            if (strcmp(palavraIngles, currentNode->englishWord) < 0) {
                currentNode = currentNode->left;
            } else {
                currentNode = currentNode->right;
            }
        }

        // Se não encontrou no nó atual, verifica à esquerda
        if (result == NULL) {
            result = SearchEnglishWordInRBTree(raiz->left, palavraIngles, unidade);
        }

        // Se não encontrou na esquerda, verifica à direita
        if (result == NULL) {
            result = SearchEnglishWordInRBTree(raiz->right, palavraIngles, unidade);
        }
    }

    return result; // Retorna o resultado armazenado
}
void removeWordFromTree(RedBlackTreePT **node, char *wordToRemove, int *totalRemovido, RedBlackTreePT **rootNode) {
    if (*node != NULL) {
        // Percorre à esquerda
        removeWordFromTree(&(*node)->left, wordToRemove, totalRemovido, rootNode);

        // Remove a palavra da árvore binária associada
        BinaryTreeNode *binaryRoot = (*node)->info.englishWordNode;
        int removed = removeEnglishWord(&binaryRoot, wordToRemove);
        if (removed) {
            (*totalRemovido)++;
            printf("[DEBUG] Palavra '%s' removida da árvore binária associada ao nó com palavra portuguesa: '%s'.\n",
                   wordToRemove, (*node)->info.portugueseWord);
        }

        // Verifica se a árvore binária ficou vazia após a remoção
        if (binaryRoot == NULL) {
            printf("[DEBUG] Árvore binária associada ao nó '%s' ficou vazia. Removendo nó da Árvore Rubro-Negra.\n",
                   (*node)->info.portugueseWord);
            removeRBTreeNode(rootNode, (*node)->info.portugueseWord);
        } else {
            (*node)->info.englishWordNode = binaryRoot;
        }

        // Percorre à direita
        removeWordFromTree(&(*node)->right, wordToRemove, totalRemovido, rootNode);
    }
}
void removeWordByUnit(RedBlackTreePT **node, char *wordToRemove, int unit, int *removidos, RedBlackTreePT **rootNode) {
    if (*node != NULL) {
        // Remove da subárvore esquerda
        removeWordByUnit(&(*node)->left, wordToRemove, unit, removidos, rootNode);

        // Remove a palavra da árvore binária associada
        BinaryTreeNode *binaryRoot = (*node)->info.englishWordNode;
        if (binaryRoot != NULL) {
            printf("[DEBUG] Tentando remover a palavra '%s' na unidade %d da árvore binária.\n", wordToRemove, unit);

            // Remove a palavra da unidade específica
            BinaryTreeNode *currentNode = binaryRoot;
            while (currentNode != NULL) {
                if (strcmp(currentNode->englishWord, wordToRemove) == 0) {
                    Unidade *novaLista = remover_unidade(currentNode->unitValues, unit);
                    currentNode->unitValues = novaLista;

                    // Se a lista de unidades ficar vazia, remove a palavra
                    if (currentNode->unitValues == NULL) {
                        printf("[DEBUG] Palavra '%s' ficou sem unidades. Removendo nó da árvore binária.\n", wordToRemove);
                        int removed = removeEnglishWord(&binaryRoot, wordToRemove);
                        if (removed) (*removidos)++;
                    }
                    break;
                }
                if (strcmp(wordToRemove, currentNode->englishWord) < 0) {
                    currentNode = currentNode->left;
                } else {
                    currentNode = currentNode->right;
                }
            }

            (*node)->info.englishWordNode = binaryRoot;

            // Verifica se a árvore binária ficou vazia
            if (binaryRoot == NULL) {
                printf("[DEBUG] Árvore binária associada ao nó '%s' ficou vazia. Removendo nó da Árvore Rubro-Negra.\n",
                       (*node)->info.portugueseWord);
                removeRBTreeNode(rootNode, (*node)->info.portugueseWord);
            }
        }

        // Remove da subárvore direita
        removeWordByUnit(&(*node)->right, wordToRemove, unit, removidos, rootNode);
    }
}
