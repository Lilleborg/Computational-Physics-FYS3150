TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    functions.cpp \
    exercises.cpp

HEADERS += \
    functions.h \
    exercises.h

INCLUDEPATH += /usr/local/Cellar/armadillo/9.100.5_1/include/
LIBS += -L/usr/local/Cellar/armadillo/9.100.5_1/lib/ -larmadillo
