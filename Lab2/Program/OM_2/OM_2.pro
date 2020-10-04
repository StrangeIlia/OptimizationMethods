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
    BasisIterator.cpp \
    ColumnDelegateCloner.cpp \
    ColumnNameChanger.cpp \
    DataProcessing.cpp \
    DelegateComboBox.cpp \
    DelegateController.cpp \
    DelegateDouble.cpp \
    DelegateLabel.cpp \
    HeaderInfo.cpp \
    LabelHeaderView.cpp \
    ResultForm.cpp \
    SizeController.cpp \
    SizeSynchronizer.cpp \
    SystemOfRestrictions.cpp \
    mainwindow.cpp \
    main.cpp

HEADERS += \
    AbstractNameGiver.hpp \
    BasisIterator.h \
    ColumnDelegateCloner.h \
    ColumnNameChanger.h \
    DataProcessing.h \
    DelegateComboBox.h \
    DelegateController.h \
    DelegateDouble.h \
    DelegateLabel.h \
    HeaderInfo.h \
    LabelHeaderView.h \
    MatrixBasisCreator.hpp \
    ResultForm.h \
    SimplexMethodIterator.hpp \
    SizeController.h \
    SizeSynchronizer.h \
    SystemOfRestrictions.h \
    VariableNameGiver.hpp \
    mainwindow.h \
    math/matrix/MatrixOnRow.hpp \
    math/matrix/MatrixOnRow.hpp

FORMS += \
    ResultForm.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
