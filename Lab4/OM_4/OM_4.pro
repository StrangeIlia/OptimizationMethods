QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    console.cpp \
    data_processing.cpp \
    delegate_double.cpp \
    distribution_method.cpp \
    enumerator.cpp \
    least_cost_method.cpp \
    main.cpp \
    main_data_struct.cpp \
    main_window.cpp \
    nortwest_corner_method.cpp

HEADERS += \
    console.h \
    data_processing.h \
    delegate_double.h \
    distribution_method.h \
    enumerator.h \
    least_cost_method.h \
    main_data_struct.h \
    main_window.h \
    math/matrix/MatrixOnRow.hpp \
    nortwest_corner_method.h

FORMS += \
    console.ui \
    main_window.ui

TRANSLATIONS += \
    OM_4_ru_RU.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
