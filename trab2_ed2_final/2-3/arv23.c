#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arv23.h"

PortugueseTree *FindWord(PortugueseTree **node, const char *wordInPortuguese)
{
    PortugueseTree *result = NULL;

    if (node != NULL && *node != NULL)
    {
        if (strcmp(wordInPortuguese, (*node)->info1.portugueseWord) == 0)
        {
            result = *node;
        }
        else if ((*node)->nInfos == 2)
        {

            if (strcmp(wordInPortuguese, (*node)->info2.portugueseWord) == 0)
            {
                result = *node;
            }
        }

        if (result == NULL)
        {
            if (strcmp(wordInPortuguese, (*node)->info1.portugueseWord) < 0)
            {
                result = FindWord(&(*node)->left, wordInPortuguese);
            }
            else if ((*node)->nInfos == 1 || strcmp(wordInPortuguese, (*node)->info2.portugueseWord) < 0)
            {
                result = FindWord(&(*node)->cent, wordInPortuguese);
            }
            else
            {
                result = FindWord(&(*node)->right, wordInPortuguese);
            }
        }
    }

    return result;
}

void insertTranslation(PortugueseTree *node, const char *portugueseWord, const char *englishWord, int unit)
{
    // Verifica se a palavra portuguesa está na primeira posição do nó
    if (strcmp(portugueseWord, node->info1.portugueseWord) == 0)
    {
        // Adiciona a tradução na árvore binária de palavras em inglês
        node->info1.englishWord = insertEnglishWord(node->info1.englishWord, englishWord, unit);
    }
    // Verifica se a palavra portuguesa está na segunda posição do nó (se existir)
    else if (node->nInfos == 2 && strcmp(portugueseWord, node->info2.portugueseWord) == 0)
    {
        // Adiciona a tradução na árvore binária de palavras em inglês
        node->info2.englishWord = insertEnglishWord(node->info2.englishWord, englishWord, unit);
    }
}

int insertPortugueseTerm(PortugueseTree **portugueseTree, char *portugueseWord, char *englishWord, int unit)
{
    NodeInfo promotionInfo;
    PortugueseTree *parentNode = NULL;
    int result = 0; // Inicializa como "novo elemento inserido"

    // Busca a palavra em português na árvore
    PortugueseTree *foundNode = FindWord(portugueseTree, portugueseWord);

    if (foundNode != NULL)
    {
        // Se a palavra em português já existe, adiciona a tradução e a unidade
        insertTranslation(foundNode, portugueseWord, englishWord, unit);
        result = 1; // Atualização realizada
    }
    else
    {
        // Cria uma nova entrada para a palavra em português
        NodeInfo novoInfo = createInfo(portugueseWord, englishWord, unit);
        insertNode23(portugueseTree, &novoInfo, &promotionInfo, &parentNode);
    }

    return result;
}

NodeInfo createInfo(char *portugueseWord, char *englishWord, int unit)
{
    NodeInfo newNodeInfo;
    newNodeInfo.portugueseWord = strdup(portugueseWord); // Armazena a palavra em português
    newNodeInfo.englishWord = NULL;
    newNodeInfo.englishWord = insertEnglishWord(newNodeInfo.englishWord, englishWord, unit);
    return newNodeInfo;
}

PortugueseTree *createNode(NodeInfo *nodeInfo, PortugueseTree *leftChild, PortugueseTree *middlechild)
{
    PortugueseTree *no = (PortugueseTree *)malloc(sizeof(PortugueseTree));
    no->info1 = *nodeInfo;
    no->info2.englishWord = NULL;
    no->info2.portugueseWord = NULL;
    no->left = leftChild;
    no->cent = middlechild;
    no->right = NULL;
    no->nInfos = 1;

    return no;
}

PortugueseTree *updateNode23(PortugueseTree *currentNode, NodeInfo *nodeInfo, PortugueseTree *childNode)
{
    if (strcmp(nodeInfo->portugueseWord, currentNode->info1.portugueseWord) > 0)
    {
        currentNode->info2 = *nodeInfo;
        currentNode->right = childNode;
    }
    else
    {
        currentNode->info2 = currentNode->info1;
        currentNode->right = currentNode->cent;
        currentNode->info1 = *nodeInfo;
        currentNode->cent = childNode;
    }
    currentNode->nInfos = 2;
    return currentNode;
}

PortugueseTree *splitNode(PortugueseTree **currentNode, NodeInfo *nodeInfo, NodeInfo *promotedInfo, PortugueseTree **childNode)
{
    PortugueseTree *largestNode;

    if (strcmp(nodeInfo->portugueseWord, (*currentNode)->info2.portugueseWord) > 0)
    {
        *promotedInfo = (*currentNode)->info2;
        if (childNode)
            largestNode = createNode(nodeInfo, (*currentNode)->right, *childNode);
        else
            largestNode = createNode(nodeInfo, (*currentNode)->right, NULL);
    }
    else if (strcmp(nodeInfo->portugueseWord, (*currentNode)->info1.portugueseWord) > 0)
    {
        *promotedInfo = *nodeInfo;
        if (childNode)
            largestNode = createNode(&(*currentNode)->info2, *childNode, (*currentNode)->right);
        else
            largestNode = createNode(&(*currentNode)->info2, NULL, (*currentNode)->right);
    }
    else
    {
        *promotedInfo = (*currentNode)->info1;
        largestNode = createNode(&(*currentNode)->info2, (*currentNode)->cent, (*currentNode)->right);
        (*currentNode)->info1 = *nodeInfo;
        if (childNode)
            (*currentNode)->cent = *childNode;
        else
            (*currentNode)->cent = NULL;
    }

    (*currentNode)->nInfos = 1;
    return largestNode;
}

