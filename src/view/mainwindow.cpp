#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "controller/profilecontroller.h"
#include "controller/electivecontroller.h"
#include "controller/studentcontroller.h"
#include "controller/enrollmentcontroller.h"
#include "controller/semestercoursecontroller.h"
#include "model/elective.h"
#include "model/query/semestercourserow.h"
#include "model/professor.h"
#include "model/student.h"
#include "model/courseenrollment.h"
#include "controller/examsheetcontroller.h"
#include "controller/usercontroller.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QHeaderView>
#include <QFormLayout>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

MainWindow::MainWindow(const User& user, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_currentUser(user)
{
    ui->setupUi(this);

    this->setWindowTitle("Управление факультативами | " + m_currentUser.login + " (" + m_currentUser.roleName + ")");

    //1. Заполнение полей профиля
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

    //4. Настройка таблицы Студентов
    m_studentsModel = new QStandardItemModel(this);
    m_studentsModel->setHorizontalHeaderLabels({"ID", "Фамилия", "Имя", "Отчество", "Телефон", "Адрес", "Мин. предметов"});
    ui->tvStudents->setModel(m_studentsModel);
    ui->tvStudents->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tvStudents->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tvStudents->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvStudents->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tvStudents->horizontalHeader()->setStretchLastSection(true);

    m_availableModel = new QStandardItemModel(this);
    m_availableModel->setHorizontalHeaderLabels({"ID", "Название"});
    ui->tvAvailableCourses->setModel(m_availableModel);
    ui->tvAvailableCourses->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tvAvailableCourses->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tvAvailableCourses->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvAvailableCourses->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    m_enrolledModel = new QStandardItemModel(this);
    m_enrolledModel->setHorizontalHeaderLabels({"ID", "Название"});
    ui->tvEnrolledCourses->setModel(m_enrolledModel);
    ui->tvEnrolledCourses->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tvEnrolledCourses->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tvEnrolledCourses->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvEnrolledCourses->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    refreshStudentsTable();

    //5. Настройка таблицы Ведомостей
    m_examSheetsModel = new QStandardItemModel(this);
    m_examSheetsModel->setHorizontalHeaderLabels({"ID", "Дата создания"});
    ui->tableView_2->setModel(m_examSheetsModel);
    ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_2->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView_2->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    //6. Настройка таблицы Оценок (содержимого Ведомостей)
    m_studentGradesModel = new QStandardItemModel(this);
    m_studentGradesModel->setHorizontalHeaderLabels({"ID Студента", "ФИО Студента", "Оценка"});
    ui->tableView->setModel(m_studentGradesModel);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    refreshExamCoursesCombo();

    //7. Настройка таблицы Пользователей
    m_usersModel = new QStandardItemModel(this);
    ui->tvUsers->setModel(m_usersModel);
    ui->tvUsers->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tvUsers->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tvUsers->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvUsers->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    refreshUsersTable();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupRoleAccess()
{
    QString role = m_currentUser.roleName;

    bool canEditElectives = (role == "Суперпользователь" || role == "Методист");
    ui->btnElectiveAdd->setEnabled(canEditElectives);
    ui->btnElectiveEdit->setEnabled(canEditElectives);
    ui->btnElectiveDelete->setEnabled(canEditElectives);

    ui->btnCourseAdd->setEnabled(canEditElectives);
    ui->btnCourseEdit->setEnabled(canEditElectives);
    ui->btnCourseDelete->setEnabled(canEditElectives);

    bool canEditStudents = (role == "Суперпользователь" || role == "Методист");
    ui->btnStudentAdd->setEnabled(canEditStudents);
    ui->btnStudentEdit->setEnabled(canEditStudents);
    ui->btnStudentDelete->setEnabled(canEditStudents);

    bool canEnroll = (role == "Суперпользователь" || role == "Методист");
    ui->btnEnroll->setEnabled(canEnroll);
    ui->btnUnenroll->setEnabled(canEnroll);

    bool canManageSheets = (role == "Суперпользователь" || role == "Методист" || role == "Преподаватель");
    bool canSetGrades = (role == "Суперпользователь" || role == "Преподаватель");

    ui->btnSheetCreate->setEnabled(canManageSheets);
    ui->btnSheetDelete->setEnabled(canManageSheets);
    ui->btnSetGrade->setEnabled(canSetGrades);

    bool isSuperUser = m_currentUser.roleName.contains("Суперпользователь");
    ui->btnAddRole->setEnabled(isSuperUser);
    ui->btnRemoveRole->setEnabled(isSuperUser);
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

    QList<Elective> list = ElectiveController::getAll();
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

    QList<SemesterCourseRow> list = SemesterCourseController::getCoursesForElective(electiveId);
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
        if (ElectiveController::create(name.trimmed())) {
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
        if (ElectiveController::update(electiveId, newName.trimmed())) {
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
        if (ElectiveController::remove(electiveId)) {
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
    int year = QInputDialog::getInt(this, "Новая нагрузка", "Учебный год (например, 2026):", 2026, 2000, 2050, 1, &ok);
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

    if (SemesterCourseController::createCourse(electiveId, year, semester, lec, prac, lab, professorId)) {
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

    int currentProfessorId = SemesterCourseController::getProfessorIdForCourse(courseId);
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

    if (SemesterCourseController::updateCourse(courseId, year, semester, lec, prac, lab, professorId)) {
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

    if (SemesterCourseController::removeCourse(courseId)) {
        refreshCoursesTable(electiveId);
        QMessageBox::information(this, "Успех", "Семестровый курс успешно удалён.");
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось удалить семестровый курс.\n"
                                              "Возможно, на него есть ссылки в других таблицах (например, ведомости).");
    }
}

int MainWindow::getProfessorIdFromDialog(int currentProfessorId)
{
    QList<Professor> profs = Professor::getAll();

    QStringList items;
    QMap<QString, int> map;
    for (const auto& p : profs) {
        QString display = p.lastName + " " + p.firstName + " " + p.middleName;
        map[display] = p.id;
        items << display;
    }

    int defaultIdx = -1;
    if (currentProfessorId != -1) {
        for (int i = 0; i < profs.size(); ++i) {
            if (profs[i].id == currentProfessorId) {
                defaultIdx = i;
                break;
            }
        }
    }

    bool ok;
    QString selected = QInputDialog::getItem(
        this,
        "Выбор преподавателя",
        "Выберите преподавателя для этого курса:",
        items,
        defaultIdx,
        false,
        &ok
        );

    if (ok && !selected.isEmpty()) {
        return map.value(selected, -1);
    }
    return -1;
}

//ЗАПИСИ СТУДЕНТОВ
void MainWindow::refreshStudentsTable()
{
    m_studentsModel->removeRows(0, m_studentsModel->rowCount());
    QList<Student> students = StudentController::getAll();
    for (const Student& s : students) {
        QList<QStandardItem*> row;
        row.append(new QStandardItem(QString::number(s.id)));
        row.append(new QStandardItem(s.lastName));
        row.append(new QStandardItem(s.firstName));
        row.append(new QStandardItem(s.middleName));
        row.append(new QStandardItem(s.phone));
        row.append(new QStandardItem(s.address));
        row.append(new QStandardItem(QString::number(s.minSubjectCount)));
        m_studentsModel->appendRow(row);
    }
}

void MainWindow::refreshEnrollmentTables(int studentId)
{
    m_availableModel->removeRows(0, m_availableModel->rowCount());
    m_enrolledModel->removeRows(0, m_enrolledModel->rowCount());

    QList<Elective> allElectives = ElectiveController::getAll();
    QList<CourseEnrollment> enrollments = EnrollmentController::getEnrollmentsByStudent(studentId);

    QSet<int> enrolledIds;
    for (const auto& e : enrollments) {
        enrolledIds.insert(e.electiveId);
    }

    for (const Elective& e : allElectives) {
        if (enrolledIds.contains(e.id)) {
            QList<QStandardItem*> row;
            row.append(new QStandardItem(QString::number(e.id)));
            row.append(new QStandardItem(e.name));
            m_enrolledModel->appendRow(row);
        } else {
            QList<QStandardItem*> row;
            row.append(new QStandardItem(QString::number(e.id)));
            row.append(new QStandardItem(e.name));
            m_availableModel->appendRow(row);
        }
    }

    auto studentOpt = StudentController::getById(studentId);
    if (studentOpt.has_value()) {
        int minCount = studentOpt->minSubjectCount;
        int enrolledCount = enrollments.size();
        QString status;
        if (minCount == 0) {
            status = QString("Записано: %1 предметов (норма не задана)").arg(enrolledCount);
        } else {
            status = QString("Записано: %1 / %2 предметов").arg(std::min(enrolledCount, minCount)).arg(minCount);
        }
        ui->lblTotalHours->setText(status);
    }
}

int MainWindow::getSelectedStudentId() const
{
    QModelIndexList selected = ui->tvStudents->selectionModel()->selectedRows();
    if (selected.isEmpty()) return -1;
    int row = selected.first().row();
    return m_studentsModel->item(row, 0)->text().toInt();
}

void MainWindow::on_btnStudentAdd_clicked()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Добавление студента");

    QFormLayout layout(&dlg);
    QLineEdit leLastName, leFirstName, leMiddleName, lePhone, leAddress;
    QLineEdit leMinSubjects;

    layout.addRow("Фамилия:", &leLastName);
    layout.addRow("Имя:", &leFirstName);
    layout.addRow("Отчество:", &leMiddleName);
    layout.addRow("Телефон:", &lePhone);
    layout.addRow("Адрес:", &leAddress);
    layout.addRow("Минимальное количество предметов:", &leMinSubjects);

    QPushButton btnOk("ОК"), btnCancel("Отмена");
    layout.addRow(&btnOk, &btnCancel);

    connect(&btnOk, &QPushButton::clicked, &dlg, &QDialog::accept);
    connect(&btnCancel, &QPushButton::clicked, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted) {
        Student s;
        s.lastName = leLastName.text().trimmed();
        s.firstName = leFirstName.text().trimmed();
        s.middleName = leMiddleName.text().trimmed();
        s.phone = lePhone.text().trimmed();
        s.address = leAddress.text().trimmed();
        s.minSubjectCount = leMinSubjects.text().toInt();

        if (s.lastName.isEmpty() || s.firstName.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Фамилия и имя обязательны!");
            return;
        }

        if (StudentController::create(s)) {
            refreshStudentsTable();
            QMessageBox::information(this, "Успех", "Студент добавлен.");
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось добавить студента.");
        }
    }
}

void MainWindow::on_btnStudentEdit_clicked()
{
    int studentId = getSelectedStudentId();
    if (studentId == -1) {
        QMessageBox::warning(this, "Внимание", "Выберите студента для редактирования!");
        return;
    }

    auto studentOpt = StudentController::getById(studentId);
    if (!studentOpt.has_value()) {
        QMessageBox::critical(this, "Ошибка", "Студент не найден.");
        return;
    }
    Student s = studentOpt.value();

    QDialog dlg(this);
    dlg.setWindowTitle("Редактирование студента");

    QFormLayout layout(&dlg);
    QLineEdit leLastName(s.lastName), leFirstName(s.firstName), leMiddleName(s.middleName);
    QLineEdit lePhone(s.phone), leAddress(s.address);
    QLineEdit leMinSubjects(QString::number(s.minSubjectCount));

    layout.addRow("Фамилия:", &leLastName);
    layout.addRow("Имя:", &leFirstName);
    layout.addRow("Отчество:", &leMiddleName);
    layout.addRow("Телефон:", &lePhone);
    layout.addRow("Адрес:", &leAddress);
    layout.addRow("Минимальное количество предметов:", &leMinSubjects);

    QPushButton btnOk("ОК"), btnCancel("Отмена");
    layout.addRow(&btnOk, &btnCancel);

    connect(&btnOk, &QPushButton::clicked, &dlg, &QDialog::accept);
    connect(&btnCancel, &QPushButton::clicked, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted) {
        s.lastName = leLastName.text().trimmed();
        s.firstName = leFirstName.text().trimmed();
        s.middleName = leMiddleName.text().trimmed();
        s.phone = lePhone.text().trimmed();
        s.address = leAddress.text().trimmed();
        s.minSubjectCount = leMinSubjects.text().toInt();

        if (s.lastName.isEmpty() || s.firstName.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Фамилия и имя обязательны!");
            return;
        }

        if (StudentController::update(s)) {
            refreshStudentsTable();
            QMessageBox::information(this, "Успех", "Данные обновлены.");
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось обновить данные.");
        }
    }
}

void MainWindow::on_btnStudentDelete_clicked()
{
    int studentId = getSelectedStudentId();
    if (studentId == -1) {
        QMessageBox::warning(this, "Внимание", "Выберите студента для удаления!");
        return;
    }

    auto reply = QMessageBox::question(this, "Подтверждение",
                                       "Вы уверены, что хотите удалить этого студента?\n"
                                       "Все его записи на факультативы будут также удалены.",
                                       QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) return;

    if (StudentController::remove(studentId)) {
        refreshStudentsTable();
        m_availableModel->removeRows(0, m_availableModel->rowCount());
        m_enrolledModel->removeRows(0, m_enrolledModel->rowCount());
        ui->lblTotalHours->clear();
        QMessageBox::information(this, "Успех", "Студент удалён.");
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось удалить студента.\n"
                                              "Возможно, у него есть записи, которые запрещено удалять.");
    }
}

void MainWindow::on_tvStudents_clicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    int studentId = getSelectedStudentId();
    if (studentId != -1) {
        refreshEnrollmentTables(studentId);
    }
}

void MainWindow::on_btnEnroll_clicked()
{
    int studentId = getSelectedStudentId();
    if (studentId == -1) {
        QMessageBox::warning(this, "Внимание", "Сначала выберите студента.");
        return;
    }

    QModelIndexList selected = ui->tvAvailableCourses->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Выберите курс для записи.");
        return;
    }
    int row = selected.first().row();
    int electiveId = m_availableModel->item(row, 0)->text().toInt();

    if (EnrollmentController::enrollStudent(studentId, electiveId)) {
        refreshEnrollmentTables(studentId);
        QMessageBox::information(this, "Успех", "Студент записан на курс.");
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось записать студента.\n"
                                              "Возможно, недостаточно прав или ошибка БД.");
    }
}

void MainWindow::on_btnUnenroll_clicked()
{
    int studentId = getSelectedStudentId();
    if (studentId == -1) {
        QMessageBox::warning(this, "Внимание", "Сначала выберите студента.");
        return;
    }

    QModelIndexList selected = ui->tvEnrolledCourses->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Выберите курс для отчисления.");
        return;
    }
    int row = selected.first().row();
    int electiveId = m_enrolledModel->item(row, 0)->text().toInt();

    auto reply = QMessageBox::question(this, "Подтверждение",
                                       "Вы уверены, что хотите отчислить студента с этого курса?",
                                       QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) return;

    if (EnrollmentController::unenrollStudent(studentId, electiveId)) {
        refreshEnrollmentTables(studentId);
        QMessageBox::information(this, "Успех", "Студент отчислен с курса.");
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось отчислить студента.");
    }
}

