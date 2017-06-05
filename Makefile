NAME	=	delta-v

SRCS	=	src/connection.c		\
		src/transfer.c			\
		src/tree.c			\
		src/xmalloc.c			\
		src/display_ip.c		\
		src/xread.c			\
		src/xwrite.c			\
		src/display.c			\
		src/get_args.c			\
		src/delta-v.c			\
		src/check_transfer_parameters.c	\
		src/delta.c

OBJS	=	$(SRCS:.c=.o)

CFLAGS	=	-I include/			\
		-W -Wall -Wextra -O

CC	=	gcc

all	:	$(NAME)

$(NAME):	$(OBJS)
		$(CC) -o $(NAME) $(OBJS)

install	:	all
		sudo cp $(NAME) /bin/

clean	:
		rm -rf $(OBJS)

fclean	:	clean
		rm -rf $(NAME)

re	:	fclean all
