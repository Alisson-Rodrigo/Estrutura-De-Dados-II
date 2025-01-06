#ifndef ARVRB_H
#define ARVRB_H

typedef struct NodeData
{
  BinaryTreeNode *englishWordNode;
  char *portugueseWord;
} NodeData;

typedef struct RedBlackTreePT
{
  NodeData info;
  int color;
  struct RedBlackTreePT *left;
  struct RedBlackTreePT *right;
} RedBlackTreePT;

int insertPortugueseWord(RedBlackTreePT **arvore, char *palavraPortugues, char *palavraIngles, int unidade);

NodeData createInfo(char *palavra, char *palavraIngles, int unidade);

RedBlackTreePT *createNode(NodeData *informacao);
int getNodeColor(RedBlackTreePT *raiz);

void switch_node_color(RedBlackTreePT **raiz);

void rotate_right(RedBlackTreePT **raiz);

void left_rotate(RedBlackTreePT **raiz);

void balanceTree(RedBlackTreePT **raiz);

int insertRedBlackNode(RedBlackTreePT **raiz, NodeData *informacao);

int insertRedBlackTree(RedBlackTreePT **raiz, NodeData *informacao);

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