//ВЕДОМОСТИ И УСПЕВАЕМОСТЬ
void MainWindow::refreshExamCoursesCombo()
{
    ui->cbSemesterCourse->blockSignals(true);
    ui->cbSemesterCourse->clear();

    QList<ExamCourseRow> courses = ExamSheetController::getAvailableCourses();

    for (const ExamCourseRow &course : courses) {
        ui->cbSemesterCourse->addItem(course.getDisplayText(), course.courseId());
    }

    ui->cbSemesterCourse->blockSignals(false);

    if (ui->cbSemesterCourse->count() > 0) {
        ui->cbSemesterCourse->setCurrentIndex(0);
        int initialCourseId = ui->cbSemesterCourse->currentData().toInt();
        refreshExamSheetsTable(initialCourseId);
    } else {
        m_examSheetsModel->removeRows(0, m_examSheetsModel->rowCount());
        m_studentGradesModel->removeRows(0, m_studentGradesModel->rowCount());
    }
}

void MainWindow::refreshExamSheetsTable(int courseId)
{
    m_examSheetsModel->removeRows(0, m_examSheetsModel->rowCount());
    m_studentGradesModel->removeRows(0, m_studentGradesModel->rowCount());

    QList<ExamSheet> sheets = ExamSheetController::getSheetsForCourse(courseId);
    for (const ExamSheet &sheet : sheets) {
        QList<QStandardItem*> rowItems;

        rowItems.append(new QStandardItem(QString::number(sheet.id)));
        rowItems.append(new QStandardItem(sheet.creationDate.toString("dd.MM.yyyy")));

        m_examSheetsModel->appendRow(rowItems);
    }
}

