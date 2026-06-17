#ifndef STUDENTCONTROLLER_H
#define STUDENTCONTROLLER_H

#include <QList>
#include <optional>
#include "model/student.h"

class StudentController {
public:
    static QList<Student> getAll();
    static std::optional<Student> getById(int id);
    static bool create(const Student& student);
    static bool update(const Student& student);
    static bool remove(int id);
};

#endif // STUDENTCONTROLLER_H
