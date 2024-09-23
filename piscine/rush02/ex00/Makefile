NAME = rush-02
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRC = 	main.c \
		ft_list.c \
		ft_list2.c \
		ft_utils.c \
		ft_utils2.c \
		ft_atoi.c \
		ft_strjoin.c \
		ft_split.c \
		ft_print.c \
		ft_parse_dictionnary.c \
		ft_parse_string.c \
		ft_parse_number.c \
		ft_check_dictionnary.c \
		ft_free.c
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