void MainWindow::refreshStudentGradesTable(int sheetId)
{
    m_studentGradesModel->removeRows(0, m_studentGradesModel->rowCount());

    QList<StudentGradeRow> records = ExamSheetController::getStudentGrades(sheetId);
    for (const auto& rec : records) {
        QList<QStandardItem*> row;

        row.append(new QStandardItem(QString::number(rec.studentId())));
        row.append(new QStandardItem(rec.studentFullName()));

        QString gradeText = (rec.grade() > 0) ? QString::number(rec.grade()) : "—";
        QStandardItem* gradeItem = new QStandardItem(gradeText);

        if (rec.grade() == 5) gradeItem->setForeground(QBrush(Qt::darkGreen));
        else if (rec.grade() == 2) gradeItem->setForeground(QBrush(Qt::red));

        row.append(gradeItem);
        m_studentGradesModel->appendRow(row);
    }
}

void MainWindow::on_cbSemesterCourse_currentIndexChanged(int index)
{
    if (index < 0) return;
    int courseId = ui->cbSemesterCourse->itemData(index).toInt();
    refreshExamSheetsTable(courseId);
}

void MainWindow::on_tableView_2_clicked(const QModelIndex &index)
{
    Q_UNUSED(index);

    QModelIndexList selected = ui->tableView_2->selectionModel()->selectedRows();

    if (selected.isEmpty()) {
        m_studentGradesModel->removeRows(0, m_studentGradesModel->rowCount());
        return;
    }

    int sheetId = m_examSheetsModel->item(selected.first().row(), 0)->text().toInt();

    refreshStudentGradesTable(sheetId);
}
void MainWindow::on_btnSheetCreate_clicked()
{
    int index = ui->cbSemesterCourse->currentIndex();
    if (index < 0) {
        QMessageBox::warning(this, "Внимание", "Пожалуйста, выберите курс из списка сверху.");
        return;
    }

    int courseId = ui->cbSemesterCourse->itemData(index).toInt();
    if (ExamSheetController::createSheet(courseId)) {
        refreshExamSheetsTable(courseId);
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось создать ведомость.\nУбедитесь, что за курсом закреплен преподаватель.");
    }
}

