#ifndef TreeNode23_H
#define TreeNode23_H

#define STATUS_OCCUPIED 0
#define ALLOCATED_STATUS_FREE 1

typedef struct info
{
    int num_start;
    int num_end;
    int status;
} Info;

typedef struct treeNode23
{
    struct treeNode23 *left;
    struct treeNode23 *center;
    struct treeNode23 *right;
    Info info1;
    Info info2;
    int n_infos;
} TreeNode23;

int isLeaf(TreeNode23 node);

int height_size(TreeNode23 *node);

TreeNode23 *allocate_nodo();

void free_nodo(TreeNode23 **node);

TreeNode23 *Create_nodo(Info info, TreeNode23 *filho_left, TreeNode23 *filho_center);

Info *getMaxNodeInfo(TreeNode23 *root);

TreeNode23 *Search_nodo(TreeNode23 *root, int info);

TreeNode23 *Find_min_child_node(TreeNode23 *root, TreeNode23 **pai);

TreeNode23 *Find_largest_child_node(TreeNode23 *root, TreeNode23 **pai, Info **maior_valor);

TreeNode23 *Find_parent_node(TreeNode23 *root, int info);

TreeNode23 *Find_max_parent_node(TreeNode23 *root, int info);

TreeNode23 *Find_smallest_parent_node(TreeNode23 *root, int info);

void TreeNode23_cleanup(TreeNode23 **root);

TreeNode23 *TreeNode23_insert(TreeNode23 **root, Info info);

int RemoveNodeFrom23Tree(TreeNode23 **root, int info, TreeNode23 *pai, TreeNode23 **origem, TreeNode23 **maior);

int TreeNode23_Delete(TreeNode23 **root, int info, TreeNode23 *pai, TreeNode23 **origem, TreeNode23 **maior);

int TreeNode23_remove(TreeNode23 **root, int info);

int TreeNode23_adjustBalance(TreeNode23 **root, int info, TreeNode23 **maior);

void showNodeInfo23(Info node);

void TreeNode23_print_in_order(TreeNode23 *root);


#endif


