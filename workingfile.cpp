#include "workingfile.h"

WorkingFile::WorkingFile(const QString& path)
{
    //因为无法在调用构造函数的情况下验证数据合理性，所以这里先假设业务流程类已经先调用了路径验证函数
    m_filePath = path;
}

void WorkingFile::AddFile(const QString& path)
{
    new WorkingFile(path);
}

FileResult WorkingFile::Change(const QString& path)
{
    // 判断文件路径是否存在
    if (!isDirExist(path)) {
        return FileResult::NFOUND;
    }
    m_filePath = path;
    return FileResult::SUCCESS;
}

FileResult WorkingFile::OpenFile(const QString& path)
{
    // 判断文件路径是否存在
    if (!QFile::exists(path)) {
        return FileResult::NFOUND;
    }
    // 存在则直接打开
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    return FileResult::SUCCESS;
}

FileResult WorkingFile::OpenFile()
{
    // 判断文件路径是否存在
    if (!QFile::exists(this->m_filePath)) {
        return FileResult::NFOUND;
    }
    // 存在则直接打开
    QDesktopServices::openUrl(QUrl::fromLocalFile(this->m_filePath));
    return FileResult::SUCCESS;
}

FileResult WorkingFile::SaveToFolder(const QString& path)
{
    QString FolderPath = QDir::currentPath();
//    QString exePath = QCoreApplication::applicationFilePath();
    QString l = "/";
//    QString FolderPath;
//    = exePath.mid(exePath.indexOf(l) + 1);

//    qDebug() << QDir::currentPath();
    FolderPath += "/backup/";
//    qDebug() << FolderPath;
    QDate date = QDate::currentDate();
    QString YMD = date.toString("yyyy.MM.dd");
    FolderPath += YMD;

    if(!isDirExist(FolderPath)) {
        // 如果路径不存在，新建文件夹
        QDir dir(FolderPath);
        bool ismkdir = dir.mkdir(FolderPath);
        if(!ismkdir) {
            // 如果新建文件夹失败
            qDebug() <<FolderPath;
            return FileResult::FAILURE;
        }

        // 新建文件夹成功
    }
    // 从文件路径获取文件名并转换为保存路径

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
        return FileResult::AlreadyExist;
    }
    // 复制文件
    QFile::copy(path, FolderPath);
    return FileResult::SUCCESS;
}

QString WorkingFile::GetFilePath()
{
    return this->m_filePath;
}

void WorkingFile::slot_openFile(const QString& path)
{
    // 判断文件路径是否存在
    if (!QFile::exists(path)) {
        return;
    }
    // 存在则直接打开
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
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

//QString PathConver(const QString &path)
//{
//    if(!path.contains(l, Qt::CaseInsensitive)) {
//        return path;
//    }
//    return QDir::fromNativeSeparators(path);
//}
