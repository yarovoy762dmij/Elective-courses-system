#include "semestercourserow.h"
#include "model/dbmanager.h"
#include <QVariant>
#include <QSqlError>
#include <QDebug>

namespace LessonType {
const int LECTURE = 1;
const int PRACTICE = 2;
const int LAB = 3;
}

SemesterCourseRow SemesterCourseRow::fromQuery(const QSqlQuery& q) {
    SemesterCourseRow row;
    row.courseId = q.value("id").toInt();
    row.electiveId = q.value("id_факультатива").toInt();
    row.academicYear = q.value("учебный_год").toInt();
    row.semesterNumber = q.value("номер_семестра").toInt();
    row.lectureHours = q.value("часы_лекций").toInt();
    row.practiceHours = q.value("часы_практик").toInt();
    row.labHours = q.value("часы_лабораторных").toInt();
    row.professorName = q.value("преподаватель").toString();
    return row;
}

QList<SemesterCourseRow> SemesterCourseRow::getCoursesForElective(int electiveId) {
    QList<SemesterCourseRow> list;
    QSqlQuery q = DBManager::instance().execQuery(
        "SELECT id, учебный_год, номер_семестра, часы_лекций, часы_практик, часы_лабораторных, преподаватель "
        "FROM view_course_details "
        "WHERE id_факультатива = ? "
        "ORDER BY id;",
        {electiveId}
        );
    while (q.next()) {
        list.append(fromQuery(q));
    }
    return list;
}

bool SemesterCourseRow::insertCourse(int electiveId, int academicYear, int semester,
                                     int lec, int prac, int lab, int professorId) {
    //1. Вставка записи в семестровые_курсы
    QSqlQuery q1 = DBManager::instance().execQuery(
        "INSERT INTO семестровые_курсы (учебный_год, номер_семестра, id_факультатива) "
        "VALUES (?, ?, ?) RETURNING id;",
        {academicYear, semester, electiveId}
        );
    if (!q1.next()) {
        qDebug() << "Ошибка создания семестрового курса:" << q1.lastError().text();
        return false;
    }
    int courseId = q1.value(0).toInt();

    //2. Для каждого вида занятия с ненулевым количеством часов создание записи в обучение_студента
    struct LessonInfo { int typeId; int hours; };
    QList<LessonInfo> lessons = {
        {LessonType::LECTURE, lec},
        {LessonType::PRACTICE, prac},
        {LessonType::LAB, lab}
    };

    for (const auto& lesson : lessons) {
        if (lesson.hours <= 0) continue;
        QSqlQuery q2 = DBManager::instance().execQuery(
            "INSERT INTO обучение_студента (количество_часов, id_вида_занятия, id_семестрового_курса, id_преподавателя) "
            "VALUES (?, ?, ?, ?);",
            {lesson.hours, lesson.typeId, courseId, professorId}
            );
        if (!q2.isActive()) {
            qDebug() << "Ошибка добавления часов для вида" << lesson.typeId << ":" << q2.lastError().text();
            return false;
        }
    }
    return true;
}

bool SemesterCourseRow::updateCourse(int courseId, int academicYear, int semester,
                                     int lec, int prac, int lab, int professorId) {
    //1. Обновление года и семестра в семестровые_курсы
    QSqlQuery q1 = DBManager::instance().execQuery(
        "UPDATE семестровые_курсы SET учебный_год = ?, номер_семестра = ? WHERE id = ?;",
        {academicYear, semester, courseId}
        );
    if (!q1.isActive()) {
        qDebug() << "Ошибка обновления семестрового курса:" << q1.lastError().text();
        return false;
    }

    //2. Удаление всех старых записей обучения_студента для этого курса
    QSqlQuery q2 = DBManager::instance().execQuery(
        "DELETE FROM обучение_студента WHERE id_семестрового_курса = ?;",
        {courseId}
        );
    if (!q2.isActive()) {
        qDebug() << "Ошибка удаления старых часов:" << q2.lastError().text();
        return false;
    }

    //3. Вставка новых записей с новыми часами и преподавателем
    struct LessonInfo { int typeId; int hours; };
    QList<LessonInfo> lessons = {
        {LessonType::LECTURE, lec},
        {LessonType::PRACTICE, prac},
        {LessonType::LAB, lab}
    };

    for (const auto& lesson : lessons) {
        if (lesson.hours <= 0) continue;
        QSqlQuery q3 = DBManager::instance().execQuery(
            "INSERT INTO обучение_студента (количество_часов, id_вида_занятия, id_семестрового_курса, id_преподавателя) "
            "VALUES (?, ?, ?, ?);",
            {lesson.hours, lesson.typeId, courseId, professorId}
            );
        if (!q3.isActive()) {
            qDebug() << "Ошибка вставки новых часов для вида" << lesson.typeId << ":" << q3.lastError().text();
            return false;
        }
    }
    return true;
}

bool SemesterCourseRow::removeCourse(int courseId) {
    QSqlQuery q = DBManager::instance().execQuery(
        "DELETE FROM семестровые_курсы WHERE id = ?;",
        {courseId}
        );
    if (!q.isActive()) {
        qDebug() << "Ошибка удаления семестрового курса:" << q.lastError().text();
    }
    return q.isActive();
}

int SemesterCourseRow::getProfessorIdForCourse(int courseId) {
    QSqlQuery q = DBManager::instance().execQuery(
        "SELECT id_преподавателя FROM обучение_студента WHERE id_семестрового_курса = ? LIMIT 1;",
        {courseId}
        );
    if (q.next()) return q.value(0).toInt();
    return -1;
}
