NAME = ft_hexdump
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRC = 	ft_hexdump.c \
		ft_str.c \
		ft_str2.c \
		ft_print_memory.c \
		ft_print_memory2.c \
		ft_memory.c \
		ft_check_args.c 
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