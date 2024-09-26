NAME = bsq
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRC = 	main.c \
		ft_solve.c \
		ft_solve_utils.c \
		ft_utils.c \
		ft_utils2.c \
		ft_str_utils.c \
		ft_output.c \
		ft_memory.c \
		ft_map.c

SRCSDIR = ./srcs/
SRCS = $(addprefix $(SRCSDIR), $(SRC))
INC = ./includes
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME) : 	$(OBJS)
			$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o:%.c
		$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

clean:
		rm -rf $(OBJS)

fclean: clean
		rm -rf $(NAME)

re: fclean all
