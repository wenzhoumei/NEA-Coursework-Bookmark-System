CONFIG += debug
TARGET = program.program
OBJECTS_DIR = ./obj
INCLUDEPATH += ./include
HEADERS += include/*.hpp
SOURCES += src/*.cpp
QMAKE_CXXFLAGS += -std=c++20 -g
LIBS += -lncursesw
