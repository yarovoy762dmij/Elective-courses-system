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

    bool isLogoutRequested() const { return m_logoutRequested; }

private slots:
    void on_btnSaveProfile_clicked();
    void on_btnLogout_clicked();

private:
    Ui::MainWindow *ui;
    User m_currentUser;
    bool m_logoutRequested = false;
};
#endif // MAINWINDOW_H
