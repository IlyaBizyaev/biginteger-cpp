TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

HEADERS += \
    biginteger.hpp \
    biginteger.tpp \
    randomtest.hpp \
    randomtest.tpp

LIBS += -lgmp -lgmpxx
