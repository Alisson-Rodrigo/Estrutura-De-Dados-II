#include "ingles.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unidade.h"

Inglesbin *initializeNode(const char *englishWord, int unit)
{
    Inglesbin *novoNo = (Inglesbin *)malloc(sizeof(Inglesbin));
    if (novoNo != NULL)
    {
        novoNo->englishWord = (char *)malloc(strlen(englishWord) + 1);
        if (novoNo->englishWord != NULL)
        {
            strcpy(novoNo->englishWord, englishWord);
        }

        novoNo->unitList = create_unit(unit);

        novoNo->leftChild = novoNo->rightChild = NULL;
    }
    return novoNo;
}

Inglesbin *insertEnglishWord(Inglesbin *root, const char *englishWord, int unit)
{
    Inglesbin *result;

    if (root == NULL)
    {

        result = initializeNode(englishWord, unit);
    }
    else
    {

        if (strcmp(englishWord, root->englishWord) == 0)
        {
            root->unitList = insert_unit_sorted(root->unitList, create_unit(unit));
            result = root;
        }
        else
        {

            if (strcmp(englishWord, root->englishWord) < 0)
            {
                root->leftChild = insertEnglishWord(root->leftChild, englishWord, unit);
            }
            else
            {
                root->rightChild = insertEnglishWord(root->rightChild, englishWord, unit);
            }
            result = root;
        }
    }

    return result;
}

void addEnglishTranslation(NodeInfo *info, const char *englishWord, int unit)
{

    if (info->englishWord == NULL)
    {

        info->englishWord = initializeNode(englishWord, unit);
    }
    else
    {

        info->englishWord = insertEnglishWord(info->englishWord, englishWord, unit);
    }

    info->englishWord->unitList = insert_unit_sorted(info->englishWord->unitList, create_unit(unit));
}

void printBinaryTree(Inglesbin *root)
{
    if (root != NULL)
    {

        printBinaryTree(root->leftChild);

        printf("\nPalavra em Inglês: %s\n", root->englishWord);

        if (root->unitList != NULL)
        {
            printf("Unidades associadas: ");
            print_units(root->unitList);
        }
        else
        {
            printf("Nenhuma unidade associada.\n");
        }

        printBinaryTree(root->rightChild);
    }
}

int isLeafNode(Inglesbin *raiz)
{
    return (raiz->leftChild == NULL && raiz->rightChild == NULL);
}

Inglesbin *getSingleChild(Inglesbin *raiz)
{
    Inglesbin *aux;
    aux = NULL;

    if (raiz->rightChild == NULL)
    {
        aux = raiz->leftChild;
    }
    else if (raiz->leftChild == NULL)
    {
        aux = raiz->rightChild;
    }

    return aux;
}

Inglesbin *getMinimumChild(Inglesbin *raiz)
{
    Inglesbin *aux;
    aux = raiz;

    if (raiz)
    {
        if (raiz->leftChild)
            aux = getMinimumChild(raiz->leftChild);
    }

    return aux;
}

int removeEnglishWord(Inglesbin **raiz, const char *englishWord, int unit)
{
    Inglesbin *childPointer = NULL;
    int isExists = 0;

    if (*raiz)
    {

        if (strcmp(englishWord, (*raiz)->englishWord) == 0)
        {
            Inglesbin *aux = *raiz;
            isExists = 1;

            if (isLeafNode(*raiz))
            {
                free(aux);
                *raiz = NULL;
            }
            else if ((childPointer = getSingleChild(*raiz)) != NULL)
            {
                free(aux);
                *raiz = childPointer;
            }
            else
            {
                childPointer = getMinimumChild((*raiz)->rightChild);
                strcpy((*raiz)->englishWord, childPointer->englishWord);
                (*raiz)->unitList = childPointer->unitList;

                removeEnglishWord(&(*raiz)->rightChild, childPointer->englishWord, unit);
            }
        }
        else if (strcmp(englishWord, (*raiz)->englishWord) < 0)
        {

            isExists = removeEnglishWord(&(*raiz)->leftChild, englishWord, unit);
        }
        else
        {

            isExists = removeEnglishWord(&(*raiz)->rightChild, englishWord, unit);
        }
    }

    return isExists;
}

void clear_binary_tree(Inglesbin *rootNode)
{
    if (rootNode != NULL)
    {

        printf("Liberando nó da árvore binária com palavra: %s\n", rootNode->englishWord);

        clear_binary_tree(rootNode->leftChild);
        clear_binary_tree(rootNode->rightChild);

        free_list(rootNode->unitList);

        free(rootNode->englishWord);

        free(rootNode);
    }
}