int isNodeLeaf(PortugueseTree *no)
{
    return (no->left == NULL);
}

PortugueseTree *insertNode23(PortugueseTree **currentNode, NodeInfo *nodeData, NodeInfo *promotionNode, PortugueseTree **parentNode)
{
    NodeInfo promotionInfo1;
    PortugueseTree *largestNode = NULL;
    if (*currentNode == NULL)
    {
        // Cria um novo nó caso seja nulo
        *currentNode = createNode(nodeData, NULL, NULL);
    }
    else
    {
        if (isNodeLeaf(*currentNode))
        { // Caso seja folha
            if ((*currentNode)->nInfos == 1)
            {
                // O nó tem espaço para a nova chave
                *currentNode = updateNode23(*currentNode, nodeData, NULL);
            }
            else
            {
                // O nó precisa ser quebrado
                largestNode = splitNode(currentNode, nodeData, promotionNode, NULL);
                if (*parentNode == NULL)
                { // Se não há pai, criar nova raiz
                    *currentNode = createNode(promotionNode, *currentNode, largestNode);
                    largestNode = NULL;
                }
            }
        }
        else
        { // Nó não e folha
            // Navega para o filho apropriado
            if (strcmp(nodeData->portugueseWord, (*currentNode)->info1.portugueseWord) < 0)
            {
                largestNode = insertNode23(&((*currentNode)->left), nodeData, promotionNode, currentNode);
            }
            else if ((*currentNode)->nInfos == 1 || strcmp(nodeData->portugueseWord, (*currentNode)->info2.portugueseWord) < 0)
            {
                largestNode = insertNode23(&((*currentNode)->cent), nodeData, promotionNode, currentNode);
            }
            else
            {
                largestNode = insertNode23(&((*currentNode)->right), nodeData, promotionNode, currentNode);
            }

            // Após inserir, verifica se houve promoção
            if (largestNode)
            {
                if ((*currentNode)->nInfos == 1)
                {
                    // Adiciona chave promovida no nó atual
                    *currentNode = updateNode23(*currentNode, promotionNode, largestNode);
                    largestNode = NULL;
                }
                else
                { // Quando não tem espaço
                    // O nó precisa ser quebrado
                    PortugueseTree *newnode;
                    newnode = splitNode(currentNode, promotionNode, &promotionInfo1, &largestNode); // Quebra o nó e sobe a informação
                    if (*parentNode == NULL)
                    {
                        PortugueseTree *novaRaiz;
                        novaRaiz = createNode(&promotionInfo1, *currentNode, newnode); // Cria nova raiz se necessário
                        *currentNode = novaRaiz;
                        largestNode = NULL;
                    }
                    else
                    {
                        largestNode = newnode; // Ajusta o novo maior nó
                        *promotionNode = promotionInfo1;
                    }
                }
            }
        }
    }

    return largestNode;
}

// ############################################## FUNÇOES PARA EXIBIR ##############################################

void print_tree23(PortugueseTree *rootNode)
{
    if (rootNode != NULL)
    {
        print_tree23(rootNode->left);
        printf("Palavra (PT): %s", rootNode->info1.portugueseWord);
        if (rootNode->info1.englishWord != NULL && rootNode->info1.englishWord->palavraIngles != NULL)
        {
            printBinaryTree(rootNode->info1.englishWord);
            printf("\n");
        }
        print_tree23(rootNode->cent);
        // Se houver o segundo elemento (nInfos == 2), exibe o segundo filho
        if (rootNode->nInfos == 2)
        {
            printf("Palavra (PT): %s", rootNode->info2.portugueseWord);

            // Exibir a tradução em inglês, se houver
            if (rootNode->info2.englishWord != NULL && rootNode->info2.englishWord->palavraIngles != NULL)
                printBinaryTree(rootNode->info2.englishWord);
            printf("\n");
        }

        if (rootNode->nInfos == 2)
        {
            print_tree23(rootNode->right);
        }
    }
}

void printWordsAtUnit(PortugueseTree *portugueseTree, int unit, int *printedUnit)
{
    if (portugueseTree)
    {
        // Percorre a subárvore esquerda
        printWordsAtUnit(portugueseTree->left, unit, printedUnit);

        // Imprime o cabeçalho da unidade uma vez
        if (!(*printedUnit))
        {
            printf("%% Unidade %d\n", unit); // Cabeçalho da unidade
            *printedUnit = 1;                // Marca como já impresso
        }

        // Imprime as traduções associadas à unidade para a primeira palavra do nó
        if (portugueseTree->info1.englishWord != NULL)
        {
            printTranslationsInFormat(portugueseTree->info1.englishWord, unit, portugueseTree->info1.portugueseWord);
        }

        // Percorre a subárvore central
        printWordsAtUnit(portugueseTree->cent, unit, printedUnit);

        // Se houver o segundo elemento no nó, imprime também suas traduções
        if (portugueseTree->nInfos == 2 && portugueseTree->info2.englishWord != NULL)
        {
            printTranslationsInFormat(portugueseTree->info2.englishWord, unit, portugueseTree->info2.portugueseWord);
        }

        // Percorre a subárvore direita, se existir
        if (portugueseTree->nInfos == 2)
        {
            printWordsAtUnit(portugueseTree->right, unit, printedUnit);
        }
    }
}

