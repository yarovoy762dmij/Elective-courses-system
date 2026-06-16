#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "controller/profilecontroller.h"
#include "model/elective.h"
#include "model/query/semestercourserow.h"
#include "model/professor.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QHeaderView>

MainWindow::MainWindow(const User& user, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_currentUser(user)
{
    ui->setupUi(this);

    this->setWindowTitle("Управление факультативами | " + m_currentUser.login + " (" + m_currentUser.roleName + ")");

    //1. Заполнение полей профиля данными авторизованного пользователя
    ui->leProfileLogin->setText(m_currentUser.login);
    ui->leProfileEmail->setText(m_currentUser.email);
    ui->lblProfileRole->setText("Ваша роль в системе: <b>" + m_currentUser.roleName + "</b>");

    //2. Настройка таблицы Факультативов
    m_electivesModel = new QStandardItemModel(this);
    m_electivesModel->setHorizontalHeaderLabels({"ID", "Название факультатива"});
    ui->tvElectives->setModel(m_electivesModel);
    ui->tvElectives->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tvElectives->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tvElectives->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tvElectives->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tvElectives->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    //3. Настройка таблицы Семестровых курсов
    m_coursesModel = new QStandardItemModel(this);
    m_coursesModel->setHorizontalHeaderLabels({"ID", "Учебный год", "Семестр", "Лекции", "Практики", "Лабы", "Преподаватель"});
    ui->tvCourses->setModel(m_coursesModel);
    ui->tvCourses->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tvCourses->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tvCourses->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tvCourses->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tvCourses->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);

    refreshElectivesTable();
    setupRoleAccess();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupRoleAccess()
{
    QString role = m_currentUser.roleName;

    //1. Доступ к управлению Факультативами и Семестровыми курсами
    //CRUD разрешен Суперпользователю и Методисту. Преподаватель только R.
    bool canEditElectives = (role == "Суперпользователь" || role == "Методист");

    ui->btnElectiveAdd->setEnabled(canEditElectives);
    ui->btnElectiveEdit->setEnabled(canEditElectives);
    ui->btnElectiveDelete->setEnabled(canEditElectives);

    ui->btnCourseAdd->setEnabled(canEditElectives);
    ui->btnCourseEdit->setEnabled(canEditElectives);
    ui->btnCourseDelete->setEnabled(canEditElectives);
}


// ЛОГИКА ПРОФИЛЯ
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

void MainWindow::refreshElectivesTable()
{
    m_electivesModel->removeRows(0, m_electivesModel->rowCount());
    m_coursesModel->removeRows(0, m_coursesModel->rowCount());

    QList<Elective> list = Elective::getAll();
    for (const auto& elective : list) {
        QList<QStandardItem*> row;
        row.append(new QStandardItem(QString::number(elective.id)));
        row.append(new QStandardItem(elective.name));
        m_electivesModel->appendRow(row);
    }
}

void MainWindow::refreshCoursesTable(int electiveId)
{
    m_coursesModel->removeRows(0, m_coursesModel->rowCount());

    QList<SemesterCourseRow> list = SemesterCourseRow::getCoursesForElective(electiveId);
    for (const auto& course : list) {
        QList<QStandardItem*> row;
        row.append(new QStandardItem(QString::number(course.courseId)));
        row.append(new QStandardItem(QString::number(course.academicYear)));
        row.append(new QStandardItem(QString::number(course.semesterNumber)));
        row.append(new QStandardItem(QString::number(course.lectureHours)));
        row.append(new QStandardItem(QString::number(course.practiceHours)));
        row.append(new QStandardItem(QString::number(course.labHours)));
        row.append(new QStandardItem(course.professorName));
        m_coursesModel->appendRow(row);
    }
}

int MainWindow::getSelectedElectiveId() const
{
    QModelIndexList selected = ui->tvElectives->selectionModel()->selectedRows();
    if (selected.isEmpty()) return -1;
    return m_electivesModel->item(selected.first().row(), 0)->text().toInt();
}

int MainWindow::getSelectedCourseId() const
{
    QModelIndexList selected = ui->tvCourses->selectionModel()->selectedRows();
    if (selected.isEmpty()) return -1;
    return m_coursesModel->item(selected.first().row(), 0)->text().toInt();
}

void MainWindow::on_tvElectives_clicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    int electiveId = getSelectedElectiveId();
    if (electiveId != -1) {
        refreshCoursesTable(electiveId);
    }
}