void MainWindow::on_btnSheetDelete_clicked()
{
    QModelIndexList selected = ui->tableView_2->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Выберите ведомость в левой таблице для удаления.");
        return;
    }

    int row = selected.first().row();
    int sheetId = m_examSheetsModel->item(row, 0)->text().toInt();

    auto answer = QMessageBox::question(this, "Удаление ведомости",
                                        "Вы действительно хотите удалить эту ведомость?\nВсе выставленные в ней оценки будут стёрты!",
                                        QMessageBox::Yes | QMessageBox::No);
    if (answer != QMessageBox::Yes) return;

    if (ExamSheetController::deleteSheet(sheetId)) {
        int index = ui->cbSemesterCourse->currentIndex();
        if (index >= 0) {
            refreshExamSheetsTable(ui->cbSemesterCourse->itemData(index).toInt());
        }
    } else {
        QMessageBox::critical(this, "Ошибка", "Произошла ошибка при удалении ведомости из базы данных.");
    }
}

void MainWindow::on_btnSetGrade_clicked()
{
    QModelIndexList sheetSelected = ui->tableView_2->selectionModel()->selectedRows();
    if (sheetSelected.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Сначала выберите конкретную ведомость в левой таблице.");
        return;
    }
    int sheetId = m_examSheetsModel->item(sheetSelected.first().row(), 0)->text().toInt();

    QModelIndexList studentSelected = ui->tableView->selectionModel()->selectedRows();
    if (studentSelected.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Выберите студента в правой таблице для выставления оценки.");
        return;
    }

    int studentRow = studentSelected.first().row();
    int studentId = m_studentGradesModel->item(studentRow, 0)->text().toInt();
    QString studentName = m_studentGradesModel->item(studentRow, 1)->text();
    QString currentGradeStr = m_studentGradesModel->item(studentRow, 2)->text();

    int defaultGrade = (currentGradeStr == "—" || currentGradeStr.isEmpty()) ? 5 : currentGradeStr.toInt();

    bool ok;
    int grade = QInputDialog::getInt(this, "Выставление оценки",
                                     "Студент: " + studentName + "\nВведите оценку (2 - 5):",
                                     defaultGrade, 2, 5, 1, &ok);
    if (!ok) return;

    if (ExamSheetController::setGrade(sheetId, studentId, grade)) {
        refreshStudentGradesTable(sheetId);
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось сохранить оценку в базе данных.");
    }
}

