#ifndef PRIMARYMENU_H
#define PRIMARYMENU_H

#include <QMenu>
#include <QAction>
#include <QMainWindow>
class PrimaryMenu : public QMenu
{
public:
//    PrimaryMenu();
    PrimaryMenu(QMainWindow* Window);
//    QAction* Action[4];
};

#endif // PRIMARYMENU_H