//CRUD ФАКУЛЬТАТИВОВ
void MainWindow::on_btnElectiveAdd_clicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Новый факультатив", "Введите название:", QLineEdit::Normal, "", &ok);
    if (ok && !name.trimmed().isEmpty()) {
        if (Elective::insert(name.trimmed())) {
            refreshElectivesTable();
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось добавить факультатив.");
        }
    }
}

void MainWindow::on_btnElectiveEdit_clicked()
{
    int electiveId = getSelectedElectiveId();
    if (electiveId == -1) {
        QMessageBox::warning(this, "Внимание", "Выберите факультатив для редактирования!");
        return;
    }

    QModelIndexList selected = ui->tvElectives->selectionModel()->selectedRows();
    QString oldName = m_electivesModel->item(selected.first().row(), 1)->text();

    bool ok;
    QString newName = QInputDialog::getText(this, "Редактирование", "Измените название:", QLineEdit::Normal, oldName, &ok);
    if (ok && !newName.trimmed().isEmpty() && newName != oldName) {
        Elective e;
        e.id = electiveId;
        e.name = newName.trimmed();
        if (e.update()) {
            refreshElectivesTable();
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось обновить название.");
        }
    }
}

void MainWindow::on_btnElectiveDelete_clicked()
{
    int electiveId = getSelectedElectiveId();
    if (electiveId == -1) {
        QMessageBox::warning(this, "Внимание", "Выберите факультатив для удаления!");
        return;
    }

    auto reply = QMessageBox::question(this, "Удаление", "Удалить факультатив и все его семестры нагрузок?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        if (Elective::remove(electiveId)) {
            refreshElectivesTable();
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось удалить факультатив.");
        }
    }
}

//CRUD СЕМЕСТРОВЫХ КУРСОВ
void MainWindow::on_btnCourseAdd_clicked()
{
    int electiveId = getSelectedElectiveId();
    if (electiveId == -1) {
        QMessageBox::warning(this, "Внимание", "Сначала выберите факультатив в левой таблице!");
        return;
    }

    bool ok;
    int year = QInputDialog::getInt(this, "Новая нагрузка", "Учебный год (например, 2024):", 2024, 2000, 2100, 1, &ok);
    if (!ok) return;

    int semester = QInputDialog::getInt(this, "Новая нагрузка", "Номер семестра (1 - Осенний, 2 - Весенний):", 1, 1, 2, 1, &ok);
    if (!ok) return;

    int lec = QInputDialog::getInt(this, "Часы", "Часы лекций:", 0, 0, 200, 1, &ok);
    if (!ok) return;

    int prac = QInputDialog::getInt(this, "Часы", "Часы практик:", 0, 0, 200, 1, &ok);
    if (!ok) return;

    int lab = QInputDialog::getInt(this, "Часы", "Часы лабораторных:", 0, 0, 200, 1, &ok);
    if (!ok) return;

    int professorId = getProfessorIdFromDialog();
    if (professorId == -1) {
        return;
    }

    if (SemesterCourseRow::insertCourse(electiveId, year, semester, lec, prac, lab, professorId)) {
        refreshCoursesTable(electiveId);
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось добавить семестровый курс.");
    }
}

