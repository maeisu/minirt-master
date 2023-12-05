NAME = minirt
# LIB = ./Libft/libft.a
# PRI = ./printf/libftprintf.a

SRCS = test.c
OBJ_DIR = ./objs
OBJS = $(SRCS:.c=.o)

OPTION = -lmlx -framework OpenGL -framework Appkit
INCLUDE = ./fdf.h
CC = cc
# CFLAGS = -Wall -Wextra -Werror

vpath %.c

all: $(NAME)

#$(OBJ_DIR):
#	mkdir -p $@

#$(OBJ_DIR)/%.o:%.c
#	$(CC) $(CFLAGS) -c -o $@ $<

$(NAME): $(OBJS)
#	$(MAKE) -C ./Libft
#	$(MAKE) -C ./printf
#	$(MAKE) -C ./gnl
	$(CC) $(CFLAGS) $(OPTION) $(OBJS) $(LIB) $(PRI) -o $@

#Debug
debug: CFLAGS += -g -fsanitize=address
debug: re

%.o: %.c
	@$(CC) $(CFLAGS) -I mlx -c -o $@ $<

clean:
#	$(MAKE) -C ./Libft fclean
#	$(MAKE) -C ./printf fclean
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all