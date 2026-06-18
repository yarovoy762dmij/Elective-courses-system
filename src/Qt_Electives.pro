QT       += core gui widgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    controller/electivecontroller.cpp \
    controller/enrollmentcontroller.cpp \
    controller/examsheetcontroller.cpp \
    controller/profilecontroller.cpp \
    controller/semestercoursecontroller.cpp \
    controller/session.cpp \
    controller/studentcontroller.cpp \
    controller/usercontroller.cpp \
    main.cpp \
    model/courseenrollment.cpp \
    model/dbmanager.cpp \
    model/elective.cpp \
    controller/authcontroller.cpp \
    model/examrecord.cpp \
    model/examsheet.cpp \
    model/lessontype.cpp \
    model/query/examcourserow.cpp \
    model/query/semestercourserow.cpp \
    model/query/studentgraderow.cpp \
    model/role.cpp \
    model/semestercourse.cpp \
    model/student.cpp \
    model/professor.cpp \
    model/professorcompetency.cpp \
    model/teachingload.cpp \
    model/user.cpp \
    view/logindialog.cpp \
    view/mainwindow.cpp \
    view/registerdialog.cpp

HEADERS += \
    controller/electivecontroller.h \
    controller/enrollmentcontroller.h \
    controller/examsheetcontroller.h \
    controller/profilecontroller.h \
    controller/semestercoursecontroller.h \
    controller/session.h \
    controller/studentcontroller.h \
    controller/usercontroller.h \
    model/courseenrollment.h \
    model/dbmanager.h \
    model/elective.h \
    controller/authcontroller.h \
    model/examrecord.h \
    model/examsheet.h \
    model/lessontype.h \
    model/query/examcourserow.h \
    model/query/semestercourserow.h \
    model/query/studentgraderow.h \
    model/role.h \
    model/semestercourse.h \
    model/student.h \
    model/professor.h \
    model/professorcompetency.h \
    model/teachingload.h \
    model/user.h \
    view/logindialog.h \
    view/mainwindow.h \
    view/registerdialog.h

FORMS += \
    view/logindialog.ui \
    view/mainwindow.ui \
    view/registerdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