void printTranslationsInFormat(Inglesbin *node, int unit, const char *portugueseWord)
{
    if (node)
    {
        // Percorre a subárvore esquerda
        printTranslationsInFormat(node->esq, unit, portugueseWord);

        // Verifica se a unidade está associada à palavra em inglês
        if (find_unit(node->unidades, unit)) // Verifica se a unidade está presente na lista
        {
            // Remove o ';' no final de palavraPortugues, se existir
            char palavraFormatada[256];
            strcpy(palavraFormatada, portugueseWord);
            size_t len = strlen(palavraFormatada);
            if (len > 0 && palavraFormatada[len - 1] == ';')
            {
                palavraFormatada[len - 1] = '\0';
            }

            printf("%s: %s;\n", palavraFormatada, node->palavraIngles); // Formata corretamente
        }

        // Percorre a subárvore direita
        printTranslationsInFormat(node->dir, unit, portugueseWord);
    }
}

void printAllTranslations(PortugueseTree *rootNode, const char *portugueseWord)
{
    int isFound = 0;

    if (rootNode == NULL)
    {
        printf("A palavra '%s' nao foi encontrada.\n", portugueseWord);
    }
    else
    {
        // Verifica o no info1
        if (strcmp(rootNode->info1.portugueseWord, portugueseWord) == 0)
        {
            printf("Traducoes para '%s':\n", portugueseWord);
            showEnglishTranslations(rootNode->info1.englishWord);
            isFound = 1;
        }

        // Verifica o no info2 (se existir e ainda nao encontrou)
        if (!isFound && rootNode->nInfos == 2 && strcmp(rootNode->info2.portugueseWord, portugueseWord) == 0)
        {
            printf("Traducoes para '%s':\n", portugueseWord);
            showEnglishTranslations(rootNode->info2.englishWord);
            isFound = 1;
        }

        // Busca recursiva se ainda nao encontrou
        if (!isFound)
        {
            if (strcmp(portugueseWord, rootNode->info1.portugueseWord) < 0)
            {
                printAllTranslations(rootNode->left, portugueseWord);
            }
            else if (rootNode->nInfos == 1 || strcmp(portugueseWord, rootNode->info2.portugueseWord) < 0)
            {
                printAllTranslations(rootNode->cent, portugueseWord);
            }
            else
            {
                printAllTranslations(rootNode->right, portugueseWord);
            }
        }
    }
}

void printPortugueseTranslation(PortugueseTree **rootNode, const char *portugueseWord)
{
    PortugueseTree *foundPortugueseWord = FindWord(rootNode, portugueseWord);
    if (foundPortugueseWord)
    {
        printf("Traduções em inglês para a palavra '%s':\n", portugueseWord);

        if (strcmp(portugueseWord, foundPortugueseWord->info1.portugueseWord) == 0)
        {
            printBinaryTree(foundPortugueseWord->info1.englishWord);
        }
        else if (foundPortugueseWord->nInfos == 2 && strcmp(portugueseWord, foundPortugueseWord->info2.portugueseWord) == 0)
        {
            printBinaryTree(foundPortugueseWord->info2.englishWord);
        }
    }
    else
    {
        printf("A palavra '%s' não foi encontrada na árvore.\n", portugueseWord);
    }
}

NodeInfo no23_maior_info(PortugueseTree *rootNode)
{
    return rootNode->nInfos == 2 ? rootNode->info2 : rootNode->info1;
}

int eh_info1(PortugueseTree node, char *info)
{
    return strcmp(info, node.info1.portugueseWord) == 0;
}
int eh_info2(PortugueseTree node, char *nodeInfo)
{
    return node.nInfos == 2 && strcmp(nodeInfo, node.info2.portugueseWord) == 0;
}

PortugueseTree *get_maximum_child_from_tree(PortugueseTree *rootNode, PortugueseTree **parentNode, NodeInfo *max_value)
{
    PortugueseTree *filho;
    filho = rootNode;

    while (!isNodeLeaf(filho))
    {
        *parentNode = filho;
        if (filho->nInfos == 1)
            filho = filho->cent;
        else
            filho = filho->right;
    }

    if (filho != NULL)
        *max_value = no23_maior_info(filho);

    return filho;
}

PortugueseTree *findMinimumChild(PortugueseTree *rootNode, PortugueseTree **parentNode)
{
    PortugueseTree *filho;
    filho = rootNode;

    while (!isNodeLeaf(filho))
    {
        *parentNode = filho;
        filho = filho->left;
    }

    return filho;
}

int calculate_height(PortugueseTree *node)
{
    int altura = -1;
    if (node != NULL)
        altura = 1 + calculate_height(node->left);
    return altura;
}

int wave_movement(NodeInfo nodeLeaving, NodeInfo *inputNode, PortugueseTree *parentNode, PortugueseTree **originPointer, PortugueseTree **rootNode, PortugueseTree **largestNode, int (*remove_node)(PortugueseTree **, char *, PortugueseTree *, PortugueseTree **, PortugueseTree **))
{
    int isRemoved = remove_node(rootNode, nodeLeaving.portugueseWord, parentNode, originPointer, largestNode);
    *inputNode = nodeLeaving;
    return isRemoved;
}

