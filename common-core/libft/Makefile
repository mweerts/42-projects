NAME = libft.a
CC = cc 
CFLAGS = -Wall -Wextra -Werror
SRCS = 	ft_isalpha.c \
		ft_isdigit.c \
		ft_isalnum.c \
		ft_isascii.c \
		ft_isprint.c \
		ft_strlen.c \
		ft_memset.c \
		ft_bzero.c \
		ft_memcpy.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME) : 	$(OBJS)
			ar rcs $(NAME) $(OBJS)

%.o:%.c
		$(CC) $(CFLAGS) -c $< -o $@

clean: 
		rm -rf $(OBJS)

fclean: clean
		rm -rf $(NAME)

re: fclean all