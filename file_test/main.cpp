#include <QCoreApplication>
#include <QFileInfo>
#include <QStringList>
#include <QDateTime>
#include <QDebug>
#include <QDir>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QDir tempDir;
    QString current_path = tempDir.currentPath();
//    qDebug() << tempDir.currentPath();

    QString l = "/";
    int last_index = current_path.lastIndexOf(l);
//    qDebug() << last_index;

    QString user_file_path = current_path.mid(0, last_index + 1) + "file_test/user.txt";
    qDebug() << user_file_path;

    QFile file(user_file_path);
    // 以只读方式打开
    if (!file.open(QIODevice::ReadOnly  | QIODevice::Text))
        qDebug() << file.errorString();
    else
        qDebug() << "文件内容：" << endl << file.readAll();

    file.close();

    return a.exec();
}

