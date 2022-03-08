#ifndef PRIMARYMENU_H
#define PRIMARYMENU_H

#include <QMenu>
#include <QAction>

class PrimaryMenu : public QMenu
{
public:
    PrimaryMenu();
    QAction* Action[4];
};

#endif // PRIMARYMENU_H
