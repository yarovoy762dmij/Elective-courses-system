#include "logindialog.h"
#include "ui_logindialog.h"
#include "controller/authcontroller.h"
#include "model/user.h"
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_btnLogin_clicked()
{
    QString loginStr = ui->leLogin->text().trimmed();
    QString passwordStr = ui->lePass->text();

    if (loginStr.isEmpty() || passwordStr.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Заполните все поля!");
        return;
    }

    if (AuthController::login(loginStr, passwordStr, m_currentUser)) {     //Логика проверки из контроллера
        accept();                                                          //Если вход успешный - "Принято"

    } else {
        ui->lblStatus->setText("Неверный логин или пароль!");
    }
}

