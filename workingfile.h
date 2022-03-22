#ifndef WORKINGFILE_H
#define WORKINGFILE_H

#include <QString>
#include <QDebug>
#include <memory>
#include <vector>
#include <QDesktopServices>
#include <QCoreApplication>
#include <QUrl>
#include <QDir>
#include <QDate>
#include <QTime>
#include <QFile>

using namespace std;

// 判断文件操作是否合理
enum class FileResult
{
    SUCCESS,
    NFOUND,
    FAILURE,
    AlreadyExist
};

class WorkingFile
{
public:
    WorkingFile(const QString&);              // 构造函数
    void AddFile(const QString&);             // 新建文件链接
    FileResult Change(const QString&);        // 修改链接
    FileResult OpenFile(const QString&);      // 使用默认打开方式打开文件链接
    FileResult OpenFile();                    // 重载，打开私有成员存储的链接
    FileResult SaveToFolder(const QString&);  // 备份文件链接

    WorkingFile& operator=(const WorkingFile& other)
    {
        if(&other == this) {
            return *this;
        }
        this->m_filePath = other.m_filePath;
        return *this;
    }
    QString GetFilePath();

private:
    // 这里需要管理员注意：windows默认的路径以\作分隔符，而qt则需要/分隔，需要确认是否做过转换。
    QString m_filePath;
};

bool isDirExist(const QString&);    // 检测路径是否存在
QString GetCurrentPath();           // 获取当前文件夹路径
//QString PathConver(const QString&); // 将路径的\转换为/，并不知道怎么实现

#endif // WORKINGFILE_H
