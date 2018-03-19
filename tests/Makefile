#-----------------------------------------------------------------------------#
#			PROJECT SPECIFICS
#-----------------------------------------------------------------------------#
# Project name and source files
NAME   := bitloom_unittests
ROOT   := ../

# Source files (relative to ROOT)
SRC    := scheduler.c \
	  spy_task.c \
          fake_timer.c \
          test_scheduler.c \
          test_spy_task.c \
          test_fake_timer.c \
          test_main.c


# Include directories (relative to ROOT)
INC    := include \
	  include/hal \
	  tests/config \
	  tests/mocks


# Include directories (absolute paths)
INC_A  	:=


#-----------------------------------------------------------------------------#
#			UNITY FRAMEWORK
#-----------------------------------------------------------------------------#
ifeq ($(UNITY_HOME),)
  $(error UNITY_HOME varaible not set in the environment)
endif

# Unity Includes
UNITY_I	:= $(UNITY_HOME)/src \
	   $(UNITY_HOME)/extras/fixture/src \
	   $(UNITY_HOME)/extras/fixture/test \

# Unity source
UNITY_S := unity.c unity_fixture.c

#-----------------------------------------------------------------------------#
#			MAKEFILE CORE
#-----------------------------------------------------------------------------#
.PHONY: clean all debug test
CC	= gcc

SRC_P   = $(addprefix $(ROOT), $(SRC))
INC_P   = $(addprefix $(ROOT), $(INC))
INC_L   = $(foreach d, $(INC_P), -I$(d))
INC_AL  = $(foreach d, $(INC_A), -I$(d))
INC_U	= $(foreach d, $(UNITY_I), -I$(d))
OBJ     = $(SRC:.c=.o) $(UNITY_S:.c=.o)

CFLAGS  := -Wall -g3 -I. $(INC_L) $(INC_AL) $(INC_U)
LDFLAGS :=

all: 	test

VPATH = $(UNITY_HOME)/src $(UNITY_HOME)/extras/fixture/src\
          ../src/scheduler mocks scheduler

$(NAME) : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

#-----------------------------------------------------------------------------#
#			CLEAN
#-----------------------------------------------------------------------------#
clean:
	rm -rf $(OBJ)
	rm -f $(NAME)

#-----------------------------------------------------------------------------#
#			TEST
#-----------------------------------------------------------------------------#
test:	$(NAME)
	./$(NAME) -v

#-----------------------------------------------------------------------------#
#			DEBUG
#-----------------------------------------------------------------------------#
debug:
	@echo "ROOT    : " $(abspath $(ROOT))
	@echo "SRC_P   : " $(SRC_P)
	@echo "INC_P   : " $(INC_P)
	@echo "INC_L   : " $(INC_L)
	@echo "INC_AL  : " $(INC_AL)
	@echo "INC_U   : " $(INC_U)
	@echo "OBJ     : " $(OBJ)
	@echo "OBJ_U   : " $(OBJ_U)
	@echo "CFLAGS  : " $(CFLAGS)
	@echo "LDFLAGS : " $(LDFLAGS)
	@echo "VPATH   : " $(VPATH)
	@echo "CURDIR  : " $(CURDIR)
