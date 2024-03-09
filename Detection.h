#ifndef DIRANALYSIS_DETECTION_H
#define DIRANALYSIS_DETECTION_H

#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <regex>
#include "Tree.h"


#define MYSTAT "D:\\learn\\hust\\program_design\\dirAnalysis\\dirAnalysis\\dirAnalysis\\cmake-build-debug\\mystat.txt"

using namespace std;


class Detection {
public:
    //读取mystat.txt，文件统计文件夹信息
    void readStatFile(filesystem::path);

    //读取myfile.txt，进行文件操作
    void readMyFile(filesystem::path, Tree *);

    //读取mydir.txt，进行目录操作
    void readMyDir(filesystem::path, Tree *);
};


/*
 * path:文件路路径
 * action:操作类型
 * time:时间
 * size:大小
 */
class Action {
public:
    filesystem::path path;
    string action;
    string time;
    string size;
};

#endif //DIRANALYSIS_DETECTION_H
