#include "Detection.h"
#include "Folder.h"
#include "Tree.h"

//实现了一个分割字符串的函数
vector<string> split(string str, char delimiter) {
    vector<string> tokens;
    auto start = str.begin();
    auto end = find(str.begin(), str.end(), delimiter);
    for (int i = 0; i < 4; i++) {
        string toke(start, end);
        tokens.push_back(toke);
        start = end + 1;
        end = find(start, str.end(), delimiter);
    }
    return tokens;
}

void Detection::readStatFile(std::filesystem::path path) {
    vector<filesystem::path> pathVec;
    std::fstream ifs(path);
    string buf;
    getline(ifs, buf);
    cout << buf;
    while (getline(ifs, buf) && buf != "end of dir") {
        //去掉字符串最后的斜线
        buf.pop_back();
        pathVec.push_back(buf);
    }
    pathVec.pop_back();
    for (int i = 0; i < pathVec.size(); i++) {
        Folder file(pathVec[i]);
        file.getFolderValue();
        cout << file.folderPath << "文件夹信息如下" << endl <<
             "最晚文件" << file.latestPath << "最晚时间" << file.latestTime << endl <<
             "最早文件" << file.earliestPath << "最早时间" << file.earliestTime << endl <<
             "文件夹数量大小" << file.folderSize << endl
             << "文件夹所占子节：" << file.fileSize << endl << endl;
    }
    ifs.close();
}

void Detection::readMyFile(filesystem::path path, Tree *tree) {
    vector<string> actionBuf;
    vector<Action> actionVec;
    Action act;
    fstream ifs(path);
    string buf;
    getline(ifs, buf);
    getline(ifs, buf);
    while (getline(ifs, buf) && buf != "end of files") {
        actionBuf = split(buf, ',');
        actionBuf[0][0] = 'C';
        actionBuf[0][3] = 'W';
        act.path = actionBuf[0];
        act.action = actionBuf[1];
        act.time = actionBuf[2];
        act.size = actionBuf[3];
        actionVec.push_back(act);
    }
    ifs.close();
    for (int i = 0; i < actionVec.size(); i++) {
        if (actionVec[i].action == "M") {
            TreeNode *targetNode = tree->findFile(actionVec[i].path);
            if (targetNode != nullptr) {
                cout << "修改前节点的信息：" << endl;
                targetNode->data->showFileInfo();
                tree->modifyTreeNode(targetNode, atoi(actionVec[i].time.c_str()), atoi(actionVec[i].size.c_str()));
            }
        } else if (actionVec[i].action == "D") {
            TreeNode *targetNode = tree->findFile(actionVec[i].path);
            if (targetNode != nullptr) {
                if (targetNode->parent == targetNode->realParent) {
                    targetNode->parent->son = targetNode->bro;
                } else {
                    targetNode->parent->bro = targetNode->bro;
                }
                cout << targetNode->realParent->data->folderPath << "下的文件：" << targetNode->data->folderPath
                     << "删除成功" << endl;
                cout << targetNode->realParent->data->folderPath << "下的文件减少一个" << endl <<
                     "文件夹大小减少" << targetNode->data->fileSize << endl;
            }
        } else if (actionVec[i].action == "A") {
            string parentStr("");
            vector<string> pasrentPathVec = split(actionVec[i].path.string(), '\\');
            for (int i = 0; i < pasrentPathVec.size() - 1; i++) {
                parentStr += pasrentPathVec[i] + "\\";
            }
            cout << parentStr << "下文件增加一个" << endl <<
                 "文件夹大小增加" << actionVec[i].size << endl;;
        }
    }


//    actionVec.push_back(act);

//    while (getline(ifs, buf) && buf != "end of files") {
//        actionBuf = split(buf, ',');
//        act.path = actionBuf[0];
//        act.action = actionBuf[1];
//        act.time = actionBuf[2];
//        act.size = actionBuf[3];
//        actionVec.push_back(act);
//    }
}


void Detection::readMyDir(filesystem::path path, Tree *tree) {
    vector<string> actionBuf;
    vector<Action> actionVec;
    Action act;
    fstream ifs(path);
    string buf;
    getline(ifs, buf);
    while (getline(ifs, buf) && buf != "end of dirs") {
        actionBuf = split(buf, ',');
        actionBuf[0].pop_back();
        actionBuf[0][0] = 'C';
        actionBuf[0][3] = 'W';
        act.path = actionBuf[0];
        act.action = actionBuf[1];
        act.time = actionBuf[2];
        act.size = actionBuf[3];
        actionVec.push_back(act);
    }
    ifs.close();
    for (int i = 0; i < actionVec.size(); i++) {
        TreeNode *targetNode = tree->findFile(actionVec[i].path);
        if (targetNode != nullptr) {
            targetNode->data->getFolderValue();
            cout << targetNode->data->fileSize << endl;
            cout << targetNode->realParent->data->folderPath << "下的" << endl
                 << targetNode->data->folderPath << "被删除" << endl
                 << "文件夹数量减1" << endl
                 << "文件夹大小减少" << targetNode->data->fileSize;
        }
    }
}
