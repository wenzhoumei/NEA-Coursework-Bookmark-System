TARGET = program.program
OBJECTS_DIR = ./obj
HEADERS += vector_entry_list.hpp file_vector_entry_list.hpp entry_list.hpp menu.hpp util.hpp
SOURCES += main.cpp vector_entry_list.cpp entry_list.cpp util.cpp
INCLUDE_PATH +=
QMAKE_CXXFLAGS += -std=c++20 -g
LIBS += -lncursesw
