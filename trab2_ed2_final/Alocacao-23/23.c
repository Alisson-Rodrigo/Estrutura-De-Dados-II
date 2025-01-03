#include <stdio.h>
#include <stdlib.h>
#include "23.h"

void DisplayInfos(TreeNode23 *root) {
    if (root == NULL) {
        return;  
    }

    if (root->left) {
        DisplayInfos(root->left);
    }

    printf("Começo: %d | Fim: %d | Status: %d\n", root->info1.num_start, root->info1.num_end, root->info1.status);

    if (root->center) {
        DisplayInfos(root->center);
    }

    if (root->n_infos == 2) {
        printf("Começo: %d | Fim: %d | Status: %d\n", root->info2.num_start, root->info2.num_end, root->info2.status);
        if (root->right) {
            DisplayInfos(root->right);
        }
    }
}

int isLeaf(TreeNode23 node)
{
    return node.left == NULL;
}

int isInfo1(TreeNode23 node, int info)
{
    return info == node.info1.num_start;
}

int isInfo2(TreeNode23 node, int info)
{
    return node.n_infos == 2 && info == node.info2.num_start;
}

int height_size(TreeNode23 *node)
{
    int height = -1;

    if(node != NULL)
        height = 1 + height_size(node->left);

    return height;
}

int confirm_remove(TreeNode23 *root)
{
    int confirm = 0;

    if(root != NULL)
    {
        confirm = root->n_infos == 2;

        if(!confirm)
        {
            confirm = confirm_remove(root->center);

            if(!confirm)
                confirm = confirm_remove(root->left);
        }
    }

    return confirm;
}

TreeNode23 *allocate_nodo()
{
    TreeNode23 *node;
    node = (TreeNode23 *)malloc(sizeof(TreeNode23));

    if (!node)
    {
        printf("Erro ao alocar nó da árvore");
        exit(EXIT_FAILURE);
    }

    return node;
}

void free_nodo(TreeNode23 **node)
{
    free(*node);
    *node = NULL;
}

TreeNode23 *Create_nodo(Info info, TreeNode23 *filho_left, TreeNode23 *filho_center)
{
    TreeNode23 *node;
    node = allocate_nodo();

    node->info1 = info;
    node->left = filho_left;
    node->center = filho_center;
    node->right = NULL;
    node->n_infos = 1;
    return node;
}

 TreeNode23 *SplitNode(TreeNode23 *node, Info info, Info *up, TreeNode23 *filho_maior)
{
    TreeNode23 *maior;
    if(info.num_start > node->info2.num_start)
    {
        *up = node->info2;
        maior = Create_nodo(info, node->right, filho_maior);
    }
    else if(info.num_start > node->info1.num_start)
    {
        *up = info;
        maior = Create_nodo(node->info2, filho_maior, node->right);
    }
    else
    {
        *up = node->info1;
        maior = Create_nodo(node->info2, node->center, node->right);
        node->info1 = info;
        node->center = filho_maior;
    }
    node->n_infos = 1;

    return maior;
}


 void add_info(TreeNode23 *node, Info info, TreeNode23 *filho_maior)
{
    if(info.num_start > node->info1.num_start)
    {
        node->info2 = info;
        node->right = filho_maior;
    }
    else
    {
        node->info2 = node->info1;
        node->right = node->center;
        node->center = filho_maior;
        node->info1 = info;
    }
    node->n_infos = 2;
}

TreeNode23 *no23_merge(TreeNode23 *filho1, Info info, TreeNode23 *maior, TreeNode23 **root)
{
    add_info(filho1, info, maior);

    (*root)->n_infos--;

    if((*root)->n_infos == 0)
        free_nodo(root);

    return filho1;
}

Info *no23_maior_info(TreeNode23 *root)
{
    return root->n_infos == 2 ? &(root->info2) : &(root->info1);
}


TreeNode23 *Search_nodo(TreeNode23 *root, int info)
{
    TreeNode23 *node;
    node = NULL;

    if(root != NULL)
    {
        if(isInfo1(*root, info) || isInfo2(*root, info))
            node = root;
        else if(info < root->info1.num_start)
            node = Search_nodo(root->left, info);
        else if(root->n_infos == 1 || info < root->info2.num_start)
            node = Search_nodo(root->center, info);
        else
            node = Search_nodo(root->right, info);
    }

    return node;
}

TreeNode23 *Search_nodo_menor_filho(TreeNode23 *root, TreeNode23 **pai)
{
    TreeNode23 *filho;
    filho = root;

    while(!isLeaf(*filho))
    {
        *pai = filho;
        filho = filho->left;
    }

    return filho;
}

