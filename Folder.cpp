#include "Folder.h"

Folder::Folder(filesystem::path path) {
    this->folderPath = path;
}


void Folder::getFolderValue() {
    int fileSize = 0;
    int folderSize = 0;
    time_t earliestTime = MAX_TIME_T;
    time_t latestTime = MIN_TIME_T;
    filesystem::path earliestFile(this->folderPath);
    filesystem::path latestFile(this->folderPath);
    for (auto i = filesystem::directory_iterator(this->folderPath); i != filesystem::directory_iterator(); i++) {
        struct stat st;
        stat(i->path().string().c_str(), &st);
        if (st.st_ctime > latestTime) {
            latestTime = st.st_ctime;
            latestFile = i->path();
        }
        if (st.st_ctime < earliestTime) {
            earliestTime = st.st_ctime;
            earliestFile = i->path();
        }
        folderSize++;
        fileSize += st.st_size;
    }
    this->folderSize = folderSize;
    this->latestTime = latestTime;
    this->earliestTime = earliestTime;
    this->latestPath = latestFile;
    this->earliestPath = earliestFile;
    this->fileSize = fileSize;
}


void Folder::showFolderInfo() {
    cout << "路径：" << this->folderPath << "信息如下" << endl
         << "此文件夹的大小为：" << this->folderSize << endl
         << "此路径下包含的子文件夹数量：" << this->folderCount << endl
         << "此路径下包含的文件数量：" << this->fileCount << endl
         << "此路径下的最长文件名是：" << this->longestPath << " 其长度为：" << this->longestPathLength << endl
         << "此文件夹中的最大深度为：" << this->folderMaxDepth << endl;
}

void Folder::showFileInfo() {
    cout << "路径：" << this->folderPath << "信息如下" << endl
         << "此文件的大小为：" << this->folderSize << endl
         << "此文件的创建时间为：" << this->ctime << endl;
}


void Folder::addSqlFile() {
    ofstream ofs;
    //以追加模式写入对应的文件
    ofs.open("./sqlFile.sql", ofstream::app);
    ofs << "insert into " << TABLE_NAME << " " << "values" << " " <<
        "(" << "'" <<
        this->folderPath.string().c_str() << "'" << "," <<
        this->folderCount << "," <<
        this->fileCount << "," <<
        this->folderSize << "," << "'" <<
        this->longestPath.string().c_str() << "'" << "," <<
        this->longestPathLength << "," <<
        this->folderMaxDepth <<
        ")" << ";" <<
        endl;
    ofs.close();
}


