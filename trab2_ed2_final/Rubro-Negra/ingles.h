#ifndef INGLESBIN_H
#define INGLESBIN_H


typedef struct inglesbin {
    char palavraIngles[100]; 
    int unidade;             
    struct inglesbin *esq;   
    struct inglesbin *dir;   
} Arv_ingles;

Arv_ingles *createNode(char *palavraIngles, int unidade); 
Arv_ingles *insertpalavraIngles(Arv_ingles *root, char *palavraIngles, int unidade); 
void printBinaryTree(Arv_ingles *root); 



int ehFolhas(Arv_ingles *raiz); 
Arv_ingles *soUmFilho(Arv_ingles *raiz); 
Arv_ingles *menorFilho(Arv_ingles *raiz); 
int removerPalavraIngles(Arv_ingles **raiz, char *palavra); 



#endif 
