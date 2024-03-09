#include <iostream>
#include <string>
#include "Folder.h"
#include "Tree.h"
#include "Detection.h"

using namespace std;

#define MYFILE "D:\\learn\\hust\\program_design\\example\\myfile.txt"
#define MYSTAT "D:\\learn\\hust\\program_design\\example\\mystat.txt"
#define MYDIR "D:\\learn\\hust\\program_design\\example\\mydir.txt"


/*
 * 展示功能菜单
 */
void showMenu() {
    cout << "1.获取根目录信息" << endl
         << "2.生成SQL语句文件" << endl
         << "3.统计树的深度" << endl
         << "4.展示文件夹信息" << endl
         << "5.文件操作" << endl
         << "6.文件夹操作" << endl
         << "0.退出系统" << endl;
}


int main() {
    system("chcp 65001");
    cout << "请选择要遍历的根目录" << endl;
    string rootPath;
    cin >> rootPath;
    system("cls");
    cout << "正在构建树" << endl;
    Folder myFolder(rootPath);
    TreeNode rootNode(&myFolder);
    Tree myTree(&rootNode);
    myTree.inintTree();
    system("cls");
    while (true) {
        showMenu();
        int select;
        cin >> select;
        switch (select) {
            case 1: {
                myTree.root->data->showFolderInfo();
                system("pause");
                system("cls");
                break;
            }
            case 2: {
                myTree.createSqlFile();
                cout << "已生成SQL语句文件:sqlFile.sql" << endl;
                system("pause");
                system("cls");
                break;
            }
            case 3: {
                myTree.getTreeDepth();
                cout << "树的深度为：" << myTree.treeDepth << endl;
                system("pause");
                system("cls");
                break;
            }
            case 4: {
                Detection det;
                det.readStatFile(MYSTAT);
                system("pause");
                system("cls");
                break;
            }
            case 5: {
                Detection det;
                det.readMyFile(MYFILE, &myTree);
                system("pause");
                system("cls");
                break;
            }
            case 6: {
                Detection det;
                det.readMyDir(MYDIR, &myTree);
                system("pause");
                system("cls");
                break;
            }
            case 0: {
                cout << "欢迎下次使用" << endl;
                system("pause");
                return 0;
            }
            default:
                break;
        }
    }
    return 0;
}
