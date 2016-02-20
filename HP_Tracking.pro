#created by Benjamin Reichardt, please mark your own work
#-------------------------------------------------
#
# Project created by QtCreator 2014-09-30T14:36:48
#
#-------------------------------------------------
QT       += core gui

INCLUDEPATH += /usr/local/Eigen/
INCLUDEPATH += /usr/local/qwt-6.1.2/include

LIBS += -L/usr/local/qwt-6.1.2/lib -lqwt
QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS_RELEASE *= -O3
LIBS += -lfftw3
LIBS += -lgsl -lgslcblas -lm
CONFIG += qwt
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HP_Tracking
TEMPLATE = app


SOURCES += main.cpp\
        maingui.cpp \
    parameter.cpp \
    referenz.cpp \
    log_reference.cpp \
    bunch.cpp \
    log_bunch.cpp \
    monitoring.cpp \
    setendpos.cpp \
    paragui.cpp \
    ramp_dist_gui.cpp \
    distortion.cpp \
    histogramm.cpp \
    controller.cpp \
    seperatrix.cpp \
    synfreq.cpp \
    freq_damp_analyser.cpp \
    exponential_fitter.cpp \
    ownplot.cpp \
    save_results.cpp \
    para_save.cpp \
    own_math.cpp \
    dummytest.cpp

HEADERS  += maingui.h \
    parameter.h \
    referenz.h \
    log_reference.h \
    bunch.h \
    log_bunch.h \
    monitoring.h \
    setendpos.h \
    paragui.h \
    ramp_dist_gui.h \
    distortion.h \
    histogramm.h \
    controller.h \
    seperatrix.h \
    synfreq.h \
    log_ref_i.h \
    freq_damp_analyser.h \
    exponential_fitter.h \
    ownplot.h \
    save_results.h \
    para_save.h \
    own_math.h \
    dummytest.h

FORMS    += maingui.ui \
    monitoring.ui \
    paragui.ui \
    ramp_dist_gui.ui \
    freq_damp_analyser.ui
