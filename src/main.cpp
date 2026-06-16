#include "view/mainwindow.h"
#include "view/logindialog.h"
#include "model/dbmanager.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Инициализация подключения к БД
    if (!DBManager::instance().connectToDatabase("localhost", 5432, "elective-courses", "postgres", "dLWsK8g_R1@01$I")) {
        QMessageBox::critical(nullptr, "Ошибка БД", "Не удалось подключиться к базе данных.");
        return -1;
    }

    //Окно авторизации
    LoginDialog loginDlg;

    //Бесконечный цикл логина для реализации переключения между авторизацией и регистрацией
    while (true) {
        LoginDialog loginDlg;

        if (loginDlg.exec() == QDialog::Accepted) {
            MainWindow w(loginDlg.getLoggedInUser());
            w.show();
            return a.exec();
        }
        else {
            break;
        }
    }

    return 0;
}