//ПОЛЬЗОВАТЕЛИ
void MainWindow::refreshUsersTable()
{
    m_usersModel->clear();
    m_usersModel->setHorizontalHeaderLabels({"ID", "Логин", "Email", "Назначенные роли"});

    QList<UserAdminRow> users = UserController::getAllUsersForAdmin();
    for (const UserAdminRow &user : users) {
        QList<QStandardItem*> rowItems;
        rowItems << new QStandardItem(QString::number(user.id))
                 << new QStandardItem(user.login)
                 << new QStandardItem(user.email)
                 << new QStandardItem(user.roles);
        m_usersModel->appendRow(rowItems);
    }
}

void MainWindow::on_btnAddRole_clicked()
{
    QModelIndexList selected = ui->tvUsers->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Выберите пользователя для добавления роли.");
        return;
    }

    int userId = m_usersModel->item(selected.first().row(), 0)->text().toInt();
    QString userLogin = m_usersModel->item(selected.first().row(), 1)->text();

    QMap<int, QString> availableRoles = UserController::getAvailableRoles(userId);
    if (availableRoles.isEmpty()) {
        QMessageBox::information(this, "Информация", "У пользователя <b>" + userLogin + "</b> уже есть все возможные роли.");
        return;
    }

    QStringList roleNames = availableRoles.values();
    QList<int> roleIds = availableRoles.keys();

    bool ok;
    QString selectedRole = QInputDialog::getItem(this, "Добавление роли", "Выберите роль для добавления:", roleNames, 0, false, &ok);
    if (!ok || selectedRole.isEmpty()) return;

    int roleId = roleIds[roleNames.indexOf(selectedRole)];

    if (UserController::addRole(userId, roleId)) {
        refreshUsersTable();
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось назначить роль.");
    }
}

