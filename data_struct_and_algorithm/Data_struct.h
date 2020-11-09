#ifndef Data_struct
#define Data_struct
#include "common.h"

typedef struct _listNode
{
    int val;
    struct _listNode *next;
} listNode;

/*
根据一棵树的前序遍历与中序遍历构造二叉树。

注意:
你可以假设树中没有重复的元素。

例如，给出

前序遍历 preorder = [3,9,20,15,7]
中序遍历 inorder = [9,3,15,20,7]

返回如下的二叉树：

    3
   / \
  9  20
    /  \
   15   7

来源：力扣（LeetCode）
链接：https://leetcode-cn.com/problems/construct-binary-tree-from-preorder-and-inorder-traversal
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */
struct TreeNode *buildTree(int *preorder, int preorderSize, int *inorder, int inorderSize);

/*
  直接插入排序算法
*/
class DirectInsert
{
public:
    int L[10] = {0};

    DirectInsert();
    ~DirectInsert();

    /*
    直接插入排序
    */
    listNode *directInsert_list(listNode *L);

    /*
    直接插入排序：L是10个无序数组。   L[0]
    */
    int *directInsert_int(int L[10]);
};

//生成一个随机数
int GetRandomNumber();
#endif