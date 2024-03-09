#include "Tree.h"

/*
 * 辅助函数
 */


/*
 * 获取传入路径的深度
 */
int getDepth(filesystem::path path) {
    int depth = 0;
    string pathStr = path.string();
    char targetChar = '\\';
    depth = count(pathStr.begin(), pathStr.end(), targetChar);
    return depth;
}

/*
 * TreeNode类下的方法
 */
TreeNode::TreeNode(Folder *data) {
    this->data = data;
}

void TreeNode::addSon(TreeNode *sonNode) {
    this->son = sonNode;
}

void TreeNode::addBro(TreeNode *broNode) {
    this->bro = broNode;
}


/*
 * Tree类下的方法
 */

Tree::Tree(TreeNode *rootNode) {
    this->root = rootNode;
}


void Tree::inintTree() {
    stack<TreeNode *> treeNodeStack;
    treeNodeStack.push(this->root);
    while (!treeNodeStack.empty()) {
        //弹出栈顶的目录
        TreeNode *currentNode = treeNodeStack.top();
        treeNodeStack.pop();
        filesystem::path currentNodeLongestPath(currentNode->data->folderPath);
        //当前节点的文件夹数量
        int currentNodeFolderCount = 0;
        int currentNodeFileCount = 0;
        int currentNodeLongestPathLength = 0;
        int currentNodeMaxDepth = 0;
        //将路径下的第一个元素设置为子节点
        auto i = filesystem::directory_iterator(currentNode->data->folderPath);
        //选出当前节点下的最长路径
        if (i->path().string().length() > currentNodeLongestPathLength) {
            currentNodeLongestPath = i->path();
            currentNodeLongestPathLength = i->path().string().length();
        }
        //选出当前节点下的最深节点
        if (getDepth(i->path()) > currentNodeMaxDepth) {
            currentNodeMaxDepth = getDepth(i->path());
        }
        Folder *sonFolder = new Folder(*i);
        TreeNode *sonNode = new TreeNode(sonFolder);
        //如果是目录且不为空，则压入栈中，以后来遍历
        try {
            if (filesystem::is_directory(*i)) {
                currentNodeFolderCount++;
                if (!filesystem::is_empty(*i)) {
                    treeNodeStack.push(sonNode);
                } else {
                    sonNode->data->fileCount = 0;
                    sonNode->data->folderSize = 0;
                }
            } else {
                currentNodeFileCount++;
                sonNode->data->folderSize = 0;
                sonNode->data->fileCount = 0;
                sonNode->data->fileSize = filesystem::file_size(sonNode->data->folderPath);
                struct stat st;
                stat(i->path().string().c_str(), &st);
                sonNode->data->ctime = st.st_ctime;
            }
        }
        catch (filesystem_error) {

        }
        currentNode->addSon(sonNode);
        sonNode->parent = currentNode;
        sonNode->realParent = currentNode;
        i++;
        //遍历剩余元素，并将一次设置为上一个元素的兄弟元素
        auto p = sonNode;
        while (i != directory_iterator()) {
            //将数据存储在堆区
            Folder *broFolder = new Folder(*i);
            TreeNode *broNode = new TreeNode(broFolder);
            if (i->path().string().length() > currentNodeLongestPathLength) {
                currentNodeLongestPath = i->path();
                currentNodeLongestPathLength = i->path().string().length();
            }

            if (getDepth(i->path()) > currentNodeMaxDepth) {
                currentNodeMaxDepth = getDepth(i->path());
            }
            //如果是目录且不为空，则压入栈中，以后来遍历
            try {
                if (filesystem::is_directory(*i)) {
                    currentNodeFolderCount++;
                    if (!filesystem::is_empty(*i)) {
                        treeNodeStack.push(broNode);
                    } else {
                        broNode->data->folderSize = 0;
                        broNode->data->fileCount = 0;
                        broNode->data->folderCount = 0;
                    }
                } else {
                    currentNodeFileCount++;
                    broNode->data->fileCount = 0;
                    broNode->data->folderSize = 0;
                    broNode->data->folderCount = 0;
                    broNode->data->folderSize = filesystem::file_size(broNode->data->folderPath);
                    struct stat st;
                    stat(i->path().string().c_str(), &st);
                    broNode->data->ctime = st.st_ctime;
                }
            }
            catch (filesystem::filesystem_error) {

            }
            p->addBro(broNode);
            broNode->parent = p;
            broNode->realParent = currentNode;
            p = p->bro;
            i++;
        }
        currentNode->data->fileCount = currentNodeFileCount;
        currentNode->data->folderCount = currentNodeFolderCount;
        currentNode->data->folderSize = currentNodeFolderCount + currentNodeFileCount;
        currentNode->data->longestPath = currentNodeLongestPath;
        currentNode->data->longestPathLength = currentNodeLongestPathLength;
        currentNode->data->folderMaxDepth = currentNodeMaxDepth;

        //更新包含该文件夹的所有的文件夹的信息
        p = currentNode;
        while (p->realParent != nullptr) {
            p->realParent->data->folderCount += currentNodeFolderCount;
            p->realParent->data->fileCount += currentNodeFileCount;
            p->realParent->data->folderSize += currentNodeFileCount + currentNodeFolderCount;
            if (p->realParent->data->longestPathLength < currentNodeLongestPathLength) {
                p->realParent->data->longestPathLength = currentNodeLongestPathLength;
                p->realParent->data->longestPath = currentNodeLongestPath;
            }
            if (p->realParent->data->folderMaxDepth < currentNodeMaxDepth) {
                p->realParent->data->folderMaxDepth = currentNodeMaxDepth;
            }
            p = p->realParent;
        }

    }
    cout << "树构造成功" << endl;
}

