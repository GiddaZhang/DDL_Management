#include "workingfile.h"

vector<shared_ptr<WorkingFile>> WorkingFile::m_AllFile{};//静态存储的文件列表

WorkingFile::WorkingFile(const QString& path):FilePath(path)
{
    //加入到文件列表末尾
    m_filePath = path;
    m_AllFile.push_back(shared_ptr<WorkingFile>(this));
}

void WorkingFile::AddFile(const QString& path)
{
    new WorkingFile(path);
}

Result WorkingFile::DeleteFile(const QString& path)
{
    auto Finder = [&path](shared_ptr<WorkingFile> ptr)->bool{return (ptr->m_filePath == path);};
    auto it = find_if(m_AllFile.begin(), m_AllFile.end(), Finder);

    // 判断文件路径是否存在
    if (it == m_AllFile.end()) {
        return Result::NFOUND;
    }
    //存在则直接erase
    else {
        it = m_AllFile.erase(it);
        return Result::SUCCESS;
    }
}

Result WorkingFile::OpenFile(const QString& path)
{
    auto Finder = [&path](shared_ptr<WorkingFile> ptr)->bool{return (ptr->m_filePath == path);};
    auto it = find_if(m_AllFile.begin(), m_AllFile.end(), Finder);

    // 判断文件路径是否存在
    if (it == m_AllFile.end()) {
        return Result::NFOUND;
    }
    // 存在则直接打开
    else {
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
        return Result::SUCCESS;
    }
}

Result WorkingFile::SaveToFolder(const QString& path)
{
    QString FolderPath = GetCurrentPath();
    FolderPath += "/backup/";
    QDate date = QDate::currentDate();
    QString YMD = date.toString("yyyy.MM.dd");
    FolderPath += YMD;

    if(!isDirExist(FolderPath)) {
        // 如果路径不存在，新建文件夹
        QDir dir(FolderPath);
        bool ismkdir = dir.mkdir(FolderPath);
        if(!ismkdir) {
            // 如果新建文件夹失败
            return Result::FAILURE;
        }
        // 新建文件夹成功
    }

    // 从文件路径获取文件名并转换为保存路径
    QString l = "/";
    QString fileName = path.mid(path.lastIndexOf(l) + 1);
    FolderPath += "/" + fileName;

    QTime cur_time = QTime::currentTime();
    l = ".";
    FolderPath.insert(FolderPath.lastIndexOf(l),
                      QString("_") +
                      QString::number(cur_time.hour())
                      + QString(".")
                      + QString::number(cur_time.minute()));


    if(QFile::exists(FolderPath)) {
        // 如果路径存在，表面已经有备份文件，报错
        return Result::AlreadyExist;
    }
    // 复制文件
    QFile::copy(path, FolderPath);
    return Result::SUCCESS;
}

bool isDirExist(const QString& fullPath)
{
    QDir dir(fullPath);
    if(dir.exists()) {
        return true;
    }
    return false;
}

QString GetCurrentPath()
{
    QDir tempDir;
    QString current_path = tempDir.currentPath();

    QString l = "/";
    int last_index = current_path.lastIndexOf(l);

    QString data_file_path = current_path.mid(0, last_index + 1) + "DDL_Management";
    return data_file_path;
}
