#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(const User& user, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_currentUser(user)
{
    ui->setupUi(this);


    this->setWindowTitle("Управление факультативами | Пользователь: "
                         + m_currentUser.login
                         + " (" + m_currentUser.roleName + ")");
}

MainWindow::~MainWindow()
{
    delete ui;
}
