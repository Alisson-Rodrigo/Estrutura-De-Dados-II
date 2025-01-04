#include <stdio.h>
#include <stdlib.h>
#include "23.h"

int TreeNode23_remove_no_interno1(TreeNode23 **origem, TreeNode23* root, Info *info, TreeNode23 *filho1, TreeNode23 *filho2, TreeNode23 **maior)
{
    int removeu;
    TreeNode23 *filho, *pai;
    Info *info_filho;

    pai = root;

    filho = Search_nodo_maior_filho(filho1, &pai, &info_filho);

    if(filho->n_infos == 2)
    {
        *info = *info_filho;
        filho->n_infos = 1;
    }
    else
    {
        filho = Search_nodo_menor_filho(filho2, &pai);
        removeu = movimento_onda(filho->info1, info, pai, origem, &filho, maior, TreeNode23_remove1);
    }

    return removeu;
}

 int TreeNode23_remove_no_interno2(TreeNode23 **origem, TreeNode23* root, Info *info, TreeNode23 *filho1, TreeNode23 *filho2, TreeNode23 **maior)
{
    int removeu;
    TreeNode23 *filho, *pai;
    Info *info_filho;

    pai = root;

    filho = Search_nodo_menor_filho(filho1, &pai);

    if(filho->n_infos == 2)
    {
        *info = filho->info1;
        filho->info1 = filho->info2;
        filho->n_infos = 1;
    }
    else
    {
        filho = Search_nodo_maior_filho(filho2, &pai, &info_filho);
        removeu = movimento_onda(*info_filho, info, pai, origem, &filho, maior, TreeNode23_remove2);
    }

    return removeu;
}

int TreeNode23_remove1(TreeNode23 **root, int info, TreeNode23 *pai, TreeNode23 **origem, TreeNode23 **maior)
{
    int removeu = 0;

    if(*root != NULL)
    {
        int info1 = isInfo1(**root, info);
        int info2 = isInfo2(**root, info);

        if(info1 || info2)
        {
            removeu = 1;
            if(isLeaf(**root))
            {
                if((*root)->n_infos == 2)
                {
                    if(info1)
                        (*root)->info1 = (*root)->info2;

                    (*root)->n_infos = 1;
                }
                else
                {
                    if(pai == NULL)
                        free_nodo(root);
                    else
                    {
                        TreeNode23 *pai_aux;
                        Info info_pai;
                        if(*root == pai->left || (pai->n_infos == 2 && *root == pai->center))
                        {
                            pai_aux = Search_nodo_pai(*origem, pai->info1.num_start);
                            
                            if(*root == pai->left)
                                info_pai = pai->info1;
                            else 
                                info_pai = pai->info2;

                            removeu = movimento_onda(info_pai, &((*root)->info1), pai_aux, origem, &pai, maior, TreeNode23_remove1);
                        }
                        else // Filho do center (com pai de 1 info) ou da direita
                        {
                            pai_aux = Search_nodo_maior_pai(*origem, (*root)->info1.num_start);

                            TreeNode23 *menor_pai;
                            menor_pai = Search_nodo_menor_pai_2_infos(*origem, (*root)->info1.num_start);


                            if(pai_aux != NULL)
                            {
                                if(pai_aux->info1.num_start > (*root)->info1.num_start)
                                    info_pai = pai_aux->info1;
                                else
                                    info_pai = pai_aux->info2;
                            }

                            int height_menor_pai = height_size(menor_pai);
                            int height_pai_aux = height_size(pai_aux);

                            if(pai_aux == NULL || (pai_aux != pai && menor_pai != NULL && height_menor_pai <= height_pai_aux && info_pai.num_start > menor_pai->info2.num_start))
                            {
                                *maior = pai;
                                (*root)->n_infos = 0;
                                removeu = -1;
                            }
                            else
                            {

                                TreeNode23 *avo;
                                avo = Search_nodo_pai(*origem, info_pai.num_start);
                                removeu = movimento_onda(info_pai, &((*root)->info1), avo, origem, &pai_aux, maior, TreeNode23_remove1);
                            }
                        }
                    }
                }
            }
            else if(info2)
                removeu = TreeNode23_remove_no_interno1(origem, *root, &((*root)->info2), (*root)->center, (*root)->right, maior);
            else if(info1)
                removeu = TreeNode23_remove_no_interno1(origem, *root, &((*root)->info1), (*root)->left, (*root)->center, maior);
        }
        else
        {
            if(info < (*root)->info1.num_start)
                removeu = TreeNode23_remove1(&(*root)->left, info, *root, origem, maior);
            else if((*root)->n_infos == 1 || info < (*root)->info2.num_start)
                removeu = TreeNode23_remove1(&(*root)->center, info, *root, origem, maior);
            else
                removeu = TreeNode23_remove1(&(*root)->right, info, *root, origem, maior);
        }
    }
    return removeu;
}