static PortugueseTree *findParentIn23Tree(PortugueseTree *rootNode, char *nodeValue)
{
    PortugueseTree *parentNode;
    parentNode = NULL;

    if (rootNode != NULL)
    {
        if (!eh_info1(*rootNode, nodeValue) && !eh_info2(*rootNode, nodeValue))
        {
            if (strcmp(nodeValue, rootNode->info1.portugueseWord) < 0)
                parentNode = findParentIn23Tree(rootNode->left, nodeValue);
            else if (rootNode->nInfos == 1 || (strcmp(nodeValue, rootNode->info2.portugueseWord) < 0))
                parentNode = findParentIn23Tree(rootNode->cent, nodeValue);
            else
                parentNode = findParentIn23Tree(rootNode->right, nodeValue);

            if (parentNode == NULL)
                parentNode = rootNode;
        }
    }

    return parentNode;
}

PortugueseTree *findLargestParentIn23Tree(PortugueseTree *rootNode, char *nodeInfo)
{
    PortugueseTree *parentNode = NULL;

    if (rootNode != NULL)
    {
        if (!eh_info1(*rootNode, nodeInfo) && !eh_info2(*rootNode, nodeInfo))
        {
            if (strcmp(nodeInfo, rootNode->info1.portugueseWord) < 0)
                parentNode = findLargestParentIn23Tree(rootNode->left, nodeInfo);
            else if (rootNode->nInfos == 1 || (strcmp(nodeInfo, rootNode->info2.portugueseWord) < 0))
                parentNode = findLargestParentIn23Tree(rootNode->cent, nodeInfo);
            else
                parentNode = findLargestParentIn23Tree(rootNode->right, nodeInfo);

            if (parentNode == NULL &&
                ((rootNode->nInfos == 1 && (strcmp(rootNode->info1.portugueseWord, nodeInfo) > 0)) ||
                 (rootNode->nInfos == 2 && (strcmp(rootNode->info2.portugueseWord, nodeInfo) > 0))))
            {
                parentNode = rootNode;
            }
        }
    }

    return parentNode;
}

PortugueseTree *findSmallestParentWithTwoInfo(PortugueseTree *rootNode, char *nodeInfo)
{
    PortugueseTree *parentNode;
    parentNode = NULL;

    if (rootNode != NULL)
    {
        if (!eh_info1(*rootNode, nodeInfo) && !eh_info2(*rootNode, nodeInfo))
        {
            if (strcmp(nodeInfo, rootNode->info1.portugueseWord) < 0)
                parentNode = findSmallestParentWithTwoInfo(rootNode->left, nodeInfo);
            else if (rootNode->nInfos == 1 || (strcmp(nodeInfo, rootNode->info2.portugueseWord) < 0))
                parentNode = findSmallestParentWithTwoInfo(rootNode->cent, nodeInfo);
            else
                parentNode = findSmallestParentWithTwoInfo(rootNode->right, nodeInfo);

            if (parentNode == NULL && rootNode->nInfos == 2 && strcmp(rootNode->info2.portugueseWord, nodeInfo) < 0)
                parentNode = rootNode;
        }
    }

    return parentNode;
}

PortugueseTree *findSmallestParent23(PortugueseTree *rootNode, char *nodeInfo)
{
    PortugueseTree *parentNode;
    parentNode = NULL;

    if (rootNode != NULL)
    {
        if (!eh_info1(*rootNode, nodeInfo) && !eh_info2(*rootNode, nodeInfo))
        {
            if (strcmp(nodeInfo, rootNode->info1.portugueseWord) < 0)
                parentNode = findSmallestParent23(rootNode->left, nodeInfo);
            else if (rootNode->nInfos == 1 || strcmp(nodeInfo, rootNode->info2.portugueseWord) < 0)
                parentNode = findSmallestParent23(rootNode->cent, nodeInfo);
            else
                parentNode = findSmallestParent23(rootNode->right, nodeInfo);

            if (parentNode == NULL && strcmp(rootNode->info1.portugueseWord, nodeInfo) < 0)
                parentNode = rootNode;
        }
    }

    return parentNode;
}

int removeNonLeafNodeFrom23Tree(PortugueseTree **treeRoot, PortugueseTree *rootNode, NodeInfo *nodeInfo, PortugueseTree *leftChild, PortugueseTree *rightChild, PortugueseTree **maxNode)
{
    int isRemoved;
    PortugueseTree *childNode, *parentNode;
    NodeInfo child_node_info;

    parentNode = rootNode;

    childNode = get_maximum_child_from_tree(leftChild, &parentNode, &child_node_info);

    if (childNode->nInfos == 2)
    {
        *nodeInfo = child_node_info;
        childNode->nInfos = 1;
        isRemoved = 1;
    }
    else
    {
        childNode = findMinimumChild(rightChild, &parentNode);
        isRemoved = wave_movement(childNode->info1, nodeInfo, parentNode, treeRoot, &childNode, maxNode, deleteFrom23Tree);
    }

    return isRemoved;
}

int nonLeafNodeDeletion(PortugueseTree **treeRoot, PortugueseTree *rootNode, NodeInfo *nodeInfo, PortugueseTree *leftChild, PortugueseTree *rightChild, PortugueseTree **maxNode)
{
    int isRemoved = 0;
    PortugueseTree *childNode, *parentNode;
    NodeInfo info_filho;

    parentNode = rootNode;

    childNode = findMinimumChild(leftChild, &parentNode);

    if (childNode->nInfos == 2)
    {
        *nodeInfo = childNode->info1;
        childNode->info1 = childNode->info2;
        childNode->nInfos = 1;
        isRemoved = 1;
    }
    else
    {
        childNode = get_maximum_child_from_tree(rightChild, &parentNode, &info_filho);
        isRemoved = wave_movement(info_filho, nodeInfo, parentNode, treeRoot, &childNode, maxNode, removeNodeFrom23Tree);
    }

    return isRemoved;
}

