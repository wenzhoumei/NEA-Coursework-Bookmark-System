TARGET = program.program
DESTDIR = ./files
QT += widgets
HEADERS += 
SOURCES += main.cpp
INCLUDEPATH += /usr/include/qt
QMAKE_CXXFLAGS += -std=c++20 -I/usr/local/include
LIBS += -L./libs -lftxui-component -lftxui-dom -lftxui-screen
