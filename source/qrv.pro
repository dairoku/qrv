QT += core gui widgets

TARGET = qrv
TEMPLATE = app
DESTDIR = ./output

CONFIG += c++17
# Before Qt 5.11 we need the following too
QMAKE_CXXFLAGS += -std=c++17
# for Visual Studio the following might work (not tested)
# QMAKE_CXXFLAGS += /std::c++17

INCLUDEPATH += \
  ../libibc/include \
  .

HEADERS += \
  ../libibc/include/ibc/qt/image_view.h \
  ../libibc/include/ibc/qt/image_scroll_area.h \
  OpenDialog.h \
  qrv.h

SOURCES += \
  qrv.cpp \
  main.cpp

FORMS += \
  qrv.ui \
  OpenDialog.ui

