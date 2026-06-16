#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "controller/authcontroller.h"
#include <QMessageBox>

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Регистрация нового пользователя");
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

bool RegisterDialog::validateFields(QString &errorMsg)                      //Валидация всех полей
{
    QString login = ui->leLogin->text().trimmed();
    QString email = ui->leEmail->text().trimmed();
    QString password = ui->lePass->text();
    QString confirmPassword = ui->leConfirmPass->text();

    if (login.isEmpty() || email.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        errorMsg = "Все поля обязательны для заполнения!";
        return false;
    }

    if (!email.contains('@')) {                                             //Минимальная проверка Email
        errorMsg = "Некорректный формат электронной почты (отсутствует символ @)!";
        return false;
    }

    if (password != confirmPassword) {
        errorMsg = "Пароли не совпадают!";
        return false;
    }

    if (!AuthController::validatePassword(password)) {
        errorMsg = "Пароль не соответствует требованиям безопасности!\n"
                   "Он должен быть не менее 8 символов, содержать цифру, заглавную букву и спецсимвол.";
        return false;
    }

    return true;
}

void RegisterDialog::on_btnRegister_clicked()
{
    QString errorMsg;
    if (!validateFields(errorMsg)) {
        QMessageBox::warning(this, "Неверно заполнены поля", errorMsg);
        return;
    }

    QString login = ui->leLogin->text().trimmed();
    QString email = ui->leEmail->text().trimmed();
    QString password = ui->lePass->text();

    int defaultRoleId = 2;                                          //По умолчанию регистрация с ролью "Методист"

    if (AuthController::registerUser(login, email, password, defaultRoleId)) {
        QMessageBox::information(this, "Успешно", "Регистрация завершена!");
        accept();
    } else {
        QMessageBox::critical(this, "Ошибка БД", "Регистрация неудачна. Этот логин или email уже заняты!");
    }
}

void RegisterDialog::on_btnCancel_clicked()
{
    reject();
}
