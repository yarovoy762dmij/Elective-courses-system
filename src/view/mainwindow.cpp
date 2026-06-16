#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "controller/profilecontroller.h"
#include <QMessageBox>

MainWindow::MainWindow(const User& user, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_currentUser(user)
{
    ui->setupUi(this);

    this->setWindowTitle("Управление факультативами | " + m_currentUser.login + " (" + m_currentUser.roleName + ")");

    //Заполнение полей профиля данными авторизованного пользователя
    ui->leProfileLogin->setText(m_currentUser.login);
    ui->leProfileEmail->setText(m_currentUser.email);
    ui->lblProfileRole->setText("Ваша роль в системе: <b>" + m_currentUser.roleName + "</b>");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnSaveProfile_clicked()
{
    QString newLogin = ui->leProfileLogin->text().trimmed();
    QString newEmail = ui->leProfileEmail->text().trimmed();

    QString oldPass = ui->leOldPass->text();
    QString newPass = ui->leNewPass->text();
    QString confirmPass = ui->leConfirmNewPass->text();

    if (newLogin.isEmpty() || newEmail.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Логин и Email не могут быть пустыми!");
        return;
    }

    bool profileUpdated = false;

    //Пользователь изменяет логин или почту
    if (newLogin != m_currentUser.login || newEmail != m_currentUser.email) {
        if (ProfileController::updateBasicInfo(m_currentUser.id, newLogin, newEmail)) {
            m_currentUser.login = newLogin;
            m_currentUser.email = newEmail;
            this->setWindowTitle("Управление факультативами | " + m_currentUser.login + " (" + m_currentUser.roleName + ")");
            profileUpdated = true;
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось обновить данные. Возможно, этот логин или email уже заняты!");
            return;
        }
    }

    //Пользователь заполняет поля для смены пароля
    if (!oldPass.isEmpty() || !newPass.isEmpty() || !confirmPass.isEmpty()) {
        if (oldPass.isEmpty() || newPass.isEmpty() || confirmPass.isEmpty()) {
            QMessageBox::warning(this, "Предупреждение", "Для смены пароля необходимо заполнить все три поля паролей!");
            return;
        }

        if (newPass != confirmPass) {
            QMessageBox::warning(this, "Ошибка", "Новые пароли не совпадают!");
            return;
        }

        QString errorMsg;
        if (ProfileController::changePassword(m_currentUser.id, oldPass, newPass, errorMsg)) {
            QMessageBox::information(this, "Успех", "Пароль успешно изменен!");
            ui->leOldPass->clear();
            ui->leNewPass->clear();
            ui->leConfirmNewPass->clear();
            profileUpdated = false;
        } else {
            QMessageBox::critical(this, "Ошибка смены пароля", errorMsg);
            return;
        }
    }

    if (profileUpdated) {
        QMessageBox::information(this, "Успех", "Регистрационные данные успешно обновлены!");
    }
}

void MainWindow::on_btnLogout_clicked()
{
    m_logoutRequested = true;
    this->close();
    QApplication::exit(0);
}