int TreeNode23_remove2(TreeNode23 **root, int info, TreeNode23 *pai, TreeNode23 **origem, TreeNode23 **maior)
{
    int removeu = 0;

    if(*root != NULL)
    {
        int info1 = isInfo1(**root, info);
        int info2 = isInfo2(**root, info);

        if(info1 || info2)
        {
            removeu = 1;
            if(isLeaf(**root))
            {
                if((*root)->n_infos == 2)
                {
                    if(info1)
                        (*root)->info1 = (*root)->info2;

                    (*root)->n_infos = 1;
                }
                else
                {
                    if(pai == NULL)
                        free_nodo(root);
                    else
                    {
                        TreeNode23 *pai_aux;
                        Info info_pai;
                        if(*root == pai->center || (pai->n_infos == 2 && *root == pai->right))
                        {
                            pai_aux = Search_nodo_pai(*origem, pai->info1.num_start);
                            
                            if(*root == pai->center)
                                info_pai = pai->info1;
                            else 
                                info_pai = pai->info2;

                            removeu = movimento_onda(info_pai, &((*root)->info1), pai_aux, origem, &pai, maior, TreeNode23_remove2);
                        }
                        else // Filho da esquerda
                        {
                            pai_aux = Search_nodo_menor_pai(*origem, (*root)->info1.num_start);

                            TreeNode23 *menor_pai;
                            menor_pai = Search_nodo_menor_pai_2_infos(*origem, (*root)->info1.num_start);

                            TreeNode23 *avo;
                            if(pai_aux == NULL || (pai_aux != pai && menor_pai != NULL))
                            {  
                                removeu = -1;
                                *maior = pai;
                            }
                            else
                            {
                                if(pai_aux->n_infos == 2 && pai_aux->info2.num_start < (*root)->info1.num_start)
                                    info_pai = pai_aux->info2;
                                else
                                    info_pai = pai_aux->info1;

                                avo = Search_nodo_pai(*origem, info_pai.num_start);
                                removeu = movimento_onda(info_pai, &((*root)->info1), avo, origem, &pai_aux, maior, TreeNode23_remove2);
                            }
                        }
                    }
                }
            }
            else if(info2)
                removeu = TreeNode23_remove_no_interno2(origem, *root, &((*root)->info2), (*root)->right, (*root)->center, maior);
            else if(info1)
                removeu = TreeNode23_remove_no_interno2(origem, *root, &((*root)->info1), (*root)->center, (*root)->left, maior);
        }
        else
        {
            if(info < (*root)->info1.num_start)
                removeu = TreeNode23_remove2(&(*root)->left, info, *root, origem, maior);
            else if((*root)->n_infos == 1 || info < (*root)->info2.num_start)
                removeu = TreeNode23_remove2(&(*root)->center, info, *root, origem, maior);
            else
                removeu = TreeNode23_remove2(&(*root)->right, info, *root, origem, maior);
        }
    }
    return removeu;
}

int TreeNode23_remove(TreeNode23 **root, int info)
{   
    TreeNode23 *maior, *posicao_juncao;
    int removeu = TreeNode23_remove1(root, info, NULL, root, &posicao_juncao);

    if(removeu == -1)
    {
        removeu = 1;
        Info valor_juncao = *(no23_maior_info(posicao_juncao));
        maior = NULL;
        int removeu_aux = TreeNode23_rebalancear(root, valor_juncao.num_start, &maior);
        
        if(removeu_aux == -1)
        {
            TreeNode23 *pai, *posicao_juncao2;
            Info *entrada;
            pai = Search_nodo_pai(*root, valor_juncao.num_start);

            if(isInfo1(*posicao_juncao, valor_juncao.num_start))
                entrada = &(posicao_juncao->center->info1);
            else
                entrada = &(posicao_juncao->right->info1);

            removeu_aux = movimento_onda(valor_juncao, entrada, pai, root, &posicao_juncao, &posicao_juncao2, TreeNode23_remove2);

            if(removeu_aux == -1)
            {
                valor_juncao = posicao_juncao2->info1;
                pai = Search_nodo_pai(*root, valor_juncao.num_start);
                removeu_aux = movimento_onda(valor_juncao, &(posicao_juncao2->left->info1), pai, root, &posicao_juncao2, &posicao_juncao, TreeNode23_remove1);

                valor_juncao = *(no23_maior_info(posicao_juncao));
                maior = NULL;
                removeu_aux = TreeNode23_rebalancear(root, valor_juncao.num_start, &maior);
            }
        }

        if(*root == NULL)
            *root = maior;
    }

    return removeu;
}