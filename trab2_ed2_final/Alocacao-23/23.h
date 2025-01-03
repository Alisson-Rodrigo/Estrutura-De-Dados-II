#ifndef TreeNode23_H
#define TreeNode23_H

#define OCUPADO 0
#define LIVRE 1


typedef struct data
{
    int num_start;
    int num_end;
    int *end_start;
    int *end_end;
    int status;
} Data;

typedef struct treeNode23
{
    Data info1;
    Data info2;
    int n_infos;
    struct treeNode23 *left;
    struct treeNode23 *center;
    struct treeNode23 *right;
} TreeNode23;


int isLeaf(TreeNode23 no);

int height_size(TreeNode23 *no);

TreeNode23 *no23_alocar();

void no23_desalocar(TreeNode23 **no);

TreeNode23 *no23_criar(Data info, TreeNode23 *filho_left, TreeNode23 *filho_center);

Data *no23_maior_info(TreeNode23 *raiz);

TreeNode23 *TreeNode23_criar();

TreeNode23 *TreeNode23_buscar(TreeNode23 *raiz, int info);

TreeNode23 *TreeNode23_buscar_menor_filho(TreeNode23 *raiz, TreeNode23 **pai);

TreeNode23 *TreeNode23_buscar_maior_filho(TreeNode23 *raiz, TreeNode23 **pai, Data **maior_valor);

TreeNode23 *TreeNode23_buscar_pai(TreeNode23 *raiz, int info);

TreeNode23 *TreeNode23_buscar_maior_pai(TreeNode23 *raiz, int info);

TreeNode23 *TreeNode23_buscar_menor_pai(TreeNode23 *raiz, int info);

void TreeNode23_desalocar(TreeNode23 **raiz);

TreeNode23 *TreeNode23_inserir(TreeNode23 **raiz, Data info);

int TreeNode23_remover1(TreeNode23 **raiz, int info, TreeNode23 *pai, TreeNode23 **origem, TreeNode23 **maior);

int TreeNode23_remover2(TreeNode23 **raiz, int info, TreeNode23 *pai, TreeNode23 **origem, TreeNode23 **maior);

int TreeNode23_remover(TreeNode23 **raiz, int info);

int TreeNode23_rebalancear(TreeNode23 **raiz, int info, TreeNode23 **maior);

void no23_exibir(Data no);

void TreeNode23_exibir_pre(TreeNode23 *raiz);

void TreeNode23_exibir_ordem(TreeNode23 *raiz);

void TreeNode23_exibir_pos(TreeNode23 *raiz);

#endif


