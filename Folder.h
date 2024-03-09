#ifndef DIRANALYSIS_FOLDER_H
#define DIRANALYSIS_FOLDER_H

#include <iostream>
#include <filesystem>
#include <stack>
#include <string>
#include <algorithm>
#include <fstream>
#include <sys/stat.h>

#define TABLE_NAME "folder_table"
#define MIN_TIME_T 0
#define MAX_TIME_T 9223372036854775807

using namespace std;


/**
 * @brief 一个表示文件的类，包含文件/文件夹的基本信息
 * 包含的成员变量：
 *      folderPath:文件/文件夹的绝对路径
 *      folderCount:文件夹所包含的文件夹数量
 *      fileCount:文件夹所包含的文件数量
 *      folderSize:文件夹所含的文件夹数量和文件的数量之和
 *      longestPath:文件夹中所包含文件的最长路径
 *      longestPathLength:最长路径的长度
 *      folderMaxDepth:文件夹的最深的深度
 *      fileSize:文件的大小（单位：字节）
 *      ctime:文件的创建时间
 */
class Folder {
public:
    filesystem::path folderPath;
    int folderCount = 0;
    int fileCount = 0;
    int folderSize = 0;
    filesystem::path longestPath;
    int longestPathLength = 0;
    int folderMaxDepth = 0;
    int fileSize = 0;
    time_t earliestTime = MAX_TIME_T;
    time_t latestTime = MIN_TIME_T;
    filesystem::path earliestPath;
    filesystem::path latestPath;
    time_t ctime;


    Folder(filesystem::path);

    Folder();


    /*
     * 展示文件夹的信息
     */
    void showFolderInfo();


    /*
     * 展示文件信息
     */
    void showFileInfo();

    /*
     * 获取文件夹的深度，通过路径中所含‘\’的数量来确定，（难道他真是个天才）
     */
    int getCurrentDepth(filesystem::path);


    void getFolderValue();

    /*
     * 创建sql文件
     */
    void addSqlFile();


};


#endif //DIRANALYSIS_FOLDER_H
