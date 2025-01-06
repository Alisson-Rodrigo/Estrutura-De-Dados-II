#include <stdio.h>
#include <stdlib.h>
#include "23.h"

int TreeNode23_remove_internal_node(TreeNode23 **treeSource, TreeNode23* root, Info *info, TreeNode23 *leftChild, TreeNode23 *rightChild, TreeNode23 **maxNode)
{
    int isRemoved;
    TreeNode23 *childNode, *parentNode;
    Info *child_info;

    parentNode = root;

    childNode = Find_largest_child_node(leftChild, &parentNode, &child_info);

    if(childNode->n_infos == 2)
    {
        *info = *child_info;
        childNode->n_infos = 1;
    }
    else
    {
        childNode = Find_min_child_node(rightChild, &parentNode);
        isRemoved = wave_movement(childNode->info1, info, parentNode, treeSource, &childNode, maxNode, RemoveNodeFrom23Tree);
    }

    return isRemoved;
}

 int TreeNode23_delete_internal_node(TreeNode23 **treeSource, TreeNode23* root, Info *info, TreeNode23 *leftChild, TreeNode23 *rightChild, TreeNode23 **maxNode)
{
    int isRemoved;
    TreeNode23 *childNode, *parentNode;
    Info *child_info;

    parentNode = root;

    childNode = Find_min_child_node(leftChild, &parentNode);

    if(childNode->n_infos == 2)
    {
        *info = childNode->info1;
        childNode->info1 = childNode->info2;
        childNode->n_infos = 1;
    }
    else
    {
        childNode = Find_largest_child_node(rightChild, &parentNode, &child_info);
        isRemoved = wave_movement(*child_info, info, parentNode, treeSource, &childNode, maxNode, TreeNode23_Delete);
    }

    return isRemoved;
}

int RemoveNodeFrom23Tree(TreeNode23 **root, int info, TreeNode23 *parentNode, TreeNode23 **treeSource, TreeNode23 **maxNode)
{
    int isRemoved = 0;

    if(*root != NULL)
    {
        int info1 = isInfo1(**root, info);
        int info2 = isInfo2(**root, info);

        if(info1 || info2)
        {
            isRemoved = 1;
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
                    if(parentNode == NULL)
                        free_nodo(root);
                    else
                    {
                        TreeNode23 *parent_node_aux;
                        Info parent_info;
                        if(*root == parentNode->left || (parentNode->n_infos == 2 && *root == parentNode->center))
                        {
                            parent_node_aux = Find_parent_node(*treeSource, parentNode->info1.num_start);
                            
                            if(*root == parentNode->left)
                                parent_info = parentNode->info1;
                            else 
                                parent_info = parentNode->info2;

                            isRemoved = wave_movement(parent_info, &((*root)->info1), parent_node_aux, treeSource, &parentNode, maxNode, RemoveNodeFrom23Tree);
                        }
                        else 
                        {
                            parent_node_aux = Find_max_parent_node(*treeSource, (*root)->info1.num_start);

                            TreeNode23 *smaller_parent;
                            smaller_parent = Find_smallest_parent_with_two_info(*treeSource, (*root)->info1.num_start);


                            if(parent_node_aux != NULL)
                            {
                                if(parent_node_aux->info1.num_start > (*root)->info1.num_start)
                                    parent_info = parent_node_aux->info1;
                                else
                                    parent_info = parent_node_aux->info2;
                            }

                            int height_of_smaller_parent = height_size(smaller_parent);
                            int parent_node_height = height_size(parent_node_aux);

                            if(parent_node_aux == NULL || (parent_node_aux != parentNode && smaller_parent != NULL && height_of_smaller_parent <= parent_node_height && parent_info.num_start > smaller_parent->info2.num_start))
                            {
                                *maxNode = parentNode;
                                (*root)->n_infos = 0;
                                isRemoved = -1;
                            }
                            else
                            {

                                TreeNode23 *avo;
                                avo = Find_parent_node(*treeSource, parent_info.num_start);
                                isRemoved = wave_movement(parent_info, &((*root)->info1), avo, treeSource, &parent_node_aux, maxNode, RemoveNodeFrom23Tree);
                            }
                        }
                    }
                }
            }
            else if(info2)
                isRemoved = TreeNode23_remove_internal_node(treeSource, *root, &((*root)->info2), (*root)->center, (*root)->right, maxNode);
            else if(info1)
                isRemoved = TreeNode23_remove_internal_node(treeSource, *root, &((*root)->info1), (*root)->left, (*root)->center, maxNode);
        }
        else
        {
            if(info < (*root)->info1.num_start)
                isRemoved = RemoveNodeFrom23Tree(&(*root)->left, info, *root, treeSource, maxNode);
            else if((*root)->n_infos == 1 || info < (*root)->info2.num_start)
                isRemoved = RemoveNodeFrom23Tree(&(*root)->center, info, *root, treeSource, maxNode);
            else
                isRemoved = RemoveNodeFrom23Tree(&(*root)->right, info, *root, treeSource, maxNode);
        }
    }
    return isRemoved;
}