void Tree::getTreeDepth() {
    int depth = 0;
    //创建一个队列
    queue<TreeNode *> nodeQueue;
    //将根目录压入队列中
    nodeQueue.push(this->root);
    //循环将树每一层的节点压入队列中，每一次循环深度加一
    while (!nodeQueue.empty()) {
        int queneSize = nodeQueue.size();
        for (int i = 0; i < queneSize; i++) {
            TreeNode *currentNode = nodeQueue.front();
            nodeQueue.pop();
            //生成改节点的sql语句，并插入txt文件中
            if (currentNode->son != nullptr) {
                nodeQueue.push(currentNode->son);
            }
            if (currentNode->bro != nullptr) {
                nodeQueue.push(currentNode->bro);
            }
        }
        depth++;

    }
    this->treeDepth = depth;
}

void Tree::createSqlFile() {
    int size = 0;
    queue<TreeNode *> nodeQueue;
    //将根目录压入队列中
    nodeQueue.push(this->root);
    //循环将树每一层的节点压入队列中，每一次循环深度加一
    while (!nodeQueue.empty()) {
        int queneSize = nodeQueue.size();
        for (int i = 0; i < queneSize; i++) {
            TreeNode *currentNode = nodeQueue.front();
            nodeQueue.pop();
            //生成改节点的sql语句，并插入txt文件中
            currentNode->data->addSqlFile();
            size++;
            if (size == 10000) {
                return;
            }
            if (currentNode->son != nullptr) {
                nodeQueue.push(currentNode->son);
            }
            if (currentNode->bro != nullptr) {
                nodeQueue.push(currentNode->bro);
            }
        }
    }
}

TreeNode *Tree::findFile(filesystem::path path) {
    queue<TreeNode *> nodeQueue;
    nodeQueue.push(this->root);
    while (!nodeQueue.empty()) {
        int queueSize = nodeQueue.size();
        for (int i = 0; i < queueSize; i++) {
            TreeNode *currentNode = nodeQueue.front();
            nodeQueue.pop();
            if (currentNode->data->folderPath == path) {
                return currentNode;
            }
            if (currentNode->son != nullptr) {
                nodeQueue.push(currentNode->son);
            }
            if (currentNode->bro != nullptr) {
                nodeQueue.push(currentNode->bro);
            }
        }
    }
    return nullptr;
}

void Tree::modifyTreeNode(TreeNode *targetNode, time_t targetTime, int targetSize) {
    targetNode->data->ctime = targetTime;
    targetNode->data->folderSize = targetSize;
    cout << "修改后的节点信息：" << endl;
    targetNode->data->showFileInfo();
}