void printTranslations(Inglesbin *node, int unit, const char *portugueseWord)
{
    if (node)
    {

        printTranslations(node->leftChild, unit, portugueseWord);

        if (find_unit(node->unitList, unit))
        {
            printf("Palavra em Português: %s\n", portugueseWord);
            printf("Tradução em Inglês: %s\n", node->englishWord);
        }

        printTranslations(node->rightChild, unit, portugueseWord);
    }
}

Inglesbin *findEnglishWord(Inglesbin *rootNode, const char *englishWord)
{
    Inglesbin *englishResult = NULL;

    if (rootNode != NULL)
    {
        int comparisonResult = strcmp(englishWord, rootNode->englishWord);

        if (comparisonResult == 0)
        {
            englishResult = rootNode;
        }
        else if (comparisonResult < 0)
        {
            englishResult = findEnglishWord(rootNode->leftChild, englishWord);
        }
        else
        {
            englishResult = findEnglishWord(rootNode->rightChild, englishWord);
        }
    }

    return englishResult;
}

void FindEnglishWord(PortugueseTree **englishTreeRoot, char *englishWord, int unitIndex, PortugueseTree **parentNode)
{
    if (*englishTreeRoot != NULL)
    {

        FindEnglishWord(&(*englishTreeRoot)->left, englishWord, unitIndex, parentNode);

        if ((*englishTreeRoot)->info1.englishWord != NULL)
        {
            Inglesbin *traducaoEncontrada = findEnglishWord((*englishTreeRoot)->info1.englishWord, englishWord);
            if (traducaoEncontrada != NULL && find_unit(traducaoEncontrada->unitList, unitIndex))
            {
                printf("Palavra em Inglês: %s encontrada na Unidade: %d\n", englishWord, unitIndex);
            }
        }

        FindEnglishWord(&(*englishTreeRoot)->cent, englishWord, unitIndex, englishTreeRoot);

        if ((*englishTreeRoot)->nInfos == 2 && (*englishTreeRoot)->info2.englishWord != NULL)
        {
            Inglesbin *foundTranslation = findEnglishWord((*englishTreeRoot)->info2.englishWord, englishWord);
            if (foundTranslation != NULL && find_unit(foundTranslation->unitList, unitIndex))
            {
                printf("Palavra em Inglês: %s encontrada na Unidade: %d\n", englishWord, unitIndex);
            }
        }

        if ((*englishTreeRoot)->nInfos == 2)
        {
            FindEnglishWord(&(*englishTreeRoot)->right, englishWord, unitIndex, englishTreeRoot);
        }
    }
}

void showEnglishTranslations(Inglesbin *englishRoot)
{
    if (englishRoot)
    {

        showEnglishTranslations(englishRoot->leftChild);

        printf("- %s\n", englishRoot->englishWord);

        showEnglishTranslations(englishRoot->rightChild);
    }
}


int Remove_english_word_from_unit(PortugueseTree **rootNode, const char *englishWord, int unit)
{
    int result = 1; 
    result = remove_english_word_by_unit(*rootNode, englishWord, unit, rootNode);
    return result;
}

int remove_english_unit(Inglesbin **rootNode, const char *englishWord, int unit)
{
    int result = 1;

    if (*rootNode)
    {
       
        if (strcmp((*rootNode)->englishWord, englishWord) == 0)
        {
            
            result = remove_unit(&(*rootNode)->unitList, unit);

            if (!(*rootNode)->unitList)
            {
                result = removeEnglishWord(rootNode, englishWord, unit);
            }
        }
        else if (strcmp((*rootNode)->englishWord, englishWord) > 0)
        {
            result = remove_english_unit(&(*rootNode)->leftChild, englishWord, unit);
        }
        else
        {
            result = remove_english_unit(&(*rootNode)->rightChild, englishWord, unit);
        }
    }

    return result;
}

int remove_english_word_by_unit(PortugueseTree *rootNode, const char *englishWord, int unit, PortugueseTree **newTopNode)
{
    int result = 0; 

    if (rootNode)
    {
     
        result = remove_english_word_by_unit(rootNode->left, englishWord, unit, newTopNode);

        result = remove_english_word_by_unit(rootNode->cent, englishWord, unit, newTopNode) || result;

        if (rootNode->nInfos == 2)
        {
            result = remove_english_word_by_unit(rootNode->right, englishWord, unit, newTopNode) || result;

            result = remove_english_unit(&(rootNode->info2.englishWord), englishWord, unit) || result;

            if (!rootNode->info2.englishWord)
            {
                result = remove_node_from23_tree(newTopNode, rootNode->info2.portugueseWord) || result;
            }
        }

        result = remove_english_unit(&(rootNode->info1.englishWord), englishWord, unit) || result;

        if (!rootNode->info1.englishWord)
        {
            result = remove_node_from23_tree(newTopNode, rootNode->info1.portugueseWord) || result;
        }
    }

    return result;
}