int deleteFrom23Tree(PortugueseTree **rootNode, char *nodeInfo, PortugueseTree *parentNode, PortugueseTree **sourceNode, PortugueseTree **maxNode)
{
    int isRemoved = 0;

    if (*rootNode != NULL)
    {
        int info1 = eh_info1(**rootNode, nodeInfo);
        int info2 = eh_info2(**rootNode, nodeInfo);

        if (info1 || info2)
        {
            isRemoved = 1;
            if (isNodeLeaf(*rootNode))
            {
                if ((*rootNode)->nInfos == 2)
                {
                    if (info1)
                    {
                        (*rootNode)->info1 = (*rootNode)->info2;
                        (*rootNode)->nInfos = 1; // Agora está claro que esta linha pertence ao bloco do if
                    }
                }
                else
                {
                    if (parentNode == NULL)
                        deallocateTree(rootNode);
                    else
                    {
                        PortugueseTree *parent_node_temp;
                        NodeInfo parent_node_info;
                        if (*rootNode == parentNode->left || (parentNode->nInfos == 2 && *rootNode == parentNode->cent))
                        {
                            parent_node_temp = findParentIn23Tree(*sourceNode, parentNode->info1.portugueseWord);

                            if (*rootNode == parentNode->left)
                                parent_node_info = parentNode->info1;
                            else
                                parent_node_info = parentNode->info2;

                            isRemoved = wave_movement(parent_node_info, &((*rootNode)->info1), parent_node_temp, sourceNode, &parentNode, maxNode, deleteFrom23Tree);
                        }
                        else
                        {
                            parent_node_temp = findLargestParentIn23Tree(*sourceNode, (*rootNode)->info1.portugueseWord);

                            PortugueseTree *smallest_parent;
                            smallest_parent = findSmallestParentWithTwoInfo(*sourceNode, (*rootNode)->info1.portugueseWord);

                            if (parent_node_temp != NULL)
                            {
                                if (strcmp(parent_node_temp->info1.portugueseWord, (*rootNode)->info1.portugueseWord) > 0)
                                    parent_node_info = parent_node_temp->info1;
                                else
                                    parent_node_info = parent_node_temp->info2;
                            }

                            int height_of_smaller_parent = calculate_height(smallest_parent);
                            int parent_node_height = calculate_height(parent_node_temp);

                            if (parent_node_temp == NULL || (parent_node_temp != parentNode && smallest_parent != NULL &&
                                                    height_of_smaller_parent <= parent_node_height && strcmp(parent_node_info.portugueseWord, smallest_parent->info2.portugueseWord) > 0))
                            {
                                *maxNode = parentNode;
                                (*rootNode)->nInfos = 0;
                                isRemoved = -1;
                            }
                            else
                            {
                                PortugueseTree *grandparent;
                                grandparent = findParentIn23Tree(*sourceNode, parent_node_info.portugueseWord);
                                isRemoved = wave_movement(parent_node_info, &((*rootNode)->info1), grandparent, sourceNode, &parent_node_temp, maxNode, deleteFrom23Tree);
                            }
                        }
                    }
                }
            }
            else if (info2)
                isRemoved = removeNonLeafNodeFrom23Tree(sourceNode, *rootNode, &((*rootNode)->info2), (*rootNode)->cent, (*rootNode)->right, maxNode);
            else if (info1)
                isRemoved = removeNonLeafNodeFrom23Tree(sourceNode, *rootNode, &((*rootNode)->info1), (*rootNode)->left, (*rootNode)->cent, maxNode);
        }
        else
        {
            if (strcmp(nodeInfo, (*rootNode)->info1.portugueseWord) < 0)
                isRemoved = deleteFrom23Tree(&(*rootNode)->left, nodeInfo, *rootNode, sourceNode, maxNode);
            else if ((*rootNode)->nInfos == 1 || (strcmp(nodeInfo, (*rootNode)->info2.portugueseWord) < 0))
                isRemoved = deleteFrom23Tree(&(*rootNode)->cent, nodeInfo, *rootNode, sourceNode, maxNode);
            else
                isRemoved = deleteFrom23Tree(&(*rootNode)->right, nodeInfo, *rootNode, sourceNode, maxNode);
        }
    }
    return isRemoved;
}

