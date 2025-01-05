#include "arvbin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unidade.h"


Inglesbin *initializeNode(const char *palavraIngles, int unidade) {
    Inglesbin *novoNo = (Inglesbin *)malloc(sizeof(Inglesbin));
    if (novoNo != NULL) {
        novoNo->palavraIngles = (char *)malloc(strlen(palavraIngles) + 1);
        if (novoNo->palavraIngles != NULL) {
            strcpy(novoNo->palavraIngles, palavraIngles);
        }
        
        // Inicializar a lista ligada de unidades com o primeiro valor
        novoNo->unidades = create_unit(unidade);
        
        // Inicializar os ponteiros da árvore binária
        novoNo->esq = novoNo->dir = NULL;
    }
    return novoNo;
}


// Função para inserir uma palavra em inglês na arvore binaria de busca
Inglesbin *insertEnglishWord(Inglesbin *root, const char *palavraIngles, int unidade) {
    Inglesbin *result;

    if (root == NULL) {
        // Cria um novo nó para a palavra em inglês e adiciona a unidade à lista
        result = initializeNode(palavraIngles, unidade);
    } else {
        // Se a palavra em inglês já existe, adiciona a unidade à lista ordenada
        if (strcmp(palavraIngles, root->palavraIngles) == 0) {
            root->unidades = insert_unit_sorted(root->unidades, create_unit(unidade));
            result = root;
        } else {
            // Caso contrário, insere recursivamente na árvore binária
            if (strcmp(palavraIngles, root->palavraIngles) < 0) {
                root->esq = insertEnglishWord(root->esq, palavraIngles, unidade);
            } else {
                root->dir = insertEnglishWord(root->dir, palavraIngles, unidade);
            }
            result = root;
        }
    }

    return result;
}

void addEnglishTranslation(NodeInfo *info, const char *palavraIng, int unidade) {
    // Verifica se a árvore binária de traduções em inglês está vazia
    if (info->englishWord == NULL) {
        // Se a árvore estiver vazia, cria a palavra em inglês com a unidade associada
        info->englishWord = initializeNode(palavraIng, unidade);
    } else {
        // Se a árvore não estiver vazia, insere a palavra em inglês de maneira ordenada na árvore
        info->englishWord = insertEnglishWord(info->englishWord, palavraIng, unidade);
    }

    // Agora associa a unidade à palavra em inglês, garantindo a ordem das unidades
    info->englishWord->unidades = insert_unit_sorted(info->englishWord->unidades, create_unit(unidade));
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
            print_units(root->unidades);  // Usa a função 'imprimir_unidades' para imprimir as unidades
        }
        else {
            printf("Nenhuma unidade associada.\n");
        }
        
        // Percorre a árvore à direita
        printBinaryTree(root->dir);
    }
}


int isLeafNode(Inglesbin *raiz){
    return (raiz->esq == NULL && raiz->dir == NULL);
}

