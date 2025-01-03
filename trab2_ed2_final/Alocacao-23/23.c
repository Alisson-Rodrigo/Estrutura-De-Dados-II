#include <stdio.h>
#include <stdlib.h>
#include "23.h"

void DisplayInfos(TreeNode23 *root) {
    if (root == NULL) {
        return;  // Se o nó é nulo, retorna sem fazer nada.
    }

    // Percorre a subárvore esquerda primeiro.
    if (root->left) {
        DisplayInfos(root->left);
    }

    // Mostra as informações do nó atual.
    printf("Começo: %d | Fim: %d | Status: %d\n", root->info1.num_start, root->info1.num_end, root->info1.status);

    // Percorre a subárvore central.
    if (root->center) {
        DisplayInfos(root->center);
    }

    // Se houver uma segunda informação, mostra e percorre a subárvore direita.
    if (root->n_infos == 2) {
        printf("Começo: %d | Fim: %d | Status: %d\n", root->info2.num_start, root->info2.num_end, root->info2.status);
        if (root->right) {
            DisplayInfos(root->right);
        }
    }
}

int isLeaf(TreeNode23 no)
{
    return no.left == NULL;
}

int isInfo1(TreeNode23 no, int info)
{
    return info == no.info1.num_start;
}

int isInfo2(TreeNode23 no, int info)
{
    return no.n_infos == 2 && info == no.info2.num_start;
}

int height_size(TreeNode23 *no)
{
    int altura = -1;

    if(no != NULL)
        altura = 1 + height_size(no->left);

    return altura;
}

int possivel_remover(TreeNode23 *raiz)
{
    int possivel = 0;

    if(raiz != NULL)
    {
        possivel = raiz->n_infos == 2;

        if(!possivel)
        {
            possivel = possivel_remover(raiz->center);

            if(!possivel)
                possivel = possivel_remover(raiz->left);
        }
    }

    return possivel;
}

TreeNode23 *no23_alocar()
{
    TreeNode23 *no;
    no = (TreeNode23 *)malloc(sizeof(TreeNode23));

    if (!no)
    {
        printf("Erro ao alocar nó da árvore");
        exit(EXIT_FAILURE);
    }

    return no;
}

void no23_desalocar(TreeNode23 **no)
{
    free(*no);
    *no = NULL;
}

TreeNode23 *no23_criar(Data info, TreeNode23 *filho_left, TreeNode23 *filho_center)
{
    TreeNode23 *no;
    no = no23_alocar();

    no->info1 = info;
    no->left = filho_left;
    no->center = filho_center;
    no->right = NULL;
    no->n_infos = 1;
    return no;
}

static TreeNode23 *no23_quebrar(TreeNode23 *no, Data info, Data *promove, TreeNode23 *filho_maior)
{
    TreeNode23 *maior;
    if(info.num_start > no->info2.num_start)
    {
        *promove = no->info2;
        maior = no23_criar(info, no->right, filho_maior);
    }
    else if(info.num_start > no->info1.num_start)
    {
        *promove = info;
        maior = no23_criar(no->info2, filho_maior, no->right);
    }
    else
    {
        *promove = no->info1;
        maior = no23_criar(no->info2, no->center, no->right);
        no->info1 = info;
        no->center = filho_maior;
    }
    no->n_infos = 1;

    return maior;
}


static void no23_adicionar_info(TreeNode23 *no, Data info, TreeNode23 *filho_maior)
{
    if(info.num_start > no->info1.num_start)
    {
        no->info2 = info;
        no->right = filho_maior;
    }
    else
    {
        no->info2 = no->info1;
        no->right = no->center;
        no->center = filho_maior;
        no->info1 = info;
    }
    no->n_infos = 2;
}

static TreeNode23 *no23_juntar(TreeNode23 *filho1, Data info, TreeNode23 *maior, TreeNode23 **raiz)
{
    no23_adicionar_info(filho1, info, maior);

    (*raiz)->n_infos--;

    if((*raiz)->n_infos == 0)
        no23_desalocar(raiz);

    return filho1;
}

