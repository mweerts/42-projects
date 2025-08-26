NAME = GeneratoreTester
MODE ?= normal
CC = c++
CFLAGS = -Wall -Wextra  -Werror  -g3

ifeq ($(MODE), hard)
CFLAGS += \
	-Werror=unused-variable -Werror=unused-function -Werror=pointer-arith \
	-Werror=uninitialized -Werror=return-type  
else ifeq ($(MODE), math)
CFLAGS += \
	-Werror=unused-variable -Werror=unused-function -Werror=pointer-arith \
	-Werror=uninitialized -Werror=return-type  \
	-Wno-error=sign-conversion  -Wno-error=conversion -Wno-error=float-conversion -Wconversion
else ifeq ($(MODE), debug)
CFLAGS += -g3 -O0 #-DDEBUG
else ifeq ($(MODE), release)
CFLAGS += -g3 -flto -march=native 
else ifeq ($(MODE), noflags)
CFLAGS = -std=c++98
endif
INC = -I ../../includes/
SRC_SRC = $(wildcard ../../srcs/parsing/*.cpp ../../srcs/lib/logger.cpp)
SRC_LIB = main.cpp
SRCS = $(SRC_SRC) $(SRC_LIB)

		 


OBJ_DIR = obj
OBJ = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(INC) -o $(NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

run: all
ifeq ($(MODE), valgrind)
	valgrind ./$(NAME)
else ifeq ($(MODE), normal)
	./$(NAME)
else ifeq ($(MODE), gdb)
	gdb ./$(NAME)
endif 

.PHONY: all clean fclean re
