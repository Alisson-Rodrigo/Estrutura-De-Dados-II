#ifndef TreeNode23_H
#define TreeNode23_H

#define OCUPADO 0
#define LIVRE 1

typedef struct info
{
    int num_start;
    int num_end;
    int status;
} Info;

typedef struct treeNode23
{
    Info info1;
    Info info2;
    int n_infos;
    struct treeNode23 *left;
    struct treeNode23 *center;
    struct treeNode23 *right;
} TreeNode23;

int isLeaf(TreeNode23 node);

int height_size(TreeNode23 *node);

TreeNode23 *allocate_nodo();

void free_nodo(TreeNode23 **node);

TreeNode23 *Create_nodo(Info info, TreeNode23 *filho_left, TreeNode23 *filho_center);

Info *no23_maior_info(TreeNode23 *root);

TreeNode23 *Search_nodo(TreeNode23 *root, int info);

TreeNode23 *Search_nodo_menor_filho(TreeNode23 *root, TreeNode23 **pai);

TreeNode23 *Search_nodo_maior_filho(TreeNode23 *root, TreeNode23 **pai, Info **maior_valor);

TreeNode23 *Search_nodo_pai(TreeNode23 *root, int info);

TreeNode23 *Search_nodo_maior_pai(TreeNode23 *root, int info);

TreeNode23 *Search_nodo_menor_pai(TreeNode23 *root, int info);

void TreeNode23_cleanup(TreeNode23 **root);

TreeNode23 *TreeNode23_insert(TreeNode23 **root, Info info);

int RemoveNodeFrom23Tree(TreeNode23 **root, int info, TreeNode23 *pai, TreeNode23 **origem, TreeNode23 **maior);

int TreeNode23_Delete(TreeNode23 **root, int info, TreeNode23 *pai, TreeNode23 **origem, TreeNode23 **maior);

int TreeNode23_remove(TreeNode23 **root, int info);

int TreeNode23_adjustBalance(TreeNode23 **root, int info, TreeNode23 **maior);

void showNodeInfo23(Info node);

void TreeNode23_exibir_pre(TreeNode23 *root);

void TreeNode23_print_in_order(TreeNode23 *root);

void TreeNode23_exibir_pos(TreeNode23 *root);

#endif


