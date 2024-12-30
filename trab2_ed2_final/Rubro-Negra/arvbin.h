#ifndef ARVBIN_H
#define ARVBIN_H

typedef struct EnglishBinaryTree
{
  char englishWord[50];
  int unitValue;
  struct EnglishBinaryTree *left;
  struct EnglishBinaryTree *rigth;
} BinaryTreeNode;

typedef struct  RedBlackTreePT  RedBlackTreePT;
BinaryTreeNode *initializeBinaryTreeNode(char *palavraIngles, int unidade);

// Função para inserir uma palavra em inglês na arvore binaria de busca
BinaryTreeNode *insertEnglishWord(BinaryTreeNode *root, char *palavraIngles, int unidade);

void addEnglishTranslation(RedBlackTreePT *raiz, char *palavraIng, int unidade);

int isLeafNodes(BinaryTreeNode *raiz);

BinaryTreeNode *singleChildNode(BinaryTreeNode *raiz);

BinaryTreeNode *minimumChildNode(BinaryTreeNode *raiz);

void printBinaryTree(BinaryTreeNode *root);

int removeEnglishWord(BinaryTreeNode **raiz, char *palavra);
void FindEnglishTerm(RedBlackTreePT **raiz, char *palavraIngles, int unidade);

#endif