#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "model/user.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const User& user, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    User m_currentUser;
};
#endif // MAINWINDOW_H