int TreeNode23_Delete(TreeNode23 **root, int info, TreeNode23 *parentNode, TreeNode23 **treeSource, TreeNode23 **maxNode)
{
    int isRemoved = 0;

    if(*root != NULL)
    {
        int info1 = isInfo1(**root, info);
        int info2 = isInfo2(**root, info);

        if(info1 || info2)
        {
            isRemoved = 1;
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
                    if(parentNode == NULL)
                        free_nodo(root);
                    else
                    {
                        TreeNode23 *parent_node_aux;
                        Info parent_info;
                        if(*root == parentNode->center || (parentNode->n_infos == 2 && *root == parentNode->right))
                        {
                            parent_node_aux = Find_parent_node(*treeSource, parentNode->info1.num_start);
                            
                            if(*root == parentNode->center)
                                parent_info = parentNode->info1;
                            else 
                                parent_info = parentNode->info2;

                            isRemoved = wave_movement(parent_info, &((*root)->info1), parent_node_aux, treeSource, &parentNode, maxNode, TreeNode23_Delete);
                        }
                        else 
                        {
                            parent_node_aux = Find_smallest_parent_node(*treeSource, (*root)->info1.num_start);

                            TreeNode23 *smaller_parent;
                            smaller_parent = Find_smallest_parent_with_two_info(*treeSource, (*root)->info1.num_start);

                            TreeNode23 *avo;
                            if(parent_node_aux == NULL || (parent_node_aux != parentNode && smaller_parent != NULL))
                            {  
                                isRemoved = -1;
                                *maxNode = parentNode;
                            }
                            else
                            {
                                if(parent_node_aux->n_infos == 2 && parent_node_aux->info2.num_start < (*root)->info1.num_start)
                                    parent_info = parent_node_aux->info2;
                                else
                                    parent_info = parent_node_aux->info1;

                                avo = Find_parent_node(*treeSource, parent_info.num_start);
                                isRemoved = wave_movement(parent_info, &((*root)->info1), avo, treeSource, &parent_node_aux, maxNode, TreeNode23_Delete);
                            }
                        }
                    }
                }
            }
            else if(info2)
                isRemoved = TreeNode23_delete_internal_node(treeSource, *root, &((*root)->info2), (*root)->right, (*root)->center, maxNode);
            else if(info1)
                isRemoved = TreeNode23_delete_internal_node(treeSource, *root, &((*root)->info1), (*root)->center, (*root)->left, maxNode);
        }
        else
        {
            if(info < (*root)->info1.num_start)
                isRemoved = TreeNode23_Delete(&(*root)->left, info, *root, treeSource, maxNode);
            else if((*root)->n_infos == 1 || info < (*root)->info2.num_start)
                isRemoved = TreeNode23_Delete(&(*root)->center, info, *root, treeSource, maxNode);
            else
                isRemoved = TreeNode23_Delete(&(*root)->right, info, *root, treeSource, maxNode);
        }
    }
    return isRemoved;
}

int TreeNode23_remove(TreeNode23 **root, int info)
{   
    TreeNode23 *maxNode, *junction_position;
    int isRemoved = RemoveNodeFrom23Tree(root, info, NULL, root, &junction_position);

    if(isRemoved == -1)
    {
        isRemoved = 1;
        Info junction_value = *(getMaxNodeInfo(junction_position));
        maxNode = NULL;
        int adjusted_balance_status = TreeNode23_adjustBalance(root, junction_value.num_start, &maxNode);
        
        if(adjusted_balance_status == -1)
        {
            TreeNode23 *parentNode, *junction_position;
            Info *enterAux;
            parentNode = Find_parent_node(*root, junction_value.num_start);

            if(isInfo1(*junction_position, junction_value.num_start))
                enterAux = &(junction_position->center->info1);
            else
                enterAux = &(junction_position->right->info1);

            adjusted_balance_status = wave_movement(junction_value, enterAux, parentNode, root, &junction_position, &junction_position, TreeNode23_Delete);

            if(adjusted_balance_status == -1)
            {
                junction_value = junction_position->info1;
                parentNode = Find_parent_node(*root, junction_value.num_start);
                adjusted_balance_status = wave_movement(junction_value, &(junction_position->left->info1), parentNode, root, &junction_position, &junction_position, RemoveNodeFrom23Tree);

                junction_value = *(getMaxNodeInfo(junction_position));
                maxNode = NULL;
                adjusted_balance_status = TreeNode23_adjustBalance(root, junction_value.num_start, &maxNode);
            }
        }

        if(*root == NULL)
            *root = maxNode;
    }

    return isRemoved;
}