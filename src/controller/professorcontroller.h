#ifndef PROFESSORCONTROLLER_H
#define PROFESSORCONTROLLER_H

#include "model/professor.h"
#include <QList>

class ProfessorController {
public:
    static QList<Professor> getAll();
};

#endif // PROFESSORCONTROLLER_H
