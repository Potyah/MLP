QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    controller/controller.cpp \
    main.cpp \
    model/cvsReader.cpp \
    model/graphNet.cpp \
    model/layer.cpp \
    model/matrixNet.cpp \
    model/neuraltransformations.cpp \
    model/neuron.cpp \
    model/s21_matrix_oop.cpp \
    view/dialoggraphic.cpp \
    view/mainwindow.cpp \
    view/paintscene.cpp

HEADERS += \
    common.h \
    controller/controller.h \
    model/cvsReader.h \
    model/graphNet.h \
    model/layer.h \
    model/neuraltransformations.h \
    model/neuron.h \
    model/interfaceNet.h \
    model/matrixNet.h \
    model/s21_matrix_oop.h \
    view/dialoggraphic.h \
    view/mainwindow.h \
    view/paintscene.h

FORMS += \
    view/dialoggraphic.ui \
    view/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

ICON = neural.png

DISTFILES += \
    ../datasets/emnist-letters-test.csv \
    ../datasets/emnist-letters-train.csv \
    config.weights

RESOURCES +=