int removeNodeFrom23Tree(PortugueseTree **treeRoot, char *nodeData, PortugueseTree *parentNode, PortugueseTree **sourceNode, PortugueseTree **largestNode)
{
    int isRemoved = 0;

    if (*treeRoot != NULL)
    {
        int info1 = eh_info1(**treeRoot, nodeData);
        int info2 = eh_info2(**treeRoot, nodeData);

        if (info1 || info2)
        {
            isRemoved = 1;
            if (isNodeLeaf(*treeRoot))
            {
                if ((*treeRoot)->nInfos == 2)
                {
                    if (info1)
                        (*treeRoot)->info1 = (*treeRoot)->info2;

                    (*treeRoot)->nInfos = 1;
                }
                else
                {
                    if (parentNode == NULL)
                        deallocateTree(treeRoot);
                    else
                    {
                        PortugueseTree *parent_node;
                        NodeInfo parent_node_info;
                        if (*treeRoot == parentNode->cent || (parentNode->nInfos == 2 && *treeRoot == parentNode->right))
                        {
                            parent_node = findParentIn23Tree(*sourceNode, parentNode->info1.portugueseWord);

                            if (*treeRoot == parentNode->cent)
                                parent_node_info = parentNode->info1;
                            else
                                parent_node_info = parentNode->info2;

                            isRemoved = wave_movement(parent_node_info, &((*treeRoot)->info1), parent_node, sourceNode, &parentNode, largestNode, removeNodeFrom23Tree);
                        }
                        else
                        {
                            parent_node = findSmallestParent23(*sourceNode, (*treeRoot)->info1.portugueseWord);

                            PortugueseTree *minimum_parent;
                            minimum_parent = findSmallestParentWithTwoInfo(*sourceNode, (*treeRoot)->info1.portugueseWord);

                            PortugueseTree *ancestorNode;
                            if (parent_node == NULL || (parent_node != parentNode && minimum_parent != NULL))
                            {
                                isRemoved = -1;
                                *largestNode = parentNode;
                            }
                            else
                            {
                                if (parent_node->nInfos == 2 && parent_node->info2.portugueseWord < (*treeRoot)->info1.portugueseWord)
                                    parent_node_info = parent_node->info2;
                                else
                                    parent_node_info = parent_node->info1;

                                ancestorNode = findParentIn23Tree(*sourceNode, parent_node_info.portugueseWord);
                                isRemoved = wave_movement(parent_node_info, &((*treeRoot)->info1), ancestorNode, sourceNode, &parent_node, largestNode, removeNodeFrom23Tree);
                            }
                        }
                    }
                }
            }
            else if (info2)
                isRemoved = nonLeafNodeDeletion(sourceNode, *treeRoot, &((*treeRoot)->info2), (*treeRoot)->right, (*treeRoot)->cent, largestNode);
            else if (info1)
                isRemoved = nonLeafNodeDeletion(sourceNode, *treeRoot, &((*treeRoot)->info1), (*treeRoot)->cent, (*treeRoot)->left, largestNode);
        }
        else
        {
            if (strcmp(nodeData, (*treeRoot)->info1.portugueseWord) < 0)
                isRemoved = removeNodeFrom23Tree(&(*treeRoot)->left, nodeData, *treeRoot, sourceNode, largestNode);
            else if ((*treeRoot)->nInfos == 1 || (strcmp(nodeData, (*treeRoot)->info2.portugueseWord) < 0))
                isRemoved = removeNodeFrom23Tree(&(*treeRoot)->cent, nodeData, *treeRoot, sourceNode, largestNode);
            else
                isRemoved = removeNodeFrom23Tree(&(*treeRoot)->right, nodeData, *treeRoot, sourceNode, largestNode);
        }
    }
    return isRemoved;
}

int can_remove_node(PortugueseTree *rootNode)
{
    int isPossible = 0;

    if (rootNode != NULL)
    {
        isPossible = rootNode->nInfos == 2;

        if (!isPossible)
        {
            isPossible = can_remove_node(rootNode->cent);

            if (!isPossible)
                isPossible = can_remove_node(rootNode->left);
        }
    }

    return isPossible;
}

PortugueseTree *mergeNode23(PortugueseTree *leftChild, NodeInfo *nodeData, PortugueseTree *largestChild, PortugueseTree **treeRoot)
{
    updateNode23(leftChild, nodeData, largestChild);

    (*treeRoot)->nInfos--;

    if ((*treeRoot)->nInfos == 0)
        deallocateTree(treeRoot);

    return leftChild;
}

void balanceTreeNodes(PortugueseTree **rootNode, PortugueseTree *leftChild, PortugueseTree **secondChild, NodeInfo nodeInfo, PortugueseTree **largestNode)
{
    if (*secondChild == NULL || (*secondChild)->nInfos == 0)
    {
        if (*secondChild != NULL)
            deallocateTree(secondChild);
        *largestNode = mergeNode23(leftChild, &nodeInfo, *largestNode, rootNode);
    }
}

int rebalanceTree23(PortugueseTree **treeRoot, char *nodeData, PortugueseTree **largestNode)
{
    int isBalanced = 0;
    if (*treeRoot != NULL)
    {
        if (!isNodeLeaf(*treeRoot))
        {
            if (strcmp(nodeData, (*treeRoot)->info1.portugueseWord) < 0)
                isBalanced = rebalanceTree23(&((*treeRoot)->left), nodeData, largestNode);
            else if ((*treeRoot)->nInfos == 1 || (strcmp(nodeData, (*treeRoot)->info2.portugueseWord) < 0))
            {
                if ((*treeRoot)->left->nInfos == 2 && !can_remove_node((*treeRoot)->cent))
                    isBalanced = -1;
                else
                    isBalanced = rebalanceTree23(&((*treeRoot)->cent), nodeData, largestNode);
            }
            else
            {
                if ((*treeRoot)->cent->nInfos == 2 && !can_remove_node((*treeRoot)->right))
                    isBalanced = -1;
                else
                    isBalanced = rebalanceTree23(&((*treeRoot)->right), nodeData, largestNode);
            }

            if (isBalanced != -1)
            {
                if ((*treeRoot)->nInfos == 1)
                    balanceTreeNodes(treeRoot, (*treeRoot)->left, &((*treeRoot)->cent), (*treeRoot)->info1, largestNode);
                else if ((*treeRoot)->nInfos == 2)
                    balanceTreeNodes(treeRoot, (*treeRoot)->cent, &((*treeRoot)->right), (*treeRoot)->info2, largestNode);
                isBalanced = 1;
            }
        }
    }

    return isBalanced;
}

