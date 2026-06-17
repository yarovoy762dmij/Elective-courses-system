#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "model/user.h"

class QStandardItemModel;
class QModelIndex;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const User& user, QWidget *parent = nullptr);
    ~MainWindow();

    bool isLogoutRequested() const { return m_logoutRequested; }

private slots:
    //Вкладка "Профиль"
    void on_btnSaveProfile_clicked();
    void on_btnLogout_clicked();

    //Вкладка "Организация факультативов"
    void on_tvElectives_clicked(const QModelIndex &index);
    void on_btnElectiveAdd_clicked();
    void on_btnElectiveEdit_clicked();
    void on_btnElectiveDelete_clicked();

    void on_btnCourseAdd_clicked();
    void on_btnCourseEdit_clicked();
    void on_btnCourseDelete_clicked();

    //Вкладка "Запись студентов"
    void on_btnStudentAdd_clicked();
    void on_btnStudentEdit_clicked();
    void on_btnStudentDelete_clicked();
    void on_tvStudents_clicked(const QModelIndex &index);
    void on_btnEnroll_clicked();
    void on_btnUnenroll_clicked();

private:
    Ui::MainWindow *ui;
    User m_currentUser;
    bool m_logoutRequested = false;

    QStandardItemModel *m_electivesModel = nullptr;
    QStandardItemModel *m_coursesModel = nullptr;

    void refreshElectivesTable();
    void refreshCoursesTable(int electiveId);
    void setupRoleAccess();

    int getSelectedElectiveId() const;
    int getSelectedCourseId() const;

    int getProfessorIdFromDialog(int currentProfessorId = -1);

    QStandardItemModel *m_studentsModel = nullptr;
    QStandardItemModel *m_availableModel = nullptr;
    QStandardItemModel *m_enrolledModel = nullptr;

    void refreshStudentsTable();
    void refreshEnrollmentTables(int studentId);
    int getSelectedStudentId() const;
};
#endif // MAINWINDOW_H
