#include "arvbin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvrb.h"
#include "unidade.h"

BinaryTreeNode *initializeBinaryTreeNode(char *palavraIngles, int unidade) {
    // Aloca memória para o novo nó
    BinaryTreeNode *novoNo = (BinaryTreeNode *)malloc(sizeof(BinaryTreeNode));
    if (novoNo == NULL) {
        perror("Erro ao alocar memória para o nó da árvore binária");
        exit(EXIT_FAILURE);
    }

    // Inicializa os campos do nó
    strcpy(novoNo->englishWord, palavraIngles);
    novoNo->unitValues = criar_unidade(unidade); // Cria a lista de unidades com a primeira unidade
    novoNo->left = NULL;
    novoNo->right = NULL;

    return novoNo;
}

BinaryTreeNode *insertEnglishWord(BinaryTreeNode *root, char *palavraIngles, int unidade) {
    BinaryTreeNode *result = root; // Variável para manter o único retorno

    if (root == NULL) {
        // Cria um novo nó da árvore binária
        BinaryTreeNode *novoNo = initializeBinaryTreeNode(palavraIngles, unidade);
        if (novoNo != NULL) {
            result = novoNo;
        } else {
            perror("Erro ao alocar memória para o nó da árvore binária.");
        }
    } else {
        int cmp = strcmp(palavraIngles, root->englishWord);
        if (cmp < 0) {
            root->left = insertEnglishWord(root->left, palavraIngles, unidade);
        } else if (cmp > 0) {
            root->right = insertEnglishWord(root->right, palavraIngles, unidade);
        } else {
            // Palavra já existe, adiciona a unidade à lista ordenada
            Unidade *novaUnidade = criar_unidade(unidade);
            if (novaUnidade != NULL) {
                root->unitValues = adicionar_unidade_ordenada(root->unitValues, novaUnidade);
            } else {
                perror("Erro ao alocar memória para a unidade.");
            }
        }
    }

    return result; // Retorna a árvore atualizada ou o novo nó
}

void addEnglishTranslation(RedBlackTreePT *raiz, char *palavraIng, int unidade) {
    if (raiz != NULL) {
        BinaryTreeNode *novoRoot = insertEnglishWord(raiz->info.englishWordNode, palavraIng, unidade);
        raiz->info.englishWordNode = novoRoot;
    } else {
        perror("Erro: Nó da árvore vermelho-preto é nulo.");
    }
}

void printBinaryTree(BinaryTreeNode *root) {
    if (root != NULL) {
        printBinaryTree(root->left);

        // Imprime a palavra em inglês
        printf("Palavra em inglês: %s\n", root->englishWord);

        // Imprime as unidades associadas (caso necessário para depuração)
        Unidade *currentUnit = root->unitValues;
        printf("Unidades: ");
        while (currentUnit != NULL) {
            printf("%d ", currentUnit->unidade);
            currentUnit = currentUnit->prox;
        }
        printf("\n");

        printBinaryTree(root->right);
    }
}



int isLeafNodes(BinaryTreeNode *raiz)
{
    return (raiz->left == NULL && raiz->right == NULL);
}

BinaryTreeNode *singleChildNode(BinaryTreeNode *raiz)
{
    BinaryTreeNode *aux;
    aux = NULL;

    if (raiz->right == NULL)
    {
        aux = raiz->left;
    }
    else if (raiz->left == NULL)
    {
        aux = raiz->right;
    }

    return aux;
}

BinaryTreeNode *minimumChildNode(BinaryTreeNode *raiz)
{
    BinaryTreeNode *aux;
    aux = raiz;

    if (raiz)
    {
        if (raiz->left)
            aux = minimumChildNode(raiz->left);
    }

    return aux;
}

int removeEnglishWord(BinaryTreeNode **raiz, char *palavra) {
    int existe = 0; // Variável para manter o único retorno
    BinaryTreeNode *endFilho = NULL;

    if (*raiz) {
        if (strcmp(palavra, (*raiz)->englishWord) == 0) {
            existe = 1;
            printf("Removendo palavra: %s\n", palavra);
            BinaryTreeNode *aux = *raiz;

            if (isLeafNodes(*raiz)) {
                // Libera a lista de unidades associada
                liberar_lista(aux->unitValues);
                free(aux);
                *raiz = NULL;
            } else if ((endFilho = singleChildNode(*raiz)) != NULL) {
                // Libera a lista de unidades associada
                liberar_lista(aux->unitValues);
                free(aux);
                *raiz = endFilho;
            } else {
                // Substitui pelo menor nó da subárvore direita
                endFilho = minimumChildNode((*raiz)->right);
                strcpy((*raiz)->englishWord, endFilho->englishWord);

                // Libera a lista antiga e copia a nova
                liberar_lista((*raiz)->unitValues);
                (*raiz)->unitValues = NULL;

                // Clona a lista de unidades do nó substituto
                Unidade *currentUnit = endFilho->unitValues;
                while (currentUnit != NULL) {
                    Unidade *novaUnidade = criar_unidade(currentUnit->unidade);
                    (*raiz)->unitValues = adicionar_unidade_ordenada((*raiz)->unitValues, novaUnidade);
                    currentUnit = currentUnit->prox;
                }

                removeEnglishWord(&(*raiz)->right, endFilho->englishWord);
            }
        } else if (strcmp(palavra, (*raiz)->englishWord) < 0) {
            existe = removeEnglishWord(&(*raiz)->left, palavra);
        } else {
            existe = removeEnglishWord(&(*raiz)->right, palavra);
        }
    }

    return existe; // Único ponto de retorno
}


void FindEnglishTerm(RedBlackTreePT **raiz, char *palavraIngles, int unidade) {
    if (*raiz != NULL) {
        // Recursão para o lado esquerdo da árvore
        FindEnglishTerm(&(*raiz)->left, palavraIngles, unidade);

        // Verifica se a árvore binária de palavras em inglês não está vazia
        if ((*raiz)->info.englishWordNode != NULL) {
            // Percorre a árvore binária para encontrar a palavra e remover a unidade
            BinaryTreeNode *currentNode = (*raiz)->info.englishWordNode;
            while (currentNode != NULL) {
                if (strcmp(currentNode->englishWord, palavraIngles) == 0) {
                    // Encontra a palavra e remove a unidade da lista
                    Unidade *novaLista = remover_unidade(currentNode->unitValues, unidade);
                    currentNode->unitValues = novaLista;

                    // Verifica se a lista de unidades está vazia
                    if (currentNode->unitValues == NULL) {
                        // Remove o nó da árvore binária se não houver mais unidades
                        removeEnglishWord(&(*raiz)->info.englishWordNode, palavraIngles);
                    }
                    break; // Palavra encontrada e tratada
                }

                // Navega para o próximo nó na árvore binária
                if (strcmp(palavraIngles, currentNode->englishWord) < 0) {
                    currentNode = currentNode->left;
                } else {
                    currentNode = currentNode->right;
                }
            }
        }

        // Recursão para o lado direito da árvore
        FindEnglishTerm(&(*raiz)->right, palavraIngles, unidade);
    }
}
