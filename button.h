#ifndef BUTTON_H
#define BUTTON_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMainWindow>

class button : public QPushButton
{
    Q_OBJECT
public:
    explicit button(QMainWindow *parent = nullptr);
    button(QLabel *parent = nullptr);

signals:

};

#endif // BUTTON_H
