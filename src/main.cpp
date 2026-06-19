#include "view/mainwindow.h"
#include "view/logindialog.h"
#include "model/dbmanager.h"
#include "controller/session.h"
#include <QApplication>
#include <QMessageBox>
#include <QSettings>
#include <QFile>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Поиск config.ini в папке с .exe файлом
    QString configPath = QApplication::applicationDirPath() + "/config.ini";

    if (!QFile::exists(configPath)) {
        //Если там нет, поиск в текущей рабочей директории (полезно при запуске из Qt Creator)
        if (!QFile::exists("config.ini")) {
            //Если файл не найден, вывод сообщения
            QMessageBox::critical(nullptr, "Ошибка конфигурации",
                                  "Не найден файл конфигурации config.ini!\n\n"
                                  "Пожалуйста, скопируйте файл 'config.ini.example.txt', "
                                  "переименуйте его в 'config.ini' и положите в папку с проектом или папку сборки.");
            return -1;
        }
    }

    QSettings settings(configPath, QSettings::IniFormat);

    QString host     = settings.value("Database/host", "localhost").toString();
    int port         = settings.value("Database/port", 5432).toInt();
    QString dbname   = settings.value("Database/dbname", "elective-courses").toString();
    QString user     = settings.value("Database/user", "postgres").toString();
    QString password = settings.value("Database/password", "").toString();

    if (!DBManager::instance().connectToDatabase(host, port, dbname, user, password)) {
        QMessageBox::critical(nullptr, "Ошибка БД",
                              QString("Не удалось подключиться к базе данных.\n"
                                      "Проверьте настройки в файле:\n%1\n\n")
                                  .arg(QDir(configPath).absolutePath()));
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
