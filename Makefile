#
# Directories
#
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

LIBS =
LIB_DIR =
INCLUDE_DIR = $(wildcard $(SRC_DIR)/*)

#
# The compiler and its flags
#
CXX = g++ 
CXXFLAGS = -g -Wall -std=c++11 -O3
CXXFLAGS += -MD -MP

#
# The library and its flags
#
AR_CMD = ar
#The option cr stands for "create and replace"
AR_OPT = cr 
AR_DIR = lib
AR_LIB = $(AR_DIR)/cowic.a


SRCS = $(wildcard $(SRC_DIR)/*/*.cpp)

OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS)) 
DEP_DIRS := $(sort $(dir $(OBJS)))
DEP_DIRS += $(AR_DIR)
DEP_DIRS += $(BIN_DIR)
DEPS = $(OBJS:.o=.d)

all : $(OBJS) 
	$(AR_CMD) $(AR_OPT) $(AR_LIB) $(OBJS)

# make objects depend on folders
$(OBJS): | $(DEP_DIRS) 

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@ $(addprefix -I,$(INCLUDE_DIR)) 

# create obj folders if not exist
$(DEP_DIRS): 
	mkdir -p $@

-include $(DEPS)

.PHONY: example clean

example:
	make -C example

clean :
	@echo "Removing objects..."
	-rm $(OBJ_DIR)/*/*.o $(OBJ_DIR)/*/*.d $(BIN_DIR)/* $(AR_LIB)
	make -C example clean