TreeNode23 *Search_nodo_maior_filho(TreeNode23 *root, TreeNode23 **pai, Info **maior_valor)
{
    TreeNode23 *filho;
    filho = root;

    while(!isLeaf(*filho))
    {
        *pai = filho;
        if(filho->n_infos == 1)
            filho = filho->center;
        else
            filho = filho->right;
    }

    if(filho != NULL)
        *maior_valor = no23_maior_info(filho);

    return filho;
}

TreeNode23 *Search_nodo_pai(TreeNode23 *root, int info)
{
    TreeNode23 *pai;
    pai = NULL;

    if(root != NULL)
    {
        if(!isInfo1(*root, info) && !isInfo2(*root, info))
        {
            if(info < root->info1.num_start)
                pai = Search_nodo_pai(root->left, info);
            else if(root->n_infos == 1 || info < root->info2.num_start)
                pai = Search_nodo_pai(root->center, info);
            else
                pai = Search_nodo_pai(root->right, info);

            if(pai == NULL)
                pai = root;
        }
    }

    return pai;
}

TreeNode23 *Search_nodo_maior_pai(TreeNode23 *root, int info)
{
    TreeNode23 *pai;
    pai = NULL;

    if(root != NULL)
    {
        if(!isInfo1(*root, info) && !isInfo2(*root, info))
        {
            if(info < root->info1.num_start)
                pai = Search_nodo_maior_pai(root->left, info);
            else if(root->n_infos == 1 || info < root->info2.num_start)
                pai = Search_nodo_maior_pai(root->center, info);
            else
                pai = Search_nodo_maior_pai(root->right, info);

            if(pai == NULL && ((root->n_infos == 1 && root->info1.num_start > info) || (root->n_infos == 2 && root->info2.num_start > info)))
                pai = root;
        }
    }

    return pai;
}

TreeNode23 *Search_nodo_menor_pai(TreeNode23 *root, int info)
{
    TreeNode23 *pai;
    pai = NULL;

    if(root != NULL)
    {
        if(!isInfo1(*root, info) && !isInfo2(*root, info))
        {
            if(info < root->info1.num_start)
                pai = Search_nodo_menor_pai(root->left, info);
            else if(root->n_infos == 1 || info < root->info2.num_start)
                pai = Search_nodo_menor_pai(root->center, info);
            else
                pai = Search_nodo_menor_pai(root->right, info);

            if(pai == NULL && root->info1.num_start < info)
                pai = root;
        }
    }

    return pai;
}

 TreeNode23 *Search_nodo_menor_pai_2_infos(TreeNode23 *root, int info)
{
    TreeNode23 *pai;
    pai = NULL;

    if(root != NULL)
    {
        if(!isInfo1(*root, info) && !isInfo2(*root, info))
        {
            if(info < root->info1.num_start)
                pai = Search_nodo_menor_pai_2_infos(root->left, info);
            else if(root->n_infos == 1 || info < root->info2.num_start)
                pai = Search_nodo_menor_pai_2_infos(root->center, info);
            else
                pai = Search_nodo_menor_pai_2_infos(root->right, info);

            if(pai == NULL && root->n_infos == 2 && root->info2.num_start < info)
                pai = root;
        }
    }

    return pai;
}

 int movimento_onda(Info saindo, Info *entrada, TreeNode23 *pai, TreeNode23 **origem, TreeNode23 **root, TreeNode23 **maior, int (*funcao_remove)(TreeNode23 **, int, TreeNode23 *, TreeNode23 **, TreeNode23 **))
{
    int removeu = funcao_remove(root, saindo.num_start, pai, origem, maior);
    *entrada = saindo;
    return removeu;
}


void TreeNode23_desalocar(TreeNode23 **root)
{
    if(*root != NULL)
    {
        TreeNode23_desalocar(&((*root)->left));
        TreeNode23_desalocar(&((*root)->center));

        if((*root)->n_infos == 2)
            TreeNode23_desalocar(&((*root)->right));

        free_nodo(root);
    }
}