void MainWindow::on_btnRemoveRole_clicked()
{
    QModelIndexList selected = ui->tvUsers->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Выберите пользователя для отзыва роли.");
        return;
    }

    int userId = m_usersModel->item(selected.first().row(), 0)->text().toInt();
    QString userLogin = m_usersModel->item(selected.first().row(), 1)->text();

    QMap<int, QString> currentRoles = UserController::getUserRoles(userId);
    if (currentRoles.isEmpty()) {
        QMessageBox::information(this, "Информация", "У пользователя <b>" + userLogin + "</b> нет назначенных ролей.");
        return;
    }

    QStringList roleNames = currentRoles.values();
    QList<int> roleIds = currentRoles.keys();

    bool ok;
    QString selectedRole = QInputDialog::getItem(this, "Удаление роли", "Выберите роль, которую нужно отозвать:", roleNames, 0, false, &ok);
    if (!ok || selectedRole.isEmpty()) return;

    //НО: Суперпользователь не может снять роль "Суперпользователь" с самого себя
    if (userId == m_currentUser.id && selectedRole == "Суперпользователь") {
        QMessageBox::warning(this, "Ошибка", "Вы не можете отозвать права суперпользователя у самого себя!");
        return;
    }

    int roleId = roleIds[roleNames.indexOf(selectedRole)];

    if (UserController::removeRole(userId, roleId)) {
        refreshUsersTable();
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось отозвать роль.");
    }
}
