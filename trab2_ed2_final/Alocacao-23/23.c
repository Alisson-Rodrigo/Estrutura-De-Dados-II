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

 int wave_movement(Info saindo, Info *entrada, TreeNode23 *pai, TreeNode23 **origem, TreeNode23 **root, TreeNode23 **maior, int (*funcao_remove)(TreeNode23 **, int, TreeNode23 *, TreeNode23 **, TreeNode23 **))
{
    int removeu = funcao_remove(root, saindo.num_start, pai, origem, maior);
    *entrada = saindo;
    return removeu;
}


void TreeNode23_cleanup(TreeNode23 **root)
{
    if(*root != NULL)
    {
        TreeNode23_cleanup(&((*root)->left));
        TreeNode23_cleanup(&((*root)->center));

        if((*root)->n_infos == 2)
            TreeNode23_cleanup(&((*root)->right));

        free_nodo(root);
    }
}

TreeNode23 *TreeNode23_insertNode(TreeNode23 **root, Info info, TreeNode23 *pai, Info *up)
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
                maior = TreeNode23_insertNode(&((*root)->left), info, *root, up);
            else if((*root)->n_infos == 1 || info.num_start < (*root)->info2.num_start)
                maior = TreeNode23_insertNode(&((*root)->center), info, *root, up);
            else
                maior = TreeNode23_insertNode(&((*root)->right), info, *root, up);

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

TreeNode23 *TreeNode23_insert(TreeNode23 **root, Info info)
{
    Info up;
    return TreeNode23_insertNode(root, info, NULL, &up);
}

 int balanceTree(TreeNode23 **root, TreeNode23 *filho1, TreeNode23 **filho2, Info info, TreeNode23 **maior)
{
    int isBalanced = 0;
    if(*filho2 == NULL || (*filho2)->n_infos == 0)
    {
        if(*filho2 != NULL)
            free_nodo(filho2);

        *maior = no23_merge(filho1, info, *maior, root);
        isBalanced = 1;
    }
    return isBalanced;
}

int TreeNode23_adjustBalance(TreeNode23 **root, int info, TreeNode23 **maior)
{
    int isBalanced = 0;
    if(*root != NULL)
    {
        if(!isLeaf(**root))
        {
            if(info < (*root)->info1.num_start)
                isBalanced = TreeNode23_adjustBalance(&((*root)->left), info, maior);
            else if((*root)->n_infos == 1 || info < (*root)->info2.num_start)
            {
                if((*root)->left->n_infos == 2 && !confirm_remove((*root)->center))
                    isBalanced = -1;
                else
                    isBalanced = TreeNode23_adjustBalance(&((*root)->center), info, maior);
            }
            else
            {
                if((*root)->center->n_infos == 2 && !confirm_remove((*root)->right))
                    isBalanced = -1;
                else
                    isBalanced = TreeNode23_adjustBalance(&((*root)->right), info, maior);
            }

            if(isBalanced != -1)
            {
                if((*root)->n_infos == 1)
                    isBalanced = balanceTree(root, (*root)->left, &((*root)->center), (*root)->info1, maior);
                else if((*root)->n_infos == 2)
                    isBalanced = balanceTree(root, (*root)->center, &((*root)->right), (*root)->info2, maior);
            }
            
        }
    }

    return isBalanced;
}

void showNodeInfo23(Info node)
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
        printf("  Aviso: Status desconhecido encontrado no no\n");
    }
}


void TreeNode23_print_in_order(TreeNode23 *root)
{
    if(root != NULL)
    {
        TreeNode23_print_in_order(root->left);
        printf("[1º] ");
        showNodeInfo23(root->info1);
        TreeNode23_print_in_order(root->center);

        if(root->n_infos == 2)
        {
            printf("[2º] ");
            showNodeInfo23(root->info2);
            TreeNode23_print_in_order(root->right);
        }
    }
}


