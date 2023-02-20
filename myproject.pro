TARGET = program.program
OBJECTS_DIR = ./obj
HEADERS += vector_entry_list.hpp entry_list.hpp menu.hpp
SOURCES += main.cpp vector_entry_list.cpp entry_list.cpp
INCLUDE_PATH +=
QMAKE_CXXFLAGS += -std=c++20 -g
LIBS += -lncursesw