void MainWindow::on_btnCourseEdit_clicked()
{
    int electiveId = getSelectedElectiveId();
    int courseId = getSelectedCourseId();
    if (courseId == -1) {
        QMessageBox::warning(this, "Внимание", "Выберите строку нагрузки справа!");
        return;
    }

    QModelIndexList selected = ui->tvCourses->selectionModel()->selectedRows();
    int rowNum = selected.first().row();
    int oldYear = m_coursesModel->item(rowNum, 1)->text().toInt();
    int oldSem = m_coursesModel->item(rowNum, 2)->text().toInt();
    int oldLec = m_coursesModel->item(rowNum, 3)->text().toInt();
    int oldPrac = m_coursesModel->item(rowNum, 4)->text().toInt();
    int oldLab = m_coursesModel->item(rowNum, 5)->text().toInt();

    int currentProfessorId = SemesterCourseRow::getProfessorIdForCourse(courseId);
    if (currentProfessorId == -1) {
        QMessageBox::warning(this, "Ошибка", "Не удалось определить преподавателя для этого курса.");
        return;
    }

    bool ok;

    int year = QInputDialog::getInt(this, "Изменение", "Учебный год:", oldYear, 2000, 2100, 1, &ok);
    if (!ok) return;

    int semester = QInputDialog::getInt(this, "Изменение", "Семестр (1 или 2):", oldSem, 1, 2, 1, &ok);
    if (!ok) return;

    int lec = QInputDialog::getInt(this, "Изменение часов", "Новые часы лекций:", oldLec, 0, 200, 1, &ok);
    if (!ok) return;

    int prac = QInputDialog::getInt(this, "Изменение часов", "Новые часы практик:", oldPrac, 0, 200, 1, &ok);
    if (!ok) return;

    int lab = QInputDialog::getInt(this, "Изменение часов", "Новые часы лабораторных:", oldLab, 0, 200, 1, &ok);
    if (!ok) return;

    int professorId = getProfessorIdFromDialog(currentProfessorId);
    if (professorId == -1) {
        return;
    }

    if (SemesterCourseRow::updateCourse(courseId, year, semester, lec, prac, lab, professorId)) {
        refreshCoursesTable(electiveId);
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось обновить данные.");
    }
}

void MainWindow::on_btnCourseDelete_clicked()
{
    int electiveId = getSelectedElectiveId();
    int courseId = getSelectedCourseId();
    if (courseId == -1) {
        QMessageBox::warning(this, "Внимание", "Выберите строку семестра для удаления!");
        return;
    }

    auto reply = QMessageBox::question(this,
                                       "Подтверждение удаления",
                                       "Вы уверены, что хотите удалить выбранный семестровый курс?\n"
                                       "Все связанные данные будут удалены.",
                                       QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
        return;
    }

    //Вызов удаления
    if (SemesterCourseRow::removeCourse(courseId)) {
        refreshCoursesTable(electiveId);
        QMessageBox::information(this, "Успех", "Семестровый курс успешно удалён.");
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось удалить семестровый курс.\n"
                                              "Возможно, на него есть ссылки в других таблицах (например, ведомости).");
    }
}

int MainWindow::getProfessorIdFromDialog(int currentProfessorId)
{
    //Список всех преподавателей из БД
    QList<Professor> profs = Professor::getAll();

    //Список для отображения в диалоге: "Фамилия Имя Отчество"
    QStringList items;
    QMap<QString, int> map;
    for (const auto& p : profs) {
        QString display = p.lastName + " " + p.firstName + " " + p.middleName;
        map[display] = p.id;
        items << display;
    }

    //Если передан currentProfessorId, находим его индекс в списке
    int defaultIdx = -1;
    if (currentProfessorId != -1) {
        for (int i = 0; i < profs.size(); ++i) {
            if (profs[i].id == currentProfessorId) {
                defaultIdx = i;
                break;
            }
        }
    }

    //Диалог выбора
    bool ok;
    QString selected = QInputDialog::getItem(
        this,
        "Выбор преподавателя",
        "Выберите преподавателя для этого курса:",
        items,
        defaultIdx,
        false,  // editable = false (нельзя вводить свой текст)
        &ok
        );

    //Если пользователь выбрал и нажал OK — возвращаем id преподавателя
    if (ok && !selected.isEmpty()) {
        return map.value(selected, -1);
    }
    return -1;
}
