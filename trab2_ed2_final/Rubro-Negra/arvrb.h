#ifndef ARVRB_H
#define ARVRB_H

typedef struct Info
{
  BinaryTreeNode *englishWordNode;
  char *portugueseWord;
} Info;

typedef struct RedBlackTreePT
{
  Info info;
  int color;
  struct RedBlackTreePT *left;
  struct RedBlackTreePT *right;
} RedBlackTreePT;

int insertPortugueseWord(RedBlackTreePT **arvore, char *palavraPortugues, char *palavraIngles, int unidade);

Info createInfo(char *palavra, char *palavraIngles, int unidade);

RedBlackTreePT *createNode(Info *informacao);
int getNodeColor(RedBlackTreePT *raiz);

void switch_node_color(RedBlackTreePT **raiz);

void rotate_right(RedBlackTreePT **raiz);

void left_rotate(RedBlackTreePT **raiz);

void balanceTree(RedBlackTreePT **raiz);

int insertRedBlackNode(RedBlackTreePT **raiz, Info *informacao);

int insertRedBlackTree(RedBlackTreePT **raiz, Info *informacao);

void shiftLeftRed(RedBlackTreePT **raiz);

void rotateRedRight(RedBlackTreePT **raiz);

void removeMinimum(RedBlackTreePT **raiz);

RedBlackTreePT *findMinimum(RedBlackTreePT *raiz);

int removeNodeFromRBTree(RedBlackTreePT **raiz, char *valor);

int removeRBTreeNode(RedBlackTreePT **raiz, char *valor);

RedBlackTreePT *SearchWordInTree(RedBlackTreePT **arvore, char *palavraPortugues);

void printWordsByUnit(RedBlackTreePT *arvore, int unidade);

void showPortugueseTranslation(RedBlackTreePT **raiz, char *palavraPortugues);

void printTranslations(BinaryTreeNode *node, int unidade, char *palavraPortugues);

void showRedBlackTree(RedBlackTreePT *raiz);


// void freeTree(PortuguesRB *no);

#endif