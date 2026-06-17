#include "studentcontroller.h"
#include "session.h"

QList<Student> StudentController::getAll() {
    return Student::getAll();
}

std::optional<Student> StudentController::getById(int id) {
    return Student::getById(id);
}

bool StudentController::create(const Student& student) {
    QString role = Session::instance().currentUser().roleName;
    if (role != "Суперпользователь" && role != "Методист")
        return false;
    return Student::insert(student);
}

bool StudentController::update(const Student& student) {
    QString role = Session::instance().currentUser().roleName;
    if (role != "Суперпользователь" && role != "Методист")
        return false;
    Student s = student;
    return s.update();
}

bool StudentController::remove(int id) {
    QString role = Session::instance().currentUser().roleName;
    if (role != "Суперпользователь" && role != "Методист")
        return false;
    return Student::remove(id);
}
