#include "semestercoursecontroller.h"
#include "model/dbmanager.h"
#include "model/query/semestercourserow.h"
#include "session.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

namespace {
const int LECTURE = 1;
const int PRACTICE = 2;
const int LAB = 3;

struct LessonInfo {
    int typeId;
    int hours;
};
}

QList<SemesterCourseRow> SemesterCourseController::getCoursesForElective(int electiveId)
{
    QList<SemesterCourseRow> list;
    QSqlQuery q = DBManager::instance().execQuery(
        "SELECT id, учебный_год, номер_семестра, часы_лекций, часы_практик, часы_лабораторных, преподаватель "
        "FROM view_course_details "
        "WHERE id_факультатива = ? "
        "ORDER BY id;",
        {electiveId});
    while (q.next()) {
        list.append(SemesterCourseRow::fromQuery(q));
    }
    return list;
}

bool SemesterCourseController::createCourse(int electiveId, int academicYear, int semester,
                                           int lec, int prac, int lab, int professorId)
{
    QString role = Session::instance().currentUser().roleName;
    if (role != "Суперпользователь" && role != "Методист") {
        return false;
    }

    QSqlQuery q1 = DBManager::instance().execQuery(
        "INSERT INTO семестровые_курсы (учебный_год, номер_семестра, id_факультатива) "
        "VALUES (?, ?, ?) RETURNING id;",
        {academicYear, semester, electiveId});
    if (!q1.next()) {
        qDebug() << "Ошибка создания семестрового курса:" << q1.lastError().text();
        return false;
    }
    int courseId = q1.value(0).toInt();

    const QList<LessonInfo> lessons = {
        {LECTURE, lec},
        {PRACTICE, prac},
        {LAB, lab}
    };

    for (const auto& lesson : lessons) {
        if (lesson.hours <= 0) {
            continue;
        }
        QSqlQuery q2 = DBManager::instance().execQuery(
            "INSERT INTO обучение_студента (количество_часов, id_вида_занятия, id_семестрового_курса, id_преподавателя) "
            "VALUES (?, ?, ?, ?);",
            {lesson.hours, lesson.typeId, courseId, professorId});
        if (!q2.isActive()) {
            qDebug() << "Ошибка добавления часов для вида" << lesson.typeId << ":" << q2.lastError().text();
            return false;
        }
    }
    return true;
}

bool SemesterCourseController::updateCourse(int courseId, int academicYear, int semester,
                                           int lec, int prac, int lab, int professorId)
{
    QString role = Session::instance().currentUser().roleName;
    if (role != "Суперпользователь" && role != "Методист") {
        return false;
    }

    QSqlQuery q1 = DBManager::instance().execQuery(
        "UPDATE семестровые_курсы SET учебный_год = ?, номер_семестра = ? WHERE id = ?;",
        {academicYear, semester, courseId});
    if (!q1.isActive()) {
        qDebug() << "Ошибка обновления семестрового курса:" << q1.lastError().text();
        return false;
    }

    QSqlQuery q2 = DBManager::instance().execQuery(
        "DELETE FROM обучение_студента WHERE id_семестрового_курса = ?;",
        {courseId});
    if (!q2.isActive()) {
        qDebug() << "Ошибка удаления старых часов:" << q2.lastError().text();
        return false;
    }

    const QList<LessonInfo> lessons = {
        {LECTURE, lec},
        {PRACTICE, prac},
        {LAB, lab}
    };

    for (const auto& lesson : lessons) {
        if (lesson.hours <= 0) {
            continue;
        }
        QSqlQuery q3 = DBManager::instance().execQuery(
            "INSERT INTO обучение_студента (количество_часов, id_вида_занятия, id_семестрового_курса, id_преподавателя) "
            "VALUES (?, ?, ?, ?);",
            {lesson.hours, lesson.typeId, courseId, professorId});
        if (!q3.isActive()) {
            qDebug() << "Ошибка вставки новых часов для вида" << lesson.typeId << ":" << q3.lastError().text();
            return false;
        }
    }
    return true;
}

bool SemesterCourseController::removeCourse(int courseId)
{
    QString role = Session::instance().currentUser().roleName;
    if (role != "Суперпользователь" && role != "Методист") {
        return false;
    }

    QSqlQuery q = DBManager::instance().execQuery(
        "DELETE FROM семестровые_курсы WHERE id = ?;",
        {courseId});
    if (!q.isActive()) {
        qDebug() << "Ошибка удаления семестрового курса:" << q.lastError().text();
    }
    return q.isActive();
}

int SemesterCourseController::getProfessorIdForCourse(int courseId)
{
    QSqlQuery q = DBManager::instance().execQuery(
        "SELECT id_преподавателя FROM обучение_студента WHERE id_семестрового_курса = ? LIMIT 1;",
        {courseId});
    if (q.next()) {
        return q.value(0).toInt();
    }
    return -1;
}