TreeNode23 *TreeNode23_inserir_no(TreeNode23 **root, Info info, TreeNode23 *pai, Info *up)
{
    TreeNode23 *maior;
    maior = NULL;

    if(*root == NULL)
        *root = Create_nodo(info, NULL, NULL);
    else
    {
        if(isLeaf(**root))
        {
            if((*root)->n_infos == 1)
                add_info(*root, info, NULL);
            else
            {
                maior = SplitNode(*root, info, up, NULL);
                if(pai == NULL)
                {
                    *root = Create_nodo(*up, *root, maior);
                    maior = NULL;
                }
            }
        }
        else
        {
            if(info.num_start < (*root)->info1.num_start)
                maior = TreeNode23_inserir_no(&((*root)->left), info, *root, up);
            else if((*root)->n_infos == 1 || info.num_start < (*root)->info2.num_start)
                maior = TreeNode23_inserir_no(&((*root)->center), info, *root, up);
            else
                maior = TreeNode23_inserir_no(&((*root)->right), info, *root, up);

            if(maior != NULL)
            {
                if((*root)->n_infos == 1)
                {
                    add_info(*root, *up, maior);
                    maior = NULL;
                }
                else
                {
                    Info promove_aux;
                    maior = SplitNode(*root, *up, &promove_aux, maior);
                    *up = promove_aux;
                    if(pai == NULL)
                    {
                        *root = Create_nodo(promove_aux, *root, maior);
                        maior = NULL;
                    }
                }
            }
        }
    }

    return maior;
}

TreeNode23 *TreeNode23_inserir(TreeNode23 **root, Info info)
{
    Info up;
    return TreeNode23_inserir_no(root, info, NULL, &up);
}


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

 int balanceamento(TreeNode23 **root, TreeNode23 *filho1, TreeNode23 **filho2, Info info, TreeNode23 **maior)
{
    int balanceou = 0;
    if(*filho2 == NULL || (*filho2)->n_infos == 0)
    {
        if(*filho2 != NULL)
            free_nodo(filho2);

        *maior = no23_merge(filho1, info, *maior, root);
        balanceou = 1;
    }
    return balanceou;
}

int TreeNode23_rebalancear(TreeNode23 **root, int info, TreeNode23 **maior)
{
    int balanceou = 0;
    if(*root != NULL)
    {
        if(!isLeaf(**root))
        {
            if(info < (*root)->info1.num_start)
                balanceou = TreeNode23_rebalancear(&((*root)->left), info, maior);
            else if((*root)->n_infos == 1 || info < (*root)->info2.num_start)
            {
                if((*root)->left->n_infos == 2 && !confirm_remove((*root)->center))
                    balanceou = -1;
                else
                    balanceou = TreeNode23_rebalancear(&((*root)->center), info, maior);
            }
            else
            {
                if((*root)->center->n_infos == 2 && !confirm_remove((*root)->right))
                    balanceou = -1;
                else
                    balanceou = TreeNode23_rebalancear(&((*root)->right), info, maior);
            }

            if(balanceou != -1)
            {
                if((*root)->n_infos == 1)
                    balanceou = balanceamento(root, (*root)->left, &((*root)->center), (*root)->info1, maior);
                else if((*root)->n_infos == 2)
                    balanceou = balanceamento(root, (*root)->center, &((*root)->right), (*root)->info2, maior);
            }
            
        }
    }

    return balanceou;
}

void no23_exibir(Info node)
{
    // Verifica se o intervalo é válido
    if (node.num_start > node.num_end) {
        printf("Erro: Intervalo invalido no no [%d] até [%d].\n", node.num_start, node.num_end);
        return;
    }

    // Exibe as informações do nó em um formato mais organizado
    printf("Bloco de [%d] até [%d] - Status: %s\n",
           node.num_start,
           node.num_end,
           node.status == LIVRE ? "Livre" : node.status == OCUPADO ? "Ocupado" : "Desconhecido");

    // Informações adicionais para depuração
    printf("Informacoes adicionais:\n");
    printf("  Inicio: %d\n", node.num_start);
    printf("  Fim: %d\n", node.num_end);
    printf("  Status: %s\n", node.status == LIVRE ? "Livre" : node.status == OCUPADO ? "Ocupado" : "Desconhecido");

    // Caso queira exibir mais informações ou mensagens de validação
    if (node.status != LIVRE && node.status != OCUPADO) {
        printf("  Aviso: Status desconhecido encontrado no no.\n");
    }
}


void TreeNode23_exibir_ordem(TreeNode23 *root)
{
    if(root != NULL)
    {
        TreeNode23_exibir_ordem(root->left);
        printf("[1º] ");
        no23_exibir(root->info1);
        TreeNode23_exibir_ordem(root->center);

        if(root->n_infos == 2)
        {
            printf("[2º] ");
            no23_exibir(root->info2);
            TreeNode23_exibir_ordem(root->right);
        }
    }
}

