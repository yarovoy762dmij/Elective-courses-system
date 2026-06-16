#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "model/user.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    User getLoggedInUser() const { return m_currentUser; }

private slots:
    void on_btnLogin_clicked();
    void on_btnReg_clicked();

private:
    Ui::LoginDialog *ui;
    User m_currentUser;
};

#endif // LOGINDIALOG_H