Data *no23_maior_info(TreeNode23 *raiz)
{
    return raiz->n_infos == 2 ? &(raiz->info2) : &(raiz->info1);
}

TreeNode23 *TreeNode23_criar()
{
    return NULL;
}

TreeNode23 *TreeNode23_buscar(TreeNode23 *raiz, int info)
{
    TreeNode23 *no;
    no = NULL;

    if(raiz != NULL)
    {
        if(isInfo1(*raiz, info) || isInfo2(*raiz, info))
            no = raiz;
        else if(info < raiz->info1.num_start)
            no = TreeNode23_buscar(raiz->left, info);
        else if(raiz->n_infos == 1 || info < raiz->info2.num_start)
            no = TreeNode23_buscar(raiz->center, info);
        else
            no = TreeNode23_buscar(raiz->right, info);
    }

    return no;
}

TreeNode23 *TreeNode23_buscar_menor_filho(TreeNode23 *raiz, TreeNode23 **pai)
{
    TreeNode23 *filho;
    filho = raiz;

    while(!isLeaf(*filho))
    {
        *pai = filho;
        filho = filho->left;
    }

    return filho;
}

TreeNode23 *TreeNode23_buscar_maior_filho(TreeNode23 *raiz, TreeNode23 **pai, Data **maior_valor)
{
    TreeNode23 *filho;
    filho = raiz;

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

TreeNode23 *TreeNode23_buscar_pai(TreeNode23 *raiz, int info)
{
    TreeNode23 *pai;
    pai = NULL;

    if(raiz != NULL)
    {
        if(!isInfo1(*raiz, info) && !isInfo2(*raiz, info))
        {
            if(info < raiz->info1.num_start)
                pai = TreeNode23_buscar_pai(raiz->left, info);
            else if(raiz->n_infos == 1 || info < raiz->info2.num_start)
                pai = TreeNode23_buscar_pai(raiz->center, info);
            else
                pai = TreeNode23_buscar_pai(raiz->right, info);

            if(pai == NULL)
                pai = raiz;
        }
    }

    return pai;
}

TreeNode23 *TreeNode23_buscar_maior_pai(TreeNode23 *raiz, int info)
{
    TreeNode23 *pai;
    pai = NULL;

    if(raiz != NULL)
    {
        if(!isInfo1(*raiz, info) && !isInfo2(*raiz, info))
        {
            if(info < raiz->info1.num_start)
                pai = TreeNode23_buscar_maior_pai(raiz->left, info);
            else if(raiz->n_infos == 1 || info < raiz->info2.num_start)
                pai = TreeNode23_buscar_maior_pai(raiz->center, info);
            else
                pai = TreeNode23_buscar_maior_pai(raiz->right, info);

            if(pai == NULL && ((raiz->n_infos == 1 && raiz->info1.num_start > info) || (raiz->n_infos == 2 && raiz->info2.num_start > info)))
                pai = raiz;
        }
    }

    return pai;
}

TreeNode23 *TreeNode23_buscar_menor_pai(TreeNode23 *raiz, int info)
{
    TreeNode23 *pai;
    pai = NULL;

    if(raiz != NULL)
    {
        if(!isInfo1(*raiz, info) && !isInfo2(*raiz, info))
        {
            if(info < raiz->info1.num_start)
                pai = TreeNode23_buscar_menor_pai(raiz->left, info);
            else if(raiz->n_infos == 1 || info < raiz->info2.num_start)
                pai = TreeNode23_buscar_menor_pai(raiz->center, info);
            else
                pai = TreeNode23_buscar_menor_pai(raiz->right, info);

            if(pai == NULL && raiz->info1.num_start < info)
                pai = raiz;
        }
    }

    return pai;
}

static TreeNode23 *TreeNode23_buscar_menor_pai_2_infos(TreeNode23 *raiz, int info)
{
    TreeNode23 *pai;
    pai = NULL;

    if(raiz != NULL)
    {
        if(!isInfo1(*raiz, info) && !isInfo2(*raiz, info))
        {
            if(info < raiz->info1.num_start)
                pai = TreeNode23_buscar_menor_pai_2_infos(raiz->left, info);
            else if(raiz->n_infos == 1 || info < raiz->info2.num_start)
                pai = TreeNode23_buscar_menor_pai_2_infos(raiz->center, info);
            else
                pai = TreeNode23_buscar_menor_pai_2_infos(raiz->right, info);

            if(pai == NULL && raiz->n_infos == 2 && raiz->info2.num_start < info)
                pai = raiz;
        }
    }

    return pai;
}

static int movimento_onda(Data saindo, Data *entrada, TreeNode23 *pai, TreeNode23 **origem, TreeNode23 **raiz, TreeNode23 **maior, int (*funcao_remover)(TreeNode23 **, int, TreeNode23 *, TreeNode23 **, TreeNode23 **))
{
    int removeu = funcao_remover(raiz, saindo.num_start, pai, origem, maior);
    *entrada = saindo;
    return removeu;
}


void TreeNode23_desalocar(TreeNode23 **raiz)
{
    if(*raiz != NULL)
    {
        TreeNode23_desalocar(&((*raiz)->left));
        TreeNode23_desalocar(&((*raiz)->center));

        if((*raiz)->n_infos == 2)
            TreeNode23_desalocar(&((*raiz)->right));

        no23_desalocar(raiz);
    }
}

TreeNode23 *TreeNode23_inserir_no(TreeNode23 **raiz, Data info, TreeNode23 *pai, Data *promove)
{
    TreeNode23 *maior;
    maior = NULL;

    if(*raiz == NULL)
        *raiz = no23_criar(info, NULL, NULL);
    else
    {
        if(isLeaf(**raiz))
        {
            if((*raiz)->n_infos == 1)
                no23_adicionar_info(*raiz, info, NULL);
            else
            {
                maior = no23_quebrar(*raiz, info, promove, NULL);
                if(pai == NULL)
                {
                    *raiz = no23_criar(*promove, *raiz, maior);
                    maior = NULL;
                }
            }
        }
        else
        {
            if(info.num_start < (*raiz)->info1.num_start)
                maior = TreeNode23_inserir_no(&((*raiz)->left), info, *raiz, promove);
            else if((*raiz)->n_infos == 1 || info.num_start < (*raiz)->info2.num_start)
                maior = TreeNode23_inserir_no(&((*raiz)->center), info, *raiz, promove);
            else
                maior = TreeNode23_inserir_no(&((*raiz)->right), info, *raiz, promove);

            if(maior != NULL)
            {
                if((*raiz)->n_infos == 1)
                {
                    no23_adicionar_info(*raiz, *promove, maior);
                    maior = NULL;
                }
                else
                {
                    Data promove_aux;
                    maior = no23_quebrar(*raiz, *promove, &promove_aux, maior);
                    *promove = promove_aux;
                    if(pai == NULL)
                    {
                        *raiz = no23_criar(promove_aux, *raiz, maior);
                        maior = NULL;
                    }
                }
            }
        }
    }

    return maior;
}

TreeNode23 *TreeNode23_inserir(TreeNode23 **raiz, Data info)
{
    Data promove;
    return TreeNode23_inserir_no(raiz, info, NULL, &promove);
}


static int TreeNode23_remover_no_interno1(TreeNode23 **origem, TreeNode23* raiz, Data *info, TreeNode23 *filho1, TreeNode23 *filho2, TreeNode23 **maior)
{
    int removeu;
    TreeNode23 *filho, *pai;
    Data *info_filho;

    pai = raiz;

    filho = TreeNode23_buscar_maior_filho(filho1, &pai, &info_filho);

    if(filho->n_infos == 2)
    {
        *info = *info_filho;
        filho->n_infos = 1;
    }
    else
    {
        filho = TreeNode23_buscar_menor_filho(filho2, &pai);
        removeu = movimento_onda(filho->info1, info, pai, origem, &filho, maior, TreeNode23_remover1);
    }

    return removeu;
}

static int TreeNode23_remover_no_interno2(TreeNode23 **origem, TreeNode23* raiz, Data *info, TreeNode23 *filho1, TreeNode23 *filho2, TreeNode23 **maior)
{
    int removeu;
    TreeNode23 *filho, *pai;
    Data *info_filho;

    pai = raiz;

    filho = TreeNode23_buscar_menor_filho(filho1, &pai);

    if(filho->n_infos == 2)
    {
        *info = filho->info1;
        filho->info1 = filho->info2;
        filho->n_infos = 1;
    }
    else
    {
        filho = TreeNode23_buscar_maior_filho(filho2, &pai, &info_filho);
        removeu = movimento_onda(*info_filho, info, pai, origem, &filho, maior, TreeNode23_remover2);
    }

    return removeu;
}

int TreeNode23_remover1(TreeNode23 **raiz, int info, TreeNode23 *pai, TreeNode23 **origem, TreeNode23 **maior)
{
    int removeu = 0;

    if(*raiz != NULL)
    {
        int info1 = isInfo1(**raiz, info);
        int info2 = isInfo2(**raiz, info);

        if(info1 || info2)
        {
            removeu = 1;
            if(isLeaf(**raiz))
            {
                if((*raiz)->n_infos == 2)
                {
                    if(info1)
                        (*raiz)->info1 = (*raiz)->info2;

                    (*raiz)->n_infos = 1;
                }
                else
                {
                    if(pai == NULL)
                        no23_desalocar(raiz);
                    else
                    {
                        TreeNode23 *pai_aux;
                        Data info_pai;
                        if(*raiz == pai->left || (pai->n_infos == 2 && *raiz == pai->center))
                        {
                            pai_aux = TreeNode23_buscar_pai(*origem, pai->info1.num_start);
                            
                            if(*raiz == pai->left)
                                info_pai = pai->info1;
                            else 
                                info_pai = pai->info2;

                            removeu = movimento_onda(info_pai, &((*raiz)->info1), pai_aux, origem, &pai, maior, TreeNode23_remover1);
                        }
                        else // Filho do center (com pai de 1 info) ou da direita
                        {
                            pai_aux = TreeNode23_buscar_maior_pai(*origem, (*raiz)->info1.num_start);

                            TreeNode23 *menor_pai;
                            menor_pai = TreeNode23_buscar_menor_pai_2_infos(*origem, (*raiz)->info1.num_start);


                            if(pai_aux != NULL)
                            {
                                if(pai_aux->info1.num_start > (*raiz)->info1.num_start)
                                    info_pai = pai_aux->info1;
                                else
                                    info_pai = pai_aux->info2;
                            }

                            int altura_menor_pai = height_size(menor_pai);
                            int altura_pai_aux = height_size(pai_aux);

                            if(pai_aux == NULL || (pai_aux != pai && menor_pai != NULL && altura_menor_pai <= altura_pai_aux && info_pai.num_start > menor_pai->info2.num_start))
                            {
                                *maior = pai;
                                (*raiz)->n_infos = 0;
                                removeu = -1;
                            }
                            else
                            {

                                TreeNode23 *avo;
                                avo = TreeNode23_buscar_pai(*origem, info_pai.num_start);
                                removeu = movimento_onda(info_pai, &((*raiz)->info1), avo, origem, &pai_aux, maior, TreeNode23_remover1);
                            }
                        }
                    }
                }
            }
            else if(info2)
                removeu = TreeNode23_remover_no_interno1(origem, *raiz, &((*raiz)->info2), (*raiz)->center, (*raiz)->right, maior);
            else if(info1)
                removeu = TreeNode23_remover_no_interno1(origem, *raiz, &((*raiz)->info1), (*raiz)->left, (*raiz)->center, maior);
        }
        else
        {
            if(info < (*raiz)->info1.num_start)
                removeu = TreeNode23_remover1(&(*raiz)->left, info, *raiz, origem, maior);
            else if((*raiz)->n_infos == 1 || info < (*raiz)->info2.num_start)
                removeu = TreeNode23_remover1(&(*raiz)->center, info, *raiz, origem, maior);
            else
                removeu = TreeNode23_remover1(&(*raiz)->right, info, *raiz, origem, maior);
        }
    }
    return removeu;
}

int TreeNode23_remover2(TreeNode23 **raiz, int info, TreeNode23 *pai, TreeNode23 **origem, TreeNode23 **maior)
{
    int removeu = 0;

    if(*raiz != NULL)
    {
        int info1 = isInfo1(**raiz, info);
        int info2 = isInfo2(**raiz, info);

        if(info1 || info2)
        {
            removeu = 1;
            if(isLeaf(**raiz))
            {
                if((*raiz)->n_infos == 2)
                {
                    if(info1)
                        (*raiz)->info1 = (*raiz)->info2;

                    (*raiz)->n_infos = 1;
                }
                else
                {
                    if(pai == NULL)
                        no23_desalocar(raiz);
                    else
                    {
                        TreeNode23 *pai_aux;
                        Data info_pai;
                        if(*raiz == pai->center || (pai->n_infos == 2 && *raiz == pai->right))
                        {
                            pai_aux = TreeNode23_buscar_pai(*origem, pai->info1.num_start);
                            
                            if(*raiz == pai->center)
                                info_pai = pai->info1;
                            else 
                                info_pai = pai->info2;

                            removeu = movimento_onda(info_pai, &((*raiz)->info1), pai_aux, origem, &pai, maior, TreeNode23_remover2);
                        }
                        else // Filho da esquerda
                        {
                            pai_aux = TreeNode23_buscar_menor_pai(*origem, (*raiz)->info1.num_start);

                            TreeNode23 *menor_pai;
                            menor_pai = TreeNode23_buscar_menor_pai_2_infos(*origem, (*raiz)->info1.num_start);

                            TreeNode23 *avo;
                            if(pai_aux == NULL || (pai_aux != pai && menor_pai != NULL))
                            {  
                                removeu = -1;
                                *maior = pai;
                            }
                            else
                            {
                                if(pai_aux->n_infos == 2 && pai_aux->info2.num_start < (*raiz)->info1.num_start)
                                    info_pai = pai_aux->info2;
                                else
                                    info_pai = pai_aux->info1;

                                avo = TreeNode23_buscar_pai(*origem, info_pai.num_start);
                                removeu = movimento_onda(info_pai, &((*raiz)->info1), avo, origem, &pai_aux, maior, TreeNode23_remover2);
                            }
                        }
                    }
                }
            }
            else if(info2)
                removeu = TreeNode23_remover_no_interno2(origem, *raiz, &((*raiz)->info2), (*raiz)->right, (*raiz)->center, maior);
            else if(info1)
                removeu = TreeNode23_remover_no_interno2(origem, *raiz, &((*raiz)->info1), (*raiz)->center, (*raiz)->left, maior);
        }
        else
        {
            if(info < (*raiz)->info1.num_start)
                removeu = TreeNode23_remover2(&(*raiz)->left, info, *raiz, origem, maior);
            else if((*raiz)->n_infos == 1 || info < (*raiz)->info2.num_start)
                removeu = TreeNode23_remover2(&(*raiz)->center, info, *raiz, origem, maior);
            else
                removeu = TreeNode23_remover2(&(*raiz)->right, info, *raiz, origem, maior);
        }
    }
    return removeu;
}

int TreeNode23_remover(TreeNode23 **raiz, int info)
{   
    TreeNode23 *maior, *posicao_juncao;
    int removeu = TreeNode23_remover1(raiz, info, NULL, raiz, &posicao_juncao);

    if(removeu == -1)
    {
        removeu = 1;
        Data valor_juncao = *(no23_maior_info(posicao_juncao));
        maior = NULL;
        int removeu_aux = TreeNode23_rebalancear(raiz, valor_juncao.num_start, &maior);
        
        if(removeu_aux == -1)
        {
            TreeNode23 *pai, *posicao_juncao2;
            Data *entrada;
            pai = TreeNode23_buscar_pai(*raiz, valor_juncao.num_start);

            if(isInfo1(*posicao_juncao, valor_juncao.num_start))
                entrada = &(posicao_juncao->center->info1);
            else
                entrada = &(posicao_juncao->right->info1);

            removeu_aux = movimento_onda(valor_juncao, entrada, pai, raiz, &posicao_juncao, &posicao_juncao2, TreeNode23_remover2);

            if(removeu_aux == -1)
            {
                valor_juncao = posicao_juncao2->info1;
                pai = TreeNode23_buscar_pai(*raiz, valor_juncao.num_start);
                removeu_aux = movimento_onda(valor_juncao, &(posicao_juncao2->left->info1), pai, raiz, &posicao_juncao2, &posicao_juncao, TreeNode23_remover1);

                valor_juncao = *(no23_maior_info(posicao_juncao));
                maior = NULL;
                removeu_aux = TreeNode23_rebalancear(raiz, valor_juncao.num_start, &maior);
            }
        }

        if(*raiz == NULL)
            *raiz = maior;
    }

    return removeu;
}

static int balanceamento(TreeNode23 **raiz, TreeNode23 *filho1, TreeNode23 **filho2, Data info, TreeNode23 **maior)
{
    int balanceou = 0;
    if(*filho2 == NULL || (*filho2)->n_infos == 0)
    {
        if(*filho2 != NULL)
            no23_desalocar(filho2);

        *maior = no23_juntar(filho1, info, *maior, raiz);
        balanceou = 1;
    }
    return balanceou;
}

int TreeNode23_rebalancear(TreeNode23 **raiz, int info, TreeNode23 **maior)
{
    int balanceou = 0;
    if(*raiz != NULL)
    {
        if(!isLeaf(**raiz))
        {
            if(info < (*raiz)->info1.num_start)
                balanceou = TreeNode23_rebalancear(&((*raiz)->left), info, maior);
            else if((*raiz)->n_infos == 1 || info < (*raiz)->info2.num_start)
            {
                if((*raiz)->left->n_infos == 2 && !possivel_remover((*raiz)->center))
                    balanceou = -1;
                else
                    balanceou = TreeNode23_rebalancear(&((*raiz)->center), info, maior);
            }
            else
            {
                if((*raiz)->center->n_infos == 2 && !possivel_remover((*raiz)->right))
                    balanceou = -1;
                else
                    balanceou = TreeNode23_rebalancear(&((*raiz)->right), info, maior);
            }

            if(balanceou != -1)
            {
                if((*raiz)->n_infos == 1)
                    balanceou = balanceamento(raiz, (*raiz)->left, &((*raiz)->center), (*raiz)->info1, maior);
                else if((*raiz)->n_infos == 2)
                    balanceou = balanceamento(raiz, (*raiz)->center, &((*raiz)->right), (*raiz)->info2, maior);
            }
            
        }
    }

    return balanceou;
}

void no23_exibir(Data no)
{
    printf("Bloco de [%d] até [%d] - [%s]\n", no.num_start, no.num_end, no.status == LIVRE ? "Livre" : "Ocupado");
    // printf("%d -> ", no.num_start);
}

void TreeNode23_exibir_ordem(TreeNode23 *raiz)
{
    if(raiz != NULL)
    {
        TreeNode23_exibir_ordem(raiz->left);
        printf("[1º] ");
        no23_exibir(raiz->info1);
        TreeNode23_exibir_ordem(raiz->center);

        if(raiz->n_infos == 2)
        {
            printf("[2º] ");
            no23_exibir(raiz->info2);
            TreeNode23_exibir_ordem(raiz->right);
        }
    }
}