Inglesbin *getSingleChild(Inglesbin *raiz){
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

Inglesbin *getMinimumChild(Inglesbin *raiz){
    Inglesbin *aux;
    aux = raiz;

    if (raiz)
    {
        if (raiz->esq)
            aux = getMinimumChild(raiz->esq);
    }

    return aux;
}

int removeEnglishWord(Inglesbin **raiz, const char *palavra, int unidade) {
    Inglesbin *endFilho = NULL;
    int existe = 0;

    if (*raiz) {
        // Verifica se a palavra em inglês corresponde ao nó atual
        if (strcmp(palavra, (*raiz)->palavraIngles) == 0) {
            Inglesbin *aux = *raiz;
            existe = 1;

            // Remove a unidade da lista associada
            Unit *novaLista = remove_unit((*raiz)->unidades, unidade);

            if (novaLista != (*raiz)->unidades) {
                (*raiz)->unidades = novaLista;

                // Se a lista de unidades ficar vazia, remover o nó
                if (novaLista == NULL) {
                    if (isLeafNode(*raiz)) {
                        free(aux);
                        *raiz = NULL;
                    } else if ((endFilho = getSingleChild(*raiz)) != NULL) {
                        free(aux);
                        *raiz = endFilho;
                    } else {
                        endFilho = getMinimumChild((*raiz)->dir);
                        strcpy((*raiz)->palavraIngles, endFilho->palavraIngles);
                        (*raiz)->unidades = endFilho->unidades;

                        removeEnglishWord(&(*raiz)->dir, endFilho->palavraIngles, unidade);
                    }
                }
            }
        } else if (strcmp(palavra, (*raiz)->palavraIngles) < 0) {
            // Continua a busca na subárvore esquerda
            existe = removeEnglishWord(&(*raiz)->esq, palavra, unidade);
        } else {
            // Continua a busca na subárvore direita
            existe = removeEnglishWord(&(*raiz)->dir, palavra, unidade);
        }
    }

    return existe;
}




void clear_binary_tree(Inglesbin *raiz) {
    if (raiz != NULL) {
        // Debug: Log the word being freed
        printf("Liberando nó da árvore binária com palavra: %s\n", raiz->palavraIngles);

        // Recursively free left and right subtrees
        clear_binary_tree(raiz->esq);
        clear_binary_tree(raiz->dir);

        // Free associated list of units
        free_list(raiz->unidades);

        // Free the English word
        free(raiz->palavraIngles);

        // Free the node itself
        free(raiz);
    }
}

void printTranslations(Inglesbin *node, int unidade, const char *palavraPortugues)
{
    if (node)
    {
        // Percorre a subárvore esquerda
        printTranslations(node->esq, unidade, palavraPortugues);

        // Verifica se a unidade está associada à palavra em inglês
        if (find_unit(node->unidades, unidade)) // Verifica se a unidade existe na lista
        {
            printf("Palavra em Português: %s\n", palavraPortugues);
            printf("Tradução em Inglês: %s\n", node->palavraIngles);
        }

        // Percorre a subárvore direita
        printTranslations(node->dir, unidade, palavraPortugues);
    }
}

Inglesbin *findEnglishWord(Inglesbin *raiz, const char *palavraIngles) {
    Inglesbin *resultado = NULL;

    if (raiz != NULL) {
        int comparacao = strcmp(palavraIngles, raiz->palavraIngles);

        if (comparacao == 0) {
            resultado = raiz; // Palavra encontrada
        } else if (comparacao < 0) {
            resultado = findEnglishWord(raiz->esq, palavraIngles);
        } else {
            resultado = findEnglishWord(raiz->dir, palavraIngles);
        }
    }

    return resultado;
}


void FindEnglishWord(PortugueseTree **raiz, char *palavraIngles, int unidade, PortugueseTree **pai)
{
    if (*raiz != NULL)
    {
        // Verifica na subárvore esquerda
        FindEnglishWord(&(*raiz)->left, palavraIngles, unidade, pai);

        // Verifica no primeiro elemento do nó
        if ((*raiz)->info1.englishWord != NULL)
        {
            Inglesbin *traducaoEncontrada = findEnglishWord((*raiz)->info1.englishWord, palavraIngles);
            if (traducaoEncontrada != NULL && find_unit(traducaoEncontrada->unidades, unidade))
            {
                printf("Palavra em Inglês: %s encontrada na Unidade: %d\n", palavraIngles, unidade);
            }
        }

        // Verifica na subárvore central
        FindEnglishWord(&(*raiz)->cent, palavraIngles, unidade, raiz);

        // Se o nó tem dois elementos, verifica o segundo
        if ((*raiz)->nInfos == 2 && (*raiz)->info2.englishWord != NULL)
        {
            Inglesbin *traducaoEncontrada = findEnglishWord((*raiz)->info2.englishWord, palavraIngles);
            if (traducaoEncontrada != NULL && find_unit(traducaoEncontrada->unidades, unidade))
            {
                printf("Palavra em Inglês: %s encontrada na Unidade: %d\n", palavraIngles, unidade);
            }
        }

        // Verifica na subárvore direita
        if ((*raiz)->nInfos == 2)
        {
            FindEnglishWord(&(*raiz)->right, palavraIngles, unidade, raiz);
        }
    }
}

void showEnglishTranslations(Inglesbin *raiz)
{
    if (raiz)
    {
        // Percorre a subárvore esquerda
        showEnglishTranslations(raiz->esq);

        // Exibe a tradução em inglês
        printf("- %s\n", raiz->palavraIngles);

        // Percorre a subárvore direita
        showEnglishTranslations(raiz->dir);
    }
}
