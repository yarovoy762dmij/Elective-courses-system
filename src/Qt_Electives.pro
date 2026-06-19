QT       += core gui widgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    controller/authcontroller.cpp \
    controller/electivecontroller.cpp \
    controller/enrollmentcontroller.cpp \
    controller/examsheetcontroller.cpp \
    controller/professorcontroller.cpp \
    controller/profilecontroller.cpp \
    controller/semestercoursecontroller.cpp \
    controller/session.cpp \
    controller/studentcontroller.cpp \
    controller/usercontroller.cpp \
    main.cpp \
    model/courseenrollment.cpp \
    model/dbmanager.cpp \
    model/elective.cpp \
    model/examsheet.cpp \
    model/professor.cpp \
    model/query/examcourserow.cpp \
    model/query/semestercourserow.cpp \
    model/query/studentgraderow.cpp \
    model/student.cpp \
    model/user.cpp \
    view/logindialog.cpp \
    view/mainwindow.cpp \
    view/registerdialog.cpp

HEADERS += \
    controller/authcontroller.h \
    controller/electivecontroller.h \
    controller/enrollmentcontroller.h \
    controller/examsheetcontroller.h \
    controller/professorcontroller.h \
    controller/profilecontroller.h \
    controller/semestercoursecontroller.h \
    controller/session.h \
    controller/studentcontroller.h \
    controller/usercontroller.h \
    model/courseenrollment.h \
    model/dbmanager.h \
    model/elective.h \
    model/examsheet.h \
    model/professor.h \
    model/query/examcourserow.h \
    model/query/semestercourserow.h \
    model/query/studentgraderow.h \
    model/student.h \
    model/user.h \
    view/logindialog.h \
    view/mainwindow.h \
    view/registerdialog.h

FORMS += \
    view/logindialog.ui \
    view/mainwindow.ui \
    view/registerdialog.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
