#ifndef NOTELISTITEM_H
#define NOTELISTITEM_H

//#include <QListWidgetItem>
#include <QAction>
#include <QMenu>
//#include <QObject>
#include <QObject>
#include <QWidget>
#include <QMainWindow>
#include <QGroupBox>
#include <QLabel>
#include <QKeyEvent>
#include <QDebug>
#include <QMouseEvent>
#include <QMessageBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QList>
#include <QDragEnterEvent>
#include <QFile>
#include <QMimeData>
#include "description.h"

class NoteListItem : public QListWidgetItem{
public:
    NoteListItem(QString);
    QAction* m_act[3];
    QMenu* m_menu;
private:
    Description* m_note;
//public slots:
//    void OnClickedNoteMenu();
};

#endif // NOTELISTITEM_H