int remove_node_from23_tree(PortugueseTree **root, char *nodeValue)
{
    PortugueseTree *largestNode, *junction_position;
    int isDeleted = deleteFrom23Tree(root, nodeValue, NULL, root, &junction_position);

    if (isDeleted == -1)
    {
        NodeInfo junction_node_value = no23_maior_info(junction_position);
        largestNode = NULL;
        isDeleted = rebalanceTree23(root, junction_node_value.portugueseWord, &largestNode);

        if (isDeleted == -1)
        {
            PortugueseTree *parentNode, *junction_position_two;
            NodeInfo *inputNode;
            parentNode = findParentIn23Tree(*root, junction_node_value.portugueseWord);

            if (eh_info1(*junction_position, junction_node_value.portugueseWord))
                inputNode = &(junction_position->cent->info1);
            else
                inputNode = &(junction_position->right->info1);

            isDeleted = wave_movement(junction_node_value, inputNode, parentNode, root, &junction_position, &junction_position_two, removeNodeFrom23Tree);

            if (isDeleted == -1)
            {
                junction_node_value = junction_position_two->info1;
                parentNode = findParentIn23Tree(*root, junction_node_value.portugueseWord);
                isDeleted = wave_movement(junction_node_value, &(junction_position_two->left->info1), parentNode, root, &junction_position_two, &junction_position, deleteFrom23Tree);

                junction_node_value = no23_maior_info(junction_position);
                largestNode = NULL;
                isDeleted = rebalanceTree23(root, junction_node_value.portugueseWord, &largestNode);
            }
        }

        if (*root == NULL)
            *root = largestNode;
    }

    return isDeleted;
}

/*#########################################FREE#######################################################*/

void deleteInfo2_3(NodeInfo *nodeInfo)
{
    if (nodeInfo != NULL)
    {
        clear_binary_tree(nodeInfo->englishWord);
        free(nodeInfo->portugueseWord);
    }
}

void deallocateTree(PortugueseTree **node)
{
    if (*node != NULL)
    {
        deallocateTree(&((*node)->left));
        deallocateTree(&((*node)->cent));
        if ((*node)->nInfos == 2)
        {
            deleteInfo2_3(&((*node)->info2));
            deallocateTree(&((*node)->right));
        }
        deleteInfo2_3(&((*node)->info1));
        free(*node);
        *node = NULL;
    }
}

void removeEnglishTranslation(PortugueseTree **rootNode, char *englishWord, int unit, PortugueseTree **parentNode)
{
    if (*rootNode != NULL)
    {
        // Percorre a subárvore esquerda
        removeEnglishTranslation(&(*rootNode)->left, englishWord, unit, parentNode);

        // Verifica no primeiro elemento do nó
        if ((*rootNode)->info1.englishWord != NULL)
        {
            removeEnglishWord(&(*rootNode)->info1.englishWord, englishWord, unit);
        }

        // Percorre a subárvore central
        removeEnglishTranslation(&(*rootNode)->cent, englishWord, unit, parentNode);

        // Verifica no segundo elemento do nó, se existir
        if ((*rootNode)->nInfos == 2 && (*rootNode)->info2.englishWord != NULL)
        {
            removeEnglishWord(&(*rootNode)->info2.englishWord, englishWord, unit);
        }

        // Percorre a subárvore direita, se existir
        if ((*rootNode)->nInfos == 2)
        {
            removeEnglishTranslation(&(*rootNode)->right, englishWord, unit, parentNode);
        }
    }
}

void printFormattedWordsByUnit(PortugueseTree *portugueseTree, int unit, int *printedUnit)
{
    if (portugueseTree)
    {
        // Percorre a subárvore esquerda
        printFormattedWordsByUnit(portugueseTree->left, unit, printedUnit);

        // Verifica as palavras associadas ao `info1` do nó
        if (portugueseTree->info1.englishWord != NULL)
        {
            Inglesbin *englishWordNode = portugueseTree->info1.englishWord;
            while (englishWordNode)
            {
                if (find_unit(englishWordNode->unidades, unit))
                {
                    if (!(*printedUnit))
                    {
                        printf("%% Unidade %d\n", unit);
                        *printedUnit = 1;
                    }
                    printf("%s: %s;\n", portugueseTree->info1.portugueseWord, englishWordNode->palavraIngles);
                }
                englishWordNode = englishWordNode->esq ? englishWordNode->esq : englishWordNode->dir;
            }
        }

        // Percorre a subárvore central
        printFormattedWordsByUnit(portugueseTree->cent, unit, printedUnit);

        // Verifica as palavras associadas ao `info2` do nó, se existir
        if (portugueseTree->nInfos == 2 && portugueseTree->info2.englishWord != NULL)
        {
            Inglesbin *englishWordPtr = portugueseTree->info2.englishWord;
            while (englishWordPtr)
            {
                if (find_unit(englishWordPtr->unidades, unit))
                {
                    if (!(*printedUnit))
                    {
                        printf("%% Unidade %d\n", unit);
                        *printedUnit = 1;
                    }
                    printf("%s: %s;\n", portugueseTree->info2.portugueseWord, englishWordPtr->palavraIngles);
                }
                englishWordPtr = englishWordPtr->esq ? englishWordPtr->esq : englishWordPtr->dir;
            }
        }

        // Percorre a subárvore direita, se existir
        if (portugueseTree->nInfos == 2)
        {
            printFormattedWordsByUnit(portugueseTree->right, unit, printedUnit);
        }
    }
}

