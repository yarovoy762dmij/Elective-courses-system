#include "view/mainwindow.h"
#include "view/logindialog.h"
#include "model/dbmanager.h"
#include "controller/session.h"
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

    bool logoutRequested = false;

    do {                                //Изменение реализации цикла для корректной работы оконного приложения
        logoutRequested = false;
        LoginDialog loginDlg;

        //Пользователь успешно авторизовался или зарегистрировался и вошел
        if (loginDlg.exec() == QDialog::Accepted) {
            Session::instance().setCurrentUser(loginDlg.getLoggedInUser());

            MainWindow w(loginDlg.getLoggedInUser());
            w.show();

            a.exec();

            if (w.isLogoutRequested()) {
                Session::instance().logout();
                logoutRequested = true;         //Флаг для нового цикла авторизации, если пользователь разлогинился
            }
        }
        else break;

    } while (logoutRequested);

    return 0;
}
