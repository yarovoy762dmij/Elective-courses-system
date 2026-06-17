#include "electivecontroller.h"
#include "model/elective.h"
#include "session.h"

QList<Elective> ElectiveController::getAll() {
    return Elective::getAll();
}

bool ElectiveController::create(const QString& name) {
    QString role = Session::instance().currentUser().roleName;
    if (role != "Суперпользователь" && role != "Методист")
        return false;
    return Elective::insert(name);
}

bool ElectiveController::update(int id, const QString& name) {
    QString role = Session::instance().currentUser().roleName;
    if (role != "Суперпользователь" && role != "Методист")
        return false;
    Elective e;
    e.id = id;
    e.name = name;
    return e.update();
}

bool ElectiveController::remove(int id) {
    QString role = Session::instance().currentUser().roleName;
    if (role != "Суперпользователь" && role != "Методист")
        return false;
    return Elective::remove(id);
}