void removePortugueseWordByUnit(PortugueseTree **rootNode, char *portugueseWord, int nodeLevel)
{
    PortugueseTree *foundPortugueseNode = FindWord(rootNode, portugueseWord);
    int isFound = 0;

    if (foundPortugueseNode != NULL)
    {
        // Verificar info1
        if (strcmp(foundPortugueseNode->info1.portugueseWord, portugueseWord) == 0)
        {
            Inglesbin *englishWordPtr = foundPortugueseNode->info1.englishWord;
            while (englishWordPtr)
            {
                if (find_unit(englishWordPtr->unidades, nodeLevel))
                {
                    isFound = 1;
                    removeEnglishWord(&(foundPortugueseNode->info1.englishWord), englishWordPtr->palavraIngles, nodeLevel);

                    if (foundPortugueseNode->info1.englishWord == NULL)
                    {
                        deleteInfo2_3(&(foundPortugueseNode->info1));
                        foundPortugueseNode->info1.portugueseWord = NULL; // Marca como vazio
                    }
                }
                englishWordPtr = englishWordPtr->esq ? englishWordPtr->esq : englishWordPtr->dir;
            }
        }

        // Verificar info2
        if (!isFound && foundPortugueseNode->nInfos == 2 && strcmp(foundPortugueseNode->info2.portugueseWord, portugueseWord) == 0)
        {
            Inglesbin *ingles = foundPortugueseNode->info2.englishWord;
            while (ingles)
            {
                if (find_unit(ingles->unidades, nodeLevel))
                {
                    isFound = 1;
                    removeEnglishWord(&(foundPortugueseNode->info2.englishWord), ingles->palavraIngles, nodeLevel);

                    if (foundPortugueseNode->info2.englishWord == NULL)
                    {
                        deleteInfo2_3(&(foundPortugueseNode->info2));
                        foundPortugueseNode->info2.portugueseWord = NULL; // Marca como vazio
                    }
                }
                ingles = ingles->esq ? ingles->esq : ingles->dir;
            }
        }
    }

    // A lógica de retorno único
}

int Delete_portuguese_word_unit(Inglesbin **rootNode, char *portugueseWord, int unit)
{
    int isConfirmed = 0;

    if (*rootNode)
    {
        // Remoção recursiva nos filhos
        isConfirmed = Delete_portuguese_word_unit(&(*rootNode)->esq, portugueseWord, unit);
        isConfirmed = Delete_portuguese_word_unit(&(*rootNode)->dir, portugueseWord, unit) || isConfirmed;

        // Atualiza a lista de unidades associadas
        Unit *novaLista = remove_unit((*rootNode)->unidades, unit);
        if (novaLista != (*rootNode)->unidades)
        {
            isConfirmed = 1; // Indica que a unidade foi removida
        }
        (*rootNode)->unidades = novaLista;

        // Se a lista de unidades ficar vazia, remove o nó da árvore binária
        if (!(*rootNode)->unidades)
        {
            isConfirmed = removeEnglishWord(rootNode, (*rootNode)->palavraIngles, unit);
        }
    }

    return isConfirmed;
}

int delete_portuguese_word_in_unit(PortugueseTree *rootNode, char *portugueseWord, int unit, PortugueseTree **topNodePtr)
{
    int isConfirmed = 0;

    if (rootNode)
    {
        if (strcmp(rootNode->info1.portugueseWord, portugueseWord) == 0)
        {
            isConfirmed = Delete_portuguese_word_unit(&rootNode->info1.englishWord, portugueseWord, unit);

            if (!rootNode->info1.englishWord)
            {
                isConfirmed = remove_node_from23_tree(topNodePtr, portugueseWord);
            }
        }
        else if (rootNode->nInfos == 2 && strcmp(rootNode->info2.portugueseWord, portugueseWord) == 0)
        {
            isConfirmed = Delete_portuguese_word_unit(&rootNode->info2.englishWord, portugueseWord, unit);

            if (!rootNode->info2.englishWord)
            {
                isConfirmed = remove_node_from23_tree(topNodePtr, portugueseWord);
            }
        }
        else if (strcmp(portugueseWord, rootNode->info1.portugueseWord) < 0)
        {
            isConfirmed = delete_portuguese_word_in_unit(rootNode->left, portugueseWord, unit, topNodePtr);
        }
        else if (rootNode->nInfos == 1 || strcmp(portugueseWord, rootNode->info2.portugueseWord) < 0)
        {
            isConfirmed = delete_portuguese_word_in_unit(rootNode->cent, portugueseWord, unit, topNodePtr);
        }
        else
        {
            isConfirmed = delete_portuguese_word_in_unit(rootNode->right, portugueseWord, unit, topNodePtr);
        }
    }

    return isConfirmed;
}

int Remove_word_from_portuguese_unit(PortugueseTree **rootNode, char *portugueseWord, int unit)
{
    int isConfirmed = 0;
    isConfirmed = delete_portuguese_word_in_unit(*rootNode, portugueseWord, unit, rootNode);
    return isConfirmed;
}