#ifndef ELECTIVECONTROLLER_H
#define ELECTIVECONTROLLER_H

#include <QList>
#include <QString>
#include "model/elective.h"

class ElectiveController {
public:
    static QList<Elective> getAll();
    static bool create(const QString& name);
    static bool update(int id, const QString& name);
    static bool remove(int id);
};

#endif // ELECTIVECONTROLLER_H
