#
# Directories
#
PREFIX = ../
BIN_DIR = $(PREFIX)bin
LIBS = 
LIB_DIR = 
INCLUDE_DIR = $(PREFIX)include/cowic
STATIC_LIB = $(PREFIX)lib/cowic.a

#
# The compiler and its flags
#
CXX = g++ 
CXXFLAGS = -g -Wall -std=c++11 -O3

#
# Source files, Objects
#
SRC = compressor_cmd_tool.cpp
OBJ = compressor_cmd_tool.o
PROGRAM = $(BIN_DIR)/compressor_cmd_tool

all : $(PROGRAM) 

$(PROGRAM): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(PROGRAM) $(OBJ) $(STATIC_LIB) $(addprefix -I,$(INCLUDE_DIR)) $(addprefix -L,$(LIB_DIR)) $(addprefix -l,$(LIBS)) 

$(OBJ): $(SRC) 
	${CXX} ${CXXFLAGS} -c $< -o $@ $(addprefix -I,$(INCLUDE_DIR)) 

.PHONY: clean

clean :
	@echo "Removing objects..."
	-rm *.o *.d
