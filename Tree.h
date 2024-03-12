#ifndef DIRANALYSIS_TREE_H
#define DIRANALYSIS_TREE_H

#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>
#include <sys/stat.h>
#include "Folder.h"

using namespace std;
using namespace std::filesystem;


/**
 * @brief 一个表示树节点的类
 * 包含的成员变量：
 *  data:一个指向节点信息的Folder指针
 *  bro:一个指向该节点兄弟节点的指针
 *  son:一个指向该节点孩子节点的指针
 *  parent:一个指向该节点父节点的指针
 *  realParent:一个指向该节点路径所在的文件夹对应节点的指针
 */
class TreeNode {
public:

    Folder *data = nullptr;
    TreeNode *bro = nullptr;
    TreeNode *son = nullptr;
    TreeNode *parent = nullptr;
    TreeNode *realParent = nullptr;

    /*
     * 初始化树节点
     */
    TreeNode(Folder *data);

    /*
     * 添加子节点
     */
    void addSon(TreeNode *);

    /*
     * 添加兄弟节点
     */
    void addBro(TreeNode *);


};

/**
 * @brief 树类
 * 包含的成员变量：
 * root:根节点指针
 * treeDepth:树的深度
 */
class Tree {
public:
    TreeNode *root = nullptr;
    int treeDepth = 0;


    Tree(TreeNode *);

    /*
     * 根据根节点来构造树
     */
    void inintTree();

    /*
     * 遍历树,并获取树的深度
     */
    void getTreeDepth();

    /*
     * 遍历树生成sql文件
     */
    void createSqlFile();

    /*
     * 读取文件操作
     */
    void readFile(string path);

    /*
     * 在树中寻找该文件的节点，返回一个指向该节点的指针
     */
    TreeNode *findFile(filesystem::path);

    /*
     * 修改文件信息
     */
    void modifyTreeNode(TreeNode *, time_t, int);

};


#endif //DIRANALYSIS_TREE_H